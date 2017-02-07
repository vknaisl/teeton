#include<iostream>
#include <fstream>

#include "type.h"
#include "environment.h"
#include "node.h"
#include "parser.h"

using namespace std;


//
///**
// * var a = scan_int()
// * print(a)
// *
// * var b = scan_char()
// * print(b)
// *
// * var sum = scan_int() + scan_int()
// * print(sum)
// */
//void example_readingInput() {
//    Environment *env = new Environment();
//    cout << "* Example reading user input" << endl;
//    AbstractNode *nodes[6] = {
//            new NodeVariableDefinition("a", new NodeScanInt()),
//            new NodePrint(new NodeVariableName("a")),
//            new NodeVariableDefinition("b", new NodeScanChar()),
//            new NodePrint(new NodeVariableName("b")),
//            new NodeVariableDefinition("sum", new NodeBinaryOperator(ADD, new NodeScanInt(), new NodeScanInt())),
//            new NodePrint(new NodeVariableName("sum"))
//    };
//    NodeBlock *root = new NodeBlock(nodes, 6);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
///**
// * var x = scan_string()
// * print(x)
// */
//void example_scanString() {
//    Environment *env = new Environment();
//    cout << "* Example scan string" << endl;
//    AbstractNode *nodes[2] = {
//            new NodeVariableDefinition("x", new NodeScanString()),
//            new NodePrint(new NodeVariableName("x"))
//    };
//    NodeBlock *root = new NodeBlock(nodes, 2);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
///**
// * var i = 0
// * while(true) {
// *   print(i)
// *   if (i > 3) {
// *     break
// *   } else {
// *     i = i + 1
// *   }
// * }
// */
//void example_break() {
//    Environment *env = new Environment();
//    cout << "* Example break. Expected result: lines [0 .. 4]" << endl;
//    AbstractNode *ifBlock[1] = {
//            new NodeBreak()
//    };
//
//    AbstractNode *elseBlock[1] = {
//            new NodeVariableDefinition("i", new NodeBinaryOperator(ADD, new NodeVariableName("i"),
//                                                                   new NodeConstant(new TypeInt(1))))
//    };
//
//    AbstractNode *whileBlock[2] = {
//            new NodePrint(new NodeVariableName("i")),
//            new NodeIfElse(
//                    new NodeBinaryOperator(GT, new NodeVariableName("i"), new NodeConstant(new TypeInt(3))),
//                    new NodeBlock(ifBlock, 1),
//                    new NodeBlock(elseBlock, 1)
//            )
//    };
//
//    AbstractNode *nodes[2] = {
//            new NodeVariableDefinition("i", new NodeConstant(new TypeInt(0))),
//            new NodeWhile(new NodeConstant(new TypeBool(true)), new NodeBlock(whileBlock, 2))
//    };
//    NodeBlock *root = new NodeBlock(nodes, 2);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
//void example_list() {
//    Environment *env = new Environment();
//    cout << "* Example list. Expected output: [1, 2, 3] [1, 2] ['a', 'b']" << endl;
//
//    vector<AbstractType *> * xs = new vector<AbstractType *>();
//    xs->push_back(new TypeInt(1));
//    xs->push_back(new TypeInt(2));
//
//    vector<AbstractType *> * ys = new vector<AbstractType *>();
//    ys->push_back(new TypeInt(3));
//
//    vector<AbstractType *> * zs = new vector<AbstractType *>();
//    zs->push_back(new TypeChar('a'));
//    zs->push_back(new TypeChar('b'));
//
//    AbstractNode *nodes[6] = {
//            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
//            new NodeVariableDefinition("ys", new NodeConstant(new TypeList(ys))),
//            new NodeVariableDefinition("zs", new NodeConstant(new TypeList(zs))),
//            new NodePrint(new NodeBinaryOperator(ADD, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeVariableName("xs")),
//            new NodePrint(new NodeVariableName("zs"))
//    };
//
//    NodeBlock *root = new NodeBlock(nodes, 6);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
//void example_listComparison() {
//    Environment *env = new Environment();
//    cout << "* Example list comparison. Expected output: False, True, False, True, False True" << endl;
//
//    vector<AbstractType *> * xs = new vector<AbstractType *>();
//    xs->push_back(new TypeChar('D'));
//    xs->push_back(new TypeChar('o'));
//    xs->push_back(new TypeChar('g'));
//    xs->push_back(new TypeChar('s'));
//
//    vector<AbstractType *> * ys = new vector<AbstractType *>();
//    ys->push_back(new TypeChar('F'));
//    ys->push_back(new TypeChar('i'));
//    ys->push_back(new TypeChar('s'));
//    ys->push_back(new TypeChar('h'));
//
//
//    AbstractNode *nodes[8] = {
//            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
//            new NodeVariableDefinition("ys", new NodeConstant(new TypeList(ys))),
//            new NodePrint(new NodeBinaryOperator(EQ, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeBinaryOperator(NEQ, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeBinaryOperator(GT, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeBinaryOperator(LT, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeBinaryOperator(GTE, new NodeVariableName("xs"), new NodeVariableName("ys"))),
//            new NodePrint(new NodeBinaryOperator(LTE, new NodeVariableName("xs"), new NodeVariableName("ys")))
//    };
//
//    NodeBlock *root = new NodeBlock(nodes, 8);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
//
//void example_listLen() {
//    Environment *env = new Environment();
//    cout << "* Example list len. Expected output: 4" << endl;
//
//    vector<AbstractType *> * xs = new vector<AbstractType *>();
//    xs->push_back(new TypeInt(1));
//    xs->push_back(new TypeInt(2));
//    xs->push_back(new TypeInt(3));
//    xs->push_back(new TypeInt(4));
//
//    AbstractNode *nodes[] = {
//            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
//            new NodePrint(new NodeLen(new NodeVariableName("xs")))
//    };
//
//    NodeBlock *root = new NodeBlock(nodes, 2);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
///**
// * var xs = [1, 2]
// * append(xs, 3)
// * print(xs)
// */
//void example_listAppend() {
//    Environment *env = new Environment();
//    cout << "* Example list len. Expected output: [1, 2, 3]" << endl;
//
//    vector<AbstractType *> * xs = new vector<AbstractType *>();
//    xs->push_back(new TypeInt(1));
//    xs->push_back(new TypeInt(2));
//
//    AbstractNode *nodes[] = {
//            new NodeVariableDefinition("xs", new NodeConstant(new TypeList(xs))),
//            new NodeAppend(new NodeVariableName("xs"), new NodeConstant(new TypeInt(3))),
//            new NodePrint(new NodeVariableName("xs"))
//    };
//
//    NodeBlock *root = new NodeBlock(nodes, 3);
//    root->evaluate(env);
//    delete root;
//    delete env;
//    cout << endl;
//}
//
//


void runProgram(char *path) {
    ifstream file(path);
    string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    Parser *parser = new Parser();

    try {
        AbstractNode *root = parser->parse(source);
        Environment *env = new Environment();
        root->evaluate(env);
        delete env;
        delete root;
    } catch (TeetonError *e) {
        cout << e->err << endl;
        delete e;
    }

    delete parser;
}

int main(int argc, char *argv[]) {
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
//    example_lexer();
//    example_parser();
    if (argc < 2) {
        // TODO repl
        cout << "No input file" << endl;
        return 1;
    }

    runProgram(argv[1]);

    return 0;
}
