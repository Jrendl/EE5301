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

class input_parser {
   private:
    map<int, vector<pair<int, int>>> sizes;
    vector<list<int>> edges;

   public:
    input_parser() {
        sizes = map<int, vector<pair<int, int>>>();
        edges = vector<list<int>>();
    }
    int parse_file(char *input);
    map<int, vector<pair<int, int>>> *get_sizes();
    vector<list<int>> *get_edges();
};