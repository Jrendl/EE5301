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

#include "input_parser.h"
#include "sizer.h"

using namespace std;
bool is_op(char c);
string init_random_polish(map<int, vector<pair<int, int>>>* shapes);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please provide input file." << endl;
        return -1;
    }

    // parse the input file
    input_parser parser = input_parser();
    parser.parse_file(argv[1]);

    sizer block_sizer = sizer();

    string polish = init_random_polish(parser.get_sizes());
    cout << polish << endl;
    // do sizing on the random initial string
    block_sizer.do_sizing(parser.get_sizes(), polish);

    // output phase 1 to file
    string outstring = string(argv[1]) + "_Rendleman_Joseph.out1";

    block_sizer.output_sizing(outstring);
}

string init_random_polish(map<int, vector<pair<int, int>>>* shapes) {
    int strlength = shapes->size();

    // make a list of available chars
    string avail_chars = "";
    for (auto it = shapes->begin(); it != shapes->end(); it++) {
        avail_chars += to_string((*it).first);
    }

    // we need spaces for n-1 operators
    strlength += (strlength - 1);
    string polish = "";

    for (int i = 0; i < strlength; i++) {
        if (i < 2) {
            // the first two chars must always be nodes for a valid polish
            // expression

            // choose a node and remove from available
            // rand int between 0 and avail_chars.length-1
            int loc = rand() % avail_chars.length();
            polish += avail_chars.c_str()[loc];
            avail_chars.erase(loc, 1);
        } else {
            // roll for op node, even odds
            int roll = rand() % 2;
            if (roll == 0 && avail_chars.length() > 0) {
                // if we rolled a 0 and there's characters available
                // choose a character
                // choose a node and remove from available
                // rand int between 0 and avail_chars.length-1
                int loc = rand() % avail_chars.length();
                polish += avail_chars.c_str()[loc];
                avail_chars.erase(loc, 1);
            } else if (roll == 0 && avail_chars.length() == 0) {
                // if we rolled a 0 and there's no characters available
                char r = polish.c_str()[i - 1];
                if (is_op(r)) {
                    if (r == '|') {
                        polish += '-';
                    } else {
                        polish += '|';
                    }
                } else {
                    // we can place an op
                    int op = rand() % 2;
                    if (op == 0) {
                        polish += '|';
                    } else {
                        polish += '-';
                    }
                }

            } else {
                // if we rolled a 1
                // sample the two characters before the one you're placing
                char l = polish.c_str()[i - 2];
                char r = polish.c_str()[i - 1];

                if (is_op(l) && is_op(r)) {
                    // both are operators
                    // i must be at least 7
                    if (i >= 7) {
                        // place an op
                        // can't be the same as r
                        if (r == '|') {
                            polish += '-';
                        } else {
                            polish += '|';
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.length();
                        polish += avail_chars.c_str()[loc];
                        avail_chars.erase(loc, 1);
                    }
                } else if (is_op(l) && !is_op(r)) {
                    // the left child is an op
                    //  i must be at least 5
                    if (i >= 5) {
                        // place an op
                        // doesn't matter which one cause right child isn't op
                        int op = rand() % 2;
                        if (op == 0) {
                            polish += '|';
                        } else {
                            polish += '-';
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.length();
                        polish += avail_chars.c_str()[loc];
                        avail_chars.erase(loc, 1);
                    }
                } else if (!is_op(l) && is_op(r)) {
                    // right child is an op
                    //  i must be at least 5
                    // we can only place the op that is not in r
                    if (i >= 5) {
                        // place the op not in r
                        if (r == '|') {
                            polish += '-';
                        } else {
                            polish += '|';
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.length();
                        polish += avail_chars.c_str()[loc];
                        avail_chars.erase(loc, 1);
                    }
                } else {
                    // neither are ops
                    // we can place an op
                    int op = rand() % 2;
                    if (op == 0) {
                        polish += '|';
                    } else {
                        polish += '-';
                    }
                }
            }
        }
    }

    return polish;
}

bool is_op(char c) {
    return (c == '|' || c == '-');
}