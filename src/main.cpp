#include<iostream>
#include <unordered_map>

using namespace std;

void runtimeError(string err) {
    cout << "RuntimeError: " << err << endl;
    exit(1);
}

enum Operator {
    ADD, SUB, MUL, DIV, MOD,  // arithmetic
    EQ, NEQ,  // equation
    GT, LT, GTE, LTE,  // comparison
    AND, OR  // logical
};

enum Type {
    CHAR, BOOL, INT
};


class AbstractType {
public:
    virtual Type type() = 0;

    virtual bool supportsOperator(Operator op);

    virtual AbstractType *applyOperator(Operator op, AbstractType *other);
};

bool AbstractType::supportsOperator(Operator op) {
    return false;
}

AbstractType *AbstractType::applyOperator(Operator op, AbstractType *other) {
    runtimeError("Operator not suported for type");
    return NULL;
}


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


// global variable storage
unordered_map<string, AbstractType *> variables;


class AbstractNode {
public:
    virtual AbstractType *evaluate() = 0;

    virtual ~AbstractNode() = 0;
};

inline AbstractNode::~AbstractNode() { }


class NodeBlock : public AbstractNode {
public:
    NodeBlock(AbstractNode **nodes, int nodes_count) : nodes(nodes), nodes_count(nodes_count) { };

    ~NodeBlock();

    virtual AbstractType *evaluate();

private:
    AbstractNode **nodes;
    int nodes_count;
};

AbstractType *NodeBlock::evaluate() {
    for (int i = 0; i < nodes_count; i++) {
        nodes[i]->evaluate();
    }
    return NULL;
}

NodeBlock::~NodeBlock() {
    for (int i = 0; i < nodes_count; i++) {
        delete nodes[i];
    }
}


class NodeVariableDefinition : public AbstractNode {
public:
    NodeVariableDefinition(string name, AbstractNode *value) : name(name), value(value) { };

    ~NodeVariableDefinition();

    virtual AbstractType *evaluate();

private:
    string name;
    AbstractNode *value;
};

AbstractType *NodeVariableDefinition::evaluate() {
    variables[name] = value->evaluate();
    return NULL;
}

NodeVariableDefinition::~NodeVariableDefinition() {
    delete value;
}


class NodeVariableName : public AbstractNode {
public:
    NodeVariableName(string name) : name(name) { };

    virtual AbstractType *evaluate();

private:
    string name;
};

AbstractType *NodeVariableName::evaluate() {
    return variables[name];
}


class NodePrint : public AbstractNode {
public:
    NodePrint(AbstractNode *value) : value(value) { };

    ~NodePrint();

    virtual AbstractType *evaluate();

private:
    AbstractNode *value;
};

AbstractType *NodePrint::evaluate() {
    AbstractType *evaluated = value->evaluate();

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


class NodeBinaryOperator : public AbstractNode {
public:
    NodeBinaryOperator(Operator op, AbstractNode *a, AbstractNode *b) : op(op), a(a), b(b) { };

    ~NodeBinaryOperator();

    virtual AbstractType *evaluate();

private:
    Operator op;
    AbstractNode *a;
    AbstractNode *b;
};

AbstractType *NodeBinaryOperator::evaluate() {
    AbstractType *t1 = a->evaluate();
    AbstractType *t2 = b->evaluate();

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


class NodeNotOperator : public AbstractNode {
public:
    NodeNotOperator(AbstractNode *a) : a(a) { };

    ~NodeNotOperator();

    virtual AbstractType *evaluate();

private:
    AbstractNode *a;
};

AbstractType *NodeNotOperator::evaluate() {
    AbstractType *t = a->evaluate();

    if (t->type() != BOOL) {
        runtimeError("Using not operator with non-boolean variable");
    }

    TypeBool *b = (TypeBool *) t;
    return new TypeBool(!b->value());
}

NodeNotOperator::~NodeNotOperator() {
    delete a;
}


class NodeConstant : public AbstractNode {
public:
    NodeConstant(AbstractType *value) : value(value) { };

    virtual AbstractType *evaluate();

private:
    AbstractType *value;
};

AbstractType *NodeConstant::evaluate() {
    return value;
}


class NodeWhile : public AbstractNode {
public:
    NodeWhile(AbstractNode *condition, NodeBlock *block) : condition(condition), block(block) { };

    ~NodeWhile();

    virtual AbstractType *evaluate();

private:
    AbstractNode *condition;
    NodeBlock *block;
};

AbstractType *NodeWhile::evaluate() {
    for (; ;) {
        AbstractType *evaluated = condition->evaluate();

        if (evaluated->type() != BOOL) {
            runtimeError("Cannot use non-bool value for condition");
        }

        TypeBool *evaluatedBool = (TypeBool *) evaluated;

        if (!evaluatedBool->value()) {
            return NULL;
        }

        block->evaluate();
    }
    return NULL;
}

NodeWhile::~NodeWhile() {
    delete condition;
    delete block;
}


class NodeIfElse : public AbstractNode {
public:
    NodeIfElse(AbstractNode *condition, NodeBlock *ifBlock, NodeBlock *elseBlock) : condition(condition),
                                                                                    ifBlock(ifBlock),
                                                                                    elseBlock(elseBlock) { };

    ~NodeIfElse();

    virtual AbstractType *evaluate();

private:
    AbstractNode *condition;
    NodeBlock *ifBlock;
    NodeBlock *elseBlock;
};

AbstractType *NodeIfElse::evaluate() {
    AbstractType *evaluated = condition->evaluate();

    if (evaluated->type() != BOOL) {
        runtimeError("Cannot use non-bool value for condition");
    }

    TypeBool *evaluatedBool = (TypeBool *) evaluated;

    if (evaluatedBool->value()) {
        ifBlock->evaluate();
    } else {
        elseBlock->evaluate();
    }

    return NULL;
}

NodeIfElse::~NodeIfElse() {
    delete ifBlock;
    delete elseBlock;
}



// EXAMPLES ---

/**
 * var a = 5
 * var b = 2
 * var c = a - b
 * print(c)
 */
void example_arithmetic() {
    cout << "* Example arithmetic. Expected result: 3" << endl;
    AbstractNode *nodes[4] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("b", new NodeConstant(new TypeInt(2))),
            new NodeVariableDefinition("c", new NodeBinaryOperator(SUB, new NodeVariableName("a"),
                                                                   new NodeVariableName("b"))),
            new NodePrint(new NodeVariableName("c"))
    };
    NodeBlock *root = new NodeBlock(nodes, 4);
    root->evaluate();
    delete root;
    cout << endl;
}

/**
 * var a = 5
 * var b = 5
 * var c = a >= b
 * print(c)
 */
void example_comparison() {
    cout << "* Example comparison. Expected result: True" << endl;
    AbstractNode *nodes[4] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("b", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("c", new NodeBinaryOperator(GTE, new NodeVariableName("a"),
                                                                   new NodeVariableName("b"))),
            new NodePrint(new NodeVariableName("c"))
    };
    NodeBlock *root = new NodeBlock(nodes, 4);
    root->evaluate();
    delete root;
    cout << endl;
}

/**
 * var i = 0
 * while(i < 10) {
 *   print(i)
 *   i = i + 1
 * }
 */
void example_whileCycle() {
    cout << "* Example while cycle. Expected result: lines 0 .. 9" << endl;
    AbstractNode *whileBlockNodes[2] = {
            new NodePrint(new NodeVariableName("i")),
            new NodeVariableDefinition("i", new NodeBinaryOperator(ADD, new NodeVariableName("i"),
                                                                   new NodeConstant(new TypeInt(1))))
    };
    AbstractNode *nodes[2] = {
            new NodeVariableDefinition("i", new NodeConstant(new TypeInt(0))),
            new NodeWhile(
                    new NodeBinaryOperator(LT, new NodeVariableName("i"), new NodeConstant(new TypeInt(10))),
                    new NodeBlock(whileBlockNodes, 2)
            )
    };
    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate();
    delete root;
    cout << endl;
}

/**
 * var i = 'x'
 * if (i == 'x') {
 *   print('a')
 * } else {
 *   print('n')
 * }
 */
void example_ifElse() {
    cout << "* Example if else. Expected result: 'a'" << endl;
    AbstractNode *ifBlockNodes[1] = {new NodePrint(new NodeConstant(new TypeChar('a')))};
    AbstractNode *elseBlockNodes[1] = {new NodePrint(new NodeConstant(new TypeChar('n')))};
    AbstractNode *nodes[2] = {
            new NodeVariableDefinition("i", new NodeConstant(new TypeChar('x'))),
            new NodeIfElse(
                    new NodeBinaryOperator(EQ, new NodeVariableName("i"), new NodeConstant(new TypeChar('x'))),
                    new NodeBlock(ifBlockNodes, 1),
                    new NodeBlock(elseBlockNodes, 1)
            )
    };
    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate();
    delete root;
    cout << endl;
}

/**
 * var i = 'x'
 * var x1 = 2
 * var x2 = 5
 * if (!((x1 < x2) && (i == 'x'))) {
 *   print('a')
 * } else {
 *   print('n')
 * }
 */
void example_complicatedCondition() {
    cout << "* Example complicated condition. Expected result: 'n'" << endl;
    AbstractNode *ifBlockNodes[1] = {new NodePrint(new NodeConstant(new TypeChar('a')))};
    AbstractNode *elseBlockNodes[1] = {new NodePrint(new NodeConstant(new TypeChar('n')))};
    AbstractNode *nodes[4] = {
            new NodeVariableDefinition("i", new NodeConstant(new TypeChar('x'))),
            new NodeVariableDefinition("x1", new NodeConstant(new TypeInt(2))),
            new NodeVariableDefinition("x2", new NodeConstant(new TypeInt(5))),
            new NodeIfElse(
                    new NodeNotOperator(new NodeBinaryOperator(
                            AND,
                            new NodeBinaryOperator(LT, new NodeVariableName("x1"), new NodeVariableName("x2")),
                            new NodeBinaryOperator(EQ, new NodeVariableName("i"), new NodeConstant(new TypeChar('x')))
                    )),
                    new NodeBlock(ifBlockNodes, 1),
                    new NodeBlock(elseBlockNodes, 1)
            )
    };
    NodeBlock *root = new NodeBlock(nodes, 4);
    root->evaluate();
    delete root;
    cout << endl;
}

int main() {
    example_arithmetic();
    example_comparison();
    example_whileCycle();
    example_ifElse();
    example_complicatedCondition();
    return 0;
}
