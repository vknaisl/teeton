#include "parser.h"

using namespace std;


const vector<string> Parser::OperatorPriority1({"!"});
const vector<string> Parser::OperatorPriority2({"*", "/", "%"});
const vector<string> Parser::OperatorPriority3({"+", "-"});
const vector<string> Parser::OperatorPriority4({">", "<", ">=", "<="});
const vector<string> Parser::OperatorPriority5({"==", "===", "!="});
const vector<string> Parser::OperatorPriority6({"&&"});
const vector<string> Parser::OperatorPriority7({"||"});


AbstractNode *Parser::parse(string source) {
    Scanner *scanner = new Scanner(source);
    lexer = new Lexer(scanner);
    AbstractNode *result = parseBlock();

    delete lexer;
    delete scanner;

    return result;
}

void Parser::assertToken(Token *token, string expectedType, string expectedCargo) {
    if (token->tokenType != expectedType || (expectedCargo.length() > 0 && token->cargo != expectedCargo)) {
        ostringstream os;
        os << "Unexpected token " << token->cargo << ", expecting ";
        if (expectedCargo.length() > 0) {
            os << expectedCargo << ".";
        } else {
            os << expectedType << ".";
        }

        parseError(os.str(), token->lineIndex, token->colIndex);
    }
}

void Parser::assertNextToken(std::string expectedType, std::string expectedCargo) {
    Token *token = lexer->get();
    assertToken(token, expectedType, expectedCargo);
    delete token;
}


NodeBlock *Parser::parseBlock() {
    vector<AbstractNode *> *nodes = new vector<AbstractNode *>();
    Token *token = lexer->get();
    while (token->tokenType != TOKEN_EOF && token->cargo != "}") {
        if (token->tokenType == TOKEN_SYMBOL) {
            if (token->cargo == "print" || token->cargo == "println") {
                nodes->push_back(parsePrint(token->cargo == "println"));
            } else if (token->cargo == "while") {
                nodes->push_back(parseWhile());
            } else if (token->cargo == "if") {
                nodes->push_back(parseIfElse());
            } else if (token->cargo == "\n") { // newlines ignored inside block
            } else if (token->cargo == "break") {
                nodes->push_back(new NodeBreak());
            } else {
                nodes->push_back(parseLineExpression(token));
            }
        } else if (token->tokenType == TOKEN_IDENTIFIER) {
            nodes->push_back(parseVarDefinition(token));
        } else {
            nodes->push_back(parseLineExpression(token));
        }

        delete token;
        token = lexer->get();
    }
    delete token;

    return new NodeBlock(nodes);
}

AbstractNode *Parser::parseLineExpression(Token *first) {
    Token *token = lexer->get();
    vector<Token *> input;
    input.push_back(first);
    while (token->cargo != "\n") {
        input.push_back(token);
        token = lexer->get();
    }
    AbstractNode *expression = parseExpression(input, token->lineIndex, token->colIndex);

    delete token;
    bool isFirst = true;
    for (auto const &value: input) {
        if (isFirst) {
            isFirst = false;
            continue;
        }
        delete value;
    }


    return expression;
}


AbstractNode *Parser::parseVarDefinition(Token *identifier) {
    assertNextToken(TOKEN_SYMBOL, "=");

    vector<Token *> input;

    Token *token = lexer->get();
    while (token->cargo != "\n") {
        input.push_back(token);
        token = lexer->get();
    }

    AbstractNode *expression = parseExpression(input, token->lineIndex, token->colIndex);
    NodeVariableDefinition *varDefinition = new NodeVariableDefinition(identifier->cargo, expression);

    delete token;
    for (auto const &value: input) {
        delete value;
    }

    return varDefinition;
}

AbstractNode *Parser::parsePrint(bool breakLine) {
    Token *token = lexer->get();
    assertToken(token, TOKEN_SYMBOL, "(");

    vector<Token *> input = readExpressionInput();
    AbstractNode *expression = parseExpression(input, token->lineIndex, token->colIndex);
    NodePrint *nodePrint = new NodePrint(expression, breakLine);

    delete token;
    for (auto const &value: input) {
        delete value;
    }

    return nodePrint;
}

AbstractNode *Parser::parseWhile() {
    Token *token = lexer->get();
    assertToken(token, TOKEN_SYMBOL, "(");

    vector<Token *> conditionInput = readExpressionInput();
    AbstractNode *condition = parseExpression(conditionInput, token->lineIndex, token->colIndex);

    assertNextToken(TOKEN_SYMBOL, "{");
    assertNextToken(TOKEN_SYMBOL, "\n");

    NodeBlock *block = parseBlock();

    assertNextToken(TOKEN_SYMBOL, "\n");

    delete token;
    for (auto const &value: conditionInput) {
        delete value;
    }

    return new NodeWhile(condition, block);
}

AbstractNode *Parser::parseIfElse() {
    Token *token = lexer->get();
    assertToken(token, TOKEN_SYMBOL, "(");

    vector<Token *> conditionInput = readExpressionInput();
    AbstractNode *condition = parseExpression(conditionInput, token->lineIndex, token->colIndex);

    assertNextToken(TOKEN_SYMBOL, "{");
    assertNextToken(TOKEN_SYMBOL, "\n");

    NodeBlock *ifBlock = parseBlock();

    assertNextToken(TOKEN_SYMBOL, "else");
    assertNextToken(TOKEN_SYMBOL, "{");

    NodeBlock *elseBlock = parseBlock();

    assertNextToken(TOKEN_SYMBOL, "\n");

    delete token;
    for (auto const &value: conditionInput) {
        delete value;
    }

    return new NodeIfElse(condition, ifBlock, elseBlock);
}

std::vector<Token *> Parser::readExpressionInput() {
    vector<Token *> input;
    Token *token = lexer->get();
    int depth = 0;
    while (!(token->cargo == ")" && depth == 0)) {
        if (token->cargo == "(") depth++;
        if (token->cargo == ")") depth--;
        if (token->cargo == "\n") parseError("Unexpected newline.", token->lineIndex, token->colIndex);

        input.push_back(token);
        token = lexer->get();
    }
    delete token;
    return input;
}

AbstractNode *Parser::parseExpression(vector<Token *> input, int lineIndex, int colIndex) {
    stack<AbstractNode *> *output = new stack<AbstractNode *>();
    stack<Token *> operatorStack;
    for (auto const &value : input) {
        if (value->tokenType == TOKEN_INT) {
            output->push(new NodeConstant(new TypeInt(stoi(value->cargo))));
        } else if (value->tokenType == TOKEN_CHAR) {
            output->push(new NodeConstant(new TypeChar(value->cargo[0])));
        } else if (value->tokenType == TOKEN_STRING) {
            vector<AbstractType *> *chars = new vector<AbstractType *>();
            for (unsigned int i = 0; i < value->cargo.length(); i++) {
                chars->push_back(new TypeChar(value->cargo[i]));
            }
            output->push(new NodeConstant(new TypeList(chars)));
        } else if (value->tokenType == TOKEN_BOOL) {
            output->push(new NodeConstant(new TypeBool(value->cargo == "True")));
        } else if (value->tokenType == TOKEN_SCAN) {
            output->push(parseScanToken(value));
        } else if (value->tokenType == TOKEN_LIST) {
            output->push(new NodeConstant(new TypeList(new vector<AbstractType *>())));
        } else if (value->tokenType == TOKEN_IDENTIFIER) {
            output->push(new NodeVariableName(value->cargo));
        } else if (value->tokenType == TOKEN_SYMBOL) {
            if (value->cargo == "(") {
                operatorStack.push(value);
            } else if (value->cargo == ")") {
                while (!operatorStack.empty() && operatorStack.top()->cargo != "(") {
                    processOperator(operatorStack.top(), output);
                    operatorStack.pop();
                }
                if (operatorStack.empty()) {
                    parseError("Missing (.", value->lineIndex, value->colIndex);
                }
                operatorStack.pop();
            } else {
                while (!operatorStack.empty() && operatorPriority(value) <= operatorPriority(operatorStack.top())) {
                    processOperator(operatorStack.top(), output);
                    operatorStack.pop();
                }
                operatorStack.push(value);
            }
        }
    }

    while (!operatorStack.empty()) {
        processOperator(operatorStack.top(), output);
        operatorStack.pop();
    }

    if (output->size() != 1) {
        parseError("Invalid expression.", lineIndex, colIndex);
    }

    AbstractNode *result = output->top();
    delete output;

    return result;
}

AbstractNode *Parser::parseScanToken(Token *token) {
    if (token->cargo == "scan_int") {
        return new NodeScanInt();
    } else if (token->cargo == "scan_char") {
        return new NodeScanChar();
    } else {
        return new NodeScanString();
    }
}

void Parser::processOperator(Token *op, std::stack<AbstractNode *> *output) {
    string val = op->cargo;

    if (val == "+") createBinaryOperator(ADD, output, op);
    else if (val == "-") createBinaryOperator(SUB, output, op);
    else if (val == "*") createBinaryOperator(MUL, output, op);
    else if (val == "/") createBinaryOperator(DIV, output, op);
    else if (val == "%") createBinaryOperator(MOD, output, op);
    else if (val == "==") createBinaryOperator(EQ, output, op);
    else if (val == "!=") createBinaryOperator(NEQ, output, op);
    else if (val == "===") createBinaryOperator(EQEQ, output, op);
    else if (val == "<") createBinaryOperator(LT, output, op);
    else if (val == ">") createBinaryOperator(GT, output, op);
    else if (val == "<=") createBinaryOperator(LTE, output, op);
    else if (val == ">=") createBinaryOperator(GTE, output, op);
    else if (val == "&&") createBinaryOperator(AND, output, op);
    else if (val == "||") createBinaryOperator(OR, output, op);
    else if (val == "!") createNotOperator(output, op);
    else if (val == "len") createLenFunction(output, op);
    else if (val == "append") createAppendFunction(output, op);
//    else cout << "we just ignore it " << val << endl;
}

void Parser::createBinaryOperator(Operator op, stack<AbstractNode *> *output, Token *token) {
    if (output->size() < 2) {
        parseError("Not enough operands for binary operator.", token->lineIndex, token->colIndex);
        return;
    }

    AbstractNode *operand2 = output->top();
    output->pop();

    AbstractNode *operand1 = output->top();
    output->pop();

    NodeBinaryOperator *binaryOperator = new NodeBinaryOperator(op, operand1, operand2);
    output->push(binaryOperator);
}

void Parser::createNotOperator(stack<AbstractNode *> *output, Token *token) {
    if (output->size() < 1) {
        parseError("Not enough operands for ! operator.", token->lineIndex, token->colIndex);
        return;
    }
    NodeNotOperator *notOperator = new NodeNotOperator(output->top());
    output->pop();
    output->push(notOperator);
}

void Parser::createLenFunction(std::stack<AbstractNode *> *output, Token *token) {
    if (output->size() < 1) {
        parseError("Missing argument for len function.", token->lineIndex, token->colIndex);
        return;
    }
    NodeLen *len = new NodeLen(output->top());
    output->pop();
    output->push(len);
}

void Parser::createAppendFunction(std::stack<AbstractNode *> *output, Token * token) {
    if (output->size() < 2) {
        parseError("Not enough arguments for append function", token->lineIndex, token->colIndex);
        return;
    }

    AbstractNode *elementExpression = output->top();
    output->pop();

    AbstractNode *listExpression = output->top();
    output->pop();

    NodeAppend *append = new NodeAppend(listExpression, elementExpression);
    output->push(append);
}

int Parser::operatorPriority(Token *op) {
    if (contains(OperatorPriority1, op->cargo)) {
        return 10;
    } else if (contains(OperatorPriority2, op->cargo)) {
        return 9;
    } else if (contains(OperatorPriority3, op->cargo)) {
        return 8;
    } else if (contains(OperatorPriority4, op->cargo)) {
        return 7;
    } else if (contains(OperatorPriority5, op->cargo)) {
        return 6;
    } else if (contains(OperatorPriority6, op->cargo)) {
        return 5;
    } else if (contains(OperatorPriority7, op->cargo)) {
        return 4;
    } else if (op->cargo == "(") {
        return 0;
    } else { // it must be function - higher priority
        return 12;
    }
}
