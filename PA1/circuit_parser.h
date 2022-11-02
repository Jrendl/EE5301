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
    /**
     * @brief If gate_num is larger than the current size of the adjacency
     * lists, resize.
     * new size = ((gate_num / 1000) + 1) * 1000
     *
     * @param gate_num
     */
    void check_resize(int gate_num);
    /**
     * @brief iterate through new fanout space and initialize new lists
     *
     * @param start beginning of new space
     * @param end
     */
    void prep_fanout(int start, int end);
    gate_lib *library;

    /**
     * @brief delimiter override to allow imbue of iss
     *
     */
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
            return &rc[0];
        }
    };

   public:
    /**
     * @brief Construct a new circuit parser object
     *
     * @param library
     * @param init_size
     */
    circuit_parser(gate_lib *library, int init_size) : library(library) {
        fanin_list.resize(init_size);
        fanout_list.resize(init_size);
        prep_fanout(0, init_size);
    }
    std::vector<std::list<int>> fanin_list;
    std::vector<std::list<int>> fanout_list;
    std::map<int, gate_t *> gate_type_map;
    /**
     * @brief parse fName and store nodes into two adjacency lists: fanin, and
     * fanout
     *
     * @param fName
     * @return int 0 if successfully parsed, -1 else
     */
    int parse_circuit_file(char *fName);
};