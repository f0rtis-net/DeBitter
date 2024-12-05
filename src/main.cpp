#include <iostream>

#include "Lexer.h"
#include "Parser.h"

#include "Optimizer.h"
#include "TreePrinter.h"

#include "Runtime.h"

void help() {
    std::cout << "Hello! This is de Bitter App!" << std::endl;

    std::cout << "Usage: " << std::endl;
    std::cout << "-repl - activates interactive console" << std::endl;
    std::cout << "-fi - path to input file" << std::endl;
    std::cout << "-fo - path to output file. If argument not presents, output was in console" << std::endl;
}

void repl() {
    std::cout << "Hello! This is de Bitter Repl console!" << std::endl;

    std::string input = "";

    bool showUnoptimized = false;
    bool showOptimized = false;

    MiniLexer lexer;
    TreePrinter printer;
    Optimizer optimizer;
    Interpretator interpreter;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);
        
        std::transform(input.begin(), input.end(), input.begin(),
                   [](unsigned char c) { return std::tolower(c); });

        if (input == "show_unopt")
            showUnoptimized = true;
        else if (input == "show_opt")
            showOptimized = true;
        else if (input == "hide_all") {
            showOptimized = false;
            showUnoptimized = false;
        }
        else if (input == "exit")
            break; 
        else {
            lexer.setTextToProcess(input);
            MiniParser parser = MiniParser(lexer);
            INode* buildedTree = parser.buildTree();
            
            if (showUnoptimized) {
                std::cout << "\nUnoptimized tree: " << std::endl;
                std::cout << printer.printTree(buildedTree) << "\n"<< std::endl;
            }

            INode* optimizedTree = optimizer.optimizeTree(buildedTree);

            if (showOptimized) {
                std::cout << "\nOptimized tree: " << std::endl;
                std::cout << printer.printTree(optimizedTree) << "\n"<< std::endl;
            }

            int ids = parser.getDeclaredIdentifiers().size();
            std::map<char, int> parsed;

            if (ids != 0) {
                std::cout << "Parsed " << ids << " ids" << std::endl;

                for (int i = 0; i < ids; ++i) {
                    char ch = parser.getDeclaredIdentifiers().at(i);
                    std::cout << "Input [" << ch << "]: ";
                    int val = 0;
                    if (!(std::cin >> val)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Invalid input. Please enter a number." << std::endl;
                        i--; 
                        continue;
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    parsed[ch] = val;
                }
            }

            interpreter.setupSymbolMap(parsed);
            std::cout << "Result: " << interpreter.evaluate(optimizedTree) << std::endl;
        }

        input.clear();
    }
}

int main(int argc, char **argv) {

    if (argc <= 1) {
        std::cout << "Invalid usage. Type -h to get help" << std::endl;
        return -1;
    }

    if (!strcmp(argv[1], "-h")) {
        help();
        return 0;
    }

    if (!strcmp(argv[1], "-repl")) {
        repl();
        return 0;
    }

    return 0;
}