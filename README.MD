# DeBitter
### This repository contain my lab for subject: C++ Programming
### The essence of this laboratory work is to write an optimized Boolean algebra performer

## The main modules of the application:

### Lexer
    A simple lexer that can recognize one, zero, parentheses and logical operators

### Parser
    A simple parser that recursively builds an expression tree based on a lexer
### Optimizer
    A recursive optimizer that so far only knows how to convolve uniquely defined binary operations (this is when the left and right parts are numbers). There are also basic methods for recognizing the possibility of convolution of predefined patterns

### Interpretator
    A simple stack virtual machine
