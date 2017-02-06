#include <algorithm>
#include <iomanip>
#include <sstream>

#include "lexer.h"

using namespace std;

bool contains(const string &str, char c) {
    return str.find(c) != string::npos;
}

bool contains(const vector<string> &vec, string s) {
    return find(vec.begin(), vec.end(), s) != vec.end();
}

// -----------------------------------------------------------------------------

const vector<string> Lexer::Keywords(
        {"var", "if", "else", "while", "print", "println", "scan_int", "scan_char", "scan_string", "list", "append",
         "len", "True", "False"});
const string Lexer::OneCharacterSymbols("=+-*/<>!()[]{}\n");

const vector<string> Lexer::TwoCharacterSymbols({"<=", ">=", "==", "&&", "||"});

const vector<string> Lexer::ThreeCharacterSymbols({"==="});

const string Lexer::IdentifierStartChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_");

const string Lexer::IdentifierChars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789");

const string Lexer::IntStartChars("0123456789");

const string Lexer::IntChars("0123456789");

const string Lexer::WhiteSpaceChars(" \t");

// -----------------------------------------------------------------------------

Token::Token(Character *startChar) {
    lineIndex = startChar->lineIndex;
    colIndex = startChar->colIndex;
    tokenType = "";
}

string Token::toString() {
    ostringstream os;
    string printable;

    if (cargo == "\n") {
        printable = "NEWLINE";
    } else {
        printable = cargo;
    }

    os << setw(4) << lineIndex << " " << setw(4) << colIndex << " " << setw(12) << tokenType << " " << printable;
    return os.str();
}

// -----------------------------------------------------------------------------

Lexer::Lexer(Scanner *newScanner) {
    scanner = newScanner;
    getChar();
}

void Lexer::getChar() {
    if (character != nullptr) {
        delete character;
    }

    character = scanner->get();
    c1 = character->cargo;

    Character *character2 = scanner->lookahead(1);
    c2 = "";
    c2.push_back(c1);
    c2.push_back(character2->cargo);
    delete character2;

    Character *character3 = scanner->lookahead(2);
    c3 = c2;
    c3.push_back(character3->cargo);
    delete character3;
}

Lexer::~Lexer() {
    delete character;
}

Token *Lexer::get() {
    // ignore whitespaces and comments
    while (contains(WhiteSpaceChars, c1) || c1 == '#') {
        // ignore whitespaces
        while (contains(WhiteSpaceChars, c1)) {
            getChar();
        }

        // ignore commment lines
        while (c1 == '#') {
            getChar();
            while (c1 != '\n' || c1 != Character::ENDMARK) {
                getChar();
            }
        }
    }

    Token *token = new Token(character);

    // end of file token
    if (c1 == Character::ENDMARK) {
        token->tokenType = TOKEN_EOF;
        return token;
    }

    // identifier token
    if (contains(IdentifierStartChars, c1)) {
        token->tokenType = TOKEN_IDENTIFIER;
        token->cargo.push_back(c1);

        getChar();

        while (contains(IdentifierChars, c1)) {
            token->cargo.push_back(c1);
            getChar();
        }

        if (contains(Keywords, token->cargo)) {
            token->tokenType = TOKEN_SYMBOL;
        }
        return token;
    }

    // int token
    if (contains(IntStartChars, c1)) {
        token->tokenType = TOKEN_INT;
        token->cargo.push_back(c1);
        getChar();

        while (contains(IntChars, c1)) {
            token->cargo.push_back(c1);
            getChar();
        }

        return token;
    }

    // char token
    if (c1 == '\'') {
        token->tokenType = TOKEN_CHAR;
        getChar();
        token->cargo.push_back(c1);
        getChar();
        if (c1 != '\'') {
            parseError("unterminated character");
        }
        getChar();
        return token;
    }

    // string token
    if (c1 == '"') {
        token->tokenType = TOKEN_STRING;

        getChar();

        while (c1 != '"') {
            if (c1 == Character::ENDMARK || c1 == '\n') {
                parseError("unterminated string");
            }

            token->cargo.push_back(c1);
            getChar();
        }

        getChar();

        return token;
    }

    // symbol token
    if (contains(ThreeCharacterSymbols, c3)) {
        token->tokenType = TOKEN_SYMBOL;
        token->cargo = c3;
        getChar();
        getChar();
        getChar();
        return token;
    }

    if (contains(TwoCharacterSymbols, c2)) {
        token->tokenType = TOKEN_SYMBOL;
        token->cargo = c2;
        getChar();
        getChar();
        return token;
    }

    if (contains(OneCharacterSymbols, c1)) {
        token->tokenType = TOKEN_SYMBOL;
        token->cargo = c1;
        getChar();
        return token;
    }

    delete token;
    parseError("unexpected character");
    return nullptr;
}


void Lexer::parseError(std::string desc) {
    ostringstream os;
    os << "parse error: " << desc << " " << c1 <<" at " << character->lineIndex << ":" << character->colIndex;
    throw os.str();
}