#include "type.h"

bool AbstractType::supportsOperator(Operator op) {
    return false;
}

AbstractType *AbstractType::applyOperator(Operator op, AbstractType *other) {
    runtimeError("Operator not suported for type");
    return NULL;
}

// -----------------------------------------------------------------------------

Type TypeBool::type() {
    return BOOL;
}

bool TypeBool::value() {
    return _value;
}

bool TypeBool::supportsOperator(Operator op) {
    switch (op) {
        case EQ:
        case NEQ:
        case AND:
        case OR:
            return true;
        default:
            return false;
    }
}

AbstractType *TypeBool::applyOperator(Operator op, AbstractType *other) {
    TypeBool *otherBool = (TypeBool *) other;
    switch (op) {
        case EQ:
            return new TypeBool(_value == otherBool->value());
        case NEQ:
            return new TypeBool(_value != otherBool->value());
        case AND:
            return new TypeBool(_value && otherBool->value());
        case OR:
            return new TypeBool(_value || otherBool->value());
        default:
            runtimeError("Operator not supported");
            return NULL;
    }
}

// -----------------------------------------------------------------------------

Type TypeChar::type() {
    return CHAR;
}

char TypeChar::value() {
    return _value;
}

bool TypeChar::supportsOperator(Operator op) {
    switch (op) {
        case EQ:
        case NEQ:
        case GT:
        case LT:
        case GTE:
        case LTE:
            return true;
        default:
            return false;
    }
}

AbstractType *TypeChar::applyOperator(Operator op, AbstractType *other) {
    TypeChar *otherChar = (TypeChar *) other;
    switch (op) {
        case EQ:
            return new TypeBool(_value == otherChar->value());
        case NEQ:
            return new TypeBool(_value != otherChar->value());
        case GT:
            return new TypeBool(_value > otherChar->value());
        case LT:
            return new TypeBool(_value < otherChar->value());
        case GTE:
            return new TypeBool(_value >= otherChar->value());
        case LTE:
            return new TypeBool(_value <= otherChar->value());
        default:
            runtimeError("Operator not supported");
            return NULL;
    }
}

// -----------------------------------------------------------------------------

Type TypeInt::type() {
    return INT;
}

int TypeInt::value() {
    return _value;
}

bool TypeInt::supportsOperator(Operator op) {
    switch (op) {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case MOD:
        case EQ:
        case NEQ:
        case GT:
        case LT:
        case GTE:
        case LTE:
            return true;
        default:
            return false;
    }
}

AbstractType *TypeInt::applyOperator(Operator op, AbstractType *other) {
    TypeInt *otherInt = (TypeInt *) other;
    switch (op) {
        case ADD:
            return new TypeInt(_value + otherInt->value());
        case SUB:
            return new TypeInt(_value - otherInt->value());
        case MUL:
            return new TypeInt(_value * otherInt->value());
        case DIV:
            return new TypeInt(_value / otherInt->value());
        case MOD:
            return new TypeInt(_value % otherInt->value());
        case EQ:
            return new TypeBool(_value == otherInt->value());
        case NEQ:
            return new TypeBool(_value != otherInt->value());
        case GT:
            return new TypeBool(_value > otherInt->value());
        case LT:
            return new TypeBool(_value < otherInt->value());
        case GTE:
            return new TypeBool(_value >= otherInt->value());
        case LTE:
            return new TypeBool(_value <= otherInt->value());
        default:
            runtimeError("Operator not supported");
            return NULL;
    }
}
