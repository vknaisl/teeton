#ifndef TEETON_ENVIRONMENT_H
#define TEETON_ENVIRONMENT_H

#include <string>
#include <unordered_map>

#include "utils.h"
#include "type.h"

class Environment {
public:
    Environment(int heapSizeLimit = 2048) : heapSizeLimit(heapSizeLimit) {};

    ~Environment();

    void setVariable(std::string name, AbstractType *value);

    AbstractType *getVariable(std::string name);

    TypeBool *allocBool(bool value);

    TypeChar *allocChar(char value);

    TypeInt *allocInt(int value);

    TypeList *allocList(std::vector<AbstractType *> *value);

private:
    int heapSizeLimit;
    std::unordered_map<std::string, AbstractType *> variables;
    std::vector<AbstractType *> heap;

    void checkHeap();

    void markAndSweep();

    void markFalse();

    void mark(AbstractType *value);

    void sweep();
};

#endif //TEETON_ENVIRONMENT_H
