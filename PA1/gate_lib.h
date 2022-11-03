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

using namespace std;

/**
 * @brief gate type definition
 * name: the type of gate implemented
 * table_indicator: variable used for parsing file.
 * True if we're parsing the delay table, false if parsing the output slew
 * capacitance: internal capacitance of the gate
 * cell_delay: 7x7 delay table
 * output_slew: 7x7 output slew rate table
 *
 */
struct gate_t {
    string name = "";
    bool table_indicator = true;
    double capacitance = 0.0;
    float index_1[7] = {0.0};
    float index_2[7] = {0.0};
    double cell_delay[7][7] = {0.0};
    double output_slew[7][7] = {0.0};
};

constexpr int MAX_NUM_GATES = 100;

class gate_lib {
   private:
    int gate_count;

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
    gate_lib() {
        gate_count = 0;
    }
    /**
     * @brief parse input file with gate information
     *
     * @param fName - relative file path
     * @return int - -1 if failed to open, number of gates otherwise
     */
    int parse_gate_library(char *fName);

    /**
     * @brief Find the index of ``gate_name`` in the gate library
     *
     * @param gate_name
     * @return int index
     */
    int find_gate_lib(string gate_name);

    /**
     * @brief global array of gates
     * size of MAX_NUM_GATES
     *
     */
    gate_t gate_lib_array[MAX_NUM_GATES];

    /**
     * @brief maps gate number to gate type library definiton
     *
     */
    std::map<int, gate_t *> gate_type_map;

    int get_gate_count(void);
    int find_index_1(float value, gate_t* gate);
    int find_index_2(float value, gate_t* gate);
};