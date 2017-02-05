#include "type.h"
#include "environment.h"

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

AbstractType *AbstractType::applyOperator(Operator op, AbstractType *other, Environment *env) {
    switch (op) {
        case EQEQ:
            return env->allocBool(this == other);
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

AbstractType *TypeBool::applyOperator(Operator op, AbstractType *other, Environment *env) {
    TypeBool *otherBool = (TypeBool *) other;
    switch (op) {
        case EQ:
            return env->allocBool(_value == otherBool->value());
        case NEQ:
            return env->allocBool(_value != otherBool->value());
        case AND:
            return env->allocBool(_value && otherBool->value());
        case OR:
            return env->allocBool(_value || otherBool->value());
        default:
            return AbstractType::applyOperator(op, other, env);
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

AbstractType *TypeChar::applyOperator(Operator op, AbstractType *other, Environment *env) {
    TypeChar *otherChar = (TypeChar *) other;
    switch (op) {
        case EQ:
            return env->allocBool(_value == otherChar->value());
        case NEQ:
            return env->allocBool(_value != otherChar->value());
        case GT:
            return env->allocBool(_value > otherChar->value());
        case LT:
            return env->allocBool(_value < otherChar->value());
        case GTE:
            return env->allocBool(_value >= otherChar->value());
        case LTE:
            return env->allocBool(_value <= otherChar->value());
        default:
            return AbstractType::applyOperator(op, other, env);
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

AbstractType *TypeInt::applyOperator(Operator op, AbstractType *other, Environment *env) {
    TypeInt *otherInt = (TypeInt *) other;
    switch (op) {
        case ADD:
            return env->allocInt(_value + otherInt->value());
        case SUB:
            return env->allocInt(_value - otherInt->value());
        case MUL:
            return env->allocInt(_value * otherInt->value());
        case DIV:
            return env->allocInt(_value / otherInt->value());
        case MOD:
            return env->allocInt(_value % otherInt->value());
        case EQ:
            return env->allocBool(_value == otherInt->value());
        case NEQ:
            return env->allocBool(_value != otherInt->value());
        case GT:
            return env->allocBool(_value > otherInt->value());
        case LT:
            return env->allocBool(_value < otherInt->value());
        case GTE:
            return env->allocBool(_value >= otherInt->value());
        case LTE:
            return env->allocBool(_value <= otherInt->value());
        default:
            return AbstractType::applyOperator(op, other, env);
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

AbstractType *TypeList::applyOperator(Operator op, AbstractType *other, Environment *env) {
    TypeList *otherList = (TypeList *) other;
    switch (op) {
        case ADD: {
            vector<AbstractType *> * new_vector = new vector<AbstractType *>(*_value);
            new_vector->insert(new_vector->end(), otherList->value()->begin(), otherList->value()->end());
            return env->allocList(new_vector);
        }
        case EQ: {
            if (_value->size() != otherList->value()->size()) {
                return env->allocBool(false);
            }
            for (unsigned i = 0; i < _value->size(); i++) {
                TypeBool *result = (TypeBool *) _value->at(i)->applyOperator(NEQ, otherList->value()->at(i), env);
                if (result->value()) {
                    return env->allocBool(false);
                }
            }
            return env->allocBool(true);
        }
        case NEQ: {
            TypeBool *result = (TypeBool *) applyOperator(EQ, other, env);
            return env->allocBool(!result->value());
        }
        case GT: {
            for (unsigned i = 0; i < _value->size(); i++) {
                if (i < otherList->value()->size()) {
                    TypeBool *result = (TypeBool *) _value->at(i)->applyOperator(LTE, otherList->value()->at(i), env);
                    if (result->value()) {
                        return env->allocBool(false);
                    }
                } else {
                    return env->allocBool(false);
                }
            }
            return env->allocBool(true);
        }
        case LT: {
            TypeBool *resultEQ = (TypeBool *) applyOperator(EQ, other, env);
            TypeBool *resultGT = (TypeBool *) applyOperator(GT, other, env);
            return env->allocBool(!resultEQ->value() && !resultGT->value());
        }
        case GTE: {
            TypeBool *result = (TypeBool *) applyOperator(LT, other, env);
            return env->allocBool(!result->value());
        }
        case LTE: {
            TypeBool *result = (TypeBool *)applyOperator(GT, other, env);
            return env->allocBool(!result->value());
        }
        default:
            return AbstractType::applyOperator(op, other, env);
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
