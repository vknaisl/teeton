#ifndef TEETON_LEXER_H
#define TEETON_LEXER_H

#include <iostream>

#include <vector>
#include <string>

#include "scanner.h"
#include "utils.h"

#define TOKEN_EOF "EOF"
#define TOKEN_IDENTIFIER "IDENTIFIER"
#define TOKEN_INT "INT"
#define TOKEN_CHAR "CHAR"
#define TOKEN_BOOL "BOOL"
#define TOKEN_STRING "STRING"
#define TOKEN_LIST "LIST"
#define TOKEN_SYMBOL "SYMBOL"
#define TOKEN_SCAN "SCAN"


class Token {
public:
    std::string cargo;
    int lineIndex;
    int colIndex;
    std::string tokenType;

    Token(Character *startChar);

    std::string toString();
};


class Lexer {
public:
    Lexer(Scanner * newScanner);
    ~Lexer();

    Token *get();

private:
    static const std::vector<std::string> Keywords;
    static const std::vector<std::string> BooleanKeywords;
    static const std::vector<std::string> ScanKeywords;
    static const std::string OneCharacterSymbols;
    static const std::vector<std::string> TwoCharacterSymbols;
    static const std::vector<std::string> ThreeCharacterSymbols;
    static const std::string IdentifierStartChars;
    static const std::string IdentifierChars;
    static const std::string IntStartChars;
    static const std::string IntChars;
    static const std::string WhiteSpaceChars;

    Scanner *scanner;

    void getChar();

    Character *character = nullptr;
    char c1;
    std::string c2;
    std::string c3;
};


#endif //TEETON_LEXER_H
