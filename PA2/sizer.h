#pragma once
#include <stdio.h>  // needed to open files in C
#include <string.h>

#include <algorithm>
#include <fstream>   // needed to open files in C++
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <locale>
#include <map>
#include <sstream>  // needed if you are using sstream (C++)
#include <vector>

using namespace std;

class sizer {
   private:
    map<int, vector<pair<int, int>>> sizes_by_loc;
    map<int, pair<int, int>> final_shapes;
    map<int, vector<pair<int, int>>> ij_pairs;
    map<int, bool> used_by_parent;
    map<int, pair<int, int>> coords;
    map<int, pair<int, int>> children;
    vector<string> polish;
    bool is_op(string c);

   public:
    sizer() {
        sizes_by_loc = map<int, vector<pair<int, int>>>();
        final_shapes = map<int, pair<int, int>>();
        ij_pairs = map<int, vector<pair<int, int>>>();
        used_by_parent = map<int, bool>();
        coords = map<int, pair<int, int>>();
        children = map<int, pair<int, int>>();
    }
    pair<vector<pair<int, int>>, vector<pair<int, int>>> vert_size(
        vector<pair<int, int>> *L, vector<pair<int, int>> *R);

    pair<vector<pair<int, int>>, vector<pair<int, int>>> hor_size(
        vector<pair<int, int>> *U, vector<pair<int, int>> *L);

    map<int, vector<pair<int, int>>> *do_sizing(
        map<int, vector<pair<int, int>>> *sizes, vector<string> polish);

    int bottom_up_recursive(int start);
    int top_down_recursive(int node, int shape);
    int output_sizing(string fout);
    map<int, pair<int, int>> get_coords();
    float get_area();
};