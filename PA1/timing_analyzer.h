#pragma once
#include <stdio.h>  // needed to open files in C
#include <string.h>

#include <algorithm>
#include <fstream>   // needed to open files in C++
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <list>
#include <locale>
#include <map>
#include <sstream>  // needed if you are using sstream (C++)
#include <vector>

#include "circuit_parser.h"
#include "gate_lib.h"

class timing_analyzer {
   private:
    circuit_parser* parser;
    gate_lib* lib;
    int forward_traverse(void);
    int backward_traverse(void);

   public:
    timing_analyzer(circuit_parser* parser, gate_lib* lib)
        : parser(parser), lib(lib) {
    }
    int analyze(char* outFile);
};