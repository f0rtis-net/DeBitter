#pragma once

#include <string>

enum TokenType {
    IDENTIFIER,
    NUMERIC,
    LOGICAL_OPERATOR,
    OPEN_PAREN,
    CLOSE_PAREN,
    _EOF
};

class MiniLexer {
    char currSymbol;
    TokenType currType;
    std::string sourceCode;
    int textLength;
    int index;

    char getChar() {
        if (index >= textLength)
            return EOF;
        return sourceCode[index++];
    }

    void ungetChar() {
        if (index > 0) --index;
    }

    static const int sizeOfReservedOperators = 4;
    char reservedOperators[sizeOfReservedOperators] = {'|', '&', '!', '^'};

public:
    void setTextToProcess(const std::string& text);
    void next();

    char getCurrSymbol() const { return currSymbol; }

    TokenType getCurrType() const { return currType; }
};