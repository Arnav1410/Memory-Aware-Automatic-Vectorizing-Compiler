/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_TINYC2_TAB_H_INCLUDED
# define YY_YY_TINYC2_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "tinyc2.y"

#include "ast.hpp"

#line 53 "tinyc2.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PARAM_NO_DECL = 258,           /* PARAM_NO_DECL  */
    IDENTIFIER = 259,              /* IDENTIFIER  */
    CONSTANT = 260,                /* CONSTANT  */
    STRING_LITERAL = 261,          /* STRING_LITERAL  */
    CASE = 262,                    /* CASE  */
    AUTO = 263,                    /* AUTO  */
    ENUM = 264,                    /* ENUM  */
    RESTRICT = 265,                /* RESTRICT  */
    UNSIGNED = 266,                /* UNSIGNED  */
    BREAK = 267,                   /* BREAK  */
    EXTERN = 268,                  /* EXTERN  */
    RETURN = 269,                  /* RETURN  */
    VOID = 270,                    /* VOID  */
    FLOAT = 271,                   /* FLOAT  */
    SHORT = 272,                   /* SHORT  */
    VOLATILE = 273,                /* VOLATILE  */
    CHAR = 274,                    /* CHAR  */
    FOR = 275,                     /* FOR  */
    SIGNED = 276,                  /* SIGNED  */
    WHILE = 277,                   /* WHILE  */
    CONST = 278,                   /* CONST  */
    GOTO = 279,                    /* GOTO  */
    SIZEOF = 280,                  /* SIZEOF  */
    BOOL = 281,                    /* BOOL  */
    CONTINUE = 282,                /* CONTINUE  */
    IF = 283,                      /* IF  */
    STATIC = 284,                  /* STATIC  */
    COMPLEX = 285,                 /* COMPLEX  */
    DEFAULT = 286,                 /* DEFAULT  */
    INLINE = 287,                  /* INLINE  */
    STRUCT = 288,                  /* STRUCT  */
    IMAGINARY = 289,               /* IMAGINARY  */
    DO = 290,                      /* DO  */
    INT = 291,                     /* INT  */
    SWITCH = 292,                  /* SWITCH  */
    DOUBLE = 293,                  /* DOUBLE  */
    LONG = 294,                    /* LONG  */
    TYPEDEF = 295,                 /* TYPEDEF  */
    ELSE = 296,                    /* ELSE  */
    REGISTER = 297,                /* REGISTER  */
    UNION = 298,                   /* UNION  */
    ARROW = 299,                   /* ARROW  */
    INC = 300,                     /* INC  */
    DEC = 301,                     /* DEC  */
    SHL = 302,                     /* SHL  */
    SHR = 303,                     /* SHR  */
    LE = 304,                      /* LE  */
    GE = 305,                      /* GE  */
    EQ = 306,                      /* EQ  */
    NE = 307,                      /* NE  */
    ANDAND = 308,                  /* ANDAND  */
    OROR = 309,                    /* OROR  */
    ELLIPSIS = 310,                /* ELLIPSIS  */
    MUL_ASSIGN = 311,              /* MUL_ASSIGN  */
    DIV_ASSIGN = 312,              /* DIV_ASSIGN  */
    MOD_ASSIGN = 313,              /* MOD_ASSIGN  */
    ADD_ASSIGN = 314,              /* ADD_ASSIGN  */
    SUB_ASSIGN = 315,              /* SUB_ASSIGN  */
    SHL_ASSIGN = 316,              /* SHL_ASSIGN  */
    SHR_ASSIGN = 317,              /* SHR_ASSIGN  */
    AND_ASSIGN = 318,              /* AND_ASSIGN  */
    XOR_ASSIGN = 319,              /* XOR_ASSIGN  */
    OR_ASSIGN = 320                /* OR_ASSIGN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "tinyc2.y"

    char* text;
    ASTNode* node;

#line 140 "tinyc2.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_TINYC2_TAB_H_INCLUDED  */
