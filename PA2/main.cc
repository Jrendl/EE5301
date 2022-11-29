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

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please provide input file." << endl;
        return -1;
    }

    input_parser parser = input_parser();
    parser.parse_file(argv[1]);

    sizer block_sizer = sizer();
    // TODO: generate real random string
    string test_string = "12|5-09|-867|34-|-|";
    block_sizer.do_sizing(parser.get_sizes(), test_string);

    // TODO: output to file
}
