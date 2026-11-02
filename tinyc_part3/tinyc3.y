%code requires {
#include "translator.hpp"
#include <cstddef>
#include <vector>
}

%{
#include "translator.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace tinyc3;

extern int yylex(void);
extern char *yytext;
extern int yylineno;
void yyerror(const char *s);
%}

%union {
    char *text;
    std::size_t index;
    tinyc::BaseKind base;
    tinyc3::ExprAttr *expr;
    tinyc3::ExprList *exprs;
    tinyc3::CapturedExpr *capexpr;
    tinyc3::CapturedStmt *capstmt;
    tinyc3::LogicalContext *lctx;
    tinyc3::DeclInfo *decl;
    tinyc3::ParamInfo *param;
    std::vector<tinyc3::ParamInfo> *params;
    tinyc3::ForUpdateChunk *chunk;
}

%token <text> IDENTIFIER CONSTANT STRING_LITERAL
%token VOID CHAR INT FLOAT
%token IF ELSE WHILE DO FOR RETURN
%token INC DEC SHL SHR LE GE EQ NE ANDAND OROR

%type <base> declaration_specifiers type_specifier
%type <decl> declarator direct_declarator pointer_opt
%type <param> parameter_declaration
%type <params> parameter_list parameter_list_opt
%type <expr> primary_expression postfix_expression
%type <expr> unary_expression cast_expression multiplicative_expression additive_expression
%type <expr> shift_expression relational_expression equality_expression and_expression
%type <expr> exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <expr> conditional_expression assignment_expression expression expression_opt
%type <expr> initializer condition_expression for_condition_opt for_update_expression_opt
%type <exprs> argument_expression_list_opt argument_expression_list
%type <capexpr> captured_expression
%type <capstmt> captured_statement
%type <chunk> for_update_clause
%type <index> marker block_scope_begin block_scope_end

%precedence IFX
%precedence ELSE

%%

translation_unit
    : external_declaration { }
    | translation_unit external_declaration { }
    ;

/* The first declarator is separated from the rest of an external declaration.
   After it has been read, '{' means a function definition; otherwise it is a
   global object/function declaration. */
external_declaration
    : declaration_specifiers declarator
      {
          gExternalBase = gTranslator->baseType($1);
          gPendingDeclarationBase = gExternalBase;
          gExternalDecl = $2;
      }
      external_declaration_tail
      {
          delete gExternalDecl;
          gExternalDecl = NULL;
          gExternalBase = NULL;
          gPendingDeclarationBase = NULL;
      }
    ;

external_declaration_tail
    : '{'
      {
          gTranslator->beginFunction(*gExternalDecl, gExternalBase);
      }
      block_item_list_opt '}'
      {
          gTranslator->endFunction();
      }
    | external_init_tail ';' { }
    ;

external_init_tail
    : %empty
      {
          gTranslator->install(*gExternalDecl, gExternalBase, NULL);
      }
    | '=' initializer
      {
          gTranslator->install(*gExternalDecl, gExternalBase, $2);
          delete $2;
      }
    | ','
      {
          gTranslator->install(*gExternalDecl, gExternalBase, NULL);
      }
      init_declarator_list { }
    ;

declaration
    : declaration_specifiers
      {
          gPendingDeclarationBase = gTranslator->baseType($1);
      }
      init_declarator_list_opt ';'
      {
          gPendingDeclarationBase = NULL;
      }
    ;

init_declarator_list_opt
    : %empty { }
    | init_declarator_list { }
    ;

init_declarator_list
    : init_declarator { }
    | init_declarator_list ',' init_declarator { }
    ;

init_declarator
    : declarator
      {
          gTranslator->install(*$1, gPendingDeclarationBase, NULL);
          delete $1;
      }
    | declarator '=' initializer
      {
          gTranslator->install(*$1, gPendingDeclarationBase, $3);
          delete $1;
          delete $3;
      }
    ;

declaration_specifiers
    : type_specifier { $$ = $1; }
    ;

type_specifier
    : VOID  { $$ = tinyc::TK_VOID; }
    | CHAR  { $$ = tinyc::TK_CHAR; }
    | INT   { $$ = tinyc::TK_INT; }
    | FLOAT { $$ = tinyc::TK_FLOAT; }
    ;

declarator
    : pointer_opt direct_declarator
      {
          $$ = $2;
          $$->pointerDepth += $1->pointerDepth;
          delete $1;
      }
    ;

pointer_opt
    : %empty
      {
          $$ = new DeclInfo();
          $$->pointerDepth = 0;
      }
    | '*' pointer_opt
      {
          $$ = $2;
          ++$$->pointerDepth;
      }
    ;

direct_declarator
    : IDENTIFIER
      {
          $$ = new DeclInfo();
          $$->name = $1;
          std::free($1);
      }
    | direct_declarator '[' CONSTANT ']'
      {
          $$ = $1;
          $$->arrayDims.push_back((std::size_t)std::stoull($3));
          std::free($3);
      }
    | direct_declarator '(' parameter_list_opt ')'
      {
          $$ = $1;
          $$->isFunction = true;
          $$->params = *$3;
          delete $3;
      }
    ;

parameter_list_opt
    : %empty
      {
          $$ = new std::vector<ParamInfo>();
      }
    | parameter_list
      {
          $$ = $1;
      }
    ;

parameter_list
    : parameter_declaration
      {
          $$ = new std::vector<ParamInfo>();
          $$->push_back(*$1);
          delete $1;
      }
    | parameter_list ',' parameter_declaration
      {
          $$ = $1;
          $$->push_back(*$3);
          delete $3;
      }
    ;

parameter_declaration
    : declaration_specifiers declarator
      {
          $$ = new ParamInfo();
          $$->name = $2->name;
          $$->type = gTranslator->buildDeclaredType(gTranslator->baseType($1), *$2);
          delete $2;
      }
    ;

primary_expression
    : IDENTIFIER
      {
          $$ = gTranslator->identifier($1);
          std::free($1);
      }
    | CONSTANT
      {
          $$ = gTranslator->constant($1);
          std::free($1);
      }
    | STRING_LITERAL
      {
          $$ = gTranslator->constant($1);
          std::free($1);
      }
    | '(' expression ')'
      {
          $$ = $2;
      }
    ;

postfix_expression
    : primary_expression
      {
          $$ = $1;
      }
    | postfix_expression '[' expression ']'
      {
          $$ = gTranslator->index($1, $3);
          delete $1;
          delete $3;
      }
    | postfix_expression '(' argument_expression_list_opt ')'
      {
          std::vector<ExprAttr*> args = $3->items;
          $$ = gTranslator->call($1, args);
          delete $1;
          delete $3;
      }
    | postfix_expression INC
      {
          $$ = gTranslator->incdec($1, true, false);
      }
    | postfix_expression DEC
      {
          $$ = gTranslator->incdec($1, false, false);
      }
    ;

argument_expression_list_opt
    : %empty
      {
          $$ = new ExprList();
      }
    | argument_expression_list
      {
          $$ = $1;
      }
    ;

argument_expression_list
    : assignment_expression
      {
          $$ = new ExprList();
          $$->items.push_back($1);
      }
    | argument_expression_list ',' assignment_expression
      {
          $$ = $1;
          $$->items.push_back($3);
      }
    ;

unary_expression
    : postfix_expression
      {
          $$ = $1;
      }
    | INC unary_expression
      {
          $$ = gTranslator->incdec($2, true, true);
      }
    | DEC unary_expression
      {
          $$ = gTranslator->incdec($2, false, true);
      }
    | '&' unary_expression
      {
          $$ = gTranslator->unary("&", $2);
          delete $2;
      }
    | '*' cast_expression
      {
          $$ = gTranslator->unary("*", $2);
          delete $2;
      }
    | '+' cast_expression
      {
          $$ = gTranslator->unary("+", $2);
          delete $2;
      }
    | '-' cast_expression
      {
          $$ = gTranslator->unary("-", $2);
          delete $2;
      }
    | '~' cast_expression
      {
          $$ = gTranslator->unary("~", $2);
          delete $2;
      }
    | '!' cast_expression
      {
          $$ = gTranslator->logicalNot($2);
          delete $2;
      }
    ;

cast_expression
    : unary_expression { $$ = $1; }
    ;

multiplicative_expression
    : cast_expression { $$ = $1; }
    | multiplicative_expression '*' cast_expression
      { $$ = gTranslator->binary("*", $1, $3); delete $1; delete $3; }
    | multiplicative_expression '/' cast_expression
      { $$ = gTranslator->binary("/", $1, $3); delete $1; delete $3; }
    | multiplicative_expression '%' cast_expression
      { $$ = gTranslator->binary("%", $1, $3); delete $1; delete $3; }
    ;

additive_expression
    : multiplicative_expression { $$ = $1; }
    | additive_expression '+' multiplicative_expression
      { $$ = gTranslator->binary("+", $1, $3); delete $1; delete $3; }
    | additive_expression '-' multiplicative_expression
      { $$ = gTranslator->binary("-", $1, $3); delete $1; delete $3; }
    ;

shift_expression
    : additive_expression { $$ = $1; }
    | shift_expression SHL additive_expression
      { $$ = gTranslator->binary("<<", $1, $3); delete $1; delete $3; }
    | shift_expression SHR additive_expression
      { $$ = gTranslator->binary(">>", $1, $3); delete $1; delete $3; }
    ;

relational_expression
    : shift_expression { $$ = $1; }
    | relational_expression '<' shift_expression
      { $$ = gTranslator->relational("<", $1, $3); delete $1; delete $3; }
    | relational_expression '>' shift_expression
      { $$ = gTranslator->relational(">", $1, $3); delete $1; delete $3; }
    | relational_expression LE shift_expression
      { $$ = gTranslator->relational("<=", $1, $3); delete $1; delete $3; }
    | relational_expression GE shift_expression
      { $$ = gTranslator->relational(">=", $1, $3); delete $1; delete $3; }
    ;

equality_expression
    : relational_expression { $$ = $1; }
    | equality_expression EQ relational_expression
      { $$ = gTranslator->relational("==", $1, $3); delete $1; delete $3; }
    | equality_expression NE relational_expression
      { $$ = gTranslator->relational("!=", $1, $3); delete $1; delete $3; }
    ;

and_expression
    : equality_expression { $$ = $1; }
    | and_expression '&' equality_expression
      { $$ = gTranslator->binary("&", $1, $3); delete $1; delete $3; }
    ;

exclusive_or_expression
    : and_expression { $$ = $1; }
    | exclusive_or_expression '^' and_expression
      { $$ = gTranslator->binary("^", $1, $3); delete $1; delete $3; }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = $1; }
    | inclusive_or_expression '|' exclusive_or_expression
      { $$ = gTranslator->binary("|", $1, $3); delete $1; delete $3; }
    ;

logical_and_expression
    : inclusive_or_expression
      {
          $$ = $1;
      }
    | logical_and_expression ANDAND
      {
          $<lctx>$ = new LogicalContext();
          $<lctx>$->lhs = gTranslator->ensureCondition($1);
          if ($<lctx>$->lhs != $1) delete $1;
          $<lctx>$->label = gTranslator->markLabel();
      }
      inclusive_or_expression
      {
          LogicalContext *ctx = $<lctx>3;
          ExprAttr *rhs = gTranslator->ensureCondition($4);
          gTranslator->backpatch(ctx->lhs->trueList, ctx->label);

          $$ = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          $$->isBool = true;
          $$->trueList = rhs->trueList;
          $$->falseList = gTranslator->merge(ctx->lhs->falseList, rhs->falseList);

          delete ctx->lhs;
          delete ctx;
          if (rhs != $4) delete $4;
          delete rhs;
      }
    ;

logical_or_expression
    : logical_and_expression
      {
          $$ = $1;
      }
    | logical_or_expression OROR
      {
          $<lctx>$ = new LogicalContext();
          $<lctx>$->lhs = gTranslator->ensureCondition($1);
          if ($<lctx>$->lhs != $1) delete $1;
          $<lctx>$->label = gTranslator->markLabel();
      }
      logical_and_expression
      {
          LogicalContext *ctx = $<lctx>3;
          ExprAttr *rhs = gTranslator->ensureCondition($4);
          gTranslator->backpatch(ctx->lhs->falseList, ctx->label);

          $$ = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          $$->isBool = true;
          $$->trueList = gTranslator->merge(ctx->lhs->trueList, rhs->trueList);
          $$->falseList = rhs->falseList;

          delete ctx->lhs;
          delete ctx;
          if (rhs != $4) delete $4;
          delete rhs;
      }
    ;

conditional_expression
    : logical_or_expression
      {
          $$ = $1;
      }
    | logical_or_expression '?' captured_expression ':' captured_expression
      {
          $$ = gTranslator->conditionalCaptured($1, $3, $5);
          delete $1;
          delete $3;
          delete $5;
      }
    ;

captured_expression
    : { gTranslator->beginCapture(); } expression
      {
          ExprAttr *e = $2;
          if (e && e->isBool) {
              ExprAttr *v = gTranslator->materializeBool(e);
              if (v != e) delete e;
              e = v;
          }

          CodeBuffer *code = gTranslator->endCapture();
          $$ = new CapturedExpr();
          $$->expr = e;
          $$->code = code;
      }
    ;

assignment_expression
    : conditional_expression
      {
          $$ = $1;
      }
    | unary_expression '=' assignment_expression
      {
          $$ = gTranslator->assign($1, $3);
          delete $1;
          delete $3;
      }
    ;

expression
    : assignment_expression { $$ = $1; }
    ;

expression_opt
    : %empty { $$ = NULL; }
    | expression { $$ = $1; }
    ;

initializer
    : assignment_expression { $$ = $1; }
    ;

marker
    : %empty
      {
          $$ = gTranslator->activeCode()->code.size();
          gTranslator->markLabel();
      }
    ;

condition_expression
    : expression
      {
          ExprAttr *c = gTranslator->ensureCondition($1);
          if (c != $1) delete $1;
          $$ = c;
      }
    ;

for_condition_opt
    : %empty
      {
          $$ = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          $$->isBool = true;
          std::size_t q = gTranslator->emit("goto", "", "", "?");
          $$->trueList = gTranslator->makelist(q);
      }
    | condition_expression
      {
          $$ = $1;
      }
    ;

statement
    : compound_statement { }
    | expression_statement { }
    | selection_statement { }
    | iteration_statement { }
    | jump_statement { }
    ;

compound_statement
    : '{' block_scope_begin block_item_list_opt '}' block_scope_end { }
    ;

block_scope_begin
    : %empty
      {
          gTranslator->enterScope("block" + std::to_string(gTranslator->activeCode()->code.size()));
          $$ = 0;
      }
    ;

block_scope_end
    : %empty
      {
          gTranslator->leaveScope();
          $$ = 0;
      }
    ;

block_item_list_opt
    : %empty { }
    | block_item_list { }
    ;

block_item_list
    : block_item { }
    | block_item_list block_item { }
    ;

block_item
    : declaration { }
    | statement { }
    ;

expression_statement
    : ';'
    | expression ';'
      {
          if ($1 && $1->isBool) {
              ExprAttr *v = gTranslator->materializeBool($1);
              if (v != $1) delete v;
          } else if ($1) {
              ExprAttr *r = gTranslator->ensureRValue($1);
              if (r != $1) delete r;
          }
          delete $1;
      }
    ;

selection_statement
    : IF '(' condition_expression ')' captured_statement %prec IFX
      {
          ExprAttr *c = $3;
          std::string thenLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->trueList, thenLabel);
          gTranslator->activeCode()->append($5->code);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);

          delete c;
          delete $5;
      }
    | IF '(' condition_expression ')' captured_statement ELSE captured_statement
      {
          ExprAttr *c = $3;
          std::string thenLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->trueList, thenLabel);
          gTranslator->activeCode()->append($5->code);
          std::size_t jumpEnd = gTranslator->emit("goto", "", "", "?");

          std::string elseLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, elseLabel);
          gTranslator->activeCode()->append($7->code);

          std::string endLabel = gTranslator->markLabel();
          std::vector<std::size_t> l = gTranslator->makelist(jumpEnd);
          gTranslator->backpatch(l, endLabel);

          delete c;
          delete $5;
          delete $7;
      }
    ;

captured_statement
    : { gTranslator->beginCapture(); } statement
      {
          CodeBuffer *code = gTranslator->endCapture();
          $$ = new CapturedStmt();
          $$->code = code;
      }
    ;

iteration_statement
    : WHILE '(' marker condition_expression ')' marker statement
      {
          ExprAttr *c = $4;
          std::string condLabel = gTranslator->activeCode()->code[$3].result;
          std::string bodyLabel = gTranslator->activeCode()->code[$6].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          gTranslator->emit("goto", "", "", condLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);
          delete c;
      }
    | DO marker statement WHILE '(' condition_expression ')' ';'
      {
          ExprAttr *c = $6;
          std::string bodyLabel = gTranslator->activeCode()->code[$2].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);
          delete c;
      }
    | FOR '(' expression_opt ';' marker for_condition_opt ';' for_update_clause ')' marker statement
      {
          ExprAttr *c = $6;
          std::string condLabel = gTranslator->activeCode()->code[$5].result;
          std::string bodyLabel = gTranslator->activeCode()->code[$10].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          gTranslator->activeCode()->append($8->code);
          gTranslator->emit("goto", "", "", condLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);

          if ($3) delete $3;
          delete c;
          delete $8;
      }
    ;

for_update_clause
    : { gTranslator->beginCapture(); } for_update_expression_opt
      {
          ExprAttr *e = $2;
          if (e && e->isBool) {
              ExprAttr *v = gTranslator->materializeBool(e);
              if (v != e) delete e;
              e = v;
          } else if (e) {
              ExprAttr *v = gTranslator->ensureRValue(e);
              if (v != e) delete v;
          }

          CodeBuffer *code = gTranslator->endCapture();
          $$ = new ForUpdateChunk();
          delete $$->code;
          $$->code = code;
          if (e) delete e;
      }
    ;

for_update_expression_opt
    : %empty { $$ = NULL; }
    | expression { $$ = $1; }
    ;

jump_statement
    : RETURN ';'
      {
          if (!gTranslator->currentReturnType ||
              gTranslator->currentReturnType->kind != tinyc::TK_VOID)
              throw std::runtime_error("return without value in non-void function");
          gTranslator->emit("return", "", "", "");
      }
    | RETURN expression ';'
      {
          if (!gTranslator->currentReturnType ||
              gTranslator->currentReturnType->kind == tinyc::TK_VOID)
              throw std::runtime_error("return value in void function");
          ExprAttr *original = $2;
          ExprAttr *e = gTranslator->ensureRValue(original);
          e = gTranslator->convert(e, gTranslator->currentReturnType);
          gTranslator->emit("return", e->place, "", "");
          if (e != original) delete e;
          delete original;
      }
    ;

%%

void yyerror(const char *s) {
    std::cerr << "tinyC3: " << s << " at line " << yylineno
              << " near '" << (yytext ? yytext : "") << "'\n";
}
