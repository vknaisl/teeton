#include<iostream>

#include "type.h"
#include "environment.h"
#include "node.h"
#include "scanner.h"
#include "lexer.h"

using namespace std;

// EXAMPLES ---

/**
 * var a = 5
 * var b = 2
 * var c = a - b
 * print(c)
 */
void example_arithmetic() {
    Environment *env = new Environment();
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
    Environment *env = new Environment();
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
    Environment *env = new Environment();
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
    Environment *env = new Environment();
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
    Environment *env = new Environment();
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

/**
 * var a = 'a'
 * var b = 'a'
 *
 * print(a == b)
 * print(a === b)
 * print(a === a)
 */
void example_referenceComparison() {
    Environment *env = new Environment();
    cout << "* Example reference comparison. Expected result: True, False, True" << endl;
    AbstractNode *nodes[5] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeChar('a'))),
            new NodeVariableDefinition("b", new NodeConstant(new TypeChar('a'))),
            new NodePrint(new NodeBinaryOperator(EQ, new NodeVariableName("a"), new NodeVariableName("b"))),
            new NodePrint(new NodeBinaryOperator(EQEQ, new NodeVariableName("a"), new NodeVariableName("b"))),
            new NodePrint(new NodeBinaryOperator(EQEQ, new NodeVariableName("a"), new NodeVariableName("a")))
    };
    NodeBlock *root = new NodeBlock(nodes, 5);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var a = scan_int()
 * print(a)
 *
 * var b = scan_char()
 * print(b)
 *
 * var sum = scan_int() + scan_int()
 * print(sum)
 */
void example_readingInput() {
    Environment *env = new Environment();
    cout << "* Example reading user input" << endl;
    AbstractNode *nodes[6] = {
            new NodeVariableDefinition("a", new NodeScanInt()),
            new NodePrint(new NodeVariableName("a")),
            new NodeVariableDefinition("b", new NodeScanChar()),
            new NodePrint(new NodeVariableName("b")),
            new NodeVariableDefinition("sum", new NodeBinaryOperator(ADD, new NodeScanInt(), new NodeScanInt())),
            new NodePrint(new NodeVariableName("sum"))
    };
    NodeBlock *root = new NodeBlock(nodes, 6);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var x = scan_string()
 * print(x)
 */
void example_scanString() {
    Environment *env = new Environment();
    cout << "* Example scan string" << endl;
    AbstractNode *nodes[2] = {
            new NodeVariableDefinition("x", new NodeScanString()),
            new NodePrint(new NodeVariableName("x"))
    };
    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var i = 0
 * while(true) {
 *   print(i)
 *   if (i > 3) {
 *     break
 *   } else {
 *     i = i + 1
 *   }
 * }
 */
void example_break() {
    Environment *env = new Environment();
    cout << "* Example break. Expected result: lines [0 .. 4]" << endl;
    AbstractNode *ifBlock[1] = {
            new NodeBreak()
    };

    AbstractNode *elseBlock[1] = {
            new NodeVariableDefinition("i", new NodeBinaryOperator(ADD, new NodeVariableName("i"),
                                                                   new NodeConstant(new TypeInt(1))))
    };

    AbstractNode *whileBlock[2] = {
            new NodePrint(new NodeVariableName("i")),
            new NodeIfElse(
                    new NodeBinaryOperator(GT, new NodeVariableName("i"), new NodeConstant(new TypeInt(3))),
                    new NodeBlock(ifBlock, 1),
                    new NodeBlock(elseBlock, 1)
            )
    };

    AbstractNode *nodes[2] = {
            new NodeVariableDefinition("i", new NodeConstant(new TypeInt(0))),
            new NodeWhile(new NodeConstant(new TypeBool(true)), new NodeBlock(whileBlock, 2))
    };
    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

void example_list() {
    Environment *env = new Environment();
    cout << "* Example list. Expected output: [1, 2, 3] [1, 2] ['a', 'b']" << endl;

    vector<AbstractType *> * xs = new vector<AbstractType *>();
    xs->push_back(new TypeInt(1));
    xs->push_back(new TypeInt(2));

    vector<AbstractType *> * ys = new vector<AbstractType *>();
    ys->push_back(new TypeInt(3));

    vector<AbstractType *> * zs = new vector<AbstractType *>();
    zs->push_back(new TypeChar('a'));
    zs->push_back(new TypeChar('b'));

    AbstractNode *nodes[6] = {
            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
            new NodeVariableDefinition("ys", new NodeConstant(new TypeList(ys))),
            new NodeVariableDefinition("zs", new NodeConstant(new TypeList(zs))),
            new NodePrint(new NodeBinaryOperator(ADD, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeVariableName("xs")),
            new NodePrint(new NodeVariableName("zs"))
    };

    NodeBlock *root = new NodeBlock(nodes, 6);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

void example_listComparison() {
    Environment *env = new Environment();
    cout << "* Example list comparison. Expected output: False, True, False, True, False True" << endl;

    vector<AbstractType *> * xs = new vector<AbstractType *>();
    xs->push_back(new TypeChar('D'));
    xs->push_back(new TypeChar('o'));
    xs->push_back(new TypeChar('g'));
    xs->push_back(new TypeChar('s'));

    vector<AbstractType *> * ys = new vector<AbstractType *>();
    ys->push_back(new TypeChar('F'));
    ys->push_back(new TypeChar('i'));
    ys->push_back(new TypeChar('s'));
    ys->push_back(new TypeChar('h'));


    AbstractNode *nodes[8] = {
            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
            new NodeVariableDefinition("ys", new NodeConstant(new TypeList(ys))),
            new NodePrint(new NodeBinaryOperator(EQ, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeBinaryOperator(NEQ, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeBinaryOperator(GT, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeBinaryOperator(LT, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeBinaryOperator(GTE, new NodeVariableName("xs"), new NodeVariableName("ys"))),
            new NodePrint(new NodeBinaryOperator(LTE, new NodeVariableName("xs"), new NodeVariableName("ys")))
    };

    NodeBlock *root = new NodeBlock(nodes, 8);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}


void example_listLen() {
    Environment *env = new Environment();
    cout << "* Example list len. Expected output: 4" << endl;

    vector<AbstractType *> * xs = new vector<AbstractType *>();
    xs->push_back(new TypeInt(1));
    xs->push_back(new TypeInt(2));
    xs->push_back(new TypeInt(3));
    xs->push_back(new TypeInt(4));

    AbstractNode *nodes[] = {
            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
            new NodePrint(new NodeLen(new NodeVariableName("xs")))
    };

    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var xs = [1, 2]
 * append(xs, 3)
 * print(xs)
 */
void example_listAppend() {
    Environment *env = new Environment();
    cout << "* Example list len. Expected output: [1, 2, 3]" << endl;

    vector<AbstractType *> * xs = new vector<AbstractType *>();
    xs->push_back(new TypeInt(1));
    xs->push_back(new TypeInt(2));

    AbstractNode *nodes[] = {
            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
            new NodeAppend(new NodeVariableName("xs"), new NodeConstant(new TypeInt(3))),
            new NodePrint(new NodeVariableName("xs"))
    };

    NodeBlock *root = new NodeBlock(nodes, 3);
    root->evaluate(env);
    delete root;
    delete env;
    cout << endl;
}

/**
 * var i = 0
 * while(i < 1000) {
 *   var a = 12
 *   i = i + 1
 * }
 */
void example_garbageCollector() {
    cout << "* Example garbage collector. Should not crash :)" << endl;
    Environment *env = new Environment(16);

    AbstractNode *whileBody[] = {
            new NodeVariableDefinition("a", new NodeConstant(new TypeInt(12))),
            new NodeVariableDefinition("i", new NodeBinaryOperator(ADD, new NodeVariableName("i"), new NodeConstant(new TypeInt(1))))
    };


    AbstractNode *nodes[] = {
            new NodeVariableDefinition("i", new NodeConstant(new TypeInt(0))),
            new NodeWhile(
                    new NodeBinaryOperator(LT, new NodeVariableName("i"), new NodeConstant(new TypeInt(1000))),
                    new NodeBlock(whileBody, 2)
            )
    };

    NodeBlock *root = new NodeBlock(nodes, 2);
    root->evaluate(env);
    delete root;
    delete env;
    cout << "done" << endl << endl;
}


void example_scanner() {
    string source = "var a = 1 + 2\nvar b = 'a'\n";
    Scanner *scanner = new Scanner(source);

    Character *character = scanner->get();
    while(true) {
        cout << character->toString() << endl;
        if (character->cargo == Character::ENDMARK) {
            break;
        }
        delete character;
        character = scanner->get();
    }
    delete character;
    delete scanner;
}

void example_lexer() {
    string source = "var a = 1 + 2\nvar b = 'a'\nvar c = \"as = df\"\nwhile(a > 2 && True){\nprint(c)\n}\n";
    Scanner *scanner = new Scanner(source);
    Lexer *lexer = new Lexer(scanner);

    try {
        Token *token = lexer->get();
        while (true) {
            cout << token->toString() << endl;
            if (token->tokenType == TOKEN_EOF) {
                break;
            }
            delete token;
            token = lexer->get();
        }
        delete token;
    } catch (string &e) {
        cout << e << endl;
    }

    delete lexer;
    delete scanner;
}


int main() {
//    example_arithmetic();
//    example_comparison();
//    example_whileCycle();
//    example_ifElse();
//    example_complicatedCondition();
//    example_referenceComparison();
////    example_readingInput();
////    example_scanString();
//    example_break();
//    example_list();
//    example_listComparison();
//    example_garbageCollector();
//    example_listLen();
//    example_listAppend();
//    example_scanner();
    example_lexer();
    return 0;
}
