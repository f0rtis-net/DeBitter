#include <iostream>

#include "Lexer.h"
#include "Parser.h"

#include "Optimizer.h"
#include "TreePrinter.h"

#include "Runtime.h"

int main(int argc, char **argv) {
    MiniLexer lexer;
    lexer.setTextToProcess("( ( (a & 1) & b ) | ( (a & 1) & !b ) )");

    MiniParser parser = MiniParser(lexer);
    INode* buildedTree = parser.buildTree();

    TreePrinter printer;

    std::cout << printer.printTree(buildedTree) << std::endl;

    Optimizer optimizer;
    INode* optimizedTree = optimizer.optimizeTree(buildedTree);

    std::cout << printer.printTree(optimizedTree) << std::endl;

    Interpretator interpreter;
    interpreter.setupSymbolMap({
        {'a', 0},
        {'b', 1}
    });
    
    std::cout << "Result: " << interpreter.evaluate(optimizedTree) << std::endl;

    return 0;
}