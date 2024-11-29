#include "TreePrinter.h"

void TreePrinter::printNode(const std::string& nodeName, const std::string& nodeValue) {
    appendIndent();
    result += nodeName;
    if (!nodeValue.empty()) {
        result += " (" + nodeValue + ")";
    }
    result += "\n";
}

std::string TreePrinter::binaryOperatorToString(BinaryOperators op) {
    switch (op) {
        case AND: return "AND";
        case OR:  return "OR";
        case XOR: return "XOR";
        default: return "UNKNOWN_BINARY_OP";
    }
}

std::string TreePrinter::unaryOperatorToString(UnaryOperators op) {
    switch (op) {
        case NOT: return "NOT";
        default: return "UNKNOWN_UNARY_OP";
    }
}

void TreePrinter::visitInteger(Integer& v) {
    printNode("Integer", std::to_string(v.getValue()));
}

void TreePrinter::visitIdentifier(Identifier& u) {
    printNode("Identifier", std::string(1, u.getName()));
}

void TreePrinter::visitBinaryOp(BinaryOperation& b) {        
    printNode("BinaryOperation", binaryOperatorToString(b.getBinOp()));
    currentIndent += indentStep;
    b.getLhs()->accept(this);
    b.getRhs()->accept(this);
    currentIndent -= indentStep;
}

void TreePrinter::visitUnaryOp(UnaryOperation& u) {
    printNode("UnaryOperation", unaryOperatorToString(u.getUnaryOp()));
    currentIndent += indentStep;
    u.getOperand()->accept(this);
    currentIndent -= indentStep;
}

std::string TreePrinter::printTree(INode* root) {
    if (!root) {
        throw std::invalid_argument("Root node is null");
    }
    result.clear();
    currentIndent = 0;
    root->accept(this);
    return result;
}