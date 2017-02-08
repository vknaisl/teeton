#include <sstream>

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
    if (variables.find(name) == variables.end()) {
        ostringstream os;
        os << "Undefined variable " << name << ".";
        runtimeError(os.str());
    }

    return variables[name];
}

TypeBool *Environment::allocBool(bool value) {
    checkHeap();
    TypeBool *newBool = new TypeBool(value);
    heap.push_back(newBool);
    return newBool;
}

TypeChar *Environment::allocChar(char value) {
    checkHeap();
    TypeChar *newChar = new TypeChar(value);
    heap.push_back(newChar);
    return newChar;
}

TypeInt *Environment::allocInt(int value) {
    checkHeap();
    TypeInt *newInt = new TypeInt(value);
    heap.push_back(newInt);
    return newInt;
}

TypeList *Environment::allocList(std::vector<AbstractType *> *value) {
    checkHeap();
    TypeList *newList = new TypeList(value);
    heap.push_back(newList);
    return newList;
}

void Environment::checkHeap() {
    if (heap.size() >= heapSizeLimit) {
        markAndSweep();
    }

    if (heap.size() >= heapSizeLimit) {
        runtimeError("Out of memory");
    }
}

void Environment::markAndSweep() {
    markFalse();

    for (auto it : variables) {
        mark(it.second);
    }

    sweep();
}

void Environment::markFalse() {
    for (auto const &value : heap) {
        value->marked = false;
    }
}

void Environment::mark(AbstractType *variable) {
    variable->marked = true;

    if (variable->type() == LIST) {
        TypeList *list = (TypeList *) variable;
        for (auto const &item : *list->value()) {
            mark(item);
        }
    }
}

void Environment::sweep() {
    for (vector<AbstractType *>::iterator it = heap.end() - 1; it != heap.begin() - 1; it--) {
        if (!(*it)->marked) {
            delete *it;
            it = heap.erase(it);
        }
    }
}

