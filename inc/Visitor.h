#pragma once

class Integer;
class BinaryOperation;
class UnaryOperation;
class Identifier;

class IVisitor {
public:
    virtual void visitInteger(Integer& v) = 0;
    virtual void visitBinaryOp(BinaryOperation& b) = 0;
    virtual void visitUnaryOp(UnaryOperation& u) = 0;
    virtual void visitIdentifier(Identifier& u) = 0;
    virtual ~IVisitor() = default;
};