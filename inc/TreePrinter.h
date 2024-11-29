#pragma once

#include <string>

#include "Ast.h"
#include "Visitor.h"

class TreePrinter : public IVisitor {
    std::string result;
    int currentIndent = 0;
    const int indentStep = 4;

    void appendIndent() {
        result.append(currentIndent, '-');
    }

    void printNode(const std::string& nodeName, const std::string& nodeValue = "");

    std::string binaryOperatorToString(BinaryOperators op);

    std::string unaryOperatorToString(UnaryOperators op);

    void visitInteger(Integer& v) override;

    void visitIdentifier(Identifier& u) override;

    void visitBinaryOp(BinaryOperation& b) override;

    void visitUnaryOp(UnaryOperation& u) override;

public:
    std::string printTree(INode* root);
};