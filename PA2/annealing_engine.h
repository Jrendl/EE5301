#pragma once
#include <math.h>
#include <stdio.h>  // needed to open files in C
#include <string.h>

#include <algorithm>
#include <fstream>   // needed to open files in C++
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <iterator>
#include <list>
#include <locale>
#include <map>
#include <sstream>  // needed if you are using sstream (C++)
#include <vector>

#include "sizer.h"

class annealing_engine {
   private:
    /**
     * @brief starting temperature of the annealing engine
     *
     */
    const float T_0 = 2000;
    /**
     * @brief percentage of moves we would like to accept at the start temp
     * used to calculate K
     *
     */
    const float ACCEPT_AT_START = 0.80;
    /**
     * @brief temperature at which to stop the annealing process
     *
     */
    const float T_FREEZE = 0.5;
    /**
     * @brief number of moves to take at every temperature
     *
     */
    const int NUM_MOVES_PER_TEMP_STEP = 50;
    /**
     * @brief the value that ``alpha`` takes when the -c option is used
     *
     */
    const float C = 0.5;

    /**
     * @brief K constant used to calulate the boltzman value
     * this is calculated once the program starts
     *
     */
    float K = 0;
    /**
     * @brief the value that decides how much weight is put on the cost of area
     * and wire length
     *
     */
    float alpha;
    /**
     * @brief the shapes of all the provided blocks
     * given by the ``input_parser`` at initialization
     *
     */
    map<int, vector<pair<int, int>>>* shapes;
    /**
     * @brief all the hyper-edges in the graph
     * given by the ``input_parser`` at initialization
     *
     */
    vector<list<int>>* edges;

    /**
     * @brief sizer object to perform sizing on each of the polish expressions
     * we generate
     *
     */
    sizer sizing;

    /**
     * @brief decide whether we accept a move
     *
     * @param delta_cost the difference in cost once we've made a move
     * @param T our current temperature
     * @return true if negative or lower than the calculated boltzman value
     * @return false if above the calculated boltzman calue
     */
    bool accept_move(float delta_cost, float T);
    /**
     * @brief calculate the cost of a polish expression
     * calls ``sizing.do_sizing`` to find area and coordinates
     * fully dependent on the area of the chip if -a is used
     * fully dependent on the Half-Perimeter Wire-Length if -w is used
     * a mix of both if -c is used
     *
     * @param solution the polish expression we would like to calculate cost for
     * @return float the cost of the layout
     */
    float cost(vector<string> solution);
    /**
     * @brief implements the cooling schedule of the annealing engine
     * was optimized by joe Rendleman on 12/12/22
     *
     * @param T the current temperature
     * @return float the new temerature
     */
    float cool_down(float T);
    /**
     * @brief helper function to tell if string ``c`` is an operator
     *
     * @param c the string to check
     * @return true
     * @return false
     */
    bool is_op(string c);
    /**
     * @brief perform one of three possible moves on the polish expression
     * OP1: exchange two operands that have no other operands in between
     * OP2: Complement a series of operators between two operands
     * OP3: Exchange adjacent operance and operator if the resulting
     * expression still a normalized polish exp
     * with equal likelihood
     *
     * @param polish the expression on which to make a move
     * @return vector<string> the resulting expression
     */
    vector<string> make_move(vector<string> polish);
    /**
     * @brief calculate the Half-Perimeter Wire-Length from the center of every
     * block
     *
     * @param coords the coordinates of every block
     * @return float the total HPWL
     */
    float wire_length(map<int, pair<float, float>> coords);
    /**
     * @brief confirm if a polish expression is valid
     * sums to 1 and never dips below 1
     * no repeated, consecutive operators
     *
     * @param polish the polish expression to check
     * @return true if valid
     * @return false
     */
    bool check_valid_polish(vector<string> polish);
    /**
     * @brief compute the K constant used in calculating the blotzman value
     * use an average delta cost over 250 moves
     * only ran once at the beginning of the process
     *
     * @param polish our initial, random polish expression
     * @return float the K value
     */
    float compute_K(vector<string> polish);

   public:
    /**
     * @brief Construct a new annealing engine object
     *
     * @param shapes pairwise shapes of blocks indexed by their block-number
     * @param edges all the hyper-edges in the graph
     * @param alpha -a, -w, or -c
     */
    annealing_engine(map<int, vector<pair<int, int>>>* shapes,
                     vector<list<int>>* edges, string alpha)
        : shapes(shapes), edges(edges) {
        sizing = sizer();
        if (alpha.compare("-w") == 0) {
            this->alpha = 0;
        } else if (alpha.compare("-a") == 0) {
            this->alpha = 1;
        } else {
            this->alpha = C;
        }
    }
    /**
     * @brief perform the annealing process on the values initialized during the
     * constructor
     *
     * @return vector<string> the final chip layout
     */
    vector<string> do_annealing();
    /**
     * @brief initialize a random (valid) polish expression
     *
     * @return vector<string> the polish expression
     */
    vector<string> init_random_polish();
    /**
     * @brief output our results to a file of name fout
     *
     * @param fout the file to output to
     * @return int 0 if success
     */
    int output(string fout);
};