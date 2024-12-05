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
    virtual int getHash() = 0;
    virtual ~INode() {}
};

class Integer : public INode {
    int value;
public:
    Integer(int v) : value(v) {}
    
    void accept(IVisitor* v) override {
        v->visitInteger(*this);
    }

    int getHash() override { return value; }

    int getValue() const { return value; }
};

class Identifier : public INode {
    char name;
public:
    Identifier(char v) : name(v) {}
    
    void accept(IVisitor* v) override {
        v->visitIdentifier(*this);
    }

    int getHash() override { return (int)name; }
    
    char getName() {return name;}
};

class BinaryOperation : public INode {
    BinaryOperators op;
    INode* lhs;
    INode* rhs;
    int hash = -1;
public:
    BinaryOperation(INode* lhs, INode* rhs, BinaryOperators op)
        : lhs(lhs), rhs(rhs), op(op) {}

    void accept(IVisitor* v) override {
        v->visitBinaryOp(*this);
    }

    int getHash() override { 
        if (hash != -1)
            return hash;

        hash = (int)op;
        hash = 31 * hash + lhs->getHash();
        hash = 31 * hash + rhs->getHash();
        return hash;
    }

    INode* getLhs() const { return lhs; }
    INode* getRhs() const { return rhs; }
    BinaryOperators getBinOp() const { return op; }
};

class UnaryOperation : public INode {
    UnaryOperators op;
    INode* operand;
    int hash = -1;
public:
    UnaryOperation(INode* operand, UnaryOperators op)
        : operand(operand), op(op) {}

    void accept(IVisitor* v) override {
        v->visitUnaryOp(*this);
    }

    int getHash() override { 
        if (hash != -1)
            return hash;

        hash = (int)op;
        hash = 31 * hash + operand->getHash();
        return hash;
    }
    
    INode* getOperand() const { return operand; }
    UnaryOperators getUnaryOp() const { return op; }
};