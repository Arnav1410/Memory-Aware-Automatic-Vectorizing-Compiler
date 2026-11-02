#ifndef TINYC3_TRANSLATOR_H
#define TINYC3_TRANSLATOR_H

#include "../common/target_info.hpp"

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

namespace tinyc3 {

using tinyc::BaseKind;
using tinyc::TargetInfo;
using tinyc::Type;
using tinyc::TypePtr;

typedef std::size_t QuadIndex;

struct Quad {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
};

struct CodeBuffer {
    std::vector<Quad> code;
    std::size_t emit(const std::string& op,
                     const std::string& arg1,
                     const std::string& arg2,
                     const std::string& result);
    void append(const CodeBuffer *other);
};

struct SymbolTable;

struct Symbol {
    std::string name;
    TypePtr type;
    std::string initialValue;
    std::size_t size;
    std::size_t offset;
    SymbolTable *nested;
    bool temporary; // Metadata distinguishes compiler temporaries from observable locals.

    Symbol();
    ~Symbol();
};

struct SymbolTable {
    std::string name;
    SymbolTable *parent;
    std::vector<Symbol*> entries;
    std::size_t nextOffset;

    SymbolTable(const std::string& name_, SymbolTable *parent_);
    ~SymbolTable();

    Symbol *findCurrent(const std::string& name) const;
    Symbol *lookup(const std::string& name) const;
    Symbol *add(const std::string& name, TypePtr type, const TargetInfo& target,
                const std::string& initialValue);
    void print(std::ostream& out, const TargetInfo& target, int depth) const;
};

struct LValue {
    enum Kind { LV_NONE, LV_VARIABLE, LV_INDEXED, LV_DEREF };
    Kind kind;
    TypePtr type;
    std::string base;
    std::string offset;
    std::string address;

    LValue();
};

struct ExprAttr {
    std::string place;
    TypePtr type;
    bool isBool;
    bool isConstant;
    std::vector<std::size_t> trueList;
    std::vector<std::size_t> falseList;
    LValue *lvalue;

    ExprAttr();
    ~ExprAttr();
};

struct ParamInfo {
    std::string name;
    TypePtr type;
};

struct DeclInfo {
    std::string name;
    std::size_t pointerDepth;
    std::vector<std::size_t> arrayDims;
    bool isFunction;
    std::vector<ParamInfo> params;

    DeclInfo();
};

struct ExprList {
    std::vector<ExprAttr*> items;
    ~ExprList();
};

struct LogicalContext {
    ExprAttr *lhs;
    std::string label;
    LogicalContext();
};

struct CapturedExpr {
    ExprAttr *expr;
    CodeBuffer *code;
    CapturedExpr();
    ~CapturedExpr();
};

struct CapturedStmt {
    CodeBuffer *code;
    CapturedStmt();
    ~CapturedStmt();
};

struct ForUpdateChunk {
    CodeBuffer *code;
    ForUpdateChunk();
    ~ForUpdateChunk();
};

class Translator {
public:
    Translator(const TargetInfo& target_);
    ~Translator();

    TargetInfo target;
    CodeBuffer quads;
    SymbolTable *global;
    SymbolTable *current;
    TypePtr currentReturnType;

    void enterScope(const std::string& name);
    void leaveScope();
    void beginFunction(const DeclInfo& decl, TypePtr returnType);
    void endFunction();

    TypePtr baseType(BaseKind kind) const;
    TypePtr buildDeclaredType(TypePtr base, const DeclInfo& decl) const;
    Symbol *install(const DeclInfo& decl, TypePtr base, ExprAttr *initializer);

    std::size_t emit(const std::string& op,
                     const std::string& a,
                     const std::string& b,
                     const std::string& r);
    std::string newTemp(TypePtr type);
    std::string newLabel();
    std::string markLabel();

    void backpatch(const std::vector<std::size_t>& list, const std::string& label);
    std::vector<std::size_t> makelist(std::size_t i);
    std::vector<std::size_t> merge(const std::vector<std::size_t>& a,
                                   const std::vector<std::size_t>& b);

    static ExprAttr *makeExpr(TypePtr type, const std::string& place);
    ExprAttr *identifier(const std::string& name);
    ExprAttr *constant(const std::string& lexeme);
    ExprAttr *index(ExprAttr *base, ExprAttr *idx);
    ExprAttr *unary(const std::string& op, ExprAttr *e);
    ExprAttr *binary(const std::string& op, ExprAttr *lhs, ExprAttr *rhs);
    ExprAttr *relational(const std::string& op, ExprAttr *lhs, ExprAttr *rhs);
    ExprAttr *logicalNot(ExprAttr *e);
    ExprAttr *conditionalCaptured(ExprAttr *cond, CapturedExpr *lhs, CapturedExpr *rhs);
    ExprAttr *assign(ExprAttr *lhs, ExprAttr *rhs);
    ExprAttr *incdec(ExprAttr *e, bool increment, bool prefix);
    ExprAttr *call(ExprAttr *callee, const std::vector<ExprAttr*>& args);

    ExprAttr *ensureRValue(ExprAttr *e);
    ExprAttr *ensureCondition(ExprAttr *e);
    ExprAttr *materializeBool(ExprAttr *e);
    TypePtr commonType(TypePtr a, TypePtr b) const;
    ExprAttr *convert(ExprAttr *e, TypePtr targetType);

    void beginCapture();
    CodeBuffer *endCapture();
    CodeBuffer *activeCode();

    void print(std::ostream& out) const;
    void printTac(std::ostream& out) const;

private:
    std::size_t tempCounter;
    std::size_t labelCounter;
};

extern Translator *gTranslator;
extern std::vector<CodeBuffer*> gCodeStack;
extern TypePtr gPendingDeclarationBase;
extern DeclInfo *gExternalDecl;
extern TypePtr gExternalBase;

} // namespace tinyc3

#endif
