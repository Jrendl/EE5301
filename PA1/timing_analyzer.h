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
    float latest_arrival;
    void forward_traverse(void);
    void backward_traverse(void);
    // pair (slew rate, arrival time, cell delay)
    std::vector<std::tuple<float, float, float>> arrival_times;
    std::vector<float> required_times;
    std::vector<float> slack;

   public:
    timing_analyzer(circuit_parser* parser, gate_lib* lib)
        : parser(parser), lib(lib) {
        arrival_times.resize(parser->fanin_list.size());
        required_times.resize(parser->fanin_list.size());
        slack.resize(parser->fanin_list.size());
        latest_arrival = 0;
    }
    int analyze(char* outFile);
};