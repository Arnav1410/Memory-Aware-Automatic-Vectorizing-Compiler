/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "tinyc2.y"

#include <cstdlib>
#include <cstdio>
#include "ast.hpp"

ASTNode* tinyc2_root = NULL;
int yylex(void);
void yyerror(const char* s);

#line 81 "tinyc2.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "tinyc2.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PARAM_NO_DECL = 3,              /* PARAM_NO_DECL  */
  YYSYMBOL_IDENTIFIER = 4,                 /* IDENTIFIER  */
  YYSYMBOL_CONSTANT = 5,                   /* CONSTANT  */
  YYSYMBOL_STRING_LITERAL = 6,             /* STRING_LITERAL  */
  YYSYMBOL_CASE = 7,                       /* CASE  */
  YYSYMBOL_AUTO = 8,                       /* AUTO  */
  YYSYMBOL_ENUM = 9,                       /* ENUM  */
  YYSYMBOL_RESTRICT = 10,                  /* RESTRICT  */
  YYSYMBOL_UNSIGNED = 11,                  /* UNSIGNED  */
  YYSYMBOL_BREAK = 12,                     /* BREAK  */
  YYSYMBOL_EXTERN = 13,                    /* EXTERN  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_VOID = 15,                      /* VOID  */
  YYSYMBOL_FLOAT = 16,                     /* FLOAT  */
  YYSYMBOL_SHORT = 17,                     /* SHORT  */
  YYSYMBOL_VOLATILE = 18,                  /* VOLATILE  */
  YYSYMBOL_CHAR = 19,                      /* CHAR  */
  YYSYMBOL_FOR = 20,                       /* FOR  */
  YYSYMBOL_SIGNED = 21,                    /* SIGNED  */
  YYSYMBOL_WHILE = 22,                     /* WHILE  */
  YYSYMBOL_CONST = 23,                     /* CONST  */
  YYSYMBOL_GOTO = 24,                      /* GOTO  */
  YYSYMBOL_SIZEOF = 25,                    /* SIZEOF  */
  YYSYMBOL_BOOL = 26,                      /* BOOL  */
  YYSYMBOL_CONTINUE = 27,                  /* CONTINUE  */
  YYSYMBOL_IF = 28,                        /* IF  */
  YYSYMBOL_STATIC = 29,                    /* STATIC  */
  YYSYMBOL_COMPLEX = 30,                   /* COMPLEX  */
  YYSYMBOL_DEFAULT = 31,                   /* DEFAULT  */
  YYSYMBOL_INLINE = 32,                    /* INLINE  */
  YYSYMBOL_STRUCT = 33,                    /* STRUCT  */
  YYSYMBOL_IMAGINARY = 34,                 /* IMAGINARY  */
  YYSYMBOL_DO = 35,                        /* DO  */
  YYSYMBOL_INT = 36,                       /* INT  */
  YYSYMBOL_SWITCH = 37,                    /* SWITCH  */
  YYSYMBOL_DOUBLE = 38,                    /* DOUBLE  */
  YYSYMBOL_LONG = 39,                      /* LONG  */
  YYSYMBOL_TYPEDEF = 40,                   /* TYPEDEF  */
  YYSYMBOL_ELSE = 41,                      /* ELSE  */
  YYSYMBOL_REGISTER = 42,                  /* REGISTER  */
  YYSYMBOL_UNION = 43,                     /* UNION  */
  YYSYMBOL_ARROW = 44,                     /* ARROW  */
  YYSYMBOL_INC = 45,                       /* INC  */
  YYSYMBOL_DEC = 46,                       /* DEC  */
  YYSYMBOL_SHL = 47,                       /* SHL  */
  YYSYMBOL_SHR = 48,                       /* SHR  */
  YYSYMBOL_LE = 49,                        /* LE  */
  YYSYMBOL_GE = 50,                        /* GE  */
  YYSYMBOL_EQ = 51,                        /* EQ  */
  YYSYMBOL_NE = 52,                        /* NE  */
  YYSYMBOL_ANDAND = 53,                    /* ANDAND  */
  YYSYMBOL_OROR = 54,                      /* OROR  */
  YYSYMBOL_ELLIPSIS = 55,                  /* ELLIPSIS  */
  YYSYMBOL_MUL_ASSIGN = 56,                /* MUL_ASSIGN  */
  YYSYMBOL_DIV_ASSIGN = 57,                /* DIV_ASSIGN  */
  YYSYMBOL_MOD_ASSIGN = 58,                /* MOD_ASSIGN  */
  YYSYMBOL_ADD_ASSIGN = 59,                /* ADD_ASSIGN  */
  YYSYMBOL_SUB_ASSIGN = 60,                /* SUB_ASSIGN  */
  YYSYMBOL_SHL_ASSIGN = 61,                /* SHL_ASSIGN  */
  YYSYMBOL_SHR_ASSIGN = 62,                /* SHR_ASSIGN  */
  YYSYMBOL_AND_ASSIGN = 63,                /* AND_ASSIGN  */
  YYSYMBOL_XOR_ASSIGN = 64,                /* XOR_ASSIGN  */
  YYSYMBOL_OR_ASSIGN = 65,                 /* OR_ASSIGN  */
  YYSYMBOL_66_ = 66,                       /* '('  */
  YYSYMBOL_67_ = 67,                       /* ')'  */
  YYSYMBOL_68_ = 68,                       /* '['  */
  YYSYMBOL_69_ = 69,                       /* ']'  */
  YYSYMBOL_70_ = 70,                       /* '.'  */
  YYSYMBOL_71_ = 71,                       /* '{'  */
  YYSYMBOL_72_ = 72,                       /* '}'  */
  YYSYMBOL_73_ = 73,                       /* ','  */
  YYSYMBOL_74_ = 74,                       /* '&'  */
  YYSYMBOL_75_ = 75,                       /* '*'  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '~'  */
  YYSYMBOL_79_ = 79,                       /* '!'  */
  YYSYMBOL_80_ = 80,                       /* '/'  */
  YYSYMBOL_81_ = 81,                       /* '%'  */
  YYSYMBOL_82_ = 82,                       /* '<'  */
  YYSYMBOL_83_ = 83,                       /* '>'  */
  YYSYMBOL_84_ = 84,                       /* '^'  */
  YYSYMBOL_85_ = 85,                       /* '|'  */
  YYSYMBOL_86_ = 86,                       /* '?'  */
  YYSYMBOL_87_ = 87,                       /* ':'  */
  YYSYMBOL_88_ = 88,                       /* '='  */
  YYSYMBOL_89_ = 89,                       /* ';'  */
  YYSYMBOL_YYACCEPT = 90,                  /* $accept  */
  YYSYMBOL_translation_unit = 91,          /* translation_unit  */
  YYSYMBOL_external_declaration = 92,      /* external_declaration  */
  YYSYMBOL_function_definition = 93,       /* function_definition  */
  YYSYMBOL_declaration_list_opt = 94,      /* declaration_list_opt  */
  YYSYMBOL_declaration_list = 95,          /* declaration_list  */
  YYSYMBOL_primary_expression = 96,        /* primary_expression  */
  YYSYMBOL_postfix_expression = 97,        /* postfix_expression  */
  YYSYMBOL_argument_expression_list_opt = 98, /* argument_expression_list_opt  */
  YYSYMBOL_argument_expression_list = 99,  /* argument_expression_list  */
  YYSYMBOL_unary_expression = 100,         /* unary_expression  */
  YYSYMBOL_unary_operator = 101,           /* unary_operator  */
  YYSYMBOL_cast_expression = 102,          /* cast_expression  */
  YYSYMBOL_multiplicative_expression = 103, /* multiplicative_expression  */
  YYSYMBOL_additive_expression = 104,      /* additive_expression  */
  YYSYMBOL_shift_expression = 105,         /* shift_expression  */
  YYSYMBOL_relational_expression = 106,    /* relational_expression  */
  YYSYMBOL_equality_expression = 107,      /* equality_expression  */
  YYSYMBOL_and_expression = 108,           /* and_expression  */
  YYSYMBOL_exclusive_or_expression = 109,  /* exclusive_or_expression  */
  YYSYMBOL_inclusive_or_expression = 110,  /* inclusive_or_expression  */
  YYSYMBOL_logical_and_expression = 111,   /* logical_and_expression  */
  YYSYMBOL_logical_or_expression = 112,    /* logical_or_expression  */
  YYSYMBOL_conditional_expression = 113,   /* conditional_expression  */
  YYSYMBOL_assignment_expression = 114,    /* assignment_expression  */
  YYSYMBOL_assignment_operator = 115,      /* assignment_operator  */
  YYSYMBOL_expression = 116,               /* expression  */
  YYSYMBOL_expression_opt = 117,           /* expression_opt  */
  YYSYMBOL_constant_expression = 118,      /* constant_expression  */
  YYSYMBOL_assignment_expression_opt = 119, /* assignment_expression_opt  */
  YYSYMBOL_declaration = 120,              /* declaration  */
  YYSYMBOL_init_declarator_list_opt = 121, /* init_declarator_list_opt  */
  YYSYMBOL_init_declarator_list = 122,     /* init_declarator_list  */
  YYSYMBOL_init_declarator = 123,          /* init_declarator  */
  YYSYMBOL_storage_class_specifier = 124,  /* storage_class_specifier  */
  YYSYMBOL_declaration_specifiers = 125,   /* declaration_specifiers  */
  YYSYMBOL_type_specifier = 126,           /* type_specifier  */
  YYSYMBOL_specifier_qualifier_list = 127, /* specifier_qualifier_list  */
  YYSYMBOL_type_qualifier = 128,           /* type_qualifier  */
  YYSYMBOL_function_specifier = 129,       /* function_specifier  */
  YYSYMBOL_declarator = 130,               /* declarator  */
  YYSYMBOL_direct_declarator = 131,        /* direct_declarator  */
  YYSYMBOL_type_qualifier_list_opt = 132,  /* type_qualifier_list_opt  */
  YYSYMBOL_type_qualifier_list = 133,      /* type_qualifier_list  */
  YYSYMBOL_pointer = 134,                  /* pointer  */
  YYSYMBOL_parameter_type_list = 135,      /* parameter_type_list  */
  YYSYMBOL_parameter_list = 136,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 137,    /* parameter_declaration  */
  YYSYMBOL_identifier_list_opt = 138,      /* identifier_list_opt  */
  YYSYMBOL_identifier_list = 139,          /* identifier_list  */
  YYSYMBOL_type_name = 140,                /* type_name  */
  YYSYMBOL_initializer = 141,              /* initializer  */
  YYSYMBOL_initializer_list = 142,         /* initializer_list  */
  YYSYMBOL_designation = 143,              /* designation  */
  YYSYMBOL_designator_list = 144,          /* designator_list  */
  YYSYMBOL_designator = 145,               /* designator  */
  YYSYMBOL_statement = 146,                /* statement  */
  YYSYMBOL_matched_statement = 147,        /* matched_statement  */
  YYSYMBOL_unmatched_statement = 148,      /* unmatched_statement  */
  YYSYMBOL_non_if_statement = 149,         /* non_if_statement  */
  YYSYMBOL_labeled_statement = 150,        /* labeled_statement  */
  YYSYMBOL_compound_statement = 151,       /* compound_statement  */
  YYSYMBOL_block_item_list_opt = 152,      /* block_item_list_opt  */
  YYSYMBOL_block_item_list = 153,          /* block_item_list  */
  YYSYMBOL_block_item = 154,               /* block_item  */
  YYSYMBOL_expression_statement = 155,     /* expression_statement  */
  YYSYMBOL_if_matched = 156,               /* if_matched  */
  YYSYMBOL_if_unmatched = 157,             /* if_unmatched  */
  YYSYMBOL_selection_statement = 158,      /* selection_statement  */
  YYSYMBOL_switch_selection = 159,         /* switch_selection  */
  YYSYMBOL_iteration_statement = 160,      /* iteration_statement  */
  YYSYMBOL_jump_statement = 161            /* jump_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  33
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1074

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  90
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  72
/* YYNRULES -- Number of rules.  */
#define YYNRULES  209
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  334

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   320


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    79,     2,     2,     2,    81,    74,     2,
      66,    67,    75,    76,    73,    77,    70,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    87,    89,
      82,    88,    83,    86,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    68,     2,    69,    84,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    71,    85,    72,    78,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    53,    53,    55,    60,    62,    67,    72,    73,    77,
      78,    82,    83,    84,    85,    89,    90,    92,    94,    96,
      98,   100,   102,   104,   109,   110,   114,   115,   119,   120,
     121,   122,   123,   124,   128,   129,   130,   131,   132,   133,
     137,   138,   142,   143,   144,   145,   149,   150,   151,   155,
     156,   157,   161,   162,   163,   164,   165,   169,   170,   171,
     175,   176,   180,   181,   185,   186,   190,   191,   195,   196,
     200,   201,   206,   207,   212,   213,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   226,   227,   231,   232,   236,
     240,   241,   245,   250,   251,   255,   256,   260,   261,   265,
     266,   267,   268,   269,   273,   274,   275,   276,   277,   278,
     279,   280,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   301,   302,   303,   304,
     308,   309,   310,   314,   318,   319,   323,   324,   325,   327,
     329,   331,   333,   335,   340,   341,   345,   346,   350,   351,
     355,   356,   360,   361,   365,   366,   370,   371,   375,   376,
     380,   384,   385,   386,   390,   391,   392,   393,   397,   401,
     402,   406,   407,   411,   412,   416,   417,   421,   425,   426,
     427,   428,   429,   430,   434,   435,   436,   440,   444,   445,
     449,   450,   454,   455,   459,   460,   464,   469,   471,   476,
     480,   484,   485,   486,   487,   491,   492,   493,   494,   495
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PARAM_NO_DECL",
  "IDENTIFIER", "CONSTANT", "STRING_LITERAL", "CASE", "AUTO", "ENUM",
  "RESTRICT", "UNSIGNED", "BREAK", "EXTERN", "RETURN", "VOID", "FLOAT",
  "SHORT", "VOLATILE", "CHAR", "FOR", "SIGNED", "WHILE", "CONST", "GOTO",
  "SIZEOF", "BOOL", "CONTINUE", "IF", "STATIC", "COMPLEX", "DEFAULT",
  "INLINE", "STRUCT", "IMAGINARY", "DO", "INT", "SWITCH", "DOUBLE", "LONG",
  "TYPEDEF", "ELSE", "REGISTER", "UNION", "ARROW", "INC", "DEC", "SHL",
  "SHR", "LE", "GE", "EQ", "NE", "ANDAND", "OROR", "ELLIPSIS",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN",
  "SHL_ASSIGN", "SHR_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN",
  "'('", "')'", "'['", "']'", "'.'", "'{'", "'}'", "','", "'&'", "'*'",
  "'+'", "'-'", "'~'", "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'",
  "'?'", "':'", "'='", "';'", "$accept", "translation_unit",
  "external_declaration", "function_definition", "declaration_list_opt",
  "declaration_list", "primary_expression", "postfix_expression",
  "argument_expression_list_opt", "argument_expression_list",
  "unary_expression", "unary_operator", "cast_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "assignment_operator",
  "expression", "expression_opt", "constant_expression",
  "assignment_expression_opt", "declaration", "init_declarator_list_opt",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "declaration_specifiers", "type_specifier", "specifier_qualifier_list",
  "type_qualifier", "function_specifier", "declarator",
  "direct_declarator", "type_qualifier_list_opt", "type_qualifier_list",
  "pointer", "parameter_type_list", "parameter_list",
  "parameter_declaration", "identifier_list_opt", "identifier_list",
  "type_name", "initializer", "initializer_list", "designation",
  "designator_list", "designator", "statement", "matched_statement",
  "unmatched_statement", "non_if_statement", "labeled_statement",
  "compound_statement", "block_item_list_opt", "block_item_list",
  "block_item", "expression_statement", "if_matched", "if_unmatched",
  "selection_statement", "switch_selection", "iteration_statement",
  "jump_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-276)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-8)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1001,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,   929,  -276,  -276,  -276,  -276,   856,
    -276,  -276,  -276,  -276,  -276,  -276,    14,    68,   -77,   -50,
    -276,  -276,  -276,  -276,  -276,   446,    69,    15,   -36,  -276,
     -14,    68,  -276,    14,   657,    70,  1001,  -276,   856,   966,
      17,    69,  -276,  -276,  -276,  -276,   -12,  -276,  -276,  -276,
     701,   736,   736,   562,   643,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,    22,   256,   758,  -276,    48,   -32,    92,   -28,
     101,    13,    19,    45,    94,   -39,  -276,  -276,  -276,   260,
    -276,  -276,  -276,   856,    31,    77,  -276,    87,    85,    68,
     780,   115,   562,  -276,   562,  -276,  -276,  -276,    -8,  -276,
    1035,  -276,    93,   758,   158,  -276,    84,   657,   -38,  -276,
     160,  -276,  -276,   758,   758,   162,  -276,  -276,  -276,  -276,
    -276,  -276,  -276,  -276,  -276,  -276,  -276,   758,  -276,  -276,
     758,   758,   758,   758,   758,   758,   758,   758,   758,   758,
     758,   758,   758,   758,   758,   758,   758,   758,   758,    90,
     758,    82,   371,   113,   124,   187,   104,   129,   111,   336,
     133,  -276,   -56,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
     128,   260,  -276,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
    -276,  -276,   894,  -276,   198,   758,   135,  -276,   136,   758,
     139,   142,  -276,   758,  -276,  -276,   679,  -276,   141,  -276,
    -276,   544,  -276,  -276,  -276,  -276,   144,   140,  -276,    58,
    -276,  -276,  -276,  -276,  -276,    48,    48,   -32,   -32,    92,
      92,    92,    92,   -28,   -28,   101,    13,    19,    45,    94,
     -25,   336,   125,  -276,  -276,   -37,   486,   758,   126,  -276,
     758,   336,   196,   758,  -276,  -276,  -276,  -276,  -276,  -276,
     151,  -276,  -276,   153,   155,   155,  -276,   643,  -276,  -276,
    -276,  -276,   657,  -276,   758,  -276,   758,  -276,   336,  -276,
     154,   145,   758,     2,  -276,    27,  -276,   165,    29,  -276,
    -276,   109,  -276,  -276,  -276,  -276,   758,   147,   336,   336,
     758,   336,  -276,   599,   148,   758,  -276,  -276,   183,    59,
    -276,  -276,   758,   166,   336,   149,   174,   336,  -276,  -276,
    -276,   336,  -276,  -276
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   102,   125,   131,   120,   100,   112,   117,   114,   132,
     113,   119,   130,   121,   101,   122,   133,   124,   123,   115,
     118,   116,    99,   103,     0,     2,     4,     5,   104,    93,
     105,   106,   107,     1,     3,   136,     0,   144,     0,    94,
      95,   108,   109,   110,   111,    97,   135,     0,     0,   146,
     148,   145,    92,     0,     0,     0,     8,     9,    93,   156,
     144,   134,   137,   149,   147,    96,    97,    11,    12,    13,
       0,     0,     0,     0,     0,    34,    35,    36,    37,    38,
      39,    15,    28,    40,     0,    42,    46,    49,    52,    57,
      60,    62,    64,    66,    68,    70,    72,   161,    98,   188,
       6,    10,   158,   155,     0,   150,   152,     0,   157,   144,
      90,   145,     0,    32,     0,    29,    30,    85,     0,   126,
     160,   127,     0,     0,     0,   166,     0,     0,     0,   169,
       0,    20,    21,    24,     0,     0,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    74,     0,    40,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   194,     0,   192,   193,   173,   174,   175,   178,   179,
       0,   189,   190,   180,   176,   177,   181,   199,   182,   183,
     154,   142,     0,   143,     0,     0,    35,    91,     0,     0,
       0,     0,    14,     0,   128,   129,     0,    89,     0,   172,
     162,     0,   164,   168,   170,    19,     0,    25,    26,     0,
      18,    73,    43,    44,    45,    47,    48,    50,    51,    55,
      56,    53,    54,    58,    59,    61,    63,    65,    67,    69,
       0,     0,     0,   207,   208,     0,    87,     0,     0,   206,
       0,     0,     0,     0,   195,   187,   191,   151,   153,   159,
       0,   141,   138,     0,    33,     0,    86,     0,    41,   171,
     163,   167,     0,    17,     0,    16,     0,   184,     0,   209,
      88,     0,    87,     0,   205,     0,   186,     0,     0,   139,
     140,     0,   165,    27,    71,   185,    87,     0,     0,     0,
       0,     0,    22,     0,     0,    87,   201,   197,   173,     0,
     200,    23,    87,     0,     0,     0,     0,     0,   196,   198,
     202,     0,   204,   203
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -276,  -276,   208,  -276,  -276,  -276,  -276,  -276,  -276,  -276,
     -46,  -276,   -80,    30,    32,    16,    35,    79,    81,    78,
      83,    80,  -276,  -117,   -54,  -276,   -71,  -264,    86,  -276,
     -42,  -276,  -276,   195,   -21,     5,   105,  -276,   143,   -19,
     -20,   203,   -49,   193,   211,  -276,  -276,    56,  -276,  -276,
      34,   -53,   -15,  -214,  -276,   173,  -166,  -275,   -17,  -276,
    -276,   248,  -276,  -276,   117,  -276,  -276,  -276,  -276,  -276,
    -276,  -276
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    24,    25,    26,    55,    56,    81,    82,   226,   227,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,   117,   147,   182,   291,   218,   208,
      27,    38,    39,    40,    28,    58,    30,   120,    31,    32,
      66,    46,    50,    51,    47,   104,   105,   106,   107,   108,
     122,   125,   126,   127,   128,   129,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      97,    98,   118,    57,   149,    29,   217,   282,    41,    45,
      44,   110,    52,   262,   101,   167,    48,   213,    35,    35,
      97,   157,   158,    53,   113,   115,   116,     3,   307,    29,
     123,    62,   124,   264,   318,     9,   213,    41,   148,    44,
      12,   118,   314,   118,   153,   154,   109,   168,   213,   328,
     223,   323,   289,   217,   159,   160,   207,   183,   326,   212,
     205,    37,   286,   229,   103,   213,   130,   131,   132,   308,
     232,   233,   234,    97,   222,   213,    54,   148,     3,   228,
      36,    36,    41,   200,    44,   287,     9,   163,   133,    37,
     134,    12,   135,   231,   309,   296,   311,   250,   201,   282,
     213,   255,   213,   164,   148,   148,   148,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     148,   148,   305,   150,   148,     3,   325,   285,   151,   152,
     165,   213,   213,     9,    42,    59,   278,    60,    12,   155,
     156,    99,   316,   317,   209,   320,   210,   166,   211,   183,
     202,   270,   161,   162,   203,   273,   220,   221,   204,   276,
     216,   332,   219,    42,   225,   333,   230,    97,   281,   304,
     148,   253,    43,   239,   240,   241,   242,   251,   119,   256,
      49,   312,   313,   235,   236,   290,   293,   237,   238,   295,
     257,   258,   298,   259,    64,   260,   243,   244,   261,   263,
     265,    43,   269,    49,   271,   272,   274,   103,    42,   275,
     279,   283,   288,   284,   292,   294,   121,   119,   297,   119,
     299,   290,   300,    97,   324,   214,   277,   213,    97,   302,
     303,   310,    34,   327,   306,   290,   315,   322,   330,   319,
     148,   331,   245,   247,   290,   246,    43,   249,    65,   248,
      61,   290,    49,   111,    64,   121,   252,   121,   268,    97,
     281,    63,   301,   215,   169,    68,    69,   170,     1,     2,
       3,     4,   171,     5,   172,     6,     7,     8,     9,    10,
     173,    11,   174,    12,   175,    70,    13,   176,   177,    14,
      15,   178,    16,    17,    18,   179,    19,   180,    20,    21,
      22,   224,    23,   100,     0,    71,    72,   329,   266,     0,
       0,     0,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,     0,     0,     0,     0,    73,     0,     0,     0,
       0,    99,     0,     0,    75,    76,    77,    78,    79,    80,
     169,    68,    69,   170,   146,     0,     0,     0,   171,   181,
     172,     0,     0,     0,     0,     0,   173,     0,   174,     0,
     175,    70,     0,   176,   177,     0,     0,   178,     0,     0,
       0,   179,     0,   180,     0,    67,    68,    69,     0,     0,
       0,    71,    72,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,     0,     0,     0,
       0,     0,    73,     0,     0,     0,     0,    99,     0,     0,
      75,    76,    77,    78,    79,    80,    71,    72,     0,     0,
       0,     0,     0,     0,     0,   181,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    73,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,     0,     0,     0,     1,     2,     3,     4,     0,     5,
     254,     6,     7,     8,     9,    10,     0,    11,     0,    12,
       0,     0,    13,     0,     0,    14,    15,     0,    16,    17,
      18,     0,    19,     0,    20,    21,    22,     0,    23,     0,
      67,    68,    69,     0,     1,     2,     3,     4,     0,     5,
       0,     6,     7,     8,     9,    10,     0,    11,     0,    12,
       0,    70,    13,     0,     0,    14,    15,    -7,    16,    17,
      18,     0,    19,     0,    20,    21,    22,     0,    23,     0,
       0,    71,    72,     0,    54,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
      69,     0,    73,     0,     0,     0,     0,     0,     0,     0,
      75,    76,    77,    78,    79,    80,    67,    68,    69,    70,
       0,     2,     3,     4,     0,     0,     0,     6,     7,     8,
       9,    10,     0,    11,     0,    12,     0,    70,    13,    71,
      72,     0,    15,     0,     0,    17,    18,     0,    19,     0,
      20,    21,     0,    67,    68,    69,     0,    71,    72,     0,
      73,     0,   123,     0,   124,    74,   280,     0,    75,    76,
      77,    78,    79,    80,    70,     0,     0,     0,    73,     0,
       0,     0,     0,     0,     0,     0,    75,    76,    77,    78,
      79,    80,     0,     0,    71,    72,     0,    67,    68,    69,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    67,    68,    69,     0,    73,     0,   123,    70,   124,
      74,   321,     0,    75,    76,    77,    78,    79,    80,     0,
       0,     0,    70,    67,    68,    69,     0,     0,    71,    72,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    71,    72,    70,    67,    68,    69,     0,    73,
       0,   123,     0,   124,    74,     0,     0,    75,    76,    77,
      78,    79,    80,    73,    71,    72,    70,     0,    74,     0,
       0,    75,    76,    77,    78,    79,    80,     0,     0,     0,
      67,    68,    69,     0,     0,    73,    71,    72,     0,     0,
     277,     0,     0,    75,    76,    77,    78,    79,    80,     0,
       0,    70,    67,    68,    69,     0,     0,   112,     0,     0,
       0,     0,     0,     0,     0,    75,    76,    77,    78,    79,
      80,    71,    72,    70,    67,    68,    69,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,    71,    72,    70,     0,     0,     0,     0,
      75,    76,    77,    78,    79,    80,     0,     0,     0,     0,
       0,     0,     0,     0,    73,    71,    72,     0,     0,     0,
       0,     0,    75,    76,    77,    78,    79,    80,     0,     0,
       0,     0,     0,     0,     0,     0,    73,     0,     0,     0,
       0,     0,     0,     0,    75,   206,    77,    78,    79,    80,
      35,     0,     0,     0,     1,     2,     3,     4,     0,     5,
       0,     6,     7,     8,     9,    10,     0,    11,     0,    12,
       0,     0,    13,     0,     0,    14,    15,     0,    16,    17,
      18,     0,    19,     0,    20,    21,    22,     0,    23,     0,
       0,     0,     1,     2,     3,     4,     0,     5,     0,     6,
       7,     8,     9,    10,     0,    11,     0,    12,     0,     0,
      13,     0,    36,    14,    15,     0,    16,    17,    18,    33,
      19,    37,    20,    21,    22,     0,    23,     1,     2,     3,
       4,     0,     5,     0,     6,     7,     8,     9,    10,   267,
      11,     0,    12,     0,     0,    13,     0,     0,    14,    15,
       0,    16,    17,    18,     0,    19,     0,    20,    21,    22,
     102,    23,     0,     0,     1,     2,     3,     4,     0,     5,
       0,     6,     7,     8,     9,    10,     0,    11,     0,    12,
       0,     0,    13,     0,     0,    14,    15,     0,    16,    17,
      18,     0,    19,     0,    20,    21,    22,     0,    23,     1,
       2,     3,     4,     0,     5,     0,     6,     7,     8,     9,
      10,     0,    11,     0,    12,     0,     0,    13,     0,     0,
      14,    15,     0,    16,    17,    18,     0,    19,     0,    20,
      21,    22,     0,    23,     2,     3,     4,     0,     0,     0,
       6,     7,     8,     9,    10,     0,    11,     0,    12,     0,
       0,    13,     0,     0,     0,    15,     0,     0,    17,    18,
       0,    19,     0,    20,    21
};

static const yytype_int16 yycheck[] =
{
      54,    54,    73,    45,    84,     0,   123,   221,    29,    29,
      29,    60,    89,   179,    56,    54,    36,    73,     4,     4,
      74,    49,    50,    73,    70,    71,    72,    10,   292,    24,
      68,    67,    70,    89,   309,    18,    73,    58,    84,    58,
      23,   112,   306,   114,    76,    77,    29,    86,    73,   324,
      88,   315,    89,   170,    82,    83,   110,    99,   322,    67,
     109,    75,    87,   134,    59,    73,    44,    45,    46,    67,
     150,   151,   152,   127,   127,    73,    88,   123,    10,   133,
      66,    66,   103,   103,   103,   251,    18,    74,    66,    75,
      68,    23,    70,   147,    67,   261,    67,   168,    67,   313,
      73,   172,    73,    84,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   288,    75,   170,    10,    67,    69,    80,    81,
      85,    73,    73,    18,    29,    66,   216,    68,    23,    47,
      48,    71,   308,   309,    29,   311,   112,    53,   114,   191,
      73,   205,    51,    52,    67,   209,    72,    73,    73,   213,
      67,   327,     4,    58,     4,   331,     4,   221,   221,   286,
     216,    89,    29,   157,   158,   159,   160,    87,    73,    66,
      37,    72,    73,   153,   154,   256,   257,   155,   156,   260,
      66,     4,   263,    89,    51,    66,   161,   162,    87,    66,
      72,    58,     4,    60,    69,    69,    67,   202,   103,    67,
      69,    67,    87,    73,   256,    89,    73,   112,    22,   114,
      69,   292,    69,   277,    41,   120,    71,    73,   282,   282,
     284,    66,    24,    67,    89,   306,    89,    89,    89,   310,
     286,    67,   163,   165,   315,   164,   103,   167,    53,   166,
      47,   322,   109,    60,   111,   112,   170,   114,   202,   313,
     313,    50,   277,   120,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,   128,    42,    55,    -1,    45,    46,   324,   191,    -1,
      -1,    -1,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    71,    -1,    -1,    74,    75,    76,    77,    78,    79,
       4,     5,     6,     7,    88,    -1,    -1,    -1,    12,    89,
      14,    -1,    -1,    -1,    -1,    -1,    20,    -1,    22,    -1,
      24,    25,    -1,    27,    28,    -1,    -1,    31,    -1,    -1,
      -1,    35,    -1,    37,    -1,     4,     5,     6,    -1,    -1,
      -1,    45,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    25,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    -1,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    78,    79,    45,    46,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      79,    -1,    -1,    -1,     8,     9,    10,    11,    -1,    13,
      89,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    29,    30,    -1,    32,    33,
      34,    -1,    36,    -1,    38,    39,    40,    -1,    42,    -1,
       4,     5,     6,    -1,     8,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      -1,    25,    26,    -1,    -1,    29,    30,    71,    32,    33,
      34,    -1,    36,    -1,    38,    39,    40,    -1,    42,    -1,
      -1,    45,    46,    -1,    88,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     4,     5,
       6,    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    79,     4,     5,     6,    25,
      -1,     9,    10,    11,    -1,    -1,    -1,    15,    16,    17,
      18,    19,    -1,    21,    -1,    23,    -1,    25,    26,    45,
      46,    -1,    30,    -1,    -1,    33,    34,    -1,    36,    -1,
      38,    39,    -1,     4,     5,     6,    -1,    45,    46,    -1,
      66,    -1,    68,    -1,    70,    71,    72,    -1,    74,    75,
      76,    77,    78,    79,    25,    -1,    -1,    -1,    66,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    45,    46,    -1,     4,     5,     6,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     4,     5,     6,    -1,    66,    -1,    68,    25,    70,
      71,    72,    -1,    74,    75,    76,    77,    78,    79,    -1,
      -1,    -1,    25,     4,     5,     6,    -1,    -1,    45,    46,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    45,    46,    25,     4,     5,     6,    -1,    66,
      -1,    68,    -1,    70,    71,    -1,    -1,    74,    75,    76,
      77,    78,    79,    66,    45,    46,    25,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    78,    79,    -1,    -1,    -1,
       4,     5,     6,    -1,    -1,    66,    45,    46,    -1,    -1,
      71,    -1,    -1,    74,    75,    76,    77,    78,    79,    -1,
      -1,    25,     4,     5,     6,    -1,    -1,    66,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,
      79,    45,    46,    25,     4,     5,     6,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    45,    46,    25,    -1,    -1,    -1,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    66,    45,    46,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    74,    75,    76,    77,    78,    79,
       4,    -1,    -1,    -1,     8,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    29,    30,    -1,    32,    33,
      34,    -1,    36,    -1,    38,    39,    40,    -1,    42,    -1,
      -1,    -1,     8,     9,    10,    11,    -1,    13,    -1,    15,
      16,    17,    18,    19,    -1,    21,    -1,    23,    -1,    -1,
      26,    -1,    66,    29,    30,    -1,    32,    33,    34,     0,
      36,    75,    38,    39,    40,    -1,    42,     8,     9,    10,
      11,    -1,    13,    -1,    15,    16,    17,    18,    19,    55,
      21,    -1,    23,    -1,    -1,    26,    -1,    -1,    29,    30,
      -1,    32,    33,    34,    -1,    36,    -1,    38,    39,    40,
       4,    42,    -1,    -1,     8,     9,    10,    11,    -1,    13,
      -1,    15,    16,    17,    18,    19,    -1,    21,    -1,    23,
      -1,    -1,    26,    -1,    -1,    29,    30,    -1,    32,    33,
      34,    -1,    36,    -1,    38,    39,    40,    -1,    42,     8,
       9,    10,    11,    -1,    13,    -1,    15,    16,    17,    18,
      19,    -1,    21,    -1,    23,    -1,    -1,    26,    -1,    -1,
      29,    30,    -1,    32,    33,    34,    -1,    36,    -1,    38,
      39,    40,    -1,    42,     9,    10,    11,    -1,    -1,    -1,
      15,    16,    17,    18,    19,    -1,    21,    -1,    23,    -1,
      -1,    26,    -1,    -1,    -1,    30,    -1,    -1,    33,    34,
      -1,    36,    -1,    38,    39
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,     9,    10,    11,    13,    15,    16,    17,    18,
      19,    21,    23,    26,    29,    30,    32,    33,    34,    36,
      38,    39,    40,    42,    91,    92,    93,   120,   124,   125,
     126,   128,   129,     0,    92,     4,    66,    75,   121,   122,
     123,   124,   126,   128,   129,   130,   131,   134,   130,   128,
     132,   133,    89,    73,    88,    94,    95,   120,   125,    66,
      68,   131,    67,   134,   128,   123,   130,     4,     5,     6,
      25,    45,    46,    66,    71,    74,    75,    76,    77,    78,
      79,    96,    97,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   141,    71,
     151,   120,     4,   125,   135,   136,   137,   138,   139,    29,
     132,   133,    66,   100,    66,   100,   100,   114,   116,   126,
     127,   128,   140,    68,    70,   141,   142,   143,   144,   145,
      44,    45,    46,    66,    68,    70,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    88,   115,   100,   102,
      75,    80,    81,    76,    77,    47,    48,    49,    50,    82,
      83,    51,    52,    74,    84,    85,    53,    54,    86,     4,
       7,    12,    14,    20,    22,    24,    27,    28,    31,    35,
      37,    89,   116,   120,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     130,    67,    73,    67,    73,   132,    75,   114,   119,    29,
     140,   140,    67,    73,   126,   128,    67,   113,   118,     4,
      72,    73,   141,    88,   145,     4,    98,    99,   114,   116,
       4,   114,   102,   102,   102,   103,   103,   104,   104,   105,
     105,   105,   105,   106,   106,   107,   108,   109,   110,   111,
     116,    87,   118,    89,    89,   116,    66,    66,     4,    89,
      66,    87,   146,    66,    89,    72,   154,    55,   137,     4,
     114,    69,    69,   114,    67,    67,   114,    71,   102,    69,
      72,   141,   143,    67,    73,    69,    87,   146,    87,    89,
     116,   117,   120,   116,    89,   116,   146,    22,   116,    69,
      69,   142,   141,   114,   113,   146,    89,   117,    67,    67,
      66,    67,    72,    73,   117,    89,   146,   146,   147,   116,
     146,    72,    89,   117,    41,    67,   117,    67,   147,   148,
      89,    67,   146,   146
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    90,    91,    91,    92,    92,    93,    94,    94,    95,
      95,    96,    96,    96,    96,    97,    97,    97,    97,    97,
      97,    97,    97,    97,    98,    98,    99,    99,   100,   100,
     100,   100,   100,   100,   101,   101,   101,   101,   101,   101,
     102,   102,   103,   103,   103,   103,   104,   104,   104,   105,
     105,   105,   106,   106,   106,   106,   106,   107,   107,   107,
     108,   108,   109,   109,   110,   110,   111,   111,   112,   112,
     113,   113,   114,   114,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   116,   116,   117,   117,   118,
     119,   119,   120,   121,   121,   122,   122,   123,   123,   124,
     124,   124,   124,   124,   125,   125,   125,   125,   125,   125,
     125,   125,   126,   126,   126,   126,   126,   126,   126,   126,
     126,   126,   126,   126,   126,   126,   127,   127,   127,   127,
     128,   128,   128,   129,   130,   130,   131,   131,   131,   131,
     131,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   135,   136,   136,   137,   137,   138,   138,   139,   139,
     140,   141,   141,   141,   142,   142,   142,   142,   143,   144,
     144,   145,   145,   146,   146,   147,   147,   148,   149,   149,
     149,   149,   149,   149,   150,   150,   150,   151,   152,   152,
     153,   153,   154,   154,   155,   155,   156,   157,   157,   158,
     159,   160,   160,   160,   160,   161,   161,   161,   161,   161
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     4,     0,     1,     1,
       2,     1,     1,     1,     3,     1,     4,     4,     3,     3,
       2,     2,     6,     7,     0,     1,     1,     3,     1,     2,
       2,     2,     2,     4,     1,     1,     1,     1,     1,     1,
       1,     4,     1,     3,     3,     3,     1,     3,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     5,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     0,     1,     1,
       0,     1,     3,     0,     1,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     1,     1,     2,     1,     1,     3,     5,     6,
       6,     5,     4,     4,     0,     1,     1,     2,     2,     3,
       1,     3,     1,     3,     2,     1,     0,     1,     1,     3,
       1,     1,     3,     4,     2,     4,     1,     3,     2,     1,
       2,     3,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     4,     3,     3,     0,     1,
       1,     2,     1,     1,     1,     2,     7,     5,     7,     1,
       5,     5,     7,     9,     8,     3,     2,     2,     2,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* translation_unit: external_declaration  */
#line 54 "tinyc2.y"
        { (yyval.node) = ast_node("translation-unit"); ast_add((yyval.node), (yyvsp[0].node)); tinyc2_root = (yyval.node); }
#line 1914 "tinyc2.tab.c"
    break;

  case 3: /* translation_unit: translation_unit external_declaration  */
#line 56 "tinyc2.y"
        { (yyval.node) = (yyvsp[-1].node); ast_add((yyval.node), (yyvsp[0].node)); tinyc2_root = (yyval.node); }
#line 1920 "tinyc2.tab.c"
    break;

  case 4: /* external_declaration: function_definition  */
#line 61 "tinyc2.y"
        { (yyval.node) = ast_node("external-declaration"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1926 "tinyc2.tab.c"
    break;

  case 5: /* external_declaration: declaration  */
#line 63 "tinyc2.y"
        { (yyval.node) = ast_node("external-declaration"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1932 "tinyc2.tab.c"
    break;

  case 6: /* function_definition: declaration_specifiers declarator declaration_list_opt compound_statement  */
#line 68 "tinyc2.y"
        { (yyval.node) = ast_node("function-definition"); ast_add((yyval.node), (yyvsp[-3].node)); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[-1].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1938 "tinyc2.tab.c"
    break;

  case 7: /* declaration_list_opt: %empty  */
#line 72 "tinyc2.y"
             { (yyval.node) = ast_node("declaration-list-opt: epsilon"); }
#line 1944 "tinyc2.tab.c"
    break;

  case 8: /* declaration_list_opt: declaration_list  */
#line 73 "tinyc2.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 1950 "tinyc2.tab.c"
    break;

  case 9: /* declaration_list: declaration  */
#line 77 "tinyc2.y"
                  { (yyval.node) = ast_node("declaration-list"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1956 "tinyc2.tab.c"
    break;

  case 10: /* declaration_list: declaration_list declaration  */
#line 78 "tinyc2.y"
                                   { (yyval.node) = (yyvsp[-1].node); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1962 "tinyc2.tab.c"
    break;

  case 11: /* primary_expression: IDENTIFIER  */
#line 82 "tinyc2.y"
                 { (yyval.node) = ast_leaf("identifier", (yyvsp[0].text)); std::free((yyvsp[0].text)); }
#line 1968 "tinyc2.tab.c"
    break;

  case 12: /* primary_expression: CONSTANT  */
#line 83 "tinyc2.y"
               { (yyval.node) = ast_leaf("constant", (yyvsp[0].text)); std::free((yyvsp[0].text)); }
#line 1974 "tinyc2.tab.c"
    break;

  case 13: /* primary_expression: STRING_LITERAL  */
#line 84 "tinyc2.y"
                     { (yyval.node) = ast_leaf("string-literal", (yyvsp[0].text)); std::free((yyvsp[0].text)); }
#line 1980 "tinyc2.tab.c"
    break;

  case 14: /* primary_expression: '(' expression ')'  */
#line 85 "tinyc2.y"
                         { (yyval.node) = ast_node("primary-expression"); ast_add((yyval.node), (yyvsp[-1].node)); }
#line 1986 "tinyc2.tab.c"
    break;

  case 15: /* postfix_expression: primary_expression  */
#line 89 "tinyc2.y"
                         { (yyval.node) = ast_node("postfix-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 1992 "tinyc2.tab.c"
    break;

  case 16: /* postfix_expression: postfix_expression '[' expression ']'  */
#line 91 "tinyc2.y"
        { (yyval.node) = (yyvsp[-3].node); ASTNode* n = ast_node("index"); ast_add(n, (yyvsp[-1].node)); ast_add((yyval.node), n); }
#line 1998 "tinyc2.tab.c"
    break;

  case 17: /* postfix_expression: postfix_expression '(' argument_expression_list_opt ')'  */
#line 93 "tinyc2.y"
        { (yyval.node) = (yyvsp[-3].node); ASTNode* n = ast_node("call"); ast_add(n, (yyvsp[-1].node)); ast_add((yyval.node), n); }
#line 2004 "tinyc2.tab.c"
    break;

  case 18: /* postfix_expression: postfix_expression '.' IDENTIFIER  */
#line 95 "tinyc2.y"
        { (yyval.node) = (yyvsp[-2].node); ASTNode* n = ast_leaf("member", (yyvsp[0].text)); std::free((yyvsp[0].text)); ast_add((yyval.node), n); }
#line 2010 "tinyc2.tab.c"
    break;

  case 19: /* postfix_expression: postfix_expression ARROW IDENTIFIER  */
#line 97 "tinyc2.y"
        { (yyval.node) = (yyvsp[-2].node); ASTNode* n = ast_leaf("ptr-member", (yyvsp[0].text)); std::free((yyvsp[0].text)); ast_add((yyval.node), n); }
#line 2016 "tinyc2.tab.c"
    break;

  case 20: /* postfix_expression: postfix_expression INC  */
#line 99 "tinyc2.y"
        { (yyval.node) = (yyvsp[-1].node); ast_add((yyval.node), ast_node("postfix++")); }
#line 2022 "tinyc2.tab.c"
    break;

  case 21: /* postfix_expression: postfix_expression DEC  */
#line 101 "tinyc2.y"
        { (yyval.node) = (yyvsp[-1].node); ast_add((yyval.node), ast_node("postfix--")); }
#line 2028 "tinyc2.tab.c"
    break;

  case 22: /* postfix_expression: '(' type_name ')' '{' initializer_list '}'  */
#line 103 "tinyc2.y"
        { (yyval.node) = ast_node("compound-literal"); ast_add((yyval.node), (yyvsp[-4].node)); ast_add((yyval.node), (yyvsp[-1].node)); }
#line 2034 "tinyc2.tab.c"
    break;

  case 23: /* postfix_expression: '(' type_name ')' '{' initializer_list ',' '}'  */
#line 105 "tinyc2.y"
        { (yyval.node) = ast_node("compound-literal"); ast_add((yyval.node), (yyvsp[-5].node)); ast_add((yyval.node), (yyvsp[-2].node)); }
#line 2040 "tinyc2.tab.c"
    break;

  case 24: /* argument_expression_list_opt: %empty  */
#line 109 "tinyc2.y"
             { (yyval.node) = ast_node("argument-expression-list-opt: epsilon"); }
#line 2046 "tinyc2.tab.c"
    break;

  case 25: /* argument_expression_list_opt: argument_expression_list  */
#line 110 "tinyc2.y"
                               { (yyval.node) = (yyvsp[0].node); }
#line 2052 "tinyc2.tab.c"
    break;

  case 26: /* argument_expression_list: assignment_expression  */
#line 114 "tinyc2.y"
                            { (yyval.node) = ast_node("argument-expression-list"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2058 "tinyc2.tab.c"
    break;

  case 27: /* argument_expression_list: argument_expression_list ',' assignment_expression  */
#line 115 "tinyc2.y"
                                                         { (yyval.node) = (yyvsp[-2].node); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2064 "tinyc2.tab.c"
    break;

  case 28: /* unary_expression: postfix_expression  */
#line 119 "tinyc2.y"
                         { (yyval.node) = ast_node("unary-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2070 "tinyc2.tab.c"
    break;

  case 29: /* unary_expression: INC unary_expression  */
#line 120 "tinyc2.y"
                           { (yyval.node) = ast_node("pre++"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2076 "tinyc2.tab.c"
    break;

  case 30: /* unary_expression: DEC unary_expression  */
#line 121 "tinyc2.y"
                           { (yyval.node) = ast_node("pre--"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2082 "tinyc2.tab.c"
    break;

  case 31: /* unary_expression: unary_operator cast_expression  */
#line 122 "tinyc2.y"
                                     { (yyval.node) = ast_node("unary-expression"); ast_add((yyval.node), (yyvsp[-1].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2088 "tinyc2.tab.c"
    break;

  case 32: /* unary_expression: SIZEOF unary_expression  */
#line 123 "tinyc2.y"
                              { (yyval.node) = ast_node("sizeof"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2094 "tinyc2.tab.c"
    break;

  case 33: /* unary_expression: SIZEOF '(' type_name ')'  */
#line 124 "tinyc2.y"
                               { (yyval.node) = ast_node("sizeof-type"); ast_add((yyval.node), (yyvsp[-1].node)); }
#line 2100 "tinyc2.tab.c"
    break;

  case 34: /* unary_operator: '&'  */
#line 128 "tinyc2.y"
          { (yyval.node) = ast_node("&"); }
#line 2106 "tinyc2.tab.c"
    break;

  case 35: /* unary_operator: '*'  */
#line 129 "tinyc2.y"
          { (yyval.node) = ast_node("*"); }
#line 2112 "tinyc2.tab.c"
    break;

  case 36: /* unary_operator: '+'  */
#line 130 "tinyc2.y"
          { (yyval.node) = ast_node("+"); }
#line 2118 "tinyc2.tab.c"
    break;

  case 37: /* unary_operator: '-'  */
#line 131 "tinyc2.y"
          { (yyval.node) = ast_node("-"); }
#line 2124 "tinyc2.tab.c"
    break;

  case 38: /* unary_operator: '~'  */
#line 132 "tinyc2.y"
          { (yyval.node) = ast_node("~"); }
#line 2130 "tinyc2.tab.c"
    break;

  case 39: /* unary_operator: '!'  */
#line 133 "tinyc2.y"
          { (yyval.node) = ast_node("!"); }
#line 2136 "tinyc2.tab.c"
    break;

  case 40: /* cast_expression: unary_expression  */
#line 137 "tinyc2.y"
                       { (yyval.node) = ast_node("cast-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2142 "tinyc2.tab.c"
    break;

  case 41: /* cast_expression: '(' type_name ')' cast_expression  */
#line 138 "tinyc2.y"
                                        { (yyval.node) = ast_node("cast"); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2148 "tinyc2.tab.c"
    break;

  case 42: /* multiplicative_expression: cast_expression  */
#line 142 "tinyc2.y"
                      { (yyval.node) = ast_node("multiplicative-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2154 "tinyc2.tab.c"
    break;

  case 43: /* multiplicative_expression: multiplicative_expression '*' cast_expression  */
#line 143 "tinyc2.y"
                                                    { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("*"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2160 "tinyc2.tab.c"
    break;

  case 44: /* multiplicative_expression: multiplicative_expression '/' cast_expression  */
#line 144 "tinyc2.y"
                                                    { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("/"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2166 "tinyc2.tab.c"
    break;

  case 45: /* multiplicative_expression: multiplicative_expression '%' cast_expression  */
#line 145 "tinyc2.y"
                                                    { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("%"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2172 "tinyc2.tab.c"
    break;

  case 46: /* additive_expression: multiplicative_expression  */
#line 149 "tinyc2.y"
                                { (yyval.node) = ast_node("additive-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2178 "tinyc2.tab.c"
    break;

  case 47: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 150 "tinyc2.y"
                                                        { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("+"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2184 "tinyc2.tab.c"
    break;

  case 48: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 151 "tinyc2.y"
                                                        { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("-"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2190 "tinyc2.tab.c"
    break;

  case 49: /* shift_expression: additive_expression  */
#line 155 "tinyc2.y"
                          { (yyval.node) = ast_node("shift-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2196 "tinyc2.tab.c"
    break;

  case 50: /* shift_expression: shift_expression SHL additive_expression  */
#line 156 "tinyc2.y"
                                               { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("<<"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2202 "tinyc2.tab.c"
    break;

  case 51: /* shift_expression: shift_expression SHR additive_expression  */
#line 157 "tinyc2.y"
                                               { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node(">>"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2208 "tinyc2.tab.c"
    break;

  case 52: /* relational_expression: shift_expression  */
#line 161 "tinyc2.y"
                       { (yyval.node) = ast_node("relational-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2214 "tinyc2.tab.c"
    break;

  case 53: /* relational_expression: relational_expression '<' shift_expression  */
#line 162 "tinyc2.y"
                                                 { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("<"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2220 "tinyc2.tab.c"
    break;

  case 54: /* relational_expression: relational_expression '>' shift_expression  */
#line 163 "tinyc2.y"
                                                 { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node(">"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2226 "tinyc2.tab.c"
    break;

  case 55: /* relational_expression: relational_expression LE shift_expression  */
#line 164 "tinyc2.y"
                                                { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("<="); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2232 "tinyc2.tab.c"
    break;

  case 56: /* relational_expression: relational_expression GE shift_expression  */
#line 165 "tinyc2.y"
                                                { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node(">="); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2238 "tinyc2.tab.c"
    break;

  case 57: /* equality_expression: relational_expression  */
#line 169 "tinyc2.y"
                            { (yyval.node) = ast_node("equality-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2244 "tinyc2.tab.c"
    break;

  case 58: /* equality_expression: equality_expression EQ relational_expression  */
#line 170 "tinyc2.y"
                                                   { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("=="); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2250 "tinyc2.tab.c"
    break;

  case 59: /* equality_expression: equality_expression NE relational_expression  */
#line 171 "tinyc2.y"
                                                   { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("!="); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2256 "tinyc2.tab.c"
    break;

  case 60: /* and_expression: equality_expression  */
#line 175 "tinyc2.y"
                          { (yyval.node) = ast_node("AND-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2262 "tinyc2.tab.c"
    break;

  case 61: /* and_expression: and_expression '&' equality_expression  */
#line 176 "tinyc2.y"
                                             { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("&"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2268 "tinyc2.tab.c"
    break;

  case 62: /* exclusive_or_expression: and_expression  */
#line 180 "tinyc2.y"
                     { (yyval.node) = ast_node("exclusive-OR-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2274 "tinyc2.tab.c"
    break;

  case 63: /* exclusive_or_expression: exclusive_or_expression '^' and_expression  */
#line 181 "tinyc2.y"
                                                 { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("^"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2280 "tinyc2.tab.c"
    break;

  case 64: /* inclusive_or_expression: exclusive_or_expression  */
#line 185 "tinyc2.y"
                              { (yyval.node) = ast_node("inclusive-OR-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2286 "tinyc2.tab.c"
    break;

  case 65: /* inclusive_or_expression: inclusive_or_expression '|' exclusive_or_expression  */
#line 186 "tinyc2.y"
                                                          { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("|"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2292 "tinyc2.tab.c"
    break;

  case 66: /* logical_and_expression: inclusive_or_expression  */
#line 190 "tinyc2.y"
                              { (yyval.node) = ast_node("logical-AND-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2298 "tinyc2.tab.c"
    break;

  case 67: /* logical_and_expression: logical_and_expression ANDAND inclusive_or_expression  */
#line 191 "tinyc2.y"
                                                            { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("&&"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2304 "tinyc2.tab.c"
    break;

  case 68: /* logical_or_expression: logical_and_expression  */
#line 195 "tinyc2.y"
                             { (yyval.node) = ast_node("logical-OR-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2310 "tinyc2.tab.c"
    break;

  case 69: /* logical_or_expression: logical_or_expression OROR logical_and_expression  */
#line 196 "tinyc2.y"
                                                        { (yyval.node) = (yyvsp[-2].node); ASTNode* n=ast_node("||"); ast_add(n,(yyvsp[0].node)); ast_add((yyval.node),n); }
#line 2316 "tinyc2.tab.c"
    break;

  case 70: /* conditional_expression: logical_or_expression  */
#line 200 "tinyc2.y"
                            { (yyval.node) = ast_node("conditional-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2322 "tinyc2.tab.c"
    break;

  case 71: /* conditional_expression: logical_or_expression '?' expression ':' conditional_expression  */
#line 202 "tinyc2.y"
        { (yyval.node) = ast_node("conditional-expression"); ast_add((yyval.node), (yyvsp[-4].node)); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2328 "tinyc2.tab.c"
    break;

  case 72: /* assignment_expression: conditional_expression  */
#line 206 "tinyc2.y"
                             { (yyval.node) = ast_node("assignment-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2334 "tinyc2.tab.c"
    break;

  case 73: /* assignment_expression: unary_expression assignment_operator assignment_expression  */
#line 208 "tinyc2.y"
        { (yyval.node) = ast_node("assignment-expression"); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[-1].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2340 "tinyc2.tab.c"
    break;

  case 74: /* assignment_operator: '='  */
#line 212 "tinyc2.y"
          { (yyval.node) = ast_node("="); }
#line 2346 "tinyc2.tab.c"
    break;

  case 75: /* assignment_operator: MUL_ASSIGN  */
#line 213 "tinyc2.y"
                 { (yyval.node) = ast_node("*="); }
#line 2352 "tinyc2.tab.c"
    break;

  case 76: /* assignment_operator: DIV_ASSIGN  */
#line 214 "tinyc2.y"
                 { (yyval.node) = ast_node("/="); }
#line 2358 "tinyc2.tab.c"
    break;

  case 77: /* assignment_operator: MOD_ASSIGN  */
#line 215 "tinyc2.y"
                 { (yyval.node) = ast_node("%="); }
#line 2364 "tinyc2.tab.c"
    break;

  case 78: /* assignment_operator: ADD_ASSIGN  */
#line 216 "tinyc2.y"
                 { (yyval.node) = ast_node("+="); }
#line 2370 "tinyc2.tab.c"
    break;

  case 79: /* assignment_operator: SUB_ASSIGN  */
#line 217 "tinyc2.y"
                 { (yyval.node) = ast_node("-="); }
#line 2376 "tinyc2.tab.c"
    break;

  case 80: /* assignment_operator: SHL_ASSIGN  */
#line 218 "tinyc2.y"
                 { (yyval.node) = ast_node("<<="); }
#line 2382 "tinyc2.tab.c"
    break;

  case 81: /* assignment_operator: SHR_ASSIGN  */
#line 219 "tinyc2.y"
                 { (yyval.node) = ast_node(">>="); }
#line 2388 "tinyc2.tab.c"
    break;

  case 82: /* assignment_operator: AND_ASSIGN  */
#line 220 "tinyc2.y"
                 { (yyval.node) = ast_node("&="); }
#line 2394 "tinyc2.tab.c"
    break;

  case 83: /* assignment_operator: XOR_ASSIGN  */
#line 221 "tinyc2.y"
                 { (yyval.node) = ast_node("^="); }
#line 2400 "tinyc2.tab.c"
    break;

  case 84: /* assignment_operator: OR_ASSIGN  */
#line 222 "tinyc2.y"
                { (yyval.node) = ast_node("|="); }
#line 2406 "tinyc2.tab.c"
    break;

  case 85: /* expression: assignment_expression  */
#line 226 "tinyc2.y"
                            { (yyval.node) = ast_node("expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2412 "tinyc2.tab.c"
    break;

  case 86: /* expression: expression ',' assignment_expression  */
#line 227 "tinyc2.y"
                                           { (yyval.node) = (yyvsp[-2].node); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2418 "tinyc2.tab.c"
    break;

  case 87: /* expression_opt: %empty  */
#line 231 "tinyc2.y"
             { (yyval.node) = ast_node("expression-opt: epsilon"); }
#line 2424 "tinyc2.tab.c"
    break;

  case 88: /* expression_opt: expression  */
#line 232 "tinyc2.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2430 "tinyc2.tab.c"
    break;

  case 89: /* constant_expression: conditional_expression  */
#line 236 "tinyc2.y"
                             { (yyval.node) = ast_node("constant-expression"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2436 "tinyc2.tab.c"
    break;

  case 90: /* assignment_expression_opt: %empty  */
#line 240 "tinyc2.y"
             { (yyval.node) = ast_node("assignment-expression-opt: epsilon"); }
#line 2442 "tinyc2.tab.c"
    break;

  case 91: /* assignment_expression_opt: assignment_expression  */
#line 241 "tinyc2.y"
                            { (yyval.node) = (yyvsp[0].node); }
#line 2448 "tinyc2.tab.c"
    break;

  case 92: /* declaration: declaration_specifiers init_declarator_list_opt ';'  */
#line 246 "tinyc2.y"
        { (yyval.node) = ast_node("declaration"); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[-1].node)); }
#line 2454 "tinyc2.tab.c"
    break;

  case 93: /* init_declarator_list_opt: %empty  */
#line 250 "tinyc2.y"
             { (yyval.node) = ast_node("init-declarator-list-opt: epsilon"); }
#line 2460 "tinyc2.tab.c"
    break;

  case 94: /* init_declarator_list_opt: init_declarator_list  */
#line 251 "tinyc2.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2466 "tinyc2.tab.c"
    break;

  case 95: /* init_declarator_list: init_declarator  */
#line 255 "tinyc2.y"
                      { (yyval.node) = ast_node("init-declarator-list"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2472 "tinyc2.tab.c"
    break;

  case 96: /* init_declarator_list: init_declarator_list ',' init_declarator  */
#line 256 "tinyc2.y"
                                               { (yyval.node) = (yyvsp[-2].node); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2478 "tinyc2.tab.c"
    break;

  case 97: /* init_declarator: declarator  */
#line 260 "tinyc2.y"
                 { (yyval.node) = ast_node("init-declarator"); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2484 "tinyc2.tab.c"
    break;

  case 98: /* init_declarator: declarator '=' initializer  */
#line 261 "tinyc2.y"
                                 { (yyval.node) = ast_node("init-declarator"); ast_add((yyval.node), (yyvsp[-2].node)); ast_add((yyval.node), (yyvsp[0].node)); }
#line 2490 "tinyc2.tab.c"
    break;

  case 99: /* storage_class_specifier: TYPEDEF  */
#line 265 "tinyc2.y"
              { (yyval.node) = ast_node("typedef"); }
#line 2496 "tinyc2.tab.c"
    break;

  case 100: /* storage_class_specifier: EXTERN  */
#line 266 "tinyc2.y"
             { (yyval.node) = ast_node("extern"); }
#line 2502 "tinyc2.tab.c"
    break;

  case 101: /* storage_class_specifier: STATIC  */
#line 267 "tinyc2.y"
             { (yyval.node) = ast_node("static"); }
#line 2508 "tinyc2.tab.c"
    break;

  case 102: /* storage_class_specifier: AUTO  */
#line 268 "tinyc2.y"
           { (yyval.node) = ast_node("auto"); }
#line 2514 "tinyc2.tab.c"
    break;

  case 103: /* storage_class_specifier: REGISTER  */
#line 269 "tinyc2.y"
               { (yyval.node) = ast_node("register"); }
#line 2520 "tinyc2.tab.c"
    break;

  case 104: /* declaration_specifiers: storage_class_specifier  */
#line 273 "tinyc2.y"
                              { (yyval.node) = ast_node("declaration-specifiers"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2526 "tinyc2.tab.c"
    break;

  case 105: /* declaration_specifiers: type_specifier  */
#line 274 "tinyc2.y"
                     { (yyval.node) = ast_node("declaration-specifiers"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2532 "tinyc2.tab.c"
    break;

  case 106: /* declaration_specifiers: type_qualifier  */
#line 275 "tinyc2.y"
                     { (yyval.node) = ast_node("declaration-specifiers"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2538 "tinyc2.tab.c"
    break;

  case 107: /* declaration_specifiers: function_specifier  */
#line 276 "tinyc2.y"
                         { (yyval.node) = ast_node("declaration-specifiers"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2544 "tinyc2.tab.c"
    break;

  case 108: /* declaration_specifiers: declaration_specifiers storage_class_specifier  */
#line 277 "tinyc2.y"
                                                     { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2550 "tinyc2.tab.c"
    break;

  case 109: /* declaration_specifiers: declaration_specifiers type_specifier  */
#line 278 "tinyc2.y"
                                            { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2556 "tinyc2.tab.c"
    break;

  case 110: /* declaration_specifiers: declaration_specifiers type_qualifier  */
#line 279 "tinyc2.y"
                                            { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2562 "tinyc2.tab.c"
    break;

  case 111: /* declaration_specifiers: declaration_specifiers function_specifier  */
#line 280 "tinyc2.y"
                                                { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2568 "tinyc2.tab.c"
    break;

  case 112: /* type_specifier: VOID  */
#line 284 "tinyc2.y"
           { (yyval.node)=ast_node("void"); }
#line 2574 "tinyc2.tab.c"
    break;

  case 113: /* type_specifier: CHAR  */
#line 285 "tinyc2.y"
           { (yyval.node)=ast_node("char"); }
#line 2580 "tinyc2.tab.c"
    break;

  case 114: /* type_specifier: SHORT  */
#line 286 "tinyc2.y"
            { (yyval.node)=ast_node("short"); }
#line 2586 "tinyc2.tab.c"
    break;

  case 115: /* type_specifier: INT  */
#line 287 "tinyc2.y"
          { (yyval.node)=ast_node("int"); }
#line 2592 "tinyc2.tab.c"
    break;

  case 116: /* type_specifier: LONG  */
#line 288 "tinyc2.y"
           { (yyval.node)=ast_node("long"); }
#line 2598 "tinyc2.tab.c"
    break;

  case 117: /* type_specifier: FLOAT  */
#line 289 "tinyc2.y"
            { (yyval.node)=ast_node("float"); }
#line 2604 "tinyc2.tab.c"
    break;

  case 118: /* type_specifier: DOUBLE  */
#line 290 "tinyc2.y"
             { (yyval.node)=ast_node("double"); }
#line 2610 "tinyc2.tab.c"
    break;

  case 119: /* type_specifier: SIGNED  */
#line 291 "tinyc2.y"
             { (yyval.node)=ast_node("signed"); }
#line 2616 "tinyc2.tab.c"
    break;

  case 120: /* type_specifier: UNSIGNED  */
#line 292 "tinyc2.y"
               { (yyval.node)=ast_node("unsigned"); }
#line 2622 "tinyc2.tab.c"
    break;

  case 121: /* type_specifier: BOOL  */
#line 293 "tinyc2.y"
           { (yyval.node)=ast_node("Bool"); }
#line 2628 "tinyc2.tab.c"
    break;

  case 122: /* type_specifier: COMPLEX  */
#line 294 "tinyc2.y"
              { (yyval.node)=ast_node("Complex"); }
#line 2634 "tinyc2.tab.c"
    break;

  case 123: /* type_specifier: IMAGINARY  */
#line 295 "tinyc2.y"
                { (yyval.node)=ast_node("Imaginary"); }
#line 2640 "tinyc2.tab.c"
    break;

  case 124: /* type_specifier: STRUCT  */
#line 296 "tinyc2.y"
             { (yyval.node)=ast_node("struct"); }
#line 2646 "tinyc2.tab.c"
    break;

  case 125: /* type_specifier: ENUM  */
#line 297 "tinyc2.y"
           { (yyval.node)=ast_node("enum"); }
#line 2652 "tinyc2.tab.c"
    break;

  case 126: /* specifier_qualifier_list: type_specifier  */
#line 301 "tinyc2.y"
                     { (yyval.node)=ast_node("specifier-qualifier-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2658 "tinyc2.tab.c"
    break;

  case 127: /* specifier_qualifier_list: type_qualifier  */
#line 302 "tinyc2.y"
                     { (yyval.node)=ast_node("specifier-qualifier-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2664 "tinyc2.tab.c"
    break;

  case 128: /* specifier_qualifier_list: specifier_qualifier_list type_specifier  */
#line 303 "tinyc2.y"
                                              { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2670 "tinyc2.tab.c"
    break;

  case 129: /* specifier_qualifier_list: specifier_qualifier_list type_qualifier  */
#line 304 "tinyc2.y"
                                              { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2676 "tinyc2.tab.c"
    break;

  case 130: /* type_qualifier: CONST  */
#line 308 "tinyc2.y"
            { (yyval.node)=ast_node("const"); }
#line 2682 "tinyc2.tab.c"
    break;

  case 131: /* type_qualifier: RESTRICT  */
#line 309 "tinyc2.y"
               { (yyval.node)=ast_node("restrict"); }
#line 2688 "tinyc2.tab.c"
    break;

  case 132: /* type_qualifier: VOLATILE  */
#line 310 "tinyc2.y"
               { (yyval.node)=ast_node("volatile"); }
#line 2694 "tinyc2.tab.c"
    break;

  case 133: /* function_specifier: INLINE  */
#line 314 "tinyc2.y"
             { (yyval.node)=ast_node("inline"); }
#line 2700 "tinyc2.tab.c"
    break;

  case 134: /* declarator: pointer direct_declarator  */
#line 318 "tinyc2.y"
                                { (yyval.node)=ast_node("declarator"); ast_add((yyval.node),(yyvsp[-1].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2706 "tinyc2.tab.c"
    break;

  case 135: /* declarator: direct_declarator  */
#line 319 "tinyc2.y"
                        { (yyval.node)=ast_node("declarator"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2712 "tinyc2.tab.c"
    break;

  case 136: /* direct_declarator: IDENTIFIER  */
#line 323 "tinyc2.y"
                 { (yyval.node)=ast_leaf("identifier",(yyvsp[0].text)); std::free((yyvsp[0].text)); }
#line 2718 "tinyc2.tab.c"
    break;

  case 137: /* direct_declarator: '(' declarator ')'  */
#line 324 "tinyc2.y"
                         { (yyval.node)=ast_node("direct-declarator"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 2724 "tinyc2.tab.c"
    break;

  case 138: /* direct_declarator: direct_declarator '[' type_qualifier_list_opt assignment_expression_opt ']'  */
#line 326 "tinyc2.y"
        { (yyval.node)=(yyvsp[-4].node); ASTNode* n=ast_node("array[]"); ast_add(n,(yyvsp[-2].node)); ast_add(n,(yyvsp[-1].node)); ast_add((yyval.node),n); }
#line 2730 "tinyc2.tab.c"
    break;

  case 139: /* direct_declarator: direct_declarator '[' STATIC type_qualifier_list_opt assignment_expression ']'  */
#line 328 "tinyc2.y"
        { (yyval.node)=(yyvsp[-5].node); ASTNode* n=ast_node("array[static]"); ast_add(n,(yyvsp[-2].node)); ast_add(n,(yyvsp[-1].node)); ast_add((yyval.node),n); }
#line 2736 "tinyc2.tab.c"
    break;

  case 140: /* direct_declarator: direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'  */
#line 330 "tinyc2.y"
        { (yyval.node)=(yyvsp[-5].node); ASTNode* n=ast_node("array[static]"); ast_add(n,(yyvsp[-3].node)); ast_add(n,(yyvsp[-1].node)); ast_add((yyval.node),n); }
#line 2742 "tinyc2.tab.c"
    break;

  case 141: /* direct_declarator: direct_declarator '[' type_qualifier_list_opt '*' ']'  */
#line 332 "tinyc2.y"
        { (yyval.node)=(yyvsp[-4].node); ast_add((yyval.node),ast_node("array[*]")); }
#line 2748 "tinyc2.tab.c"
    break;

  case 142: /* direct_declarator: direct_declarator '(' parameter_type_list ')'  */
#line 334 "tinyc2.y"
        { (yyval.node)=(yyvsp[-3].node); ASTNode* n=ast_node("function-declarator"); ast_add(n,(yyvsp[-1].node)); ast_add((yyval.node),n); }
#line 2754 "tinyc2.tab.c"
    break;

  case 143: /* direct_declarator: direct_declarator '(' identifier_list_opt ')'  */
#line 336 "tinyc2.y"
        { (yyval.node)=(yyvsp[-3].node); ASTNode* n=ast_node("function-declarator"); ast_add(n,(yyvsp[-1].node)); ast_add((yyval.node),n); }
#line 2760 "tinyc2.tab.c"
    break;

  case 144: /* type_qualifier_list_opt: %empty  */
#line 340 "tinyc2.y"
             { (yyval.node)=ast_node("type-qualifier-list-opt: epsilon"); }
#line 2766 "tinyc2.tab.c"
    break;

  case 145: /* type_qualifier_list_opt: type_qualifier_list  */
#line 341 "tinyc2.y"
                          { (yyval.node)=(yyvsp[0].node); }
#line 2772 "tinyc2.tab.c"
    break;

  case 146: /* type_qualifier_list: type_qualifier  */
#line 345 "tinyc2.y"
                     { (yyval.node)=ast_node("type-qualifier-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2778 "tinyc2.tab.c"
    break;

  case 147: /* type_qualifier_list: type_qualifier_list type_qualifier  */
#line 346 "tinyc2.y"
                                         { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2784 "tinyc2.tab.c"
    break;

  case 148: /* pointer: '*' type_qualifier_list_opt  */
#line 350 "tinyc2.y"
                                  { (yyval.node)=ast_node("pointer"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2790 "tinyc2.tab.c"
    break;

  case 149: /* pointer: '*' type_qualifier_list_opt pointer  */
#line 351 "tinyc2.y"
                                          { (yyval.node)=(yyvsp[0].node); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 2796 "tinyc2.tab.c"
    break;

  case 150: /* parameter_type_list: parameter_list  */
#line 355 "tinyc2.y"
                     { (yyval.node)=ast_node("parameter-type-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2802 "tinyc2.tab.c"
    break;

  case 151: /* parameter_type_list: parameter_list ',' ELLIPSIS  */
#line 356 "tinyc2.y"
                                  { (yyval.node)=(yyvsp[-2].node); ast_add((yyval.node),ast_node("...")); }
#line 2808 "tinyc2.tab.c"
    break;

  case 152: /* parameter_list: parameter_declaration  */
#line 360 "tinyc2.y"
                            { (yyval.node)=ast_node("parameter-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2814 "tinyc2.tab.c"
    break;

  case 153: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 361 "tinyc2.y"
                                               { (yyval.node)=(yyvsp[-2].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2820 "tinyc2.tab.c"
    break;

  case 154: /* parameter_declaration: declaration_specifiers declarator  */
#line 365 "tinyc2.y"
                                        { (yyval.node)=ast_node("parameter-declaration"); ast_add((yyval.node),(yyvsp[-1].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2826 "tinyc2.tab.c"
    break;

  case 155: /* parameter_declaration: declaration_specifiers  */
#line 366 "tinyc2.y"
                                                 { (yyval.node)=ast_node("parameter-declaration"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2832 "tinyc2.tab.c"
    break;

  case 156: /* identifier_list_opt: %empty  */
#line 370 "tinyc2.y"
             { (yyval.node)=ast_node("identifier-list-opt: epsilon"); }
#line 2838 "tinyc2.tab.c"
    break;

  case 157: /* identifier_list_opt: identifier_list  */
#line 371 "tinyc2.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 2844 "tinyc2.tab.c"
    break;

  case 158: /* identifier_list: IDENTIFIER  */
#line 375 "tinyc2.y"
                 { (yyval.node)=ast_node("identifier-list"); ast_add((yyval.node),ast_leaf("identifier",(yyvsp[0].text))); std::free((yyvsp[0].text)); }
#line 2850 "tinyc2.tab.c"
    break;

  case 159: /* identifier_list: identifier_list ',' IDENTIFIER  */
#line 376 "tinyc2.y"
                                     { (yyval.node)=(yyvsp[-2].node); ast_add((yyval.node),ast_leaf("identifier",(yyvsp[0].text))); std::free((yyvsp[0].text)); }
#line 2856 "tinyc2.tab.c"
    break;

  case 160: /* type_name: specifier_qualifier_list  */
#line 380 "tinyc2.y"
                               { (yyval.node)=ast_node("type-name"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2862 "tinyc2.tab.c"
    break;

  case 161: /* initializer: assignment_expression  */
#line 384 "tinyc2.y"
                            { (yyval.node)=ast_node("initializer"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2868 "tinyc2.tab.c"
    break;

  case 162: /* initializer: '{' initializer_list '}'  */
#line 385 "tinyc2.y"
                               { (yyval.node)=ast_node("initializer"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 2874 "tinyc2.tab.c"
    break;

  case 163: /* initializer: '{' initializer_list ',' '}'  */
#line 386 "tinyc2.y"
                                   { (yyval.node)=ast_node("initializer"); ast_add((yyval.node),(yyvsp[-2].node)); }
#line 2880 "tinyc2.tab.c"
    break;

  case 164: /* initializer_list: designation initializer  */
#line 390 "tinyc2.y"
                              { (yyval.node)=ast_node("initializer-list"); ast_add((yyval.node),(yyvsp[-1].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2886 "tinyc2.tab.c"
    break;

  case 165: /* initializer_list: initializer_list ',' designation initializer  */
#line 391 "tinyc2.y"
                                                   { (yyval.node)=(yyvsp[-3].node); ast_add((yyval.node),(yyvsp[-1].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2892 "tinyc2.tab.c"
    break;

  case 166: /* initializer_list: initializer  */
#line 392 "tinyc2.y"
                  { (yyval.node)=ast_node("initializer-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2898 "tinyc2.tab.c"
    break;

  case 167: /* initializer_list: initializer_list ',' initializer  */
#line 393 "tinyc2.y"
                                       { (yyval.node)=(yyvsp[-2].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2904 "tinyc2.tab.c"
    break;

  case 168: /* designation: designator_list '='  */
#line 397 "tinyc2.y"
                          { (yyval.node)=ast_node("designation"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 2910 "tinyc2.tab.c"
    break;

  case 169: /* designator_list: designator  */
#line 401 "tinyc2.y"
                 { (yyval.node)=ast_node("designator-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2916 "tinyc2.tab.c"
    break;

  case 170: /* designator_list: designator_list designator  */
#line 402 "tinyc2.y"
                                 { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 2922 "tinyc2.tab.c"
    break;

  case 171: /* designator: '[' constant_expression ']'  */
#line 406 "tinyc2.y"
                                  { (yyval.node)=ast_node("designator[]"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 2928 "tinyc2.tab.c"
    break;

  case 172: /* designator: '.' IDENTIFIER  */
#line 407 "tinyc2.y"
                     { (yyval.node)=ast_leaf("designator.member",(yyvsp[0].text)); std::free((yyvsp[0].text)); }
#line 2934 "tinyc2.tab.c"
    break;

  case 173: /* statement: matched_statement  */
#line 411 "tinyc2.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2940 "tinyc2.tab.c"
    break;

  case 174: /* statement: unmatched_statement  */
#line 412 "tinyc2.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 2946 "tinyc2.tab.c"
    break;

  case 175: /* matched_statement: non_if_statement  */
#line 416 "tinyc2.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 2952 "tinyc2.tab.c"
    break;

  case 176: /* matched_statement: if_matched  */
#line 417 "tinyc2.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2958 "tinyc2.tab.c"
    break;

  case 177: /* unmatched_statement: if_unmatched  */
#line 421 "tinyc2.y"
                   { (yyval.node) = (yyvsp[0].node); }
#line 2964 "tinyc2.tab.c"
    break;

  case 178: /* non_if_statement: labeled_statement  */
#line 425 "tinyc2.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 2970 "tinyc2.tab.c"
    break;

  case 179: /* non_if_statement: compound_statement  */
#line 426 "tinyc2.y"
                         { (yyval.node) = (yyvsp[0].node); }
#line 2976 "tinyc2.tab.c"
    break;

  case 180: /* non_if_statement: expression_statement  */
#line 427 "tinyc2.y"
                           { (yyval.node) = (yyvsp[0].node); }
#line 2982 "tinyc2.tab.c"
    break;

  case 181: /* non_if_statement: selection_statement  */
#line 428 "tinyc2.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 2988 "tinyc2.tab.c"
    break;

  case 182: /* non_if_statement: iteration_statement  */
#line 429 "tinyc2.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 2994 "tinyc2.tab.c"
    break;

  case 183: /* non_if_statement: jump_statement  */
#line 430 "tinyc2.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 3000 "tinyc2.tab.c"
    break;

  case 184: /* labeled_statement: IDENTIFIER ':' statement  */
#line 434 "tinyc2.y"
                               { (yyval.node)=ast_node("label"); ast_add((yyval.node),ast_leaf("identifier",(yyvsp[-2].text))); std::free((yyvsp[-2].text)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3006 "tinyc2.tab.c"
    break;

  case 185: /* labeled_statement: CASE constant_expression ':' statement  */
#line 435 "tinyc2.y"
                                             { (yyval.node)=ast_node("case"); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3012 "tinyc2.tab.c"
    break;

  case 186: /* labeled_statement: DEFAULT ':' statement  */
#line 436 "tinyc2.y"
                            { (yyval.node)=ast_node("default"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3018 "tinyc2.tab.c"
    break;

  case 187: /* compound_statement: '{' block_item_list_opt '}'  */
#line 440 "tinyc2.y"
                                  { (yyval.node)=ast_node("compound-statement"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 3024 "tinyc2.tab.c"
    break;

  case 188: /* block_item_list_opt: %empty  */
#line 444 "tinyc2.y"
             { (yyval.node)=ast_node("block-item-list-opt: epsilon"); }
#line 3030 "tinyc2.tab.c"
    break;

  case 189: /* block_item_list_opt: block_item_list  */
#line 445 "tinyc2.y"
                      { (yyval.node)=(yyvsp[0].node); }
#line 3036 "tinyc2.tab.c"
    break;

  case 190: /* block_item_list: block_item  */
#line 449 "tinyc2.y"
                 { (yyval.node)=ast_node("block-item-list"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3042 "tinyc2.tab.c"
    break;

  case 191: /* block_item_list: block_item_list block_item  */
#line 450 "tinyc2.y"
                                 { (yyval.node)=(yyvsp[-1].node); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3048 "tinyc2.tab.c"
    break;

  case 192: /* block_item: declaration  */
#line 454 "tinyc2.y"
                  { (yyval.node)=ast_node("block-item"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3054 "tinyc2.tab.c"
    break;

  case 193: /* block_item: statement  */
#line 455 "tinyc2.y"
                { (yyval.node)=ast_node("block-item"); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3060 "tinyc2.tab.c"
    break;

  case 194: /* expression_statement: ';'  */
#line 459 "tinyc2.y"
          { (yyval.node)=ast_node("expression-statement: empty"); }
#line 3066 "tinyc2.tab.c"
    break;

  case 195: /* expression_statement: expression ';'  */
#line 460 "tinyc2.y"
                     { (yyval.node)=ast_node("expression-statement"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 3072 "tinyc2.tab.c"
    break;

  case 196: /* if_matched: IF '(' expression ')' matched_statement ELSE matched_statement  */
#line 465 "tinyc2.y"
        { (yyval.node)=ast_node("if-else"); ast_add((yyval.node),(yyvsp[-4].node)); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3078 "tinyc2.tab.c"
    break;

  case 197: /* if_unmatched: IF '(' expression ')' statement  */
#line 470 "tinyc2.y"
        { (yyval.node)=ast_node("if"); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3084 "tinyc2.tab.c"
    break;

  case 198: /* if_unmatched: IF '(' expression ')' matched_statement ELSE unmatched_statement  */
#line 472 "tinyc2.y"
        { (yyval.node)=ast_node("if-else"); ast_add((yyval.node),(yyvsp[-4].node)); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3090 "tinyc2.tab.c"
    break;

  case 199: /* selection_statement: switch_selection  */
#line 476 "tinyc2.y"
                       { (yyval.node) = (yyvsp[0].node); }
#line 3096 "tinyc2.tab.c"
    break;

  case 200: /* switch_selection: SWITCH '(' expression ')' statement  */
#line 480 "tinyc2.y"
                                          { (yyval.node)=ast_node("switch"); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3102 "tinyc2.tab.c"
    break;

  case 201: /* iteration_statement: WHILE '(' expression ')' statement  */
#line 484 "tinyc2.y"
                                         { (yyval.node)=ast_node("while"); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3108 "tinyc2.tab.c"
    break;

  case 202: /* iteration_statement: DO statement WHILE '(' expression ')' ';'  */
#line 485 "tinyc2.y"
                                                { (yyval.node)=ast_node("do-while"); ast_add((yyval.node),(yyvsp[-5].node)); ast_add((yyval.node),(yyvsp[-2].node)); }
#line 3114 "tinyc2.tab.c"
    break;

  case 203: /* iteration_statement: FOR '(' expression_opt ';' expression_opt ';' expression_opt ')' statement  */
#line 486 "tinyc2.y"
                                                                                 { (yyval.node)=ast_node("for"); ast_add((yyval.node),(yyvsp[-6].node)); ast_add((yyval.node),(yyvsp[-4].node)); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3120 "tinyc2.tab.c"
    break;

  case 204: /* iteration_statement: FOR '(' declaration expression_opt ';' expression_opt ')' statement  */
#line 487 "tinyc2.y"
                                                                          { (yyval.node)=ast_node("for-declaration"); ast_add((yyval.node),(yyvsp[-5].node)); ast_add((yyval.node),(yyvsp[-4].node)); ast_add((yyval.node),(yyvsp[-2].node)); ast_add((yyval.node),(yyvsp[0].node)); }
#line 3126 "tinyc2.tab.c"
    break;

  case 205: /* jump_statement: GOTO IDENTIFIER ';'  */
#line 491 "tinyc2.y"
                          { (yyval.node)=ast_node("goto"); ast_add((yyval.node),ast_leaf("identifier",(yyvsp[-1].text))); std::free((yyvsp[-1].text)); }
#line 3132 "tinyc2.tab.c"
    break;

  case 206: /* jump_statement: CONTINUE ';'  */
#line 492 "tinyc2.y"
                   { (yyval.node)=ast_node("continue"); }
#line 3138 "tinyc2.tab.c"
    break;

  case 207: /* jump_statement: BREAK ';'  */
#line 493 "tinyc2.y"
                { (yyval.node)=ast_node("break"); }
#line 3144 "tinyc2.tab.c"
    break;

  case 208: /* jump_statement: RETURN ';'  */
#line 494 "tinyc2.y"
                 { (yyval.node)=ast_node("return"); }
#line 3150 "tinyc2.tab.c"
    break;

  case 209: /* jump_statement: RETURN expression ';'  */
#line 495 "tinyc2.y"
                            { (yyval.node)=ast_node("return"); ast_add((yyval.node),(yyvsp[-1].node)); }
#line 3156 "tinyc2.tab.c"
    break;


#line 3160 "tinyc2.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 498 "tinyc2.y"


void yyerror(const char* s) {
    std::fprintf(stderr, "tinyC2: %s\n", s);
}
