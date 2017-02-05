#include "environment.h"

using namespace std;

Environment::~Environment() {
    for (auto const &value : heap) {
        delete value;
    }
}

void Environment::setVariable(string name, AbstractType *value) {
    variables[name] = value;
}

AbstractType *Environment::getVariable(string name) {
    return variables[name];
}

TypeBool *Environment::allocBool(bool value) {
    TypeBool *newBool = new TypeBool(value);
    heap.push_back(newBool);
    return newBool;
}

TypeChar *Environment::allocChar(char value) {
    TypeChar *newChar = new TypeChar(value);
    heap.push_back(newChar);
    return newChar;
}

TypeInt *Environment::allocInt(int value) {
    TypeInt *newInt = new TypeInt(value);
    heap.push_back(newInt);
    return newInt;
}

TypeList *Environment::allocList(std::vector<AbstractType *> *value) {
    TypeList *newList = new TypeList(value);
    heap.push_back(newList);
    return newList;
}
