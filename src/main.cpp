#include<iostream>

#include "type.h"
#include "environment.h"
#include "node.h"

using namespace std;

// EXAMPLES ---

/**
 * var a = 5
 * var b = 2
 * var c = a - b
 * print(c)
 */
void example_arithmetic() {
    Environment * env = new Environment();
    cout << "* Example arithmetic. Expected result: 3" << endl;
    AbstractNode *nodes[4] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("b", new NodeConstant(new TypeInt(2))),
            new NodeVariableDefinition("c", new NodeBinaryOperator(SUB, new NodeVariableName("a"),
                                                                   new NodeVariableName("b"))),
            new NodePrint(new NodeVariableName("c"))
    };
    NodeBlock *root = new NodeBlock(nodes, 4);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var a = 5
 * var b = 5
 * var c = a >= b
 * print(c)
 */
void example_comparison() {
    Environment * env = new Environment();
    cout << "* Example comparison. Expected result: True" << endl;
    AbstractNode *nodes[4] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("b", new NodeConstant(new TypeInt(5))),
            new NodeVariableDefinition("c", new NodeBinaryOperator(GTE, new NodeVariableName("a"),
                                                                   new NodeVariableName("b"))),
            new NodePrint(new NodeVariableName("c"))
    };
    NodeBlock *root = new NodeBlock(nodes, 4);
    root->evaluate(env);
    delete root;
    delete env;
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
    Environment * env = new Environment();
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
    root->evaluate(env);
    delete root;
    delete env;
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
    Environment * env = new Environment();
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
    root->evaluate(env);
    delete root;
    delete env;
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
    Environment * env = new Environment();
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
    root->evaluate(env);
    delete root;
    delete env;
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
