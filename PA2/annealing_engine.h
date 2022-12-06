#pragma once
#include <math.h>
#include <stdio.h>  // needed to open files in C
#include <string.h>

#include <algorithm>
#include <fstream>   // needed to open files in C++
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <locale>
#include <map>
#include <sstream>  // needed if you are using sstream (C++)
#include <vector>

#include "sizer.h"

class annealing_engine {
   private:
    const float T_0 = 40000;
    const float T_FREEZE = 0.1;
    const int NUM_MOVES_PER_TEMP_STEP = 5;
    const float K = 0.5;
    const float COOLING_RATE = 0.95;
    const float C = 0.5;

    map<int, vector<pair<int, int>>>* shapes;
    float alpha;

    sizer sizing;

    bool accept_move(float delta_cost, float T);
    float cost(string solution);
    float cool_down(float T);
    string init_random_polish();
    bool is_op(char c);
    string make_move(string polish);
    float wire_length(map<int, pair<int, int>> coords);
    bool check_valid_polish(string polish);

   public:
    annealing_engine(map<int, vector<pair<int, int>>>* shapes, string alpha)
        : shapes(shapes) {
        sizing = sizer();
        if (alpha.compare("-w") == 0) {
            this->alpha = 0;
        } else if (alpha.compare("-a") == 0) {
            this->alpha = 1;
        } else {
            this->alpha = C;
        }
    }
    string do_annealing();
};