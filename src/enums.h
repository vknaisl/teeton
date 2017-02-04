#ifndef TEETON_ENUMS_H
#define TEETON_ENUMS_H

enum Operator {
    ADD, SUB, MUL, DIV, MOD,  // arithmetic
    EQ, NEQ,  // equation
    GT, LT, GTE, LTE,  // comparison
    AND, OR  // logical
};

enum Type {
    CHAR, BOOL, INT
};

#endif //TEETON_ENUMS_H
