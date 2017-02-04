#include "type.h"

using namespace std;

inline AbstractType::~AbstractType() { }

bool AbstractType::supportsOperator(Operator op) {
    switch (op) {
        case EQEQ:
            return true;
        default:
            return false;
    }
}

AbstractType *AbstractType::applyOperator(Operator op, AbstractType *other) {
    switch (op) {
        case EQEQ:
            return new TypeBool(this == other);
        default:
            runtimeError("Operator not suported for type");
            return NULL;
    }
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
            return AbstractType::supportsOperator(op);
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
            return AbstractType::applyOperator(op, other);
    }
}

string TypeBool::toString() {
    return _value ? "True" : "False";
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
            return AbstractType::supportsOperator(op);
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
            return AbstractType::applyOperator(op, other);
    }
}

string TypeChar::toString() {
    string s;
    s.push_back('\'');
    s.push_back(_value);
    s.push_back('\'');
    return s;
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
            return AbstractType::supportsOperator(op);
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
            return AbstractType::applyOperator(op, other);
    }
}

string TypeInt::toString() {
    return to_string(_value);
}

// -----------------------------------------------------------------------------

TypeList::~TypeList() {
    delete _value;
}

Type TypeList::type() {
    return LIST;
}

bool TypeList::supportsOperator(Operator op) {
    switch (op) {
        case ADD:
        case EQ:
        case NEQ:
        case GT:
        case LT:
        case GTE:
        case LTE:
            return true;
        default:
            return AbstractType::supportsOperator(op);
    }
}

AbstractType *TypeList::applyOperator(Operator op, AbstractType *other) {
    TypeList *otherList = (TypeList *) other;
    switch (op) {
        case ADD: {
            vector<AbstractType *> * new_vector = new vector<AbstractType *>(*_value);
            new_vector->insert(new_vector->end(), otherList->value()->begin(), otherList->value()->end());
            return new TypeList(new_vector);
        }
        case EQ: {
            if (_value->size() != otherList->value()->size()) {
                return new TypeBool(false);
            }
            for (unsigned i = 0; i < _value->size(); i++) {
                TypeBool *result = (TypeBool *) _value->at(i)->applyOperator(NEQ, otherList->value()->at(i));
                if (result->value()) {
                    return new TypeBool(false);
                }
            }
            return new TypeBool(true);
        }
        case NEQ: {
            TypeBool *result = (TypeBool *) applyOperator(EQ, other);
            return new TypeBool(!result->value());
        }
        case GT: {
            for (unsigned i = 0; i < _value->size(); i++) {
                if (i < otherList->value()->size()) {
                    TypeBool *result = (TypeBool *) _value->at(i)->applyOperator(LTE, otherList->value()->at(i));
                    if (result->value()) {
                        return new TypeBool(false);
                    }
                } else {
                    return new TypeBool(false);
                }
            }
            return new TypeBool(true);
        }
        case LT: {
            TypeBool *resultEQ = (TypeBool *) applyOperator(EQ, other);
            TypeBool *resultGT = (TypeBool *) applyOperator(GT, other);
            return new TypeBool(!resultEQ->value() && !resultGT->value());
        }
        case GTE: {
            TypeBool *result = (TypeBool *) applyOperator(LT, other);
            return new TypeBool(!result->value());
        }
        case LTE: {
            TypeBool *result = (TypeBool *)applyOperator(GT, other);
            return new TypeBool(!result->value());
        }
        default:
            return AbstractType::applyOperator(op, other);
    }
}

vector<AbstractType *> * TypeList::value() {
    return _value;
}

string TypeList::toString() {
    stringstream ss;
    ss << "[";
    for (unsigned i = 0; i < _value->size(); i++) {
        if (i > 0) {
            ss << ", ";
        }
        ss << _value->at(i)->toString();
    }
    ss << "]";
    return ss.str();
}
