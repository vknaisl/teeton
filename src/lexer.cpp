#include <algorithm>
#include <iomanip>
#include <sstream>

#include "lexer.h"

using namespace std;



// -----------------------------------------------------------------------------

const vector<string> Lexer::Keywords(
        {"if", "else", "while", "print", "println", "list", "append", "len", "get", "set", "break"});

const vector<string> Lexer::BooleanKeywords({"True", "False"});

const vector<string> Lexer::ScanKeywords({"scan_int", "scan_char", "scan_string"});

const string Lexer::OneCharacterSymbols("=+-*/%<>!(){},\n");

const vector<string> Lexer::TwoCharacterSymbols({"<=", ">=", "==", "&&", "||", "!="});

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
        } else if (contains(BooleanKeywords, token->cargo)) {
            token->tokenType = TOKEN_BOOL;
        } else if (contains(ScanKeywords, token->cargo)) {
            token->tokenType = TOKEN_SCAN;
        }
        return token;
    }

    // int token
    if (contains(IntStartChars, c1) || (c1 == '-' && contains(IntStartChars, c2[1]))) {
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
            parseError("Unterminated character literal.", character->lineIndex, character->colIndex);
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
                parseError("Unterminated string literal.", character->lineIndex, character->colIndex);
            }

            token->cargo.push_back(c1);
            getChar();
        }

        getChar();

        return token;
    }

    // array token
    if (c2 == "[]") {
        token->tokenType = TOKEN_LIST;
        token->cargo = c2;
        getChar();
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
    ostringstream os;
    os << "Unexpected character '" << c1 << "'.";
    parseError(os.str(), character->lineIndex, character->colIndex);
    return nullptr;
}
