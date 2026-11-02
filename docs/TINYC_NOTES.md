# TinyC notes

This implementation follows the supplied TinyC assignment specifications rather than full ISO C.

## Part 1

The lexer recognizes the specified keywords, identifiers, constants, strings, punctuators and comments. Identifiers follow the supplied grammar: alphabetic characters followed by alphabetic characters or digits.

## Part 2

The parser follows the supplied expression, declaration, statement and external-definition structure. The standard matched/unmatched statement form is used to make `if`/`else` association deterministic.

## Part 3

The translator supports the Part 3 subset stated in the assignment: arithmetic, shift, relational, bit, logical and simple assignment expressions; simple variable/pointer/array/function declarations; and the supported statements including `return`.

The implementation does not claim to be a full C compiler.
