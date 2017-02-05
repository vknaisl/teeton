#ifndef TEETON_ENVIRONMENT_H
#define TEETON_ENVIRONMENT_H

#include <string>
#include <unordered_map>

#include "type.h"

class Environment {
public:
    void setVariable(std::string name, AbstractType *value);

    AbstractType *getVariable(std::string name);

    ~Environment();

    TypeBool *allocBool(bool value);

    TypeChar *allocChar(char value);

    TypeInt *allocInt(int value);

    TypeList *allocList(std::vector<AbstractType *> *value);

private:
    std::unordered_map<std::string, AbstractType *> variables;
    std::vector<AbstractType *> heap;
};

#endif //TEETON_ENVIRONMENT_H
