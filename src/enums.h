#ifndef TEETON_ENUMS_H
#define TEETON_ENUMS_H

enum Operator {
    ADD, SUB, MUL, DIV, MOD,  // arithmetic
    EQ, NEQ,  // equation
    EQEQ, // same reference
    GT, LT, GTE, LTE,  // comparison
    AND, OR  // logical
};

enum Type {
    CHAR, BOOL, INT, LIST
};

#endif //TEETON_ENUMS_H
