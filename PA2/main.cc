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
    cout << "parsed input file" << endl;

    // sizer block_sizer = sizer();
    annealing_engine engine =
        annealing_engine(parser.get_sizes(), parser.get_edges(), (argv[2]));
    cout << "initialized engine" << endl;

    engine.do_annealing();
    cout << "finished annealing process" << endl;

    // output phase 1 to file
    string input_file = string(argv[1]);
    size_t cut_loc = input_file.find('.', 0);
    string outstring = input_file.substr(0, cut_loc);
    outstring += string("_Rendleman_Joseph.out2") + argv[2][1];

    cout << "outputting to file: " << outstring << endl;
    engine.output(outstring);
}

bool is_op(char c) {
    return (c == '|' || c == '-');
}