#include "Optimizer.h"

bool Optimizer::convoluteBinaryOP(BinaryOperation& b) {
    INode* optimizedLhs = optimizeNode(b.getLhs());
    INode* optimizedRhs = optimizeNode(b.getRhs());
        
    //( ( a & b ) | ( a & !b ) ) = a
        
    if (b.getBinOp() == OR) {
        if (BinaryOperation* andOp1 = dynamic_cast<BinaryOperation*>(optimizedLhs); andOp1->getBinOp() == AND) {
            if (BinaryOperation* andOp2 = dynamic_cast<BinaryOperation*>(optimizedRhs); andOp2->getBinOp() == AND) {
                 if (dynamic_cast<UnaryOperation*>(andOp2->getRhs())){
                    optimizedTree = andOp1->getLhs();
                    return true;
                }
            }
        }
    } 
        
    //( ( a | b ) & ( a | !b ) ) = a
        
    if (b.getBinOp() == AND) {
        if (BinaryOperation* andOp1 = dynamic_cast<BinaryOperation*>(optimizedLhs); andOp1 && andOp1->getBinOp() == OR) {
            if (BinaryOperation* andOp2 = dynamic_cast<BinaryOperation*>(optimizedRhs); andOp2 && andOp2->getBinOp() == OR) {
                if (dynamic_cast<UnaryOperation*>(andOp2->getRhs())) {
                    optimizedTree = andOp1->getLhs();
                    return true;
                }
            }
        }
    } 
        
    return false;
}
    
void Optimizer::visitBinaryOp(BinaryOperation& b) {
    INode* optimizedLhs = optimizeNode(b.getLhs());
    INode* optimizedRhs = optimizeNode(b.getRhs());

    if (auto lhsInt = dynamic_cast<Integer*>(optimizedLhs)) {
        if (auto rhsInt = dynamic_cast<Integer*>(optimizedRhs)) {
            int leftValue = lhsInt->getValue();
            int rightValue = rhsInt->getValue();
            int result = 0;

            switch (b.getBinOp()) {
                case AND: result = leftValue & rightValue; break;
                case OR:  result = leftValue | rightValue; break;
                case XOR: result = leftValue ^ rightValue; break;
                default: throw std::logic_error("Unknown binary operator");
            }

            optimizedTree = new Integer(result);
            return;
        }
    }
        
    if (convoluteBinaryOP(b)) {
        return;
    }
        
    optimizedTree = new BinaryOperation(optimizedLhs, optimizedRhs, b.getBinOp());
}

void Optimizer::visitUnaryOp(UnaryOperation& u) {
    INode* optimizedOperand = optimizeNode(u.getOperand());

    if (auto operandInt = dynamic_cast<Integer*>(optimizedOperand)) {
        int operandValue = operandInt->getValue();
        int result = 0;

        switch (u.getUnaryOp()) {
            case NOT: result = !operandValue; break;
            default: throw std::logic_error("Unknown unary operator");
        }

        optimizedTree = new Integer(result);
        return;
    }

    optimizedTree = new UnaryOperation(optimizedOperand, u.getUnaryOp());
}

INode* Optimizer::optimizeTree(INode* root) {
    if (!root) {
        throw std::invalid_argument("Root node is null");
    }
    return optimizeNode(root);
}