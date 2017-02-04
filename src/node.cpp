#include "environment.h"
#include "node.h"
#include "type.h"

using namespace std;

inline AbstractNode::~AbstractNode() { }

AbstractType *NodeBlock::evaluate(Environment * env) {
    for (int i = 0; i < nodes_count; i++) {
        nodes[i]->evaluate(env);
    }
    return NULL;
}

NodeBlock::~NodeBlock() {
    for (int i = 0; i < nodes_count; i++) {
        delete nodes[i];
    }
}




AbstractType *NodeVariableDefinition::evaluate(Environment * env) {
    env->setVariable(name, value->evaluate(env));
    return NULL;
}

NodeVariableDefinition::~NodeVariableDefinition() {
    delete value;
}




AbstractType *NodeVariableName::evaluate(Environment * env) {
    return env->getVariable(name);
}




AbstractType *NodePrint::evaluate(Environment * env) {
    AbstractType *evaluated = value->evaluate(env);

    switch (evaluated->type()) {
        case BOOL:
            cout << (((TypeBool *) evaluated)->value() ? "True" : "False") << endl;
            break;
        case CHAR:
            cout << ((TypeChar *) evaluated)->value() << endl;
            break;
        case INT:
            cout << ((TypeInt *) evaluated)->value() << endl;
            break;
    }

    return NULL;
}

NodePrint::~NodePrint() {
    delete value;
}




AbstractType *NodeBinaryOperator::evaluate(Environment * env) {
    AbstractType *t1 = a->evaluate(env);
    AbstractType *t2 = b->evaluate(env);

    if (t1->type() != t2->type()) {
        runtimeError("Cannot apply operator for different types");
    }

    if (!t1->supportsOperator(op)) {
        runtimeError("Operator not supported by type");
    }

    return t1->applyOperator(op, t2);
}

NodeBinaryOperator::~NodeBinaryOperator() {
    delete a;
    delete b;
}




AbstractType *NodeNotOperator::evaluate(Environment * env) {
    AbstractType *t = a->evaluate(env);

    if (t->type() != BOOL) {
        runtimeError("Using not operator with non-boolean variable");
    }

    TypeBool *b = (TypeBool *) t;
    return new TypeBool(!b->value());
}

NodeNotOperator::~NodeNotOperator() {
    delete a;
}




AbstractType *NodeConstant::evaluate(Environment * env) {
    return value;
}




AbstractType *NodeWhile::evaluate(Environment * env) {
    for (; ;) {
        AbstractType *evaluated = condition->evaluate(env);

        if (evaluated->type() != BOOL) {
            runtimeError("Cannot use non-bool value for condition");
        }

        TypeBool *evaluatedBool = (TypeBool *) evaluated;

        if (!evaluatedBool->value()) {
            return NULL;
        }

        block->evaluate(env);
    }
    return NULL;
}

NodeWhile::~NodeWhile() {
    delete condition;
    delete block;
}




AbstractType *NodeIfElse::evaluate(Environment * env) {
    AbstractType *evaluated = condition->evaluate(env);

    if (evaluated->type() != BOOL) {
        runtimeError("Cannot use non-bool value for condition");
    }

    TypeBool *evaluatedBool = (TypeBool *) evaluated;

    if (evaluatedBool->value()) {
        ifBlock->evaluate(env);
    } else {
        elseBlock->evaluate(env);
    }

    return NULL;
}

NodeIfElse::~NodeIfElse() {
    delete ifBlock;
    delete elseBlock;
}