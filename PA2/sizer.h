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
    /**
     * @brief the shapes of each sub-floorplan in the floorplan
     * indexed by their location within the polish expression, not their block
     * number
     *
     */
    map<int, vector<pair<int, int>>> sizes_by_loc;
    /**
     * @brief Final shapes of all the sub-floorplans
     * indexed by their location in the polish expression
     * set in ``top_down_recursive``
     *
     */
    map<int, pair<int, int>> final_shapes;
    /**
     * @brief records i and j for each of the nodes during the ``vert_size`` and
     * ``hor_size`` routines
     * this is helpful when we need to pass shapes back down to a split's
     * children in ``top_down_recursive``
     *
     */
    map<int, vector<pair<int, int>>> ij_pairs;
    /**
     * @brief flag a node once it has been consumed in ``bottom_up_recursive``
     * so we can tell which nodes are free to be a child of a new split
     *
     */
    map<int, bool> used_by_parent;
    /**
     * @brief the coordinates of each of the sub-floorplans
     * indexed by location in the polish expression
     *
     */
    map<int, pair<int, int>> coords;
    /**
     * @brief map of pairs of both children of each split
     * parents are indexed by location in the polish expression
     * children are referenced by location in the polish expression
     *
     */
    map<int, pair<int, int>> children;
    /**
     * @brief The polish expression on which we must perform sizing
     * represented as a vector of strings so that we're able to store
     * multi-digit ints and both of the operators
     *
     */
    vector<string> polish;

    /**
     * @brief helper function to tell if string ``c`` is an operator
     *
     * @param c the string to check
     * @return true
     * @return false
     */
    bool is_op(string c);
    /**
     * @brief perform sizing on a vertical split
     * takes sum of width and max of height
     *
     * @param L sorted ascending order in a and descending order in b
     * @param R sorded in ascending order in x and descending order in y
     * @return (vector of the sized shapes, vector of (i,j) for each shape)
     */
    pair<vector<pair<int, int>>, vector<pair<int, int>>> vert_size(
        vector<pair<int, int>> *L, vector<pair<int, int>> *R);

    /**
     * @brief perform sixing on a horizontal split
     * takes max of width and sum of heights
     *
     * @param U sorted ascending order in a and descending order in b
     * @param L sorted ascending order in x and descending order in y
     * @return (vector of the sized shapes, vector of (i,j) for each shape)
     */
    pair<vector<pair<int, int>>, vector<pair<int, int>>> hor_size(
        vector<pair<int, int>> *U, vector<pair<int, int>> *L);

    /**
     * @brief perform the bottom-up recurssion on the polish expression
     * this is where we find all possible sizes for all sub-floorplans
     *
     * @param start the location within the polish expression to start, will
     * increment as we recurse
     * @return int 0 if success
     */
    int bottom_up_recursive(int start);
    /**
     * @brief perform the top-down recurssion of the polish expression
     * This is where we find the smallest area and trickle down the correct
     * shapes
     *
     * @param node the index of the sub-floorplan we're at within the polish
     * expression
     * @param shape which shape to choose in it's list of possible shapes
     * @return int 0 if success
     */
    int top_down_recursive(int node, int shape);

   public:
    /**
     * @brief Construct a new sizer object
     *
     */
    sizer() {
        sizes_by_loc = map<int, vector<pair<int, int>>>();
        final_shapes = map<int, pair<int, int>>();
        ij_pairs = map<int, vector<pair<int, int>>>();
        used_by_parent = map<int, bool>();
        coords = map<int, pair<int, int>>();
        children = map<int, pair<int, int>>();
    }

    /**
     * @brief does bottom-up and top-down recurssion for sizing
     * clears necessary internal variables
     * calculates the smallest area between the bottom-up recurssion and the
     * top-down
     *
     * @param sizes the shapes of all the blocks using their block-number
     * @param polish the polish expression to perform sizing on
     * @return int 0 if success
     */
    int do_sizing(map<int, vector<pair<int, int>>> *sizes,
                  vector<string> polish);

    /**
     * @brief format output to a file
     *
     * @param fout the file to output to
     * @return int 0 if success
     */
    int output_sizing(string fout);
    /**
     * @brief Get the coordinates of blocks
     * taken at the center of a block
     * indexed by block-number
     *
     * @return map<int, pair<float, float>>
     */
    map<int, pair<float, float>> get_coords();
    /**
     * @brief Get the total area of the floorplan
     *
     * @return float
     */
    float get_area();
};