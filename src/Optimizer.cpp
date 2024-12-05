#include "Optimizer.h"

bool Optimizer::convoluteBasicBinOpSituations(BinaryOperation& b) {
    INode* optimizedLhs = optimizeNode(b.getLhs());
    INode* optimizedRhs = optimizeNode(b.getRhs());

    if (b.getBinOp() == AND || b.getBinOp() == OR) {
        if (optimizedLhs->getHash() == optimizedRhs->getHash()) {
            optimizedTree = optimizedLhs;
            return true;
        }
    }

    Integer* castedLhs = dynamic_cast<Integer*>(optimizedLhs);
    Integer* castedRhs = dynamic_cast<Integer*>(optimizedRhs);

    if (castedLhs && castedLhs->getValue() == 0) {
        if (b.getBinOp() == AND) {
            optimizedTree = castedLhs;
            return true;
        } else if (b.getBinOp() == OR) {
            optimizedTree = optimizedRhs;
            return true;
        }
    }

    if (castedRhs && castedRhs->getValue() == 0) {
        if (b.getBinOp() == AND) {
            optimizedTree = castedRhs;
            return true;
        } else if (b.getBinOp() == OR) {
            optimizedTree = optimizedLhs;
            return true;
        }
    }

    return false;
}

bool Optimizer::convoluteBinaryOP(BinaryOperation& b) {
    INode* optimizedLhs = optimizeNode(b.getLhs());
    INode* optimizedRhs = optimizeNode(b.getRhs());

    if (b.getBinOp() != OR && b.getBinOp() != AND) {
        return false;
    }

    BinaryOperation* andOp1 = dynamic_cast<BinaryOperation*>(optimizedLhs);
    BinaryOperation* andOp2 = dynamic_cast<BinaryOperation*>(optimizedRhs);

    if (!andOp1 || !andOp2) {
        return false;
    }

    if ((b.getBinOp() == OR && (andOp1->getBinOp() != AND || andOp2->getBinOp() != AND)) ||
        (b.getBinOp() == AND && (andOp1->getBinOp() != OR || andOp2->getBinOp() != OR))) {
        return false;
    }

    UnaryOperation* unOp = dynamic_cast<UnaryOperation*>(andOp2->getRhs());

    if (!unOp) {
        return false;
    }

    bool leftHashesEq = andOp1->getLhs()->getHash() == andOp2->getLhs()->getHash();
    bool rightHashesEq = andOp1->getRhs()->getHash() == unOp->getOperand()->getHash();

    if (!leftHashesEq || !rightHashesEq) {
        return false;
    }

    optimizedTree = andOp1->getLhs();
    return true;
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

    bool expressionOptimized = false;

    if (!expressionOptimized) {
        expressionOptimized = convoluteBinaryOP(b);
    }

    if (!expressionOptimized) {
        expressionOptimized = convoluteBasicBinOpSituations(b);
    }

    if (!expressionOptimized) {
        optimizedTree = new BinaryOperation(optimizedLhs, optimizedRhs, b.getBinOp());
    }
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
