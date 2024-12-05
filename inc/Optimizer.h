#pragma once

#include <iostream>

#include "Visitor.h"
#include "Ast.h"

class Optimizer : public IVisitor {
private:
    INode* optimizedTree;

    INode* optimizeNode(INode* node) {
        if (!node) return nullptr;
        node->accept(this);
        return optimizedTree;
    }

    void visitInteger(Integer& v) override {
        optimizedTree = &v;
    }

    void visitIdentifier(Identifier& u) override {
        optimizedTree = &u;
    }

    bool convoluteBinaryOP(BinaryOperation& b);
    bool convoluteBasicBinOpSituations(BinaryOperation& b);
    
    void visitBinaryOp(BinaryOperation& b) override;
    void visitUnaryOp(UnaryOperation& u) override;
public:

    INode* optimizeTree(INode* root);
};