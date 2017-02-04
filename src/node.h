#ifndef TEETON_NODE_H
#define TEETON_NODE_H


#include <iostream>
#include <string>

class AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env) = 0;

    virtual ~AbstractNode() = 0;
};

// -----------------------------------------------------------------------------

class NodeBlock : public AbstractNode {
public:
    NodeBlock(AbstractNode **nodes, int nodes_count) : nodes(nodes), nodes_count(nodes_count) { };

    ~NodeBlock();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode **nodes;
    int nodes_count;
};

// -----------------------------------------------------------------------------

class NodeVariableDefinition : public AbstractNode {
public:
    NodeVariableDefinition(std::string name, AbstractNode *value) : name(name), value(value) { };

    ~NodeVariableDefinition();

    virtual AbstractType *evaluate(Environment *env);

private:
    std::string name;
    AbstractNode *value;
};

// -----------------------------------------------------------------------------

class NodeVariableName : public AbstractNode {
public:
    NodeVariableName(std::string name) : name(name) { };

    virtual AbstractType *evaluate(Environment *env);

private:
    std::string name;
};

// -----------------------------------------------------------------------------

class NodePrint : public AbstractNode {
public:
    NodePrint(AbstractNode *value) : value(value) { };

    ~NodePrint();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *value;
};

// -----------------------------------------------------------------------------

class NodeBinaryOperator : public AbstractNode {
public:
    NodeBinaryOperator(Operator op, AbstractNode *a, AbstractNode *b) : op(op), a(a), b(b) { };

    ~NodeBinaryOperator();

    virtual AbstractType *evaluate(Environment *env);

private:
    Operator op;
    AbstractNode *a;
    AbstractNode *b;
};

// -----------------------------------------------------------------------------

class NodeNotOperator : public AbstractNode {
public:
    NodeNotOperator(AbstractNode *a) : a(a) { };

    ~NodeNotOperator();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *a;
};

// -----------------------------------------------------------------------------

class NodeConstant : public AbstractNode {
public:
    NodeConstant(AbstractType *value) : value(value) { };

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractType *value;
};

// -----------------------------------------------------------------------------

class NodeWhile : public AbstractNode {
public:
    NodeWhile(AbstractNode *condition, NodeBlock *block) : condition(condition), block(block) { };

    ~NodeWhile();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *condition;
    NodeBlock *block;
};

// -----------------------------------------------------------------------------

class NodeIfElse : public AbstractNode {
public:
    NodeIfElse(AbstractNode *condition, NodeBlock *ifBlock, NodeBlock *elseBlock) : condition(condition),
                                                                                    ifBlock(ifBlock),
                                                                                    elseBlock(elseBlock) { };

    ~NodeIfElse();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *condition;
    NodeBlock *ifBlock;
    NodeBlock *elseBlock;
};

// -----------------------------------------------------------------------------

class NodeScanInt : public AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env);
};

// -----------------------------------------------------------------------------

class NodeScanChar : public AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env);
};

#endif //TEETON_NODE_H
