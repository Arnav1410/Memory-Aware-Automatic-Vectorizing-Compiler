%code requires {
#include "ast.hpp"
}

%{
#include <cstdlib>
#include <cstdio>
#include "ast.hpp"

ASTNode* tinyc2_root = NULL;
int yylex(void);
void yyerror(const char* s);
%}

%define parse.error verbose
%expect 1

/* In a parameter-declaration, prefer the declarator form when an IDENTIFIER follows
   declaration-specifiers. This resolves the otherwise inherent int-vs-int name
   ambiguity deterministically for this teaching subset. */
%precedence PARAM_NO_DECL
%precedence IDENTIFIER

%union {
    char* text;
    ASTNode* node;
}

%token <text> IDENTIFIER CONSTANT STRING_LITERAL
%token CASE AUTO ENUM RESTRICT UNSIGNED BREAK EXTERN RETURN VOID FLOAT SHORT VOLATILE CHAR FOR SIGNED WHILE CONST GOTO SIZEOF BOOL CONTINUE IF STATIC COMPLEX DEFAULT INLINE STRUCT IMAGINARY DO INT SWITCH DOUBLE LONG TYPEDEF ELSE REGISTER UNION
%token ARROW INC DEC SHL SHR LE GE EQ NE ANDAND OROR ELLIPSIS
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN SHL_ASSIGN SHR_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN

%type <node> translation_unit external_declaration function_definition
%type <node> declaration declaration_specifiers init_declarator_list_opt init_declarator_list init_declarator
%type <node> storage_class_specifier type_specifier specifier_qualifier_list type_qualifier function_specifier
%type <node> declarator direct_declarator pointer type_qualifier_list_opt type_qualifier_list
%type <node> parameter_type_list parameter_list parameter_declaration identifier_list_opt identifier_list
%type <node> type_name initializer initializer_list designation designator_list designator
%type <node> unary_operator primary_expression postfix_expression argument_expression_list_opt argument_expression_list
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression assignment_operator expression constant_expression
%type <node> statement matched_statement unmatched_statement non_if_statement if_matched if_unmatched switch_selection
%type <node> labeled_statement compound_statement block_item_list_opt block_item_list block_item
%type <node> expression_statement selection_statement iteration_statement jump_statement declaration_list_opt declaration_list
%type <node> assignment_expression_opt expression_opt


%%

translation_unit
    : external_declaration
        { $$ = ast_node("translation-unit"); ast_add($$, $1); tinyc2_root = $$; }
    | translation_unit external_declaration
        { $$ = $1; ast_add($$, $2); tinyc2_root = $$; }
    ;

external_declaration
    : function_definition
        { $$ = ast_node("external-declaration"); ast_add($$, $1); }
    | declaration
        { $$ = ast_node("external-declaration"); ast_add($$, $1); }
    ;

function_definition
    : declaration_specifiers declarator declaration_list_opt compound_statement
        { $$ = ast_node("function-definition"); ast_add($$, $1); ast_add($$, $2); ast_add($$, $3); ast_add($$, $4); }
    ;

declaration_list_opt
    : %empty { $$ = ast_node("declaration-list-opt: epsilon"); }
    | declaration_list { $$ = $1; }
    ;

declaration_list
    : declaration { $$ = ast_node("declaration-list"); ast_add($$, $1); }
    | declaration_list declaration { $$ = $1; ast_add($$, $2); }
    ;

primary_expression
    : IDENTIFIER { $$ = ast_leaf("identifier", $1); std::free($1); }
    | CONSTANT { $$ = ast_leaf("constant", $1); std::free($1); }
    | STRING_LITERAL { $$ = ast_leaf("string-literal", $1); std::free($1); }
    | '(' expression ')' { $$ = ast_node("primary-expression"); ast_add($$, $2); }
    ;

postfix_expression
    : primary_expression { $$ = ast_node("postfix-expression"); ast_add($$, $1); }
    | postfix_expression '[' expression ']'
        { $$ = $1; ASTNode* n = ast_node("index"); ast_add(n, $3); ast_add($$, n); }
    | postfix_expression '(' argument_expression_list_opt ')'
        { $$ = $1; ASTNode* n = ast_node("call"); ast_add(n, $3); ast_add($$, n); }
    | postfix_expression '.' IDENTIFIER
        { $$ = $1; ASTNode* n = ast_leaf("member", $3); std::free($3); ast_add($$, n); }
    | postfix_expression ARROW IDENTIFIER
        { $$ = $1; ASTNode* n = ast_leaf("ptr-member", $3); std::free($3); ast_add($$, n); }
    | postfix_expression INC
        { $$ = $1; ast_add($$, ast_node("postfix++")); }
    | postfix_expression DEC
        { $$ = $1; ast_add($$, ast_node("postfix--")); }
    | '(' type_name ')' '{' initializer_list '}'
        { $$ = ast_node("compound-literal"); ast_add($$, $2); ast_add($$, $5); }
    | '(' type_name ')' '{' initializer_list ',' '}'
        { $$ = ast_node("compound-literal"); ast_add($$, $2); ast_add($$, $5); }
    ;

argument_expression_list_opt
    : %empty { $$ = ast_node("argument-expression-list-opt: epsilon"); }
    | argument_expression_list { $$ = $1; }
    ;

argument_expression_list
    : assignment_expression { $$ = ast_node("argument-expression-list"); ast_add($$, $1); }
    | argument_expression_list ',' assignment_expression { $$ = $1; ast_add($$, $3); }
    ;

unary_expression
    : postfix_expression { $$ = ast_node("unary-expression"); ast_add($$, $1); }
    | INC unary_expression { $$ = ast_node("pre++"); ast_add($$, $2); }
    | DEC unary_expression { $$ = ast_node("pre--"); ast_add($$, $2); }
    | unary_operator cast_expression { $$ = ast_node("unary-expression"); ast_add($$, $1); ast_add($$, $2); }
    | SIZEOF unary_expression { $$ = ast_node("sizeof"); ast_add($$, $2); }
    | SIZEOF '(' type_name ')' { $$ = ast_node("sizeof-type"); ast_add($$, $3); }
    ;

unary_operator
    : '&' { $$ = ast_node("&"); }
    | '*' { $$ = ast_node("*"); }
    | '+' { $$ = ast_node("+"); }
    | '-' { $$ = ast_node("-"); }
    | '~' { $$ = ast_node("~"); }
    | '!' { $$ = ast_node("!"); }
    ;

cast_expression
    : unary_expression { $$ = ast_node("cast-expression"); ast_add($$, $1); }
    | '(' type_name ')' cast_expression { $$ = ast_node("cast"); ast_add($$, $2); ast_add($$, $4); }
    ;

multiplicative_expression
    : cast_expression { $$ = ast_node("multiplicative-expression"); ast_add($$, $1); }
    | multiplicative_expression '*' cast_expression { $$ = $1; ASTNode* n=ast_node("*"); ast_add(n,$3); ast_add($$,n); }
    | multiplicative_expression '/' cast_expression { $$ = $1; ASTNode* n=ast_node("/"); ast_add(n,$3); ast_add($$,n); }
    | multiplicative_expression '%' cast_expression { $$ = $1; ASTNode* n=ast_node("%"); ast_add(n,$3); ast_add($$,n); }
    ;

additive_expression
    : multiplicative_expression { $$ = ast_node("additive-expression"); ast_add($$, $1); }
    | additive_expression '+' multiplicative_expression { $$ = $1; ASTNode* n=ast_node("+"); ast_add(n,$3); ast_add($$,n); }
    | additive_expression '-' multiplicative_expression { $$ = $1; ASTNode* n=ast_node("-"); ast_add(n,$3); ast_add($$,n); }
    ;

shift_expression
    : additive_expression { $$ = ast_node("shift-expression"); ast_add($$, $1); }
    | shift_expression SHL additive_expression { $$ = $1; ASTNode* n=ast_node("<<"); ast_add(n,$3); ast_add($$,n); }
    | shift_expression SHR additive_expression { $$ = $1; ASTNode* n=ast_node(">>"); ast_add(n,$3); ast_add($$,n); }
    ;

relational_expression
    : shift_expression { $$ = ast_node("relational-expression"); ast_add($$, $1); }
    | relational_expression '<' shift_expression { $$ = $1; ASTNode* n=ast_node("<"); ast_add(n,$3); ast_add($$,n); }
    | relational_expression '>' shift_expression { $$ = $1; ASTNode* n=ast_node(">"); ast_add(n,$3); ast_add($$,n); }
    | relational_expression LE shift_expression { $$ = $1; ASTNode* n=ast_node("<="); ast_add(n,$3); ast_add($$,n); }
    | relational_expression GE shift_expression { $$ = $1; ASTNode* n=ast_node(">="); ast_add(n,$3); ast_add($$,n); }
    ;

equality_expression
    : relational_expression { $$ = ast_node("equality-expression"); ast_add($$, $1); }
    | equality_expression EQ relational_expression { $$ = $1; ASTNode* n=ast_node("=="); ast_add(n,$3); ast_add($$,n); }
    | equality_expression NE relational_expression { $$ = $1; ASTNode* n=ast_node("!="); ast_add(n,$3); ast_add($$,n); }
    ;

and_expression
    : equality_expression { $$ = ast_node("AND-expression"); ast_add($$, $1); }
    | and_expression '&' equality_expression { $$ = $1; ASTNode* n=ast_node("&"); ast_add(n,$3); ast_add($$,n); }
    ;

exclusive_or_expression
    : and_expression { $$ = ast_node("exclusive-OR-expression"); ast_add($$, $1); }
    | exclusive_or_expression '^' and_expression { $$ = $1; ASTNode* n=ast_node("^"); ast_add(n,$3); ast_add($$,n); }
    ;

inclusive_or_expression
    : exclusive_or_expression { $$ = ast_node("inclusive-OR-expression"); ast_add($$, $1); }
    | inclusive_or_expression '|' exclusive_or_expression { $$ = $1; ASTNode* n=ast_node("|"); ast_add(n,$3); ast_add($$,n); }
    ;

logical_and_expression
    : inclusive_or_expression { $$ = ast_node("logical-AND-expression"); ast_add($$, $1); }
    | logical_and_expression ANDAND inclusive_or_expression { $$ = $1; ASTNode* n=ast_node("&&"); ast_add(n,$3); ast_add($$,n); }
    ;

logical_or_expression
    : logical_and_expression { $$ = ast_node("logical-OR-expression"); ast_add($$, $1); }
    | logical_or_expression OROR logical_and_expression { $$ = $1; ASTNode* n=ast_node("||"); ast_add(n,$3); ast_add($$,n); }
    ;

conditional_expression
    : logical_or_expression { $$ = ast_node("conditional-expression"); ast_add($$, $1); }
    | logical_or_expression '?' expression ':' conditional_expression
        { $$ = ast_node("conditional-expression"); ast_add($$, $1); ast_add($$, $3); ast_add($$, $5); }
    ;

assignment_expression
    : conditional_expression { $$ = ast_node("assignment-expression"); ast_add($$, $1); }
    | unary_expression assignment_operator assignment_expression
        { $$ = ast_node("assignment-expression"); ast_add($$, $1); ast_add($$, $2); ast_add($$, $3); }
    ;

assignment_operator
    : '=' { $$ = ast_node("="); }
    | MUL_ASSIGN { $$ = ast_node("*="); }
    | DIV_ASSIGN { $$ = ast_node("/="); }
    | MOD_ASSIGN { $$ = ast_node("%="); }
    | ADD_ASSIGN { $$ = ast_node("+="); }
    | SUB_ASSIGN { $$ = ast_node("-="); }
    | SHL_ASSIGN { $$ = ast_node("<<="); }
    | SHR_ASSIGN { $$ = ast_node(">>="); }
    | AND_ASSIGN { $$ = ast_node("&="); }
    | XOR_ASSIGN { $$ = ast_node("^="); }
    | OR_ASSIGN { $$ = ast_node("|="); }
    ;

expression
    : assignment_expression { $$ = ast_node("expression"); ast_add($$, $1); }
    | expression ',' assignment_expression { $$ = $1; ast_add($$, $3); }
    ;

expression_opt
    : %empty { $$ = ast_node("expression-opt: epsilon"); }
    | expression { $$ = $1; }
    ;

constant_expression
    : conditional_expression { $$ = ast_node("constant-expression"); ast_add($$, $1); }
    ;

assignment_expression_opt
    : %empty { $$ = ast_node("assignment-expression-opt: epsilon"); }
    | assignment_expression { $$ = $1; }
    ;

declaration
    : declaration_specifiers init_declarator_list_opt ';'
        { $$ = ast_node("declaration"); ast_add($$, $1); ast_add($$, $2); }
    ;

init_declarator_list_opt
    : %empty { $$ = ast_node("init-declarator-list-opt: epsilon"); }
    | init_declarator_list { $$ = $1; }
    ;

init_declarator_list
    : init_declarator { $$ = ast_node("init-declarator-list"); ast_add($$, $1); }
    | init_declarator_list ',' init_declarator { $$ = $1; ast_add($$, $3); }
    ;

init_declarator
    : declarator { $$ = ast_node("init-declarator"); ast_add($$, $1); }
    | declarator '=' initializer { $$ = ast_node("init-declarator"); ast_add($$, $1); ast_add($$, $3); }
    ;

storage_class_specifier
    : TYPEDEF { $$ = ast_node("typedef"); }
    | EXTERN { $$ = ast_node("extern"); }
    | STATIC { $$ = ast_node("static"); }
    | AUTO { $$ = ast_node("auto"); }
    | REGISTER { $$ = ast_node("register"); }
    ;

declaration_specifiers
    : storage_class_specifier { $$ = ast_node("declaration-specifiers"); ast_add($$,$1); }
    | type_specifier { $$ = ast_node("declaration-specifiers"); ast_add($$,$1); }
    | type_qualifier { $$ = ast_node("declaration-specifiers"); ast_add($$,$1); }
    | function_specifier { $$ = ast_node("declaration-specifiers"); ast_add($$,$1); }
    | declaration_specifiers storage_class_specifier { $$=$1; ast_add($$,$2); }
    | declaration_specifiers type_specifier { $$=$1; ast_add($$,$2); }
    | declaration_specifiers type_qualifier { $$=$1; ast_add($$,$2); }
    | declaration_specifiers function_specifier { $$=$1; ast_add($$,$2); }
    ;

type_specifier
    : VOID { $$=ast_node("void"); }
    | CHAR { $$=ast_node("char"); }
    | SHORT { $$=ast_node("short"); }
    | INT { $$=ast_node("int"); }
    | LONG { $$=ast_node("long"); }
    | FLOAT { $$=ast_node("float"); }
    | DOUBLE { $$=ast_node("double"); }
    | SIGNED { $$=ast_node("signed"); }
    | UNSIGNED { $$=ast_node("unsigned"); }
    | BOOL { $$=ast_node("Bool"); }
    | COMPLEX { $$=ast_node("Complex"); }
    | IMAGINARY { $$=ast_node("Imaginary"); }
    | STRUCT { $$=ast_node("struct"); }
    | ENUM { $$=ast_node("enum"); }
    ;

specifier_qualifier_list
    : type_specifier { $$=ast_node("specifier-qualifier-list"); ast_add($$,$1); }
    | type_qualifier { $$=ast_node("specifier-qualifier-list"); ast_add($$,$1); }
    | specifier_qualifier_list type_specifier { $$=$1; ast_add($$,$2); }
    | specifier_qualifier_list type_qualifier { $$=$1; ast_add($$,$2); }
    ;

type_qualifier
    : CONST { $$=ast_node("const"); }
    | RESTRICT { $$=ast_node("restrict"); }
    | VOLATILE { $$=ast_node("volatile"); }
    ;

function_specifier
    : INLINE { $$=ast_node("inline"); }
    ;

declarator
    : pointer direct_declarator { $$=ast_node("declarator"); ast_add($$,$1); ast_add($$,$2); }
    | direct_declarator { $$=ast_node("declarator"); ast_add($$,$1); }
    ;

direct_declarator
    : IDENTIFIER { $$=ast_leaf("identifier",$1); std::free($1); }
    | '(' declarator ')' { $$=ast_node("direct-declarator"); ast_add($$,$2); }
    | direct_declarator '[' type_qualifier_list_opt assignment_expression_opt ']'
        { $$=$1; ASTNode* n=ast_node("array[]"); ast_add(n,$3); ast_add(n,$4); ast_add($$,n); }
    | direct_declarator '[' STATIC type_qualifier_list_opt assignment_expression ']'
        { $$=$1; ASTNode* n=ast_node("array[static]"); ast_add(n,$4); ast_add(n,$5); ast_add($$,n); }
    | direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
        { $$=$1; ASTNode* n=ast_node("array[static]"); ast_add(n,$3); ast_add(n,$5); ast_add($$,n); }
    | direct_declarator '[' type_qualifier_list_opt '*' ']'
        { $$=$1; ast_add($$,ast_node("array[*]")); }
    | direct_declarator '(' parameter_type_list ')'
        { $$=$1; ASTNode* n=ast_node("function-declarator"); ast_add(n,$3); ast_add($$,n); }
    | direct_declarator '(' identifier_list_opt ')'
        { $$=$1; ASTNode* n=ast_node("function-declarator"); ast_add(n,$3); ast_add($$,n); }
    ;

type_qualifier_list_opt
    : %empty { $$=ast_node("type-qualifier-list-opt: epsilon"); }
    | type_qualifier_list { $$=$1; }
    ;

type_qualifier_list
    : type_qualifier { $$=ast_node("type-qualifier-list"); ast_add($$,$1); }
    | type_qualifier_list type_qualifier { $$=$1; ast_add($$,$2); }
    ;

pointer
    : '*' type_qualifier_list_opt { $$=ast_node("pointer"); ast_add($$,$2); }
    | '*' type_qualifier_list_opt pointer { $$=$3; ast_add($$,$2); }
    ;

parameter_type_list
    : parameter_list { $$=ast_node("parameter-type-list"); ast_add($$,$1); }
    | parameter_list ',' ELLIPSIS { $$=$1; ast_add($$,ast_node("...")); }
    ;

parameter_list
    : parameter_declaration { $$=ast_node("parameter-list"); ast_add($$,$1); }
    | parameter_list ',' parameter_declaration { $$=$1; ast_add($$,$3); }
    ;

parameter_declaration
    : declaration_specifiers declarator { $$=ast_node("parameter-declaration"); ast_add($$,$1); ast_add($$,$2); }
    | declaration_specifiers %prec PARAM_NO_DECL { $$=ast_node("parameter-declaration"); ast_add($$,$1); }
    ;

identifier_list_opt
    : %empty { $$=ast_node("identifier-list-opt: epsilon"); }
    | identifier_list { $$=$1; }
    ;

identifier_list
    : IDENTIFIER { $$=ast_node("identifier-list"); ast_add($$,ast_leaf("identifier",$1)); std::free($1); }
    | identifier_list ',' IDENTIFIER { $$=$1; ast_add($$,ast_leaf("identifier",$3)); std::free($3); }
    ;

type_name
    : specifier_qualifier_list { $$=ast_node("type-name"); ast_add($$,$1); }
    ;

initializer
    : assignment_expression { $$=ast_node("initializer"); ast_add($$,$1); }
    | '{' initializer_list '}' { $$=ast_node("initializer"); ast_add($$,$2); }
    | '{' initializer_list ',' '}' { $$=ast_node("initializer"); ast_add($$,$2); }
    ;

initializer_list
    : designation initializer { $$=ast_node("initializer-list"); ast_add($$,$1); ast_add($$,$2); }
    | initializer_list ',' designation initializer { $$=$1; ast_add($$,$3); ast_add($$,$4); }
    | initializer { $$=ast_node("initializer-list"); ast_add($$,$1); }
    | initializer_list ',' initializer { $$=$1; ast_add($$,$3); }
    ;

designation
    : designator_list '=' { $$=ast_node("designation"); ast_add($$,$1); }
    ;

designator_list
    : designator { $$=ast_node("designator-list"); ast_add($$,$1); }
    | designator_list designator { $$=$1; ast_add($$,$2); }
    ;

designator
    : '[' constant_expression ']' { $$=ast_node("designator[]"); ast_add($$,$2); }
    | '.' IDENTIFIER { $$=ast_leaf("designator.member",$2); std::free($2); }
    ;

statement
    : matched_statement { $$ = $1; }
    | unmatched_statement { $$ = $1; }
    ;

matched_statement
    : non_if_statement { $$ = $1; }
    | if_matched { $$ = $1; }
    ;

unmatched_statement
    : if_unmatched { $$ = $1; }
    ;

non_if_statement
    : labeled_statement { $$ = $1; }
    | compound_statement { $$ = $1; }
    | expression_statement { $$ = $1; }
    | selection_statement { $$ = $1; }
    | iteration_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    ;

labeled_statement
    : IDENTIFIER ':' statement { $$=ast_node("label"); ast_add($$,ast_leaf("identifier",$1)); std::free($1); ast_add($$,$3); }
    | CASE constant_expression ':' statement { $$=ast_node("case"); ast_add($$,$2); ast_add($$,$4); }
    | DEFAULT ':' statement { $$=ast_node("default"); ast_add($$,$3); }
    ;

compound_statement
    : '{' block_item_list_opt '}' { $$=ast_node("compound-statement"); ast_add($$,$2); }
    ;

block_item_list_opt
    : %empty { $$=ast_node("block-item-list-opt: epsilon"); }
    | block_item_list { $$=$1; }
    ;

block_item_list
    : block_item { $$=ast_node("block-item-list"); ast_add($$,$1); }
    | block_item_list block_item { $$=$1; ast_add($$,$2); }
    ;

block_item
    : declaration { $$=ast_node("block-item"); ast_add($$,$1); }
    | statement { $$=ast_node("block-item"); ast_add($$,$1); }
    ;

expression_statement
    : ';' { $$=ast_node("expression-statement: empty"); }
    | expression ';' { $$=ast_node("expression-statement"); ast_add($$,$1); }
    ;

if_matched
    : IF '(' expression ')' matched_statement ELSE matched_statement
        { $$=ast_node("if-else"); ast_add($$,$3); ast_add($$,$5); ast_add($$,$7); }
    ;

if_unmatched
    : IF '(' expression ')' statement
        { $$=ast_node("if"); ast_add($$,$3); ast_add($$,$5); }
    | IF '(' expression ')' matched_statement ELSE unmatched_statement
        { $$=ast_node("if-else"); ast_add($$,$3); ast_add($$,$5); ast_add($$,$7); }
    ;

selection_statement
    : switch_selection { $$ = $1; }
    ;

switch_selection
    : SWITCH '(' expression ')' statement { $$=ast_node("switch"); ast_add($$,$3); ast_add($$,$5); }
    ;

iteration_statement
    : WHILE '(' expression ')' statement { $$=ast_node("while"); ast_add($$,$3); ast_add($$,$5); }
    | DO statement WHILE '(' expression ')' ';' { $$=ast_node("do-while"); ast_add($$,$2); ast_add($$,$5); }
    | FOR '(' expression_opt ';' expression_opt ';' expression_opt ')' statement { $$=ast_node("for"); ast_add($$,$3); ast_add($$,$5); ast_add($$,$7); ast_add($$,$9); }
    | FOR '(' declaration expression_opt ';' expression_opt ')' statement { $$=ast_node("for-declaration"); ast_add($$,$3); ast_add($$,$4); ast_add($$,$6); ast_add($$,$8); }
    ;

jump_statement
    : GOTO IDENTIFIER ';' { $$=ast_node("goto"); ast_add($$,ast_leaf("identifier",$2)); std::free($2); }
    | CONTINUE ';' { $$=ast_node("continue"); }
    | BREAK ';' { $$=ast_node("break"); }
    | RETURN ';' { $$=ast_node("return"); }
    | RETURN expression ';' { $$=ast_node("return"); ast_add($$,$2); }
    ;

%%

void yyerror(const char* s) {
    std::fprintf(stderr, "tinyC2: %s\n", s);
}
