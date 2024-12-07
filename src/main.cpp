#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>
#include <stdexcept>

#include "Lexer.h"
#include "Parser.h"
#include "Optimizer.h"
#include "TreePrinter.h"
#include "Runtime.h"

void printHelp() {
    std::cout << "Hello! This is de Bitter App!\n"
              << "Usage:\n"
              << "-repl : Activates interactive console\n"
              << "-fi <input_file> : Specify input file\n"
              << "-fo <output_file> : Specify output file\n"
              << "-o : Show optimized tree\n"
              << "-un : Show unoptimized tree\n"
              << "-s_all : Show both optimized and unoptimized trees\n"
              << "-h : Show help\n";
}

std::map<std::string, std::string> parseArguments(int argc, char** argv) {
    std::map<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                args[arg] = argv[++i]; // Флаг с аргументом
            } else {
                args[arg] = ""; // Флаг без аргумента
            }
        }
    }
    return args;
}

std::string processInput(const std::string& input, bool showOptimized, bool showUnoptimized) {
    MiniLexer lexer;
    TreePrinter printer;
    Optimizer optimizer;
    Interpretator interpreter;

    lexer.setTextToProcess(input);
    MiniParser parser(lexer);
    INode* unoptimizedTree = parser.buildTree();

    std::string result;
    if (showUnoptimized) {
        result += "Unoptimized tree:\n" + printer.printTree(unoptimizedTree) + "\n";
    }

    INode* optimizedTree = optimizer.optimizeTree(unoptimizedTree);
    if (showOptimized) {
        result += "Optimized tree:\n" + printer.printTree(optimizedTree) + "\n";
    }

    int ids = parser.getDeclaredIdentifiers().size();
    std::map<char, int> symbolMap;
    if (ids > 0) {
        std::cout << "Parsed " << ids << " identifiers.\n";
        for (int i = 0; i < ids; ++i) {
            char id = parser.getDeclaredIdentifiers().at(i);
            std::cout << "Input value for [" << id << "]: ";
            int value;
            while (!(std::cin >> value)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
            symbolMap[id] = value;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    interpreter.setupSymbolMap(symbolMap);
    result += "Result: " + std::to_string(interpreter.evaluate(optimizedTree)) + "\n";
    return result;
}

void replMode() {
    std::cout << "Welcome to de Bitter REPL console!\n";
    std::string input;
    bool showUnoptimized = false;
    bool showOptimized = false;

    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, input);

        std::transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "show_unopt") {
            showUnoptimized = true;
        } else if (input == "show_opt") {
            showOptimized = true;
        } else if (input == "hide_all") {
            showUnoptimized = false;
            showOptimized = false;
        } else if (input == "exit") {
            break;
        } else {
            std::cout << processInput(input, showOptimized, showUnoptimized);
        }
    }
}

std::string processFile(const std::string& fileName, bool showOptimized, bool showUnoptimized) {
    std::ifstream inFile(fileName);
    if (!inFile) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
    return processInput(input, showOptimized, showUnoptimized);
}

int main(int argc, char** argv) {
    auto args = parseArguments(argc, argv);

    if (args.empty() || args.count("-h")) {
        printHelp();
        return 0;
    }

    if (args.count("-repl")) {
        replMode();
        return 0;
    }

    if (args.count("-fi")) {
        std::string inputFile = args["-fi"];
        std::string outputFile = args.count("-fo") ? args["-fo"] : "";

        bool showUnoptimized = args.count("-un");
        bool showOptimized = args.count("-o");
        if (args.count("-s_all")) {
            showUnoptimized = true;
            showOptimized = true;
        }

        try {
            std::string result = processFile(inputFile, showOptimized, showUnoptimized);
            if (!outputFile.empty()) {
                std::ofstream outFile(outputFile);
                if (!outFile) {
                    throw std::runtime_error("Failed to open output file: " + outputFile);
                }
                outFile << result;
            } else {
                std::cout << result;
            }
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << '\n';
            return -1;
        }
        return 0;
    }

    printHelp();
    return -1;
}