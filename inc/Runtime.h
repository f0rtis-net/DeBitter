#pragma once

#include <iostream>
#include <map>
#include <stack>

#include "Visitor.h"
#include "Ast.h"

class Interpretator : public IVisitor {
    std::stack<int> resultStack;
    std::map<char, int> symbolMap;
    
    void visitInteger(Integer& v) override;
    
    void visitIdentifier(Identifier& i) override;
    
    void visitBinaryOp(BinaryOperation& b) override;
    
    void visitUnaryOp(UnaryOperation& u) override;
    
public: 
    void setupSymbolMap(std::map<char, int> map) {
        this->symbolMap = map;
    }
    
    int evaluate(INode* rootNode);
};