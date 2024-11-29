#include "Lexer.h"

void MiniLexer::setTextToProcess(const std::string& text) {
    sourceCode = text;
    textLength = text.length();
     index = 0;

    next();
}


void MiniLexer::next() {
    currSymbol = getChar();

     while (isspace(currSymbol)) {
        currSymbol = getChar();
    }

    if (isdigit(currSymbol)) {
        if (currSymbol != '0' && currSymbol != '1') {
            throw std::runtime_error("Only binary digits (0 or 1) are allowed");
        }
        currType = TokenType::NUMERIC;
        return;
    }

    if (isalpha(currSymbol)) {
        currType = TokenType::IDENTIFIER;
        return;
    }

    for (int i = 0; i < sizeOfReservedOperators; ++i) {
        if (reservedOperators[i] == currSymbol) {
            currType = TokenType::LOGICAL_OPERATOR;
            return;
        }
    }

    if (currSymbol == '(') {
        currType = TokenType::OPEN_PAREN;
        return;
    }

    if (currSymbol == ')') {
         currType = TokenType::CLOSE_PAREN;
        return;
    }

    if (currSymbol == EOF) {
        currType = TokenType::_EOF;
        return;
    }

    throw std::runtime_error("Unknown token: " + std::string(1, currSymbol));
}