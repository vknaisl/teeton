#ifndef TEETON_TYPE_H
#define TEETON_TYPE_H

#include "enums.h"
#include "utils.h"

class AbstractType {
public:
    virtual Type type() = 0;

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);
};

class TypeBool : public AbstractType {
public:
    TypeBool(bool value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    bool value();

private:
    bool _value;
};

class TypeChar : public AbstractType {
public:
    TypeChar(char value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    char value();

private:
    char _value;
};

class TypeInt : public AbstractType {
public:
    TypeInt(int value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    int value();

private:
    int _value;
};

#endif //TEETON_TYPE_H
