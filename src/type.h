#ifndef TEETON_TYPE_H
#define TEETON_TYPE_H

#include <vector>
#include <sstream>

#include "enums.h"
#include "utils.h"

class AbstractType {
public:
    virtual ~AbstractType() = 0;

    virtual Type type() = 0;

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    virtual std::string toString() = 0;
};

// -----------------------------------------------------------------------------

class TypeBool : public AbstractType {
public:
    TypeBool(bool value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    bool value();

    virtual std::string toString();

private:
    bool _value;
};

// -----------------------------------------------------------------------------

class TypeChar : public AbstractType {
public:
    TypeChar(char value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    char value();

    virtual std::string toString();

private:
    char _value;
};

// -----------------------------------------------------------------------------

class TypeInt : public AbstractType {
public:
    TypeInt(int value) : _value(value) { };

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    int value();

    virtual std::string toString();

private:
    int _value;
};


// -----------------------------------------------------------------------------

class TypeList : public AbstractType {
public:
    TypeList(std::vector<AbstractType *> *value) : _value(value) { };

    ~TypeList();

    virtual Type type();

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);

    std::vector<AbstractType *> *value();

    virtual std::string toString();

private:
    std::vector<AbstractType *> *_value;
};

#endif //TEETON_TYPE_H
