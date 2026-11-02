%option noyywrap yylineno nodefault nounput noinput
%{
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include "tinyc2.tab.h"

static char *dup_text(const char *s) {
    std::size_t n = std::strlen(s);
    char *p = (char*)std::malloc(n + 1);
    if (!p) std::abort();
    std::memcpy(p, s, n + 1);
    return p;
}
%}

DIGIT [0-9]
LETTER [A-Za-z]
ID {LETTER}({LETTER}|{DIGIT})*
ESCAPE \\([\'\"?\\abfnrtv])
CHAR_CONST \'([^\\\'\n]|{ESCAPE})\'
STRING_CONST \"([^\\\"\n]|{ESCAPE})*\"

%%

[ \t\r\n]+ ;
[/][*]([^*]|[*]+[^*/])*[*]+[/] ;
[/][/][^\n]* ;

"auto" { return AUTO; }
"enum" { return ENUM; }
"restrict" { return RESTRICT; }
"unsigned" { return UNSIGNED; }
"break" { return BREAK; }
"extern" { return EXTERN; }
"return" { return RETURN; }
"void" { return VOID; }
"float" { return FLOAT; }
"short" { return SHORT; }
"volatile" { return VOLATILE; }
"char" { return CHAR; }
"case" { return CASE; }
"for" { return FOR; }
"signed" { return SIGNED; }
"while" { return WHILE; }
"const" { return CONST; }
"goto" { return GOTO; }
"sizeof" { return SIZEOF; }
"Bool" { return BOOL; }
"continue" { return CONTINUE; }
"if" { return IF; }
"static" { return STATIC; }
"Complex" { return COMPLEX; }
"default" { return DEFAULT; }
"inline" { return INLINE; }
"struct" { return STRUCT; }
"Imaginary" { return IMAGINARY; }
"do" { return DO; }
"int" { return INT; }
"switch" { return SWITCH; }
"double" { return DOUBLE; }
"long" { return LONG; }
"typedef" { return TYPEDEF; }
"else" { return ELSE; }
"register" { return REGISTER; }
"union" { return UNION; }

"->" { return ARROW; }
"++" { return INC; }
"--" { return DEC; }
"<<" { return SHL; }
">>" { return SHR; }
"<=" { return LE; }
">=" { return GE; }
"==" { return EQ; }
"!=" { return NE; }
"&&" { return ANDAND; }
"||" { return OROR; }
"..." { return ELLIPSIS; }
"*=" { return MUL_ASSIGN; }
"/=" { return DIV_ASSIGN; }
"%=" { return MOD_ASSIGN; }
"+=" { return ADD_ASSIGN; }
"-=" { return SUB_ASSIGN; }
"<<=" { return SHL_ASSIGN; }
">>=" { return SHR_ASSIGN; }
"&=" { return AND_ASSIGN; }
"^=" { return XOR_ASSIGN; }
"|=" { return OR_ASSIGN; }

{ID} { yylval.text = dup_text(yytext); return IDENTIFIER; }
{CHAR_CONST} { yylval.text = dup_text(yytext); return CONSTANT; }
(({DIGIT}+\.{DIGIT}*|{DIGIT}*\.{DIGIT}+)([eE][+-]?{DIGIT}+)?|{DIGIT}+[eE][+-]?{DIGIT}+) { yylval.text = dup_text(yytext); return CONSTANT; }
{DIGIT}+ { yylval.text = dup_text(yytext); return CONSTANT; }
{STRING_CONST} { yylval.text = dup_text(yytext); return STRING_LITERAL; }

. { return (unsigned char)yytext[0]; }
