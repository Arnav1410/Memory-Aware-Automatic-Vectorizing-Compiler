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
#line 7 "tinyc3.y"

#include "translator.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace tinyc3;

extern int yylex(void);
extern char *yytext;
extern int yylineno;
void yyerror(const char *s);

#line 85 "tinyc3.tab.c"

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

#include "tinyc3.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENTIFIER = 3,                 /* IDENTIFIER  */
  YYSYMBOL_CONSTANT = 4,                   /* CONSTANT  */
  YYSYMBOL_STRING_LITERAL = 5,             /* STRING_LITERAL  */
  YYSYMBOL_VOID = 6,                       /* VOID  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_INT = 8,                        /* INT  */
  YYSYMBOL_FLOAT = 9,                      /* FLOAT  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_ELSE = 11,                      /* ELSE  */
  YYSYMBOL_WHILE = 12,                     /* WHILE  */
  YYSYMBOL_DO = 13,                        /* DO  */
  YYSYMBOL_FOR = 14,                       /* FOR  */
  YYSYMBOL_RETURN = 15,                    /* RETURN  */
  YYSYMBOL_INC = 16,                       /* INC  */
  YYSYMBOL_DEC = 17,                       /* DEC  */
  YYSYMBOL_SHL = 18,                       /* SHL  */
  YYSYMBOL_SHR = 19,                       /* SHR  */
  YYSYMBOL_LE = 20,                        /* LE  */
  YYSYMBOL_GE = 21,                        /* GE  */
  YYSYMBOL_EQ = 22,                        /* EQ  */
  YYSYMBOL_NE = 23,                        /* NE  */
  YYSYMBOL_ANDAND = 24,                    /* ANDAND  */
  YYSYMBOL_OROR = 25,                      /* OROR  */
  YYSYMBOL_IFX = 26,                       /* IFX  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* ';'  */
  YYSYMBOL_30_ = 30,                       /* '='  */
  YYSYMBOL_31_ = 31,                       /* ','  */
  YYSYMBOL_32_ = 32,                       /* '*'  */
  YYSYMBOL_33_ = 33,                       /* '['  */
  YYSYMBOL_34_ = 34,                       /* ']'  */
  YYSYMBOL_35_ = 35,                       /* '('  */
  YYSYMBOL_36_ = 36,                       /* ')'  */
  YYSYMBOL_37_ = 37,                       /* '&'  */
  YYSYMBOL_38_ = 38,                       /* '+'  */
  YYSYMBOL_39_ = 39,                       /* '-'  */
  YYSYMBOL_40_ = 40,                       /* '~'  */
  YYSYMBOL_41_ = 41,                       /* '!'  */
  YYSYMBOL_42_ = 42,                       /* '/'  */
  YYSYMBOL_43_ = 43,                       /* '%'  */
  YYSYMBOL_44_ = 44,                       /* '<'  */
  YYSYMBOL_45_ = 45,                       /* '>'  */
  YYSYMBOL_46_ = 46,                       /* '^'  */
  YYSYMBOL_47_ = 47,                       /* '|'  */
  YYSYMBOL_48_ = 48,                       /* '?'  */
  YYSYMBOL_49_ = 49,                       /* ':'  */
  YYSYMBOL_YYACCEPT = 50,                  /* $accept  */
  YYSYMBOL_translation_unit = 51,          /* translation_unit  */
  YYSYMBOL_external_declaration = 52,      /* external_declaration  */
  YYSYMBOL_53_1 = 53,                      /* $@1  */
  YYSYMBOL_external_declaration_tail = 54, /* external_declaration_tail  */
  YYSYMBOL_55_2 = 55,                      /* $@2  */
  YYSYMBOL_external_init_tail = 56,        /* external_init_tail  */
  YYSYMBOL_57_3 = 57,                      /* $@3  */
  YYSYMBOL_declaration = 58,               /* declaration  */
  YYSYMBOL_59_4 = 59,                      /* $@4  */
  YYSYMBOL_init_declarator_list_opt = 60,  /* init_declarator_list_opt  */
  YYSYMBOL_init_declarator_list = 61,      /* init_declarator_list  */
  YYSYMBOL_init_declarator = 62,           /* init_declarator  */
  YYSYMBOL_declaration_specifiers = 63,    /* declaration_specifiers  */
  YYSYMBOL_type_specifier = 64,            /* type_specifier  */
  YYSYMBOL_declarator = 65,                /* declarator  */
  YYSYMBOL_pointer_opt = 66,               /* pointer_opt  */
  YYSYMBOL_direct_declarator = 67,         /* direct_declarator  */
  YYSYMBOL_parameter_list_opt = 68,        /* parameter_list_opt  */
  YYSYMBOL_parameter_list = 69,            /* parameter_list  */
  YYSYMBOL_parameter_declaration = 70,     /* parameter_declaration  */
  YYSYMBOL_primary_expression = 71,        /* primary_expression  */
  YYSYMBOL_postfix_expression = 72,        /* postfix_expression  */
  YYSYMBOL_argument_expression_list_opt = 73, /* argument_expression_list_opt  */
  YYSYMBOL_argument_expression_list = 74,  /* argument_expression_list  */
  YYSYMBOL_unary_expression = 75,          /* unary_expression  */
  YYSYMBOL_cast_expression = 76,           /* cast_expression  */
  YYSYMBOL_multiplicative_expression = 77, /* multiplicative_expression  */
  YYSYMBOL_additive_expression = 78,       /* additive_expression  */
  YYSYMBOL_shift_expression = 79,          /* shift_expression  */
  YYSYMBOL_relational_expression = 80,     /* relational_expression  */
  YYSYMBOL_equality_expression = 81,       /* equality_expression  */
  YYSYMBOL_and_expression = 82,            /* and_expression  */
  YYSYMBOL_exclusive_or_expression = 83,   /* exclusive_or_expression  */
  YYSYMBOL_inclusive_or_expression = 84,   /* inclusive_or_expression  */
  YYSYMBOL_logical_and_expression = 85,    /* logical_and_expression  */
  YYSYMBOL_86_5 = 86,                      /* @5  */
  YYSYMBOL_logical_or_expression = 87,     /* logical_or_expression  */
  YYSYMBOL_88_6 = 88,                      /* @6  */
  YYSYMBOL_conditional_expression = 89,    /* conditional_expression  */
  YYSYMBOL_captured_expression = 90,       /* captured_expression  */
  YYSYMBOL_91_7 = 91,                      /* $@7  */
  YYSYMBOL_assignment_expression = 92,     /* assignment_expression  */
  YYSYMBOL_expression = 93,                /* expression  */
  YYSYMBOL_expression_opt = 94,            /* expression_opt  */
  YYSYMBOL_initializer = 95,               /* initializer  */
  YYSYMBOL_marker = 96,                    /* marker  */
  YYSYMBOL_condition_expression = 97,      /* condition_expression  */
  YYSYMBOL_for_condition_opt = 98,         /* for_condition_opt  */
  YYSYMBOL_statement = 99,                 /* statement  */
  YYSYMBOL_compound_statement = 100,       /* compound_statement  */
  YYSYMBOL_block_scope_begin = 101,        /* block_scope_begin  */
  YYSYMBOL_block_scope_end = 102,          /* block_scope_end  */
  YYSYMBOL_block_item_list_opt = 103,      /* block_item_list_opt  */
  YYSYMBOL_block_item_list = 104,          /* block_item_list  */
  YYSYMBOL_block_item = 105,               /* block_item  */
  YYSYMBOL_expression_statement = 106,     /* expression_statement  */
  YYSYMBOL_selection_statement = 107,      /* selection_statement  */
  YYSYMBOL_captured_statement = 108,       /* captured_statement  */
  YYSYMBOL_109_8 = 109,                    /* $@8  */
  YYSYMBOL_iteration_statement = 110,      /* iteration_statement  */
  YYSYMBOL_for_update_clause = 111,        /* for_update_clause  */
  YYSYMBOL_112_9 = 112,                    /* $@9  */
  YYSYMBOL_for_update_expression_opt = 113, /* for_update_expression_opt  */
  YYSYMBOL_jump_statement = 114            /* jump_statement  */
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
typedef yytype_uint8 yy_state_t;

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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   271

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  50
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  65
/* YYNRULES -- Number of rules.  */
#define YYNRULES  132
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  214

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


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
       2,     2,     2,    41,     2,     2,     2,    43,    37,     2,
      35,    36,    32,    38,    31,    39,     2,    42,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    49,    29,
      44,    30,    45,    48,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,    46,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,    47,    28,    40,     2,     2,     2,
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
      25,    26
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    63,    63,    64,    72,    71,    88,    87,    95,    99,
     103,   109,   108,   117,   116,   127,   128,   132,   133,   137,
     142,   151,   155,   156,   157,   158,   162,   171,   176,   184,
     190,   196,   206,   210,   217,   223,   232,   242,   247,   252,
     257,   264,   268,   274,   281,   285,   292,   296,   303,   308,
     316,   320,   324,   328,   333,   338,   343,   348,   353,   361,
     365,   366,   368,   370,   375,   376,   378,   383,   384,   386,
     391,   392,   394,   396,   398,   403,   404,   406,   411,   412,
     417,   418,   423,   424,   429,   434,   433,   459,   464,   463,
     489,   493,   503,   503,   520,   524,   533,   537,   538,   542,
     546,   554,   563,   570,   577,   578,   579,   580,   581,   585,
     589,   597,   605,   606,   610,   611,   615,   616,   620,   621,
     635,   647,   670,   670,   679,   690,   699,   717,   717,   738,
     739,   743,   750
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENTIFIER",
  "CONSTANT", "STRING_LITERAL", "VOID", "CHAR", "INT", "FLOAT", "IF",
  "ELSE", "WHILE", "DO", "FOR", "RETURN", "INC", "DEC", "SHL", "SHR", "LE",
  "GE", "EQ", "NE", "ANDAND", "OROR", "IFX", "'{'", "'}'", "';'", "'='",
  "','", "'*'", "'['", "']'", "'('", "')'", "'&'", "'+'", "'-'", "'~'",
  "'!'", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "$accept",
  "translation_unit", "external_declaration", "$@1",
  "external_declaration_tail", "$@2", "external_init_tail", "$@3",
  "declaration", "$@4", "init_declarator_list_opt", "init_declarator_list",
  "init_declarator", "declaration_specifiers", "type_specifier",
  "declarator", "pointer_opt", "direct_declarator", "parameter_list_opt",
  "parameter_list", "parameter_declaration", "primary_expression",
  "postfix_expression", "argument_expression_list_opt",
  "argument_expression_list", "unary_expression", "cast_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "and_expression",
  "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "@5", "logical_or_expression", "@6",
  "conditional_expression", "captured_expression", "$@7",
  "assignment_expression", "expression", "expression_opt", "initializer",
  "marker", "condition_expression", "for_condition_opt", "statement",
  "compound_statement", "block_scope_begin", "block_scope_end",
  "block_item_list_opt", "block_item_list", "block_item",
  "expression_statement", "selection_statement", "captured_statement",
  "$@8", "iteration_statement", "for_update_clause", "$@9",
  "for_update_expression_opt", "jump_statement", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-157)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-28)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      46,  -157,  -157,  -157,  -157,    22,  -157,   -15,  -157,  -157,
    -157,   -15,  -157,    20,  -157,    60,  -157,    70,  -157,    81,
    -157,  -157,    -3,    32,    46,   191,  -157,  -157,  -157,    81,
      81,    81,    81,    81,    81,    81,    81,    81,  -157,    27,
      12,  -157,     8,    73,    90,     4,    92,    34,    13,    41,
      50,    -9,  -157,  -157,  -157,   -15,  -157,    59,   -15,    95,
      65,  -157,    82,    97,  -157,    98,    29,  -157,  -157,  -157,
    -157,  -157,   106,  -157,  -157,   108,   191,  -157,  -157,  -157,
    -157,  -157,  -157,  -157,  -157,  -157,   101,  -157,  -157,  -157,
    -157,  -157,  -157,  -157,    81,    81,    81,    81,    81,    81,
      81,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    81,    81,  -157,  -157,  -157,   107,  -157,   109,  -157,
    -157,  -157,    46,    81,  -157,   230,    81,  -157,   112,   191,
       9,  -157,  -157,  -157,  -157,   111,   113,   116,  -157,  -157,
    -157,  -157,  -157,     8,     8,    73,    73,    90,    90,    90,
      90,     4,     4,    92,    34,    13,    81,    81,    94,    81,
     -15,    81,  -157,  -157,   114,    81,   136,  -157,   122,  -157,
     124,   125,   107,  -157,  -157,    81,    41,    50,  -157,  -157,
    -157,  -157,  -157,   117,   120,  -157,  -157,  -157,  -157,  -157,
     146,   230,  -157,    81,    81,  -157,  -157,  -157,   230,   123,
    -157,   131,  -157,  -157,   132,  -157,  -157,   128,    81,  -157,
    -157,  -157,   230,  -157
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    22,    23,    24,    25,     0,     2,    27,    21,     1,
       3,    27,     4,     0,    28,     9,    29,    26,     6,     0,
      11,     5,     0,     0,    32,   112,    37,    38,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    41,    50,
      59,    60,    64,    67,    70,    75,    78,    80,    82,    84,
      87,    90,    94,    99,    10,    27,     8,     0,    27,     0,
      33,    34,     0,     0,   100,     0,     0,   110,   118,   116,
      13,    96,     0,   117,   104,     0,   113,   114,   105,   106,
     107,   108,    51,    52,    59,    54,     0,    53,    55,    56,
      57,    58,    44,    45,     0,    46,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    85,    88,    92,    12,    17,    19,    30,
      36,    31,     0,     0,   100,     0,    97,   131,     0,   112,
      15,   119,     7,   115,    40,     0,     0,    47,    48,    95,
      61,    62,    63,    65,    66,    68,    69,    73,    74,    71,
      72,    76,    77,    79,    81,    83,     0,     0,     0,     0,
      27,     0,    35,   101,     0,     0,     0,    98,     0,   132,
       0,     0,    16,    42,    43,     0,    86,    89,    92,    93,
      18,    20,   122,     0,     0,   100,   111,    14,    49,    91,
     120,     0,   100,     0,   102,   109,   122,   123,     0,     0,
     103,     0,   121,   124,     0,   127,   125,     0,   129,   100,
     130,   128,     0,   126
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -157,  -157,   160,  -157,  -157,  -157,  -157,  -157,  -157,  -157,
    -157,    36,     7,     3,  -157,    -2,   157,  -157,  -157,  -157,
      47,  -157,  -157,  -157,  -157,   -19,   -16,    23,    24,    -5,
      21,    61,    62,    58,    25,    19,  -157,  -157,  -157,  -157,
       0,  -157,   -17,   -31,  -157,    26,  -120,  -156,  -157,  -118,
    -157,  -157,  -157,    51,  -157,   110,  -157,  -157,   -14,  -157,
    -157,  -157,  -157,  -157,  -157
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,    15,    21,    25,    22,    55,    69,   130,
     171,   116,   117,    70,     8,   118,    13,    17,    59,    60,
      61,    38,    39,   136,   137,    84,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,   156,    51,   157,    52,
     158,   159,    71,    72,   168,    54,   125,   164,   201,    73,
      74,   129,   195,    75,    76,    77,    78,    79,   190,   191,
      80,   207,   208,   211,    81
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,    86,    53,     7,   165,    12,    40,   166,     7,   183,
      82,    83,   -27,    40,    87,    85,   114,    11,    88,    89,
      90,    91,     9,    16,   104,   105,    56,    58,     1,     2,
       3,     4,    26,    27,    28,   128,    57,   199,   200,   115,
      97,    11,    96,    92,    93,    29,    30,    40,   106,   107,
      98,    99,     1,     2,     3,     4,   120,    40,   127,   111,
      94,    31,    95,   135,    32,   194,    33,    34,    35,    36,
      37,   110,   198,   197,   113,    40,    40,    40,   138,   139,
     203,   140,   141,   142,    26,    27,    28,    18,   112,   212,
      19,    20,   163,   119,   213,   167,   122,    29,    30,   147,
     148,   149,   150,    23,    40,    24,    40,    40,   102,   103,
      40,   100,   101,    31,   108,   109,    32,   123,    33,    34,
      35,    36,    37,   143,   144,    58,   145,   146,   179,   151,
     152,   121,   124,   126,   163,   131,   132,   134,   160,   161,
      40,   169,    40,   178,    53,   173,    40,   175,   184,   174,
     182,   185,   186,   192,   187,   193,    40,   196,   188,   204,
     205,   206,   163,   163,   209,    10,   172,   180,    14,   162,
     155,   153,    40,   154,    40,    40,   177,   210,   189,    40,
     170,   176,   202,     0,     0,     0,   133,   181,     0,    40,
       0,     0,     0,    40,    26,    27,    28,     1,     2,     3,
       4,    62,     0,    63,    64,    65,    66,    29,    30,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,     0,
      68,     0,     0,    31,     0,     0,    32,     0,    33,    34,
      35,    36,    37,    26,    27,    28,     0,     0,     0,     0,
      62,     0,    63,    64,    65,    66,    29,    30,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    67,     0,    68,
       0,     0,    31,     0,     0,    32,     0,    33,    34,    35,
      36,    37
};

static const yytype_int16 yycheck[] =
{
      19,    32,    19,     0,   124,     7,    25,   125,     5,   165,
      29,    30,     3,    32,    33,    31,    25,    32,    34,    35,
      36,    37,     0,     3,    20,    21,    29,    24,     6,     7,
       8,     9,     3,     4,     5,    66,     4,   193,   194,    48,
      32,    32,    30,    16,    17,    16,    17,    66,    44,    45,
      42,    43,     6,     7,     8,     9,    58,    76,    29,    46,
      33,    32,    35,    94,    35,   185,    37,    38,    39,    40,
      41,    37,   192,   191,    24,    94,    95,    96,    95,    96,
     198,    97,    98,    99,     3,     4,     5,    27,    47,   209,
      30,    31,   123,    34,   212,   126,    31,    16,    17,   104,
     105,   106,   107,    33,   123,    35,   125,   126,    18,    19,
     129,    38,    39,    32,    22,    23,    35,    35,    37,    38,
      39,    40,    41,   100,   101,   122,   102,   103,   159,   108,
     109,    36,    35,    35,   165,    29,    28,    36,    31,    30,
     159,    29,   161,    49,   161,    34,   165,    31,    12,    36,
      36,    29,    28,    36,    29,    35,   175,    11,   175,    36,
      29,    29,   193,   194,    36,     5,   130,   160,    11,   122,
     112,   110,   191,   111,   193,   194,   157,   208,   178,   198,
     129,   156,   196,    -1,    -1,    -1,    76,   161,    -1,   208,
      -1,    -1,    -1,   212,     3,     4,     5,     6,     7,     8,
       9,    10,    -1,    12,    13,    14,    15,    16,    17,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,
      29,    -1,    -1,    32,    -1,    -1,    35,    -1,    37,    38,
      39,    40,    41,     3,     4,     5,    -1,    -1,    -1,    -1,
      10,    -1,    12,    13,    14,    15,    16,    17,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    27,    -1,    29,
      -1,    -1,    32,    -1,    -1,    35,    -1,    37,    38,    39,
      40,    41
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     6,     7,     8,     9,    51,    52,    63,    64,     0,
      52,    32,    65,    66,    66,    53,     3,    67,    27,    30,
      31,    54,    56,    33,    35,    55,     3,     4,     5,    16,
      17,    32,    35,    37,    38,    39,    40,    41,    71,    72,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    87,    89,    92,    95,    57,    29,     4,    63,    68,
      69,    70,    10,    12,    13,    14,    15,    27,    29,    58,
      63,    92,    93,    99,   100,   103,   104,   105,   106,   107,
     110,   114,    75,    75,    75,    76,    93,    75,    76,    76,
      76,    76,    16,    17,    33,    35,    30,    32,    42,    43,
      38,    39,    18,    19,    20,    21,    44,    45,    22,    23,
      37,    46,    47,    24,    25,    48,    61,    62,    65,    34,
      65,    36,    31,    35,    35,    96,    35,    29,    93,   101,
      59,    29,    28,   105,    36,    93,    73,    74,    92,    92,
      76,    76,    76,    77,    77,    78,    78,    79,    79,    79,
      79,    80,    80,    81,    82,    83,    86,    88,    90,    91,
      31,    30,    70,    93,    97,    96,    99,    93,    94,    29,
     103,    60,    61,    34,    36,    31,    84,    85,    49,    93,
      62,    95,    36,    97,    12,    29,    28,    29,    92,    90,
     108,   109,    36,    35,    96,   102,    11,    99,    96,    97,
      97,    98,   108,    99,    36,    29,    29,   111,   112,    36,
      93,   113,    96,    99
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    50,    51,    51,    53,    52,    55,    54,    54,    56,
      56,    57,    56,    59,    58,    60,    60,    61,    61,    62,
      62,    63,    64,    64,    64,    64,    65,    66,    66,    67,
      67,    67,    68,    68,    69,    69,    70,    71,    71,    71,
      71,    72,    72,    72,    72,    72,    73,    73,    74,    74,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    76,
      77,    77,    77,    77,    78,    78,    78,    79,    79,    79,
      80,    80,    80,    80,    80,    81,    81,    81,    82,    82,
      83,    83,    84,    84,    85,    86,    85,    87,    88,    87,
      89,    89,    91,    90,    92,    92,    93,    94,    94,    95,
      96,    97,    98,    98,    99,    99,    99,    99,    99,   100,
     101,   102,   103,   103,   104,   104,   105,   105,   106,   106,
     107,   107,   109,   108,   110,   110,   110,   112,   111,   113,
     113,   114,   114
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     4,     0,     4,     2,     0,
       2,     0,     3,     0,     4,     0,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     2,     0,     2,     1,
       4,     4,     0,     1,     1,     3,     2,     1,     1,     1,
       3,     1,     4,     4,     2,     2,     0,     1,     1,     3,
       1,     2,     2,     2,     2,     2,     2,     2,     2,     1,
       1,     3,     3,     3,     1,     3,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       1,     3,     1,     3,     1,     0,     4,     1,     0,     4,
       1,     5,     0,     2,     1,     3,     1,     0,     1,     1,
       0,     1,     0,     1,     1,     1,     1,     1,     1,     5,
       0,     0,     0,     1,     1,     2,     1,     1,     1,     2,
       5,     7,     0,     2,     7,     8,    11,     0,     2,     0,
       1,     2,     3
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
#line 63 "tinyc3.y"
                           { }
#line 1367 "tinyc3.tab.c"
    break;

  case 3: /* translation_unit: translation_unit external_declaration  */
#line 64 "tinyc3.y"
                                            { }
#line 1373 "tinyc3.tab.c"
    break;

  case 4: /* $@1: %empty  */
#line 72 "tinyc3.y"
      {
          gExternalBase = gTranslator->baseType((yyvsp[-1].base));
          gPendingDeclarationBase = gExternalBase;
          gExternalDecl = (yyvsp[0].decl);
      }
#line 1383 "tinyc3.tab.c"
    break;

  case 5: /* external_declaration: declaration_specifiers declarator $@1 external_declaration_tail  */
#line 78 "tinyc3.y"
      {
          delete gExternalDecl;
          gExternalDecl = NULL;
          gExternalBase = NULL;
          gPendingDeclarationBase = NULL;
      }
#line 1394 "tinyc3.tab.c"
    break;

  case 6: /* $@2: %empty  */
#line 88 "tinyc3.y"
      {
          gTranslator->beginFunction(*gExternalDecl, gExternalBase);
      }
#line 1402 "tinyc3.tab.c"
    break;

  case 7: /* external_declaration_tail: '{' $@2 block_item_list_opt '}'  */
#line 92 "tinyc3.y"
      {
          gTranslator->endFunction();
      }
#line 1410 "tinyc3.tab.c"
    break;

  case 8: /* external_declaration_tail: external_init_tail ';'  */
#line 95 "tinyc3.y"
                             { }
#line 1416 "tinyc3.tab.c"
    break;

  case 9: /* external_init_tail: %empty  */
#line 100 "tinyc3.y"
      {
          gTranslator->install(*gExternalDecl, gExternalBase, NULL);
      }
#line 1424 "tinyc3.tab.c"
    break;

  case 10: /* external_init_tail: '=' initializer  */
#line 104 "tinyc3.y"
      {
          gTranslator->install(*gExternalDecl, gExternalBase, (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1433 "tinyc3.tab.c"
    break;

  case 11: /* $@3: %empty  */
#line 109 "tinyc3.y"
      {
          gTranslator->install(*gExternalDecl, gExternalBase, NULL);
      }
#line 1441 "tinyc3.tab.c"
    break;

  case 12: /* external_init_tail: ',' $@3 init_declarator_list  */
#line 112 "tinyc3.y"
                           { }
#line 1447 "tinyc3.tab.c"
    break;

  case 13: /* $@4: %empty  */
#line 117 "tinyc3.y"
      {
          gPendingDeclarationBase = gTranslator->baseType((yyvsp[0].base));
      }
#line 1455 "tinyc3.tab.c"
    break;

  case 14: /* declaration: declaration_specifiers $@4 init_declarator_list_opt ';'  */
#line 121 "tinyc3.y"
      {
          gPendingDeclarationBase = NULL;
      }
#line 1463 "tinyc3.tab.c"
    break;

  case 15: /* init_declarator_list_opt: %empty  */
#line 127 "tinyc3.y"
             { }
#line 1469 "tinyc3.tab.c"
    break;

  case 16: /* init_declarator_list_opt: init_declarator_list  */
#line 128 "tinyc3.y"
                           { }
#line 1475 "tinyc3.tab.c"
    break;

  case 17: /* init_declarator_list: init_declarator  */
#line 132 "tinyc3.y"
                      { }
#line 1481 "tinyc3.tab.c"
    break;

  case 18: /* init_declarator_list: init_declarator_list ',' init_declarator  */
#line 133 "tinyc3.y"
                                               { }
#line 1487 "tinyc3.tab.c"
    break;

  case 19: /* init_declarator: declarator  */
#line 138 "tinyc3.y"
      {
          gTranslator->install(*(yyvsp[0].decl), gPendingDeclarationBase, NULL);
          delete (yyvsp[0].decl);
      }
#line 1496 "tinyc3.tab.c"
    break;

  case 20: /* init_declarator: declarator '=' initializer  */
#line 143 "tinyc3.y"
      {
          gTranslator->install(*(yyvsp[-2].decl), gPendingDeclarationBase, (yyvsp[0].expr));
          delete (yyvsp[-2].decl);
          delete (yyvsp[0].expr);
      }
#line 1506 "tinyc3.tab.c"
    break;

  case 21: /* declaration_specifiers: type_specifier  */
#line 151 "tinyc3.y"
                     { (yyval.base) = (yyvsp[0].base); }
#line 1512 "tinyc3.tab.c"
    break;

  case 22: /* type_specifier: VOID  */
#line 155 "tinyc3.y"
            { (yyval.base) = tinyc::TK_VOID; }
#line 1518 "tinyc3.tab.c"
    break;

  case 23: /* type_specifier: CHAR  */
#line 156 "tinyc3.y"
            { (yyval.base) = tinyc::TK_CHAR; }
#line 1524 "tinyc3.tab.c"
    break;

  case 24: /* type_specifier: INT  */
#line 157 "tinyc3.y"
            { (yyval.base) = tinyc::TK_INT; }
#line 1530 "tinyc3.tab.c"
    break;

  case 25: /* type_specifier: FLOAT  */
#line 158 "tinyc3.y"
            { (yyval.base) = tinyc::TK_FLOAT; }
#line 1536 "tinyc3.tab.c"
    break;

  case 26: /* declarator: pointer_opt direct_declarator  */
#line 163 "tinyc3.y"
      {
          (yyval.decl) = (yyvsp[0].decl);
          (yyval.decl)->pointerDepth += (yyvsp[-1].decl)->pointerDepth;
          delete (yyvsp[-1].decl);
      }
#line 1546 "tinyc3.tab.c"
    break;

  case 27: /* pointer_opt: %empty  */
#line 172 "tinyc3.y"
      {
          (yyval.decl) = new DeclInfo();
          (yyval.decl)->pointerDepth = 0;
      }
#line 1555 "tinyc3.tab.c"
    break;

  case 28: /* pointer_opt: '*' pointer_opt  */
#line 177 "tinyc3.y"
      {
          (yyval.decl) = (yyvsp[0].decl);
          ++(yyval.decl)->pointerDepth;
      }
#line 1564 "tinyc3.tab.c"
    break;

  case 29: /* direct_declarator: IDENTIFIER  */
#line 185 "tinyc3.y"
      {
          (yyval.decl) = new DeclInfo();
          (yyval.decl)->name = (yyvsp[0].text);
          std::free((yyvsp[0].text));
      }
#line 1574 "tinyc3.tab.c"
    break;

  case 30: /* direct_declarator: direct_declarator '[' CONSTANT ']'  */
#line 191 "tinyc3.y"
      {
          (yyval.decl) = (yyvsp[-3].decl);
          (yyval.decl)->arrayDims.push_back((std::size_t)std::stoull((yyvsp[-1].text)));
          std::free((yyvsp[-1].text));
      }
#line 1584 "tinyc3.tab.c"
    break;

  case 31: /* direct_declarator: direct_declarator '(' parameter_list_opt ')'  */
#line 197 "tinyc3.y"
      {
          (yyval.decl) = (yyvsp[-3].decl);
          (yyval.decl)->isFunction = true;
          (yyval.decl)->params = *(yyvsp[-1].params);
          delete (yyvsp[-1].params);
      }
#line 1595 "tinyc3.tab.c"
    break;

  case 32: /* parameter_list_opt: %empty  */
#line 207 "tinyc3.y"
      {
          (yyval.params) = new std::vector<ParamInfo>();
      }
#line 1603 "tinyc3.tab.c"
    break;

  case 33: /* parameter_list_opt: parameter_list  */
#line 211 "tinyc3.y"
      {
          (yyval.params) = (yyvsp[0].params);
      }
#line 1611 "tinyc3.tab.c"
    break;

  case 34: /* parameter_list: parameter_declaration  */
#line 218 "tinyc3.y"
      {
          (yyval.params) = new std::vector<ParamInfo>();
          (yyval.params)->push_back(*(yyvsp[0].param));
          delete (yyvsp[0].param);
      }
#line 1621 "tinyc3.tab.c"
    break;

  case 35: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 224 "tinyc3.y"
      {
          (yyval.params) = (yyvsp[-2].params);
          (yyval.params)->push_back(*(yyvsp[0].param));
          delete (yyvsp[0].param);
      }
#line 1631 "tinyc3.tab.c"
    break;

  case 36: /* parameter_declaration: declaration_specifiers declarator  */
#line 233 "tinyc3.y"
      {
          (yyval.param) = new ParamInfo();
          (yyval.param)->name = (yyvsp[0].decl)->name;
          (yyval.param)->type = gTranslator->buildDeclaredType(gTranslator->baseType((yyvsp[-1].base)), *(yyvsp[0].decl));
          delete (yyvsp[0].decl);
      }
#line 1642 "tinyc3.tab.c"
    break;

  case 37: /* primary_expression: IDENTIFIER  */
#line 243 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->identifier((yyvsp[0].text));
          std::free((yyvsp[0].text));
      }
#line 1651 "tinyc3.tab.c"
    break;

  case 38: /* primary_expression: CONSTANT  */
#line 248 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->constant((yyvsp[0].text));
          std::free((yyvsp[0].text));
      }
#line 1660 "tinyc3.tab.c"
    break;

  case 39: /* primary_expression: STRING_LITERAL  */
#line 253 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->constant((yyvsp[0].text));
          std::free((yyvsp[0].text));
      }
#line 1669 "tinyc3.tab.c"
    break;

  case 40: /* primary_expression: '(' expression ')'  */
#line 258 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[-1].expr);
      }
#line 1677 "tinyc3.tab.c"
    break;

  case 41: /* postfix_expression: primary_expression  */
#line 265 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 1685 "tinyc3.tab.c"
    break;

  case 42: /* postfix_expression: postfix_expression '[' expression ']'  */
#line 269 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->index((yyvsp[-3].expr), (yyvsp[-1].expr));
          delete (yyvsp[-3].expr);
          delete (yyvsp[-1].expr);
      }
#line 1695 "tinyc3.tab.c"
    break;

  case 43: /* postfix_expression: postfix_expression '(' argument_expression_list_opt ')'  */
#line 275 "tinyc3.y"
      {
          std::vector<ExprAttr*> args = (yyvsp[-1].exprs)->items;
          (yyval.expr) = gTranslator->call((yyvsp[-3].expr), args);
          delete (yyvsp[-3].expr);
          delete (yyvsp[-1].exprs);
      }
#line 1706 "tinyc3.tab.c"
    break;

  case 44: /* postfix_expression: postfix_expression INC  */
#line 282 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->incdec((yyvsp[-1].expr), true, false);
      }
#line 1714 "tinyc3.tab.c"
    break;

  case 45: /* postfix_expression: postfix_expression DEC  */
#line 286 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->incdec((yyvsp[-1].expr), false, false);
      }
#line 1722 "tinyc3.tab.c"
    break;

  case 46: /* argument_expression_list_opt: %empty  */
#line 293 "tinyc3.y"
      {
          (yyval.exprs) = new ExprList();
      }
#line 1730 "tinyc3.tab.c"
    break;

  case 47: /* argument_expression_list_opt: argument_expression_list  */
#line 297 "tinyc3.y"
      {
          (yyval.exprs) = (yyvsp[0].exprs);
      }
#line 1738 "tinyc3.tab.c"
    break;

  case 48: /* argument_expression_list: assignment_expression  */
#line 304 "tinyc3.y"
      {
          (yyval.exprs) = new ExprList();
          (yyval.exprs)->items.push_back((yyvsp[0].expr));
      }
#line 1747 "tinyc3.tab.c"
    break;

  case 49: /* argument_expression_list: argument_expression_list ',' assignment_expression  */
#line 309 "tinyc3.y"
      {
          (yyval.exprs) = (yyvsp[-2].exprs);
          (yyval.exprs)->items.push_back((yyvsp[0].expr));
      }
#line 1756 "tinyc3.tab.c"
    break;

  case 50: /* unary_expression: postfix_expression  */
#line 317 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 1764 "tinyc3.tab.c"
    break;

  case 51: /* unary_expression: INC unary_expression  */
#line 321 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->incdec((yyvsp[0].expr), true, true);
      }
#line 1772 "tinyc3.tab.c"
    break;

  case 52: /* unary_expression: DEC unary_expression  */
#line 325 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->incdec((yyvsp[0].expr), false, true);
      }
#line 1780 "tinyc3.tab.c"
    break;

  case 53: /* unary_expression: '&' unary_expression  */
#line 329 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->unary("&", (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1789 "tinyc3.tab.c"
    break;

  case 54: /* unary_expression: '*' cast_expression  */
#line 334 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->unary("*", (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1798 "tinyc3.tab.c"
    break;

  case 55: /* unary_expression: '+' cast_expression  */
#line 339 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->unary("+", (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1807 "tinyc3.tab.c"
    break;

  case 56: /* unary_expression: '-' cast_expression  */
#line 344 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->unary("-", (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1816 "tinyc3.tab.c"
    break;

  case 57: /* unary_expression: '~' cast_expression  */
#line 349 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->unary("~", (yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1825 "tinyc3.tab.c"
    break;

  case 58: /* unary_expression: '!' cast_expression  */
#line 354 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->logicalNot((yyvsp[0].expr));
          delete (yyvsp[0].expr);
      }
#line 1834 "tinyc3.tab.c"
    break;

  case 59: /* cast_expression: unary_expression  */
#line 361 "tinyc3.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 1840 "tinyc3.tab.c"
    break;

  case 60: /* multiplicative_expression: cast_expression  */
#line 365 "tinyc3.y"
                      { (yyval.expr) = (yyvsp[0].expr); }
#line 1846 "tinyc3.tab.c"
    break;

  case 61: /* multiplicative_expression: multiplicative_expression '*' cast_expression  */
#line 367 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("*", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1852 "tinyc3.tab.c"
    break;

  case 62: /* multiplicative_expression: multiplicative_expression '/' cast_expression  */
#line 369 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("/", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1858 "tinyc3.tab.c"
    break;

  case 63: /* multiplicative_expression: multiplicative_expression '%' cast_expression  */
#line 371 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("%", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1864 "tinyc3.tab.c"
    break;

  case 64: /* additive_expression: multiplicative_expression  */
#line 375 "tinyc3.y"
                                { (yyval.expr) = (yyvsp[0].expr); }
#line 1870 "tinyc3.tab.c"
    break;

  case 65: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 377 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("+", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1876 "tinyc3.tab.c"
    break;

  case 66: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 379 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("-", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1882 "tinyc3.tab.c"
    break;

  case 67: /* shift_expression: additive_expression  */
#line 383 "tinyc3.y"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 1888 "tinyc3.tab.c"
    break;

  case 68: /* shift_expression: shift_expression SHL additive_expression  */
#line 385 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("<<", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1894 "tinyc3.tab.c"
    break;

  case 69: /* shift_expression: shift_expression SHR additive_expression  */
#line 387 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary(">>", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1900 "tinyc3.tab.c"
    break;

  case 70: /* relational_expression: shift_expression  */
#line 391 "tinyc3.y"
                       { (yyval.expr) = (yyvsp[0].expr); }
#line 1906 "tinyc3.tab.c"
    break;

  case 71: /* relational_expression: relational_expression '<' shift_expression  */
#line 393 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational("<", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1912 "tinyc3.tab.c"
    break;

  case 72: /* relational_expression: relational_expression '>' shift_expression  */
#line 395 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational(">", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1918 "tinyc3.tab.c"
    break;

  case 73: /* relational_expression: relational_expression LE shift_expression  */
#line 397 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational("<=", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1924 "tinyc3.tab.c"
    break;

  case 74: /* relational_expression: relational_expression GE shift_expression  */
#line 399 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational(">=", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1930 "tinyc3.tab.c"
    break;

  case 75: /* equality_expression: relational_expression  */
#line 403 "tinyc3.y"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 1936 "tinyc3.tab.c"
    break;

  case 76: /* equality_expression: equality_expression EQ relational_expression  */
#line 405 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational("==", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1942 "tinyc3.tab.c"
    break;

  case 77: /* equality_expression: equality_expression NE relational_expression  */
#line 407 "tinyc3.y"
      { (yyval.expr) = gTranslator->relational("!=", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1948 "tinyc3.tab.c"
    break;

  case 78: /* and_expression: equality_expression  */
#line 411 "tinyc3.y"
                          { (yyval.expr) = (yyvsp[0].expr); }
#line 1954 "tinyc3.tab.c"
    break;

  case 79: /* and_expression: and_expression '&' equality_expression  */
#line 413 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("&", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1960 "tinyc3.tab.c"
    break;

  case 80: /* exclusive_or_expression: and_expression  */
#line 417 "tinyc3.y"
                     { (yyval.expr) = (yyvsp[0].expr); }
#line 1966 "tinyc3.tab.c"
    break;

  case 81: /* exclusive_or_expression: exclusive_or_expression '^' and_expression  */
#line 419 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("^", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1972 "tinyc3.tab.c"
    break;

  case 82: /* inclusive_or_expression: exclusive_or_expression  */
#line 423 "tinyc3.y"
                              { (yyval.expr) = (yyvsp[0].expr); }
#line 1978 "tinyc3.tab.c"
    break;

  case 83: /* inclusive_or_expression: inclusive_or_expression '|' exclusive_or_expression  */
#line 425 "tinyc3.y"
      { (yyval.expr) = gTranslator->binary("|", (yyvsp[-2].expr), (yyvsp[0].expr)); delete (yyvsp[-2].expr); delete (yyvsp[0].expr); }
#line 1984 "tinyc3.tab.c"
    break;

  case 84: /* logical_and_expression: inclusive_or_expression  */
#line 430 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 1992 "tinyc3.tab.c"
    break;

  case 85: /* @5: %empty  */
#line 434 "tinyc3.y"
      {
          (yyval.lctx) = new LogicalContext();
          (yyval.lctx)->lhs = gTranslator->ensureCondition((yyvsp[-1].expr));
          if ((yyval.lctx)->lhs != (yyvsp[-1].expr)) delete (yyvsp[-1].expr);
          (yyval.lctx)->label = gTranslator->markLabel();
      }
#line 2003 "tinyc3.tab.c"
    break;

  case 86: /* logical_and_expression: logical_and_expression ANDAND @5 inclusive_or_expression  */
#line 441 "tinyc3.y"
      {
          LogicalContext *ctx = (yyvsp[-1].lctx);
          ExprAttr *rhs = gTranslator->ensureCondition((yyvsp[0].expr));
          gTranslator->backpatch(ctx->lhs->trueList, ctx->label);

          (yyval.expr) = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          (yyval.expr)->isBool = true;
          (yyval.expr)->trueList = rhs->trueList;
          (yyval.expr)->falseList = gTranslator->merge(ctx->lhs->falseList, rhs->falseList);

          delete ctx->lhs;
          delete ctx;
          if (rhs != (yyvsp[0].expr)) delete (yyvsp[0].expr);
          delete rhs;
      }
#line 2023 "tinyc3.tab.c"
    break;

  case 87: /* logical_or_expression: logical_and_expression  */
#line 460 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 2031 "tinyc3.tab.c"
    break;

  case 88: /* @6: %empty  */
#line 464 "tinyc3.y"
      {
          (yyval.lctx) = new LogicalContext();
          (yyval.lctx)->lhs = gTranslator->ensureCondition((yyvsp[-1].expr));
          if ((yyval.lctx)->lhs != (yyvsp[-1].expr)) delete (yyvsp[-1].expr);
          (yyval.lctx)->label = gTranslator->markLabel();
      }
#line 2042 "tinyc3.tab.c"
    break;

  case 89: /* logical_or_expression: logical_or_expression OROR @6 logical_and_expression  */
#line 471 "tinyc3.y"
      {
          LogicalContext *ctx = (yyvsp[-1].lctx);
          ExprAttr *rhs = gTranslator->ensureCondition((yyvsp[0].expr));
          gTranslator->backpatch(ctx->lhs->falseList, ctx->label);

          (yyval.expr) = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          (yyval.expr)->isBool = true;
          (yyval.expr)->trueList = gTranslator->merge(ctx->lhs->trueList, rhs->trueList);
          (yyval.expr)->falseList = rhs->falseList;

          delete ctx->lhs;
          delete ctx;
          if (rhs != (yyvsp[0].expr)) delete (yyvsp[0].expr);
          delete rhs;
      }
#line 2062 "tinyc3.tab.c"
    break;

  case 90: /* conditional_expression: logical_or_expression  */
#line 490 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 2070 "tinyc3.tab.c"
    break;

  case 91: /* conditional_expression: logical_or_expression '?' captured_expression ':' captured_expression  */
#line 494 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->conditionalCaptured((yyvsp[-4].expr), (yyvsp[-2].capexpr), (yyvsp[0].capexpr));
          delete (yyvsp[-4].expr);
          delete (yyvsp[-2].capexpr);
          delete (yyvsp[0].capexpr);
      }
#line 2081 "tinyc3.tab.c"
    break;

  case 92: /* $@7: %empty  */
#line 503 "tinyc3.y"
      { gTranslator->beginCapture(); }
#line 2087 "tinyc3.tab.c"
    break;

  case 93: /* captured_expression: $@7 expression  */
#line 504 "tinyc3.y"
      {
          ExprAttr *e = (yyvsp[0].expr);
          if (e && e->isBool) {
              ExprAttr *v = gTranslator->materializeBool(e);
              if (v != e) delete e;
              e = v;
          }

          CodeBuffer *code = gTranslator->endCapture();
          (yyval.capexpr) = new CapturedExpr();
          (yyval.capexpr)->expr = e;
          (yyval.capexpr)->code = code;
      }
#line 2105 "tinyc3.tab.c"
    break;

  case 94: /* assignment_expression: conditional_expression  */
#line 521 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 2113 "tinyc3.tab.c"
    break;

  case 95: /* assignment_expression: unary_expression '=' assignment_expression  */
#line 525 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->assign((yyvsp[-2].expr), (yyvsp[0].expr));
          delete (yyvsp[-2].expr);
          delete (yyvsp[0].expr);
      }
#line 2123 "tinyc3.tab.c"
    break;

  case 96: /* expression: assignment_expression  */
#line 533 "tinyc3.y"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 2129 "tinyc3.tab.c"
    break;

  case 97: /* expression_opt: %empty  */
#line 537 "tinyc3.y"
             { (yyval.expr) = NULL; }
#line 2135 "tinyc3.tab.c"
    break;

  case 98: /* expression_opt: expression  */
#line 538 "tinyc3.y"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2141 "tinyc3.tab.c"
    break;

  case 99: /* initializer: assignment_expression  */
#line 542 "tinyc3.y"
                            { (yyval.expr) = (yyvsp[0].expr); }
#line 2147 "tinyc3.tab.c"
    break;

  case 100: /* marker: %empty  */
#line 547 "tinyc3.y"
      {
          (yyval.index) = gTranslator->activeCode()->code.size();
          gTranslator->markLabel();
      }
#line 2156 "tinyc3.tab.c"
    break;

  case 101: /* condition_expression: expression  */
#line 555 "tinyc3.y"
      {
          ExprAttr *c = gTranslator->ensureCondition((yyvsp[0].expr));
          if (c != (yyvsp[0].expr)) delete (yyvsp[0].expr);
          (yyval.expr) = c;
      }
#line 2166 "tinyc3.tab.c"
    break;

  case 102: /* for_condition_opt: %empty  */
#line 564 "tinyc3.y"
      {
          (yyval.expr) = gTranslator->makeExpr(Type::make(tinyc::TK_BOOL), "");
          (yyval.expr)->isBool = true;
          std::size_t q = gTranslator->emit("goto", "", "", "?");
          (yyval.expr)->trueList = gTranslator->makelist(q);
      }
#line 2177 "tinyc3.tab.c"
    break;

  case 103: /* for_condition_opt: condition_expression  */
#line 571 "tinyc3.y"
      {
          (yyval.expr) = (yyvsp[0].expr);
      }
#line 2185 "tinyc3.tab.c"
    break;

  case 104: /* statement: compound_statement  */
#line 577 "tinyc3.y"
                         { }
#line 2191 "tinyc3.tab.c"
    break;

  case 105: /* statement: expression_statement  */
#line 578 "tinyc3.y"
                           { }
#line 2197 "tinyc3.tab.c"
    break;

  case 106: /* statement: selection_statement  */
#line 579 "tinyc3.y"
                          { }
#line 2203 "tinyc3.tab.c"
    break;

  case 107: /* statement: iteration_statement  */
#line 580 "tinyc3.y"
                          { }
#line 2209 "tinyc3.tab.c"
    break;

  case 108: /* statement: jump_statement  */
#line 581 "tinyc3.y"
                     { }
#line 2215 "tinyc3.tab.c"
    break;

  case 109: /* compound_statement: '{' block_scope_begin block_item_list_opt '}' block_scope_end  */
#line 585 "tinyc3.y"
                                                                    { }
#line 2221 "tinyc3.tab.c"
    break;

  case 110: /* block_scope_begin: %empty  */
#line 590 "tinyc3.y"
      {
          gTranslator->enterScope("block" + std::to_string(gTranslator->activeCode()->code.size()));
          (yyval.index) = 0;
      }
#line 2230 "tinyc3.tab.c"
    break;

  case 111: /* block_scope_end: %empty  */
#line 598 "tinyc3.y"
      {
          gTranslator->leaveScope();
          (yyval.index) = 0;
      }
#line 2239 "tinyc3.tab.c"
    break;

  case 112: /* block_item_list_opt: %empty  */
#line 605 "tinyc3.y"
             { }
#line 2245 "tinyc3.tab.c"
    break;

  case 113: /* block_item_list_opt: block_item_list  */
#line 606 "tinyc3.y"
                      { }
#line 2251 "tinyc3.tab.c"
    break;

  case 114: /* block_item_list: block_item  */
#line 610 "tinyc3.y"
                 { }
#line 2257 "tinyc3.tab.c"
    break;

  case 115: /* block_item_list: block_item_list block_item  */
#line 611 "tinyc3.y"
                                 { }
#line 2263 "tinyc3.tab.c"
    break;

  case 116: /* block_item: declaration  */
#line 615 "tinyc3.y"
                  { }
#line 2269 "tinyc3.tab.c"
    break;

  case 117: /* block_item: statement  */
#line 616 "tinyc3.y"
                { }
#line 2275 "tinyc3.tab.c"
    break;

  case 119: /* expression_statement: expression ';'  */
#line 622 "tinyc3.y"
      {
          if ((yyvsp[-1].expr) && (yyvsp[-1].expr)->isBool) {
              ExprAttr *v = gTranslator->materializeBool((yyvsp[-1].expr));
              if (v != (yyvsp[-1].expr)) delete v;
          } else if ((yyvsp[-1].expr)) {
              ExprAttr *r = gTranslator->ensureRValue((yyvsp[-1].expr));
              if (r != (yyvsp[-1].expr)) delete r;
          }
          delete (yyvsp[-1].expr);
      }
#line 2290 "tinyc3.tab.c"
    break;

  case 120: /* selection_statement: IF '(' condition_expression ')' captured_statement  */
#line 636 "tinyc3.y"
      {
          ExprAttr *c = (yyvsp[-2].expr);
          std::string thenLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->trueList, thenLabel);
          gTranslator->activeCode()->append((yyvsp[0].capstmt)->code);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);

          delete c;
          delete (yyvsp[0].capstmt);
      }
#line 2306 "tinyc3.tab.c"
    break;

  case 121: /* selection_statement: IF '(' condition_expression ')' captured_statement ELSE captured_statement  */
#line 648 "tinyc3.y"
      {
          ExprAttr *c = (yyvsp[-4].expr);
          std::string thenLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->trueList, thenLabel);
          gTranslator->activeCode()->append((yyvsp[-2].capstmt)->code);
          std::size_t jumpEnd = gTranslator->emit("goto", "", "", "?");

          std::string elseLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, elseLabel);
          gTranslator->activeCode()->append((yyvsp[0].capstmt)->code);

          std::string endLabel = gTranslator->markLabel();
          std::vector<std::size_t> l = gTranslator->makelist(jumpEnd);
          gTranslator->backpatch(l, endLabel);

          delete c;
          delete (yyvsp[-2].capstmt);
          delete (yyvsp[0].capstmt);
      }
#line 2330 "tinyc3.tab.c"
    break;

  case 122: /* $@8: %empty  */
#line 670 "tinyc3.y"
      { gTranslator->beginCapture(); }
#line 2336 "tinyc3.tab.c"
    break;

  case 123: /* captured_statement: $@8 statement  */
#line 671 "tinyc3.y"
      {
          CodeBuffer *code = gTranslator->endCapture();
          (yyval.capstmt) = new CapturedStmt();
          (yyval.capstmt)->code = code;
      }
#line 2346 "tinyc3.tab.c"
    break;

  case 124: /* iteration_statement: WHILE '(' marker condition_expression ')' marker statement  */
#line 680 "tinyc3.y"
      {
          ExprAttr *c = (yyvsp[-3].expr);
          std::string condLabel = gTranslator->activeCode()->code[(yyvsp[-4].index)].result;
          std::string bodyLabel = gTranslator->activeCode()->code[(yyvsp[-1].index)].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          gTranslator->emit("goto", "", "", condLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);
          delete c;
      }
#line 2361 "tinyc3.tab.c"
    break;

  case 125: /* iteration_statement: DO marker statement WHILE '(' condition_expression ')' ';'  */
#line 691 "tinyc3.y"
      {
          ExprAttr *c = (yyvsp[-2].expr);
          std::string bodyLabel = gTranslator->activeCode()->code[(yyvsp[-6].index)].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);
          delete c;
      }
#line 2374 "tinyc3.tab.c"
    break;

  case 126: /* iteration_statement: FOR '(' expression_opt ';' marker for_condition_opt ';' for_update_clause ')' marker statement  */
#line 700 "tinyc3.y"
      {
          ExprAttr *c = (yyvsp[-5].expr);
          std::string condLabel = gTranslator->activeCode()->code[(yyvsp[-6].index)].result;
          std::string bodyLabel = gTranslator->activeCode()->code[(yyvsp[-1].index)].result;
          gTranslator->backpatch(c->trueList, bodyLabel);
          gTranslator->activeCode()->append((yyvsp[-3].chunk)->code);
          gTranslator->emit("goto", "", "", condLabel);
          std::string endLabel = gTranslator->markLabel();
          gTranslator->backpatch(c->falseList, endLabel);

          if ((yyvsp[-8].expr)) delete (yyvsp[-8].expr);
          delete c;
          delete (yyvsp[-3].chunk);
      }
#line 2393 "tinyc3.tab.c"
    break;

  case 127: /* $@9: %empty  */
#line 717 "tinyc3.y"
      { gTranslator->beginCapture(); }
#line 2399 "tinyc3.tab.c"
    break;

  case 128: /* for_update_clause: $@9 for_update_expression_opt  */
#line 718 "tinyc3.y"
      {
          ExprAttr *e = (yyvsp[0].expr);
          if (e && e->isBool) {
              ExprAttr *v = gTranslator->materializeBool(e);
              if (v != e) delete e;
              e = v;
          } else if (e) {
              ExprAttr *v = gTranslator->ensureRValue(e);
              if (v != e) delete v;
          }

          CodeBuffer *code = gTranslator->endCapture();
          (yyval.chunk) = new ForUpdateChunk();
          delete (yyval.chunk)->code;
          (yyval.chunk)->code = code;
          if (e) delete e;
      }
#line 2421 "tinyc3.tab.c"
    break;

  case 129: /* for_update_expression_opt: %empty  */
#line 738 "tinyc3.y"
             { (yyval.expr) = NULL; }
#line 2427 "tinyc3.tab.c"
    break;

  case 130: /* for_update_expression_opt: expression  */
#line 739 "tinyc3.y"
                 { (yyval.expr) = (yyvsp[0].expr); }
#line 2433 "tinyc3.tab.c"
    break;

  case 131: /* jump_statement: RETURN ';'  */
#line 744 "tinyc3.y"
      {
          if (!gTranslator->currentReturnType ||
              gTranslator->currentReturnType->kind != tinyc::TK_VOID)
              throw std::runtime_error("return without value in non-void function");
          gTranslator->emit("return", "", "", "");
      }
#line 2444 "tinyc3.tab.c"
    break;

  case 132: /* jump_statement: RETURN expression ';'  */
#line 751 "tinyc3.y"
      {
          if (!gTranslator->currentReturnType ||
              gTranslator->currentReturnType->kind == tinyc::TK_VOID)
              throw std::runtime_error("return value in void function");
          ExprAttr *original = (yyvsp[-1].expr);
          ExprAttr *e = gTranslator->ensureRValue(original);
          e = gTranslator->convert(e, gTranslator->currentReturnType);
          gTranslator->emit("return", e->place, "", "");
          if (e != original) delete e;
          delete original;
      }
#line 2460 "tinyc3.tab.c"
    break;


#line 2464 "tinyc3.tab.c"

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
      yyerror (YY_("syntax error"));
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

  return yyresult;
}

#line 764 "tinyc3.y"


void yyerror(const char *s) {
    std::cerr << "tinyC3: " << s << " at line " << yylineno
              << " near '" << (yytext ? yytext : "") << "'\n";
}
