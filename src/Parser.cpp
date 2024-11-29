#include "Parser.h"

BinaryOperators MiniParser::translateCharToBinaryOperator(char ch) {
    switch(ch) {
        case '|': return BinaryOperators::OR;
        case '&': return BinaryOperators::AND;
        case '^': return BinaryOperators::XOR;
        default: throw std::runtime_error("Unknown binary operator: " + std::string(1, ch));
    }
}
    
UnaryOperators MiniParser::translateCharToUnaryOperator(char ch) {
    switch(ch) {
        case '!': return UnaryOperators::NOT;
        default: throw std::runtime_error("Unknown binary operator: " + std::string(1, ch));
    }
}

INode* MiniParser::buildIdentifier() {
    char name = _lexer->getCurrSymbol();
        
    if (std::find(
        this->declaredIdentifiers.begin(), 
        this->declaredIdentifiers.end(), 
        name
    ) == declaredIdentifiers.end()) {
        this->declaredIdentifiers.push_back(name);
    }
        
    this->_lexer->next();
        
    return new Identifier(name);
}
    
INode* MiniParser::buildNumber() {
    int value = _lexer->getCurrSymbol() - '0';
    this->_lexer->next();
    return new Integer(value);
}
    
INode* MiniParser::buildBinaryOperation(int exprPrec, INode* lhs) {
    while (true) {
        int tokPrec = getTokPrecedence();
            
        if (tokPrec < exprPrec)
            return lhs;
    
        char op = _lexer->getCurrSymbol();
        _lexer->next();
    
        INode* rhs = parseTopLevel();
        if (!rhs) return nullptr;
    
        int nextPrec = getTokPrecedence();
        if (tokPrec < nextPrec) {
            rhs = buildBinaryOperation(tokPrec + 1, rhs);
            if (!rhs) return nullptr;
        }
    
        lhs = new BinaryOperation(lhs, rhs, translateCharToBinaryOperator(op));
    }
}
    
INode* MiniParser::buildExpression() {
    INode* lhs = this->parseTopLevel();

    return this->buildBinaryOperation(0, lhs);       
}
    
INode* MiniParser::buildParenExpression() {
    if (_lexer->getCurrType() != TokenType::OPEN_PAREN) {
        throw std::runtime_error("Expected open parenthesis '('");
    }
    _lexer->next(); 
    INode* expr = buildExpression();
    
    if (_lexer->getCurrType() != TokenType::CLOSE_PAREN) {
        throw std::runtime_error("Close parenthesis ')' not found during parenthesis expression parsing");
    }
    _lexer->next(); 
    return expr;
}
    
INode* MiniParser::buildUnary() {
    UnaryOperators op = this->translateCharToUnaryOperator(this->_lexer->getCurrSymbol());
    this->_lexer->next();
    return new UnaryOperation(this->parseTopLevel(), op);
}
    
INode* MiniParser::parseTopLevel() {
    switch (this->_lexer->getCurrType()) {
        case TokenType::IDENTIFIER: return this->buildIdentifier();
        case TokenType::NUMERIC: return this->buildNumber();
        case TokenType::LOGICAL_OPERATOR: return this->buildUnary();
        case TokenType::OPEN_PAREN: return this->buildParenExpression();
        default: throw std::runtime_error("parser error");
    }
}