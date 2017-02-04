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

private:
    std::unordered_map<std::string, AbstractType *> variables;
};

#endif //TEETON_ENVIRONMENT_H
