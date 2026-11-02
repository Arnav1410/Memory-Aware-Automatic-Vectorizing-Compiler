#include "translator.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace tinyc3 {

Translator *gTranslator = NULL;
std::vector<CodeBuffer*> gCodeStack;
TypePtr gPendingDeclarationBase = NULL;
DeclInfo *gExternalDecl = NULL;
TypePtr gExternalBase = NULL;

std::size_t CodeBuffer::emit(const std::string& op, const std::string& arg1,
                             const std::string& arg2, const std::string& result) {
    Quad q;
    q.op = op; q.arg1 = arg1; q.arg2 = arg2; q.result = result;
    code.push_back(q);
    return code.size() - 1;
}

void CodeBuffer::append(const CodeBuffer *other) {
    if (!other) return;
    for (std::size_t i = 0; i < other->code.size(); ++i)
        code.push_back(other->code[i]);
}

Symbol::Symbol() : type(NULL), size(0), offset(0), nested(NULL), temporary(false) {}
Symbol::~Symbol() { if (nested) delete nested; }

SymbolTable::SymbolTable(const std::string& name_, SymbolTable *parent_)
    : name(name_), parent(parent_), nextOffset(0) {}

SymbolTable::~SymbolTable() {
    for (std::size_t i = 0; i < entries.size(); ++i) delete entries[i];
}

Symbol *SymbolTable::findCurrent(const std::string& name_) const {
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i]->name == name_) return entries[i];
    }
    return NULL;
}

Symbol *SymbolTable::lookup(const std::string& name_) const {
    const SymbolTable *t = this;
    while (t) {
        Symbol *s = t->findCurrent(name_);
        if (s) return s;
        t = t->parent;
    }
    return NULL;
}

static std::size_t roundUp(std::size_t x, std::size_t a) {
    if (a <= 1) return x;
    std::size_t r = x % a;
    return r == 0 ? x : x + (a - r);
}

Symbol *SymbolTable::add(const std::string& name_, TypePtr type_, const TargetInfo& target,
                         const std::string& initialValue) {
    if (!type_) throw std::runtime_error("null type for symbol " + name_);

    Symbol *old = findCurrent(name_);
    if (old) {
        if (old->type && old->type->str() != type_->str())
            throw std::runtime_error("conflicting declaration for " + name_);
        if (!initialValue.empty()) old->initialValue = initialValue;
        return old;
    }

    Symbol *s = new Symbol();
    s->name = name_;
    s->type = type_;
    s->size = target.sizeOf(type_);
    s->offset = roundUp(nextOffset, target.alignOf(type_));
    s->initialValue = initialValue;
    nextOffset = s->offset + s->size;

    entries.push_back(s);
    return s;
}

static void printIndent(std::ostream& out, int depth) {
    int i;
    for (i = 0; i < depth; ++i) out << "  ";
}

void SymbolTable::print(std::ostream& out, const TargetInfo& target, int depth) const {
    printIndent(out, depth);
    out << "ST(" << name << ")\n";
    printIndent(out, depth);
    out << "Name | Type | Initial | Size | Offset | Nested\n";
    printIndent(out, depth);
    out << "-----+------+---------+------+--------+-------\n";

    for (std::size_t i = 0; i < entries.size(); ++i) {
        const Symbol *s = entries[i];
        if (s->name.size() >= 7 && s->name.substr(0, 7) == "<scope>") continue;
        printIndent(out, depth);
        out << s->name << " | " << (s->type ? s->type->str() : "?") << " | "
            << (s->initialValue.empty() ? "null" : s->initialValue) << " | "
            << s->size << " | " << s->offset << " | "
            << (s->nested ? s->nested->name : "null") << "\n";
    }

    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i]->nested) entries[i]->nested->print(out, target, depth + 1);
    }
}

LValue::LValue() : kind(LV_NONE), type(NULL) {}

ExprAttr::ExprAttr()
    : type(NULL), isBool(false), isConstant(false), lvalue(NULL) {}

ExprAttr::~ExprAttr() { if (lvalue) delete lvalue; }

DeclInfo::DeclInfo() : pointerDepth(0), isFunction(false) {}

ExprList::~ExprList() {
    for (std::size_t i = 0; i < items.size(); ++i) delete items[i];
}

LogicalContext::LogicalContext() : lhs(NULL) {}

CapturedExpr::CapturedExpr() : expr(NULL), code(NULL) {}
CapturedExpr::~CapturedExpr() {
    if (expr) delete expr;
    if (code) delete code;
}

CapturedStmt::CapturedStmt() : code(NULL) {}
CapturedStmt::~CapturedStmt() {
    if (code) delete code;
}

ForUpdateChunk::ForUpdateChunk() : code(new CodeBuffer()) {}
ForUpdateChunk::~ForUpdateChunk() { delete code; }

Translator::Translator(const TargetInfo& target_)
    : target(target_), global(new SymbolTable("global", NULL)), current(NULL),
      currentReturnType(NULL), tempCounter(0), labelCounter(0) {
    current = global;
    gCodeStack.clear();
    gPendingDeclarationBase = NULL;
    gExternalDecl = NULL;
    gExternalBase = NULL;
}

Translator::~Translator() {
    while (!gCodeStack.empty()) {
        delete gCodeStack.back();
        gCodeStack.pop_back();
    }
    if (global) delete global;
    global = NULL;
    current = NULL;
    Type::clearPool();
}

void Translator::enterScope(const std::string& name_) {
    SymbolTable *child = new SymbolTable(name_, current);
    Symbol *owner = new Symbol();
    owner->name = "<scope>" + name_;
    owner->type = Type::make(tinyc::TK_VOID);
    owner->nested = child;
    current->entries.push_back(owner);
    current = child;
}

void Translator::leaveScope() {
    if (!current || !current->parent) throw std::runtime_error("scope underflow");
    current = current->parent;
}

void Translator::beginFunction(const DeclInfo& decl, TypePtr returnType) {
    if (current != global) throw std::runtime_error("function must begin in global scope");
    if (!decl.isFunction) throw std::runtime_error("expected function declarator");

    std::vector<std::pair<std::string, Type*> > params;
    for (std::size_t i = 0; i < decl.params.size(); ++i)
        params.push_back(std::make_pair(decl.params[i].name, decl.params[i].type));

    TypePtr fnType = Type::makeFunction(returnType, params);
    Symbol *f = global->add(decl.name, fnType, target, "");

    if (f->nested) delete f->nested;
    f->nested = new SymbolTable(decl.name, global);
    current = f->nested;

    for (std::size_t i = 0; i < decl.params.size(); ++i)
        current->add(decl.params[i].name, decl.params[i].type, target, "");

    currentReturnType = returnType;
    emit("func", decl.name, "", "");
}

void Translator::endFunction() {
    if (!current || current == global) throw std::runtime_error("not inside function");
    emit("endfunc", "", "", "");
    current = global;
    currentReturnType = NULL;
}

TypePtr Translator::baseType(BaseKind kind) const { return Type::make(kind); }

TypePtr Translator::buildDeclaredType(TypePtr base, const DeclInfo& decl) const {
    TypePtr t = base;
    if (decl.isFunction) {
        std::vector<std::pair<std::string, Type*> > params;
        for (std::size_t i = 0; i < decl.params.size(); ++i)
            params.push_back(std::make_pair(decl.params[i].name, decl.params[i].type));
        return Type::makeFunction(base, params);
    }

    std::size_t i;
    for (i = 0; i < decl.pointerDepth; ++i) t = Type::makePointer(t);
    for (i = decl.arrayDims.size(); i > 0; --i)
        t = Type::makeArray(t, decl.arrayDims[i - 1]);
    return t;
}

Symbol *Translator::install(const DeclInfo& decl, TypePtr base, ExprAttr *initializer) {
    TypePtr type = buildDeclaredType(base, decl);
    std::string initial = "";
    if (initializer && initializer->isConstant && !initializer->isBool) initial = initializer->place;

    Symbol *s = current->add(decl.name, type, target, initial);

    if (initializer && !decl.isFunction) {
        if (type->kind == tinyc::TK_ARRAY)
            throw std::runtime_error("array initialization is outside Part 3 subset");

        ExprAttr *rhs = ensureRValue(initializer);
        rhs = convert(rhs, type);
        if (current == global) {
            if (!rhs->isConstant) throw std::runtime_error("global initializer must be constant");
            s->initialValue = rhs->place;
        } else {
            emit("=", rhs->place, "", s->name);
        }
        if (rhs != initializer) delete rhs;
    }
    return s;
}

CodeBuffer *Translator::activeCode() {
    if (gCodeStack.empty()) return &quads;
    return gCodeStack.back();
}

std::size_t Translator::emit(const std::string& op, const std::string& a,
                             const std::string& b, const std::string& r) {
    return activeCode()->emit(op, a, b, r);
}

std::string Translator::newTemp(TypePtr type) {
    std::string name;
    do { name = "t" + std::to_string(tempCounter++); } while (current->lookup(name));
    current->add(name, type, target, "")->temporary = true;
    return name;
}

std::string Translator::newLabel() {
    return "L" + std::to_string(labelCounter++);
}

std::string Translator::markLabel() {
    std::string label = newLabel();
    emit("label", "", "", label);
    return label;
}

void Translator::backpatch(const std::vector<std::size_t>& list, const std::string& label) {
    CodeBuffer *c = activeCode();
    for (std::size_t k = 0; k < list.size(); ++k) {
        if (list[k] >= c->code.size()) throw std::runtime_error("invalid backpatch index");
        c->code[list[k]].result = label;
    }
}

std::vector<std::size_t> Translator::makelist(std::size_t i) {
    std::vector<std::size_t> x;
    x.push_back(i);
    return x;
}

std::vector<std::size_t> Translator::merge(const std::vector<std::size_t>& a,
                                            const std::vector<std::size_t>& b) {
    std::vector<std::size_t> x = a;
    for (std::size_t i = 0; i < b.size(); ++i) x.push_back(b[i]);
    return x;
}

ExprAttr *Translator::makeExpr(TypePtr type, const std::string& place) {
    ExprAttr *e = new ExprAttr();
    e->type = type;
    e->place = place;
    return e;
}

ExprAttr *Translator::identifier(const std::string& name) {
    Symbol *s = current->lookup(name);
    if (!s) throw std::runtime_error("undeclared identifier: " + name);
    ExprAttr *e = makeExpr(s->type, s->name);
    if (s->type->kind == tinyc::TK_FUNCTION) return e;
    e->lvalue = new LValue();
    e->lvalue->kind = LValue::LV_VARIABLE;
    e->lvalue->type = s->type;
    e->lvalue->base = s->name;
    return e;
}

static bool looksFloat(const std::string& s) {
    return s.find('.') != std::string::npos || s.find('e') != std::string::npos ||
           s.find('E') != std::string::npos;
}

ExprAttr *Translator::constant(const std::string& lexeme) {
    TypePtr type;
    if (!lexeme.empty() && lexeme[0] == '\'') type = Type::make(tinyc::TK_CHAR);
    else if (looksFloat(lexeme)) type = Type::make(tinyc::TK_FLOAT);
    else if (!lexeme.empty() && lexeme[0] == '"') type = Type::makePointer(Type::make(tinyc::TK_CHAR));
    else type = Type::make(tinyc::TK_INT);

    ExprAttr *e = makeExpr(type, lexeme);
    e->isConstant = true;
    return e;
}

ExprAttr *Translator::ensureRValue(ExprAttr *e) {
    if (!e) throw std::runtime_error("null expression");
    if (e->isBool) return materializeBool(e);
    if (!e->lvalue) return e;

    if (e->type->kind == tinyc::TK_ARRAY) {
        TypePtr pType = Type::makePointer(e->type->element);
        std::string t = newTemp(pType);
        if (e->lvalue->kind == LValue::LV_VARIABLE) {
            emit("=&", e->lvalue->base, "", t);
        } else if (e->lvalue->kind == LValue::LV_INDEXED) {
            TypePtr arrayPtr = Type::makePointer(e->type);
            std::string baseAddr = newTemp(arrayPtr);
            emit("=&", e->lvalue->base, "", baseAddr);
            emit("+", baseAddr, e->lvalue->offset, t);
        } else {
            emit("=", e->lvalue->address, "", t);
        }
        delete e->lvalue; e->lvalue = NULL;
        return makeExpr(pType, t);
    }

    if (e->lvalue->kind == LValue::LV_VARIABLE) {
        delete e->lvalue; e->lvalue = NULL;
        return e;
    }

    std::string t = newTemp(e->type);
    if (e->lvalue->kind == LValue::LV_INDEXED)
        emit("=[]", e->lvalue->base, e->lvalue->offset, t);
    else
        emit("=*", e->lvalue->address, "", t);

    delete e->lvalue; e->lvalue = NULL;
    e->place = t;
    return e;
}

ExprAttr *Translator::index(ExprAttr *base, ExprAttr *idx) {
    if (!base || !idx) throw std::runtime_error("invalid indexing");
    ExprAttr *ob = base;
    ExprAttr *oi = idx;
    idx = ensureRValue(idx);
    base = ensureRValue(base);

    if (base->type->kind != tinyc::TK_POINTER)
        throw std::runtime_error("indexing requires pointer or array");

    TypePtr elem = base->type->element;
    std::size_t width = target.sizeOf(elem);
    std::string offset = idx->place;
    if (width != 1) {
        std::string scaled = newTemp(Type::make(tinyc::TK_INT));
        emit("*", idx->place, std::to_string(width), scaled);
        offset = scaled;
    }

    TypePtr addressType = Type::makePointer(elem);
    std::string address = newTemp(addressType);
    emit("+", base->place, offset, address);

    ExprAttr *e = makeExpr(elem, "");
    e->lvalue = new LValue();
    e->lvalue->kind = LValue::LV_DEREF;
    e->lvalue->type = elem;
    e->lvalue->address = address;

    if (base != ob) delete base;
    if (idx != oi) delete idx;
    return e;
}

static LValue *copyLValue(const LValue *x) {
    if (!x) return NULL;
    LValue *y = new LValue();
    y->kind = x->kind;
    y->type = x->type;
    y->base = x->base;
    y->offset = x->offset;
    y->address = x->address;
    return y;
}

ExprAttr *Translator::unary(const std::string& op, ExprAttr *e) {
    if (!e) throw std::runtime_error("null operand");

    if (op == "&") {
        if (!e->lvalue) throw std::runtime_error("address-of needs an lvalue");
        TypePtr ptrType = Type::makePointer(e->type);
        std::string t = newTemp(ptrType);
        if (e->lvalue->kind == LValue::LV_VARIABLE)
            emit("=&", e->lvalue->base, "", t);
        else if (e->lvalue->kind == LValue::LV_DEREF)
            emit("=", e->lvalue->address, "", t);
        else
            throw std::runtime_error("unsupported address-of operand");
        return makeExpr(ptrType, t);
    }

    if (op == "*") {
        ExprAttr *original = e;
        e = ensureRValue(e);
        if (e->type->kind != tinyc::TK_POINTER)
            throw std::runtime_error("dereference needs pointer");
        ExprAttr *r = makeExpr(e->type->element, "");
        r->lvalue = new LValue();
        r->lvalue->kind = LValue::LV_DEREF;
        r->lvalue->type = r->type;
        r->lvalue->address = e->place;
        if (e != original) delete e;
        return r;
    }

    if (op == "!") return logicalNot(e);

    ExprAttr *original = e;
    e = ensureRValue(e);
    std::string t = newTemp(e->type);
    emit(op, e->place, "", t);
    TypePtr resultType = e->type;
    if (e != original) delete e;
    return makeExpr(resultType, t);
}

TypePtr Translator::commonType(TypePtr a, TypePtr b) const {
    if (!a || !b) throw std::runtime_error("null type in commonType");
    if (a->kind == tinyc::TK_POINTER || b->kind == tinyc::TK_POINTER) {
        if (a->kind == tinyc::TK_POINTER && b->kind == tinyc::TK_POINTER && a->str() == b->str()) return a;
        throw std::runtime_error("incompatible pointer types");
    }
    if (a->kind == tinyc::TK_FLOAT || b->kind == tinyc::TK_FLOAT) return Type::make(tinyc::TK_FLOAT);
    if (a->kind == tinyc::TK_INT || b->kind == tinyc::TK_INT) return Type::make(tinyc::TK_INT);
    return Type::make(tinyc::TK_CHAR);
}

ExprAttr *Translator::convert(ExprAttr *e, TypePtr targetType) {
    if (!e || !targetType) throw std::runtime_error("bad conversion");
    e = ensureRValue(e);
    if (e->type->str() == targetType->str()) return e;

    bool sp = e->type->kind == tinyc::TK_POINTER;
    bool tp = targetType->kind == tinyc::TK_POINTER;
    if (sp || tp) {
        if (!sp && tp && e->isConstant && e->type->kind == tinyc::TK_INT && e->place == "0") {
            e->type = targetType;
            return e;
        }
        throw std::runtime_error("unsupported pointer conversion");
    }

    std::string t = newTemp(targetType);
    emit("conv_" + e->type->str() + "_" + targetType->str(), e->place, "", t);
    e->type = targetType;
    e->place = t;
    e->isConstant = false;
    return e;
}

ExprAttr *Translator::binary(const std::string& op, ExprAttr *lhs, ExprAttr *rhs) {
    if (!lhs || !rhs) throw std::runtime_error("null operand in binary expression");
    ExprAttr *ol = lhs;
    ExprAttr *orr = rhs;
    lhs = ensureRValue(lhs);
    rhs = ensureRValue(rhs);

    bool lp = lhs->type->kind == tinyc::TK_POINTER;
    bool rp = rhs->type->kind == tinyc::TK_POINTER;

    if (lp || rp) {
        if (op == "+" && lp && !rp) {
            if (rhs->type->kind != tinyc::TK_INT && rhs->type->kind != tinyc::TK_CHAR)
                throw std::runtime_error("pointer + integer required");
            rhs = convert(rhs, Type::make(tinyc::TK_INT));
            std::size_t width = target.sizeOf(lhs->type->element);
            std::string off = rhs->place;
            if (width != 1) {
                std::string s = newTemp(Type::make(tinyc::TK_INT));
                emit("*", rhs->place, std::to_string(width), s);
                off = s;
            }
            TypePtr outType = lhs->type;
            std::string out = newTemp(outType);
            emit("+", lhs->place, off, out);
            if (lhs != ol) delete lhs;
            if (rhs != orr) delete rhs;
            return makeExpr(outType, out);
        }
        if (op == "+" && !lp && rp) return binary(op, rhs, lhs);
        if (op == "-" && lp && !rp) {
            if (rhs->type->kind != tinyc::TK_INT && rhs->type->kind != tinyc::TK_CHAR)
                throw std::runtime_error("pointer - integer required");
            rhs = convert(rhs, Type::make(tinyc::TK_INT));
            std::size_t width = target.sizeOf(lhs->type->element);
            std::string off = rhs->place;
            if (width != 1) {
                std::string s = newTemp(Type::make(tinyc::TK_INT));
                emit("*", rhs->place, std::to_string(width), s);
                off = s;
            }
            TypePtr outType = lhs->type;
            std::string out = newTemp(outType);
            emit("-", lhs->place, off, out);
            if (lhs != ol) delete lhs;
            if (rhs != orr) delete rhs;
            return makeExpr(outType, out);
        }
        if (op == "-" && lp && rp) {
            if (lhs->type->str() != rhs->type->str()) throw std::runtime_error("incompatible pointer subtraction");
            std::string bytes = newTemp(Type::make(tinyc::TK_INT));
            emit("-", lhs->place, rhs->place, bytes);
            std::size_t width = target.sizeOf(lhs->type->element);
            if (width != 1) {
                std::string elems = newTemp(Type::make(tinyc::TK_INT));
                emit("/", bytes, std::to_string(width), elems);
                if (lhs != ol) delete lhs;
                if (rhs != orr) delete rhs;
                return makeExpr(Type::make(tinyc::TK_INT), elems);
            }
            if (lhs != ol) delete lhs;
            if (rhs != orr) delete rhs;
            return makeExpr(Type::make(tinyc::TK_INT), bytes);
        }
        throw std::runtime_error("unsupported pointer operation");
    }

    TypePtr t = commonType(lhs->type, rhs->type);
    lhs = convert(lhs, t);
    rhs = convert(rhs, t);
    std::string out = newTemp(t);
    emit(op, lhs->place, rhs->place, out);
    if (lhs != ol) delete lhs;
    if (rhs != orr) delete rhs;
    return makeExpr(t, out);
}

ExprAttr *Translator::relational(const std::string& op, ExprAttr *lhs, ExprAttr *rhs) {
    ExprAttr *ol = lhs;
    ExprAttr *orr = rhs;
    lhs = ensureRValue(lhs);
    rhs = ensureRValue(rhs);
    bool lp = lhs->type->kind == tinyc::TK_POINTER;
    bool rp = rhs->type->kind == tinyc::TK_POINTER;
    if (lp || rp) {
        if (!(lp && rp) && !(lp && rhs->type->kind == tinyc::TK_INT && rhs->isConstant && rhs->place == "0") &&
            !(rp && lhs->type->kind == tinyc::TK_INT && lhs->isConstant && lhs->place == "0"))
            throw std::runtime_error("bad pointer comparison");
    } else {
        TypePtr t = commonType(lhs->type, rhs->type);
        lhs = convert(lhs, t);
        rhs = convert(rhs, t);
    }

    std::size_t q1 = emit("if" + op, lhs->place, rhs->place, "?");
    std::size_t q2 = emit("goto", "", "", "?");
    ExprAttr *e = makeExpr(Type::make(tinyc::TK_BOOL), "");
    e->isBool = true;
    e->trueList = makelist(q1);
    e->falseList = makelist(q2);
    if (lhs != ol) delete lhs;
    if (rhs != orr) delete rhs;
    return e;
}

ExprAttr *Translator::ensureCondition(ExprAttr *e) {
    if (!e) throw std::runtime_error("null condition");
    if (e->isBool) return e;
    e = ensureRValue(e);
    std::size_t q1 = emit("if", e->place, "0", "?");
    std::size_t q2 = emit("goto", "", "", "?");
    ExprAttr *b = makeExpr(Type::make(tinyc::TK_BOOL), "");
    b->isBool = true;
    b->trueList = makelist(q1);
    b->falseList = makelist(q2);
    return b;
}

ExprAttr *Translator::logicalNot(ExprAttr *e) {
    ExprAttr *c = ensureCondition(e);
    ExprAttr *out = makeExpr(Type::make(tinyc::TK_BOOL), "");
    out->isBool = true;
    out->trueList = c->falseList;
    out->falseList = c->trueList;
    if (c != e) delete c;
    return out;
}

ExprAttr *Translator::materializeBool(ExprAttr *e) {
    if (!e || !e->isBool) return e;
    std::string t = newTemp(Type::make(tinyc::TK_INT));
    std::string trueLabel = markLabel();
    backpatch(e->trueList, trueLabel);
    emit("=", "1", "", t);
    std::size_t jumpEnd = emit("goto", "", "", "?");
    std::string falseLabel = markLabel();
    backpatch(e->falseList, falseLabel);
    emit("=", "0", "", t);
    std::string endLabel = markLabel();
    std::vector<std::size_t> l = makelist(jumpEnd);
    backpatch(l, endLabel);
    return makeExpr(Type::make(tinyc::TK_INT), t);
}

ExprAttr *Translator::conditionalCaptured(ExprAttr *cond, CapturedExpr *lhs, CapturedExpr *rhs) {
    if (!cond || !lhs || !rhs || !lhs->expr || !rhs->expr) throw std::runtime_error("bad conditional expression");

    ExprAttr *originalCond = cond;
    cond = ensureCondition(cond);
    std::string trueLabel = markLabel();
    backpatch(cond->trueList, trueLabel);

    TypePtr t = commonType(lhs->expr->type, rhs->expr->type);
    std::string result = newTemp(t);

    activeCode()->append(lhs->code);
    ExprAttr *lv = ensureRValue(lhs->expr);
    lv = convert(lv, t);
    emit("=", lv->place, "", result);
    if (lv != lhs->expr) delete lv;
    std::size_t jumpEnd = emit("goto", "", "", "?");

    std::string falseLabel = markLabel();
    backpatch(cond->falseList, falseLabel);
    activeCode()->append(rhs->code);
    ExprAttr *rv = ensureRValue(rhs->expr);
    rv = convert(rv, t);
    emit("=", rv->place, "", result);
    if (rv != rhs->expr) delete rv;

    std::string endLabel = markLabel();
    std::vector<std::size_t> x = makelist(jumpEnd);
    backpatch(x, endLabel);

    if (cond != originalCond) delete cond;
    return makeExpr(t, result);
}

ExprAttr *Translator::assign(ExprAttr *lhs, ExprAttr *rhs) {
    if (!lhs || !lhs->lvalue) throw std::runtime_error("assignment needs lvalue");
    if (lhs->type->kind == tinyc::TK_ARRAY) throw std::runtime_error("array assignment not supported");
    ExprAttr *orhs = rhs;
    rhs = ensureRValue(rhs);
    rhs = convert(rhs, lhs->type);

    if (lhs->lvalue->kind == LValue::LV_VARIABLE) emit("=", rhs->place, "", lhs->lvalue->base);
    else if (lhs->lvalue->kind == LValue::LV_INDEXED) emit("[]=", lhs->lvalue->base, lhs->lvalue->offset, rhs->place);
    else if (lhs->lvalue->kind == LValue::LV_DEREF) emit("*=", lhs->lvalue->address, rhs->place, "");
    else throw std::runtime_error("unsupported lvalue");

    std::string place = rhs->place;
    TypePtr type = lhs->type;
    if (rhs != orhs) delete rhs;
    return makeExpr(type, place);
}

ExprAttr *Translator::incdec(ExprAttr *e, bool increment, bool prefix) {
    if (!e || !e->lvalue)
        throw std::runtime_error("++/-- needs lvalue");

    TypePtr type = e->type;
    LValue *lv = copyLValue(e->lvalue);

    /* Save the value before changing the lvalue. */
    ExprAttr *oldValue = ensureRValue(e);
    std::string oldPlace = oldValue->place;

    std::string step = "1";
    if (type->kind == tinyc::TK_POINTER) {
        std::size_t width = target.sizeOf(type->element);
        if (width != 1) {
            step = newTemp(Type::make(tinyc::TK_INT));
            emit("*", "1", std::to_string(width), step);
        }
    }

    std::string next = newTemp(type);
    emit(increment ? "+" : "-", oldPlace, step, next);

    ExprAttr *target = makeExpr(type, oldPlace);
    target->lvalue = lv;
    ExprAttr *value = makeExpr(type, next);
    ExprAttr *ignored = assign(target, value);
    delete ignored;
    delete target;
    delete value;

    /* incdec owns and destroys its input expression.  It always returns a
       fresh expression, so the grammar never has to guess who owns e. */
    if (oldValue != e) delete oldValue;
    delete e;

    if (prefix) return makeExpr(type, next);
    return makeExpr(type, oldPlace);
}

ExprAttr *Translator::call(ExprAttr *callee, const std::vector<ExprAttr*>& args) {
    if (!callee || callee->type->kind != tinyc::TK_FUNCTION) throw std::runtime_error("not a function");
    if (args.size() != callee->type->params.size()) throw std::runtime_error("wrong number of arguments");

    for (std::size_t i = 0; i < args.size(); ++i) {
        ExprAttr *original = args[i];
        ExprAttr *arg = ensureRValue(original);
        arg = convert(arg, callee->type->params[i].second);
        emit("param", arg->place, "", "");
        if (arg != original) delete arg;
    }

    std::string n = std::to_string(args.size());
    if (callee->type->returnType->kind == tinyc::TK_VOID) {
        emit("call", callee->place, n, "");
        return makeExpr(callee->type->returnType, "");
    }

    std::string t = newTemp(callee->type->returnType);
    emit("call", callee->place, n, t);
    return makeExpr(callee->type->returnType, t);
}

void Translator::beginCapture() { gCodeStack.push_back(new CodeBuffer()); }

CodeBuffer *Translator::endCapture() {
    if (gCodeStack.empty()) throw std::runtime_error("capture stack underflow");
    CodeBuffer *p = gCodeStack.back();
    gCodeStack.pop_back();
    return p;
}

void Translator::print(std::ostream& out) const {
    out << "=== Target Information ===\n" << target.describe() << "\n\n";
    out << "=== Symbol Tables ===\n";
    global->print(out, target, 0);
    out << "\n=== Quad Array ===\n";
    out << "Index | op | arg1 | arg2 | result\n";
    out << "------+----+------+------+-------\n";
    for (std::size_t i = 0; i < quads.code.size(); ++i) {
        const Quad& q = quads.code[i];
        out << i << " | " << q.op << " | " << q.arg1 << " | " << q.arg2 << " | " << q.result << "\n";
    }
}

static std::string metadataType(TypePtr t) {
    if (t->kind == tinyc::TK_INT) return "i32";
    if (t->kind == tinyc::TK_VOID) return "void";
    if (t->kind == tinyc::TK_POINTER && t->element->kind == tinyc::TK_INT) return "ptr32";
    if (t->kind == tinyc::TK_ARRAY && t->element->kind == tinyc::TK_INT)
        return "array32:" + std::to_string(t->arrayCount);
    return "unsupported:" + t->str();
}

static void metadataSymbols(std::ostream& out, const SymbolTable *table,
                            const Symbol *fn) {
    for (std::size_t i = 0; i < table->entries.size(); ++i) {
        const Symbol *s = table->entries[i];
        if (s->nested) { metadataSymbols(out, s->nested, fn); continue; }
        std::string role = s->temporary ? "temp" : "local";
        if (table == fn->nested) {
            for (std::size_t p = 0; p < fn->type->params.size(); ++p)
                if (fn->type->params[p].first == s->name) role = "param:" + std::to_string(p);
        }
        out << "#@symbol " << fn->name << ' ' << s->name << ' '
            << metadataType(s->type) << ' ' << role << '\n';
    }
}

void Translator::printTac(std::ostream& out) const {
    out << "#@target " << target.intSize << ' ' << target.pointerSize << '\n';
    for (std::size_t i = 0; i < global->entries.size(); ++i) {
        const Symbol *f = global->entries[i];
        if (f->type->kind == tinyc::TK_FUNCTION && f->nested) {
            out << "#@function " << f->name << ' ' << metadataType(f->type->returnType) << '\n';
            metadataSymbols(out, f->nested, f);
        }
    }
    for (std::size_t i = 0; i < quads.code.size(); ++i) {
        const Quad& q = quads.code[i];
        out << q.op << '\t' << q.arg1 << '\t' << q.arg2 << '\t' << q.result << '\n';
    }
}

} // namespace tinyc3
