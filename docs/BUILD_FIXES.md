# Build fixes included in the current pack

This file is kept as a simple change log so that the code does not become a black box.

## Lexer fixes

All keyword and multi-character-token rules use proper Flex actions:

```lex
"int" { return INT; }
"->"  { return ARROW; }
```

`case` is declared and returned in Part 2 because it appears in the supplied TinyC grammar.

The identifier rule follows the supplied TinyC lexical specification: identifiers use letters and digits and do not introduce an extra underscore character that is not present in that specification.

The scanners use `nounput` and `noinput`. Part 1's Makefile defines the POSIX feature macro needed by the generated `fileno()` code.

## Parser fixes

Part 2 removes unreachable optional grammar rules and uses the matched/unmatched statement form for `if`/`else`.

Part 3 removes the duplicate `exprs` semantic-value field and avoids the earlier nullable helper productions for logical expressions and `for` updates.

The Part 3 conditional and loop actions use ordinary raw pointers to `CodeBuffer` objects. There is no smart-pointer ownership or move operation.

## Translator fixes

Array indexing, pointer arithmetic, pointer subtraction, short-circuit boolean expressions, conditional expressions, and prefix/postfix increment/decrement have explicit ownership handling.

Target-dependent sizes are obtained from `TargetInfo` rather than being scattered as magic constants in the translator.

## Backend fixes

The TAC loader ignores comments. The x86-64 prototype does not allocate stack slots for labels or branch targets and uses the configured integer width for its scalar instruction suffixes.
