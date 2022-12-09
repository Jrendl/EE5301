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

#include "annealing_engine.h"
#include "input_parser.h"
#include "sizer.h"

using namespace std;
bool is_op(char c);
string init_random_polish(map<int, vector<pair<int, int>>>* shapes);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Please provide input file and alpha flag" << endl;
        return -1;
    }
    srand(time(0));

    // parse the input file
    input_parser parser = input_parser();
    parser.parse_file(argv[1]);
    cout << "made it" << endl;

    // sizer block_sizer = sizer();
    annealing_engine engine =
        annealing_engine(parser.get_sizes(), parser.get_edges(), (argv[2]));
    cout << "initialized engine" << endl;
    vector<string> test_polish = engine.init_random_polish();
    for (auto it = test_polish.begin(); it != test_polish.end(); it++) {
        cout << (*it);
    }
    cout << endl;

    sizer block_sizer = sizer();

    // do sizing on the random initial string
    block_sizer.do_sizing(parser.get_sizes(), test_polish);

    // output phase 1 to file
    string outstring = string(argv[1]) + "_Rendleman_Joseph.out1";

    block_sizer.output_sizing(outstring);
}

bool is_op(char c) {
    return (c == '|' || c == '-');
}