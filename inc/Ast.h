#pragma once

#include "Visitor.h"

enum BinaryOperators {
    AND,
    OR,
    XOR
};

enum UnaryOperators {
    NOT
};

class INode {
public:
    virtual void accept(IVisitor* v) = 0;
    virtual ~INode() {}
};

class Integer : public INode {
    int value;
public:
    Integer(int v) : value(v) {}
    
    void accept(IVisitor* v) override {
        v->visitInteger(*this);
    }
    int getValue() const { return value; }
};

class Identifier : public INode {
    char name;
public:
    Identifier(char v) : name(v) {}
    
    void accept(IVisitor* v) override {
        v->visitIdentifier(*this);
    }
    
    char getName() {return name;}
};

class BinaryOperation : public INode {
    BinaryOperators op;
    INode* lhs;
    INode* rhs;
public:
    BinaryOperation(INode* lhs, INode* rhs, BinaryOperators op)
        : lhs(lhs), rhs(rhs), op(op) {}

    void accept(IVisitor* v) override {
        v->visitBinaryOp(*this);
    }
    INode* getLhs() const { return lhs; }
    INode* getRhs() const { return rhs; }
    BinaryOperators getBinOp() const { return op; }
};

class UnaryOperation : public INode {
    UnaryOperators op;
    INode* operand;
public:
    UnaryOperation(INode* operand, UnaryOperators op)
        : operand(operand), op(op) {}

    void accept(IVisitor* v) override {
        v->visitUnaryOp(*this);
    }
    INode* getOperand() const { return operand; }
    UnaryOperators getUnaryOp() const { return op; }
};