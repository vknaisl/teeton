#ifndef TEETON_NODE_H
#define TEETON_NODE_H


#include <iostream>
#include <exception>
#include <vector>

#include "type.h"


class AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env) = 0;

    virtual ~AbstractNode() = 0;
};

// -----------------------------------------------------------------------------

class NodeBlock : public AbstractNode {
public:
    NodeBlock(std::vector<AbstractNode *> *nodes) : nodes(nodes) { };

    ~NodeBlock();

    virtual AbstractType *evaluate(Environment *env);

private:
    std::vector<AbstractNode *> *nodes;
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
    NodePrint(AbstractNode *value, bool breakLine = true) : value(value), breakLine(breakLine) { };

    ~NodePrint();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *value;
    bool breakLine;
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

    ~NodeConstant();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractType *alloc(AbstractType *type, Environment *env);

    void clean(AbstractType *type);

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

// -----------------------------------------------------------------------------

class NodeScanString : public AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env);
};

// -----------------------------------------------------------------------------

class NodeBreak : public AbstractNode {
public:
    virtual AbstractType *evaluate(Environment *env);

    class BreakException : public std::exception {
    } breakException;
};

// -----------------------------------------------------------------------------

class NodeLen : public AbstractNode {
public:
    NodeLen(AbstractNode *expression) : expression(expression) { };

    ~NodeLen();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *expression;
};

// -----------------------------------------------------------------------------

class NodeAppend : public AbstractNode {
public:
    NodeAppend(AbstractNode *listExpression, AbstractNode *valueExpression) : listExpression(listExpression),
                                                                              valueExpression(valueExpression) { };

    ~NodeAppend();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *listExpression;
    AbstractNode *valueExpression;
};

// -----------------------------------------------------------------------------

class NodeGet : public AbstractNode {
public :
    NodeGet(AbstractNode *listExpression, AbstractNode *indexExpression) : listExpression(listExpression),
                                                                           indexExpression(indexExpression) { };

    ~NodeGet();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *listExpression;
    AbstractNode *indexExpression;
};

// -----------------------------------------------------------------------------

class NodeSet : public AbstractNode {
public :
    NodeSet(AbstractNode *listExpression, AbstractNode *indexExpression, AbstractNode *valueExpression)
            : listExpression(listExpression), indexExpression(indexExpression), valueExpression(valueExpression) { };

    ~NodeSet();

    virtual AbstractType *evaluate(Environment *env);

private:
    AbstractNode *listExpression;
    AbstractNode *indexExpression;
    AbstractNode *valueExpression;
};


#endif //TEETON_NODE_H
