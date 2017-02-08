#include<iostream>
#include <fstream>

#include "type.h"
#include "environment.h"
#include "node.h"
#include "parser.h"

using namespace std;

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
    if (argc < 2) {
        // TODO repl
        cout << "No input file" << endl;
        return 1;
    }

    runProgram(argv[1]);

    return 0;
}
