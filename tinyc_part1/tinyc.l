%option noyywrap yylineno nodefault nounput noinput
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

static const char *current_kind = "unknown";
const char *tinyc1_current_token_kind(void) { return current_kind; }

#define RETURN_KIND(kind) do { current_kind = (kind); return 1; } while (0)
%}

DIGIT      [0-9]
LETTER     [A-Za-z]
ID         {LETTER}({LETTER}|{DIGIT})*
ESCAPE       \\([\'\"?\\abfnrtv])
CHAR_CONST   \'([^\\\'\n]|{ESCAPE})\'
STRING_CONST \"([^\\\"\n]|{ESCAPE})*\"

%%

[ \t\r\n]+                         ;
[/][*]([^*]|[*]+[^*/])*[*]+[/]     ;
[/][/][^\n]*                       ;

"auto"              { RETURN_KIND("keyword"); }
"enum"              { RETURN_KIND("keyword"); }
"restrict"          { RETURN_KIND("keyword"); }
"unsigned"          { RETURN_KIND("keyword"); }
"break"             { RETURN_KIND("keyword"); }
"extern"            { RETURN_KIND("keyword"); }
"return"            { RETURN_KIND("keyword"); }
"void"              { RETURN_KIND("keyword"); }
"float"             { RETURN_KIND("keyword"); }
"short"             { RETURN_KIND("keyword"); }
"volatile"          { RETURN_KIND("keyword"); }
"char"              { RETURN_KIND("keyword"); }
"case"              { RETURN_KIND("keyword"); }
"for"               { RETURN_KIND("keyword"); }
"signed"            { RETURN_KIND("keyword"); }
"while"             { RETURN_KIND("keyword"); }
"const"             { RETURN_KIND("keyword"); }
"goto"              { RETURN_KIND("keyword"); }
"sizeof"            { RETURN_KIND("keyword"); }
"Bool"              { RETURN_KIND("keyword"); }
"continue"          { RETURN_KIND("keyword"); }
"if"                { RETURN_KIND("keyword"); }
"static"            { RETURN_KIND("keyword"); }
"Complex"           { RETURN_KIND("keyword"); }
"default"           { RETURN_KIND("keyword"); }
"inline"            { RETURN_KIND("keyword"); }
"struct"            { RETURN_KIND("keyword"); }
"Imaginary"         { RETURN_KIND("keyword"); }
"do"                { RETURN_KIND("keyword"); }
"int"               { RETURN_KIND("keyword"); }
"switch"            { RETURN_KIND("keyword"); }
"double"            { RETURN_KIND("keyword"); }
"long"              { RETURN_KIND("keyword"); }
"typedef"           { RETURN_KIND("keyword"); }
"else"              { RETURN_KIND("keyword"); }
"register"          { RETURN_KIND("keyword"); }
"union"             { RETURN_KIND("keyword"); }

"->"                { RETURN_KIND("punctuator"); }
"++"                { RETURN_KIND("punctuator"); }
"--"                { RETURN_KIND("punctuator"); }
"<<"                { RETURN_KIND("punctuator"); }
">>"                { RETURN_KIND("punctuator"); }
"<="                { RETURN_KIND("punctuator"); }
">="                { RETURN_KIND("punctuator"); }
"=="                { RETURN_KIND("punctuator"); }
"!="                { RETURN_KIND("punctuator"); }
"&&"                { RETURN_KIND("punctuator"); }
"||"                { RETURN_KIND("punctuator"); }
"..."               { RETURN_KIND("punctuator"); }
"*="                { RETURN_KIND("punctuator"); }
"/="                { RETURN_KIND("punctuator"); }
"%="                { RETURN_KIND("punctuator"); }
"+="                { RETURN_KIND("punctuator"); }
"-="                { RETURN_KIND("punctuator"); }
"<<="               { RETURN_KIND("punctuator"); }
">>="               { RETURN_KIND("punctuator"); }
"&="                { RETURN_KIND("punctuator"); }
"^="                { RETURN_KIND("punctuator"); }
"|="                { RETURN_KIND("punctuator"); }

{ID}                { RETURN_KIND("identifier"); }

{CHAR_CONST}      { RETURN_KIND("constant"); }

(({DIGIT}+\.{DIGIT}*|{DIGIT}*\.{DIGIT}+)([eE][+-]?{DIGIT}+)?|{DIGIT}+[eE][+-]?{DIGIT}+)  { RETURN_KIND("constant"); }

{DIGIT}+            { RETURN_KIND("constant"); }

{STRING_CONST}      { RETURN_KIND("string-literal"); }

"["|"]"|"("|")"|"{"|"}"|"."|"&"|"*"|"+"|"-"|"~"|"!"|"/"|"%"|"<"|">"|"^"|"|"|"?"|":"|";"|"="|","|"#" { RETURN_KIND("punctuator"); }

.                   {
                        fprintf(stderr, "tinyC1: unexpected character '%s' on line %d\n", yytext, yylineno);
                        RETURN_KIND("invalid");
                    }
