#ifndef TEETON_UTILS_H
#define TEETON_UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <exception>

void runtimeError(std::string err);

void parseError(std::string err, int lineIndex, int colIndex);

bool contains(const std::string &str, char c);

bool contains(const std::vector<std::string> &vec, std::string s);

class TeetonError : public std::exception {
public:
    TeetonError(std::string err) : err(err) { };

    const std::string err;
};

#endif //TEETON_UTILS_H
