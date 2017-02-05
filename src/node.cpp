#include "environment.h"
#include "node.h"

using namespace std;

inline AbstractNode::~AbstractNode() { }

AbstractType *NodeBlock::evaluate(Environment *env) {
    for (int i = 0; i < nodes_count; i++) {
        nodes[i]->evaluate(env);
    }
    return nullptr;
}

NodeBlock::~NodeBlock() {
    for (int i = 0; i < nodes_count; i++) {
        delete nodes[i];
    }
}

// -----------------------------------------------------------------------------

AbstractType *NodeVariableDefinition::evaluate(Environment *env) {
    env->setVariable(name, value->evaluate(env));
    return nullptr;
}

NodeVariableDefinition::~NodeVariableDefinition() {
    delete value;
}

// -----------------------------------------------------------------------------

AbstractType *NodeVariableName::evaluate(Environment *env) {
    return env->getVariable(name);
}

// -----------------------------------------------------------------------------

AbstractType *NodePrint::evaluate(Environment *env) {
    AbstractType *evaluated = value->evaluate(env);
    cout << evaluated->toString() << endl;
    return nullptr;
}

NodePrint::~NodePrint() {
    delete value;
}

// -----------------------------------------------------------------------------

AbstractType *NodeBinaryOperator::evaluate(Environment *env) {
    AbstractType *t1 = a->evaluate(env);
    AbstractType *t2 = b->evaluate(env);

    if (t1->type() != t2->type()) {
        runtimeError("Cannot apply operator for different types");
    }

    if (!t1->supportsOperator(op)) {
        runtimeError("Operator not supported by type");
    }

    return t1->applyOperator(op, t2, env);
}

NodeBinaryOperator::~NodeBinaryOperator() {
    delete a;
    delete b;
}

// -----------------------------------------------------------------------------

AbstractType *NodeNotOperator::evaluate(Environment *env) {
    AbstractType *t = a->evaluate(env);

    if (t->type() != BOOL) {
        runtimeError("Using not operator with non-boolean variable");
    }

    TypeBool *b = (TypeBool *) t;
    return env->allocBool(!b->value());
}

NodeNotOperator::~NodeNotOperator() {
    delete a;
}

// -----------------------------------------------------------------------------

NodeConstant::~NodeConstant() {
    clean(value);
}

AbstractType *NodeConstant::evaluate(Environment *env) {
    return alloc(value, env);
}

AbstractType *NodeConstant::alloc(AbstractType *type, Environment *env) {
    AbstractType *evaluated = nullptr;

    switch (type->type()) {
        case BOOL:
            evaluated = env->allocBool(((TypeBool *) type)->value());
            break;
        case CHAR:
            evaluated = env->allocChar(((TypeChar *) type)->value());
            break;
        case INT:
            evaluated = env->allocInt(((TypeInt *) type)->value());
            break;
        case LIST: {
            TypeList *originalList = (TypeList *) type;
            vector<AbstractType *> *vectorCopy = new vector<AbstractType *>();

            for (auto const &item : *originalList->value()) {
                vectorCopy->push_back(alloc(item, env));
            }

            evaluated = env->allocList(vectorCopy);
        }
    }

    return evaluated;
}

void NodeConstant::clean(AbstractType *type) {
    if (type->type() == LIST) {
        TypeList *list = (TypeList *) type;
        for (auto const &item : *list->value()) {
            clean(item);
        }
    }

    delete type;
}

// -----------------------------------------------------------------------------

AbstractType *NodeWhile::evaluate(Environment *env) {
    for (; ;) {
        AbstractType *evaluated = condition->evaluate(env);

        if (evaluated->type() != BOOL) {
            runtimeError("Cannot use non-bool value for condition");
        }

        TypeBool *evaluatedBool = (TypeBool *) evaluated;

        if (!evaluatedBool->value()) {
            return nullptr;
        }

        try {
            block->evaluate(env);
        } catch (NodeBreak::BreakException e) {
            return nullptr;
        }
    }
}

NodeWhile::~NodeWhile() {
    delete condition;
    delete block;
}

// -----------------------------------------------------------------------------

AbstractType *NodeIfElse::evaluate(Environment *env) {
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

    return nullptr;
}

NodeIfElse::~NodeIfElse() {
    delete condition;
    delete ifBlock;
    delete elseBlock;
}

// -----------------------------------------------------------------------------

AbstractType *NodeScanInt::evaluate(Environment *env) {
    int number;
    cin >> number;
    return env->allocInt(number);
}

// -----------------------------------------------------------------------------

AbstractType *NodeScanChar::evaluate(Environment *env) {
    char character;
    cin >> character;
    return env->allocChar(character);
}

// -----------------------------------------------------------------------------

AbstractType *NodeScanString::evaluate(Environment *env) {
    string input;
    cin >> input;

    vector<AbstractType *> *list = new vector<AbstractType *>();

    for (char &c : input) {
        list->push_back(env->allocChar(c));
    }

    return env->allocList(list);
}

// -----------------------------------------------------------------------------

AbstractType *NodeBreak::evaluate(Environment *env) {
    throw breakException;
}


