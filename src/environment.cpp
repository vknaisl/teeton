#include "environment.h"

using namespace std;

Environment::~Environment() {
    for (auto it : variables) {
        delete it.second;
    }
}

void Environment::setVariable(string name, AbstractType *value) {
    variables[name] = value;
}

AbstractType *Environment::getVariable(string name) {
    return variables[name];
}