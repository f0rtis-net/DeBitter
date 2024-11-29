#pragma once

#include "Lexer.h"
#include "Ast.h"

#include <vector>
#include <map>

class MiniParser {
    MiniLexer* _lexer;
    std::vector<char> declaredIdentifiers;
    
    std::map<char, int> operatorPrecedence = {
        {'!', 40}, {'&', 30}, {'|', 20}, {'^', 20}
    };
    
    int getTokPrecedence() {
        auto token = _lexer->getCurrSymbol();
        
        auto it = operatorPrecedence.find(token);
        
        if (it == operatorPrecedence.end())
            return -1;
            
        return it->second;
    }

    BinaryOperators translateCharToBinaryOperator(char ch);
    
    UnaryOperators translateCharToUnaryOperator(char ch);
    
    INode* buildIdentifier();
    
    INode* buildNumber();
    
    INode* buildBinaryOperation(int exprPrec, INode* lhs);

    INode* buildExpression();

    INode* buildParenExpression();
    
    INode* buildUnary();
    
    INode* parseTopLevel();
    
public:
    MiniParser(MiniLexer& lexer): _lexer(&lexer) {}
    
    INode* buildTree() {
        return this->parseTopLevel();
    }
    
    std::vector<char> getDeclaredIdentifiers() {
        return this->declaredIdentifiers;
    }
};