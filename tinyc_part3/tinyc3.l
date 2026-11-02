%option noyywrap yylineno nodefault nounput noinput
%{
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include "tinyc3.tab.h"

#define RETURN_TOKEN(tok) do { return (tok); } while (0)

static char* dup_text(const char* s) {
    const std::size_t n = std::strlen(s);
    char* p = static_cast<char*>(std::malloc(n + 1));
    if (!p) std::abort();
    std::memcpy(p, s, n + 1);
    return p;
}
%}

DIGIT   [0-9]
LETTER  [A-Za-z]
ID      {LETTER}({LETTER}|{DIGIT})*
ESCAPE       \\([\'\"?\\abfnrtv])
CHAR_CONST   \'([^\\\'\n]|{ESCAPE})\'
STRING_CONST \"([^\\\"\n]|{ESCAPE})*\"

%%

[ \t\r\n]+                         ;
[/][*]([^*]|[*]+[^*/])*[*]+[/]     ;
[/][/][^\n]*                       ;

"void" { RETURN_TOKEN(VOID); }
"char" { RETURN_TOKEN(CHAR); }
"int" { RETURN_TOKEN(INT); }
"float" { RETURN_TOKEN(FLOAT); }
"if" { RETURN_TOKEN(IF); }
"else" { RETURN_TOKEN(ELSE); }
"while" { RETURN_TOKEN(WHILE); }
"do" { RETURN_TOKEN(DO); }
"for" { RETURN_TOKEN(FOR); }
"return" { RETURN_TOKEN(RETURN); }

"++" { RETURN_TOKEN(INC); }
"--" { RETURN_TOKEN(DEC); }
"<<" { RETURN_TOKEN(SHL); }
">>" { RETURN_TOKEN(SHR); }
"<=" { RETURN_TOKEN(LE); }
">=" { RETURN_TOKEN(GE); }
"==" { RETURN_TOKEN(EQ); }
"!=" { RETURN_TOKEN(NE); }
"&&" { RETURN_TOKEN(ANDAND); }
"||" { RETURN_TOKEN(OROR); }

{ID} { yylval.text = dup_text(yytext); return IDENTIFIER; }

{CHAR_CONST} { yylval.text = dup_text(yytext); return CONSTANT; }

(({DIGIT}+\.{DIGIT}*|{DIGIT}*\.{DIGIT}+)([eE][+-]?{DIGIT}+)?|{DIGIT}+[eE][+-]?{DIGIT}+) { yylval.text = dup_text(yytext); return CONSTANT; }

{DIGIT}+ { yylval.text = dup_text(yytext); return CONSTANT; }

{STRING_CONST} { yylval.text = dup_text(yytext); return STRING_LITERAL; }

. { return static_cast<unsigned char>(yytext[0]); }
