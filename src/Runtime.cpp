#include "Runtime.h"

void Interpretator::visitInteger(Integer& v) {
    resultStack.push(v.getValue());
}
    
void Interpretator::visitIdentifier(Identifier& i) {
    char name = i.getName();
        
    auto it = symbolMap.find(name);
        
    if (it == symbolMap.end())
        throw std::runtime_error("Invalid identifier declared. Runtime error");
          
    resultStack.push(it->second);
}
    
void Interpretator::visitBinaryOp(BinaryOperation& b) {
    b.getLhs()->accept(this);
    int leftValue = resultStack.top(); resultStack.pop();
        
    b.getRhs()->accept(this);
    int rightValue = resultStack.top(); resultStack.pop();
        
    int result = 0;
    switch (b.getBinOp()) {
        case AND: result = leftValue & rightValue; break;
        case OR: result = leftValue | rightValue; break;
        case XOR: result = leftValue ^ rightValue; break;
        default: throw std::logic_error("Unknown binary operator");
    }
    resultStack.push(result);
}
    
void Interpretator::visitUnaryOp(UnaryOperation& u) {
    u.getOperand()->accept(this);
    int operandValue = resultStack.top(); resultStack.pop();
        
    int result = 0;
    switch (u.getUnaryOp()) {
        case NOT: result = !operandValue; break;
        default: throw std::logic_error("Unknown unary operator");
    }
    resultStack.push(result);
}

int Interpretator::evaluate(INode* rootNode) {
    if (rootNode == nullptr) {
        throw std::invalid_argument("Root node is null");
    }
        
    rootNode->accept(this);
    int result = resultStack.top();
    resultStack.pop();
    return result;
}