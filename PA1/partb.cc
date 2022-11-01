// FileIO.cpp : Defines the entry point for the console application.
//

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

#include "circuit_parser.h"
#include "gate_lib.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Format Parameters: Library Filename, Circuit Filename, node "
                "numbers"
             << endl;
        return -1;
    }

    gate_lib library = gate_lib();
    circuit_parser parser = circuit_parser(&library);

    // parse Library File
    library.parse_gate_library(argv[1]);
    if (library.get_gate_count() == -1) {
        return -1;
    }

    // parse circuit File

    parser.adj_list.resize(1000);  // initial assumption of 1000 gates
    parser.parse_circuit_file(argv[2]);

    // Output node info
    // outputFile(gate_count);
    return 0;
}
