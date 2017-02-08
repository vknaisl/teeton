#include <iostream>
#include <fstream>
#include <sstream>

#include "type.h"
#include "environment.h"
#include "node.h"
#include "parser.h"

using namespace std;

// -- Running program ----------------------------------------------------------

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

// -- REPL ---------------------------------------------------------------------

string readInput() {
    char c;
    vector<char> input;

    while (!(input.size() > 2 && input[input.size() - 1] == '\n' && input[input.size() - 2] == '\n')) {
        cin >> noskipws >> c;
        input.push_back(c);
    }

    ostringstream os;

    for (auto const &value : input) {
        os << value;
    }

    return os.str();
}

void repl() {
    cout << "TEETON console " << endl;
    cout << "use ctrl + C to exit" << endl;

    Environment *env = new Environment();
    Parser *parser = new Parser();

    for (; ;) {
        cout << "T> ";
        string source = readInput();
        try {
            AbstractNode *root = parser->parse(source);
            AbstractType *evaluated = root->evaluate(env);
            if (evaluated != nullptr) {
                cout << evaluated->toString() << endl;
            }
        } catch (TeetonError *e) {
            cout << e->err << endl;
            delete e;
        }
    }
}

// -- main ---------------------------------------------------------------------

int main(int argc, char *argv[]) {
    if (argc < 2) {
        repl();
    }
    runProgram(argv[1]);
    return 0;
}
