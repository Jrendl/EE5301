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
 * @brief parsing class for EE5301 PA2
 * reads in block shapes and hyper-edges
 *
 */
class input_parser {
   private:
    /**
     * @brief corresponding shapes for each block
     * indexed by their block number (an int)
     *
     */
    map<int, vector<pair<int, int>>> sizes;
    /**
     * @brief a vector of all the hyper-edges in the graph
     * each hyper-edge is a linked list
     *
     */
    vector<list<int>> edges;

   public:
    /**
     * @brief Construct a new input parser object
     *
     */
    input_parser() {
        sizes = map<int, vector<pair<int, int>>>();
        edges = vector<list<int>>();
    }
    /**
     * @brief parse the given input file
     * expects layout given in the project assignment
     *
     * @param input FileName of the file to parse
     * @return int 0 if successful, -1 if failed
     */
    int parse_file(char *input);
    /**
     * @brief Get the sizes object
     *
     * @return map<int, vector<pair<int, int>>>*
     */
    map<int, vector<pair<int, int>>> *get_sizes();
    /**
     * @brief Get the edges object
     *
     * @return vector<list<int>>*
     */
    vector<list<int>> *get_edges();
};