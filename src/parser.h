#ifndef TEETON_PARSER_H
#define TEETON_PARSER_H

#include <vector>
#include <stack>

#include "lexer.h"
#include "node.h"
#include "utils.h"

class Parser {
public:
    AbstractNode *parse(std::string source);

private:
    Lexer *lexer;

    void assertToken(Token *token, std::string expectedType, std::string expectedCargo);

    void assertNextToken(std::string expectedType, std::string expectedCargo);

    NodeBlock *parseBlock();

    AbstractNode *parseLineExpression(Token *first);

    AbstractNode *parseExpression(std::vector<Token *> input, int lineIndex, int colIndex);

    AbstractNode *parseVarDefinition(Token *identifier);

    AbstractNode *parsePrint(bool breakLine);

    AbstractNode *parseWhile();

    AbstractNode *parseIfElse();

    AbstractNode *parseScanToken(Token *token);

    std::vector<Token *> readExpressionInput();

    int operatorPriority(Token *op);

    void processOperator(Token *op, std::stack<AbstractNode *> *output);

    void createBinaryOperator(Operator op, std::stack<AbstractNode *> *output, Token *token);

    void createNotOperator(std::stack<AbstractNode *> *output, Token *token);

    void createLenFunction(std::stack<AbstractNode *> *output, Token *token);

    void createAppendFunction(std::stack<AbstractNode *> *output, Token *token);

    void createGetFunction(std::stack<AbstractNode *> *output, Token *token);

    void createSetFunction(std::stack<AbstractNode *> *output, Token *token);

    static const std::vector<std::string> OperatorPriority1;
    static const std::vector<std::string> OperatorPriority2;
    static const std::vector<std::string> OperatorPriority3;
    static const std::vector<std::string> OperatorPriority4;
    static const std::vector<std::string> OperatorPriority5;
    static const std::vector<std::string> OperatorPriority6;
    static const std::vector<std::string> OperatorPriority7;
};


#endif //TEETON_PARSER_H
