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

#include "gate_lib.h"

using namespace std;

class circuit_parser {
   private:
    void check_resize(int gate_num);
    gate_lib *library;

    struct delimiters : std::ctype<char> {
        delimiters() : std::ctype<char>(get_table()) {
        }
        static mask const *get_table() {
            static mask rc[table_size];
            rc['('] = std::ctype_base::space;
            rc[')'] = std::ctype_base::space;
            rc[','] = std::ctype_base::space;
            rc['='] = std::ctype_base::space;
            rc[' '] = std::ctype_base::space;
            rc['\t'] = std::ctype_base::space;
            rc[':'] = std::ctype_base::space;
            rc['\"'] = std::ctype_base::space;
            rc[';'] = std::ctype_base::space;
            rc['\r'] = std::ctype_base::space;
            return &rc[0];
        }
    };

   public:
    circuit_parser(gate_lib *library) : library(library) {
    }
    std::vector<std::list<int>> adj_list;
    std::map<int, gate_t *> gate_type_map;
    int parse_circuit_file(char *fName);
};