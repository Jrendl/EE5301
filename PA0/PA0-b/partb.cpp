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

using namespace std;

// TODO: add map for gate number to struct
// TODO: add vector of lists for adjacency table
// TODO: parse Circuit file
// TODO: output for gate numbers

constexpr int MAX_NUM_GATES = 100;
/**
 * @brief gate type definition
 * name: the type of gate implemented
 * table_indicator: variable used for parsing file.
 * True if we're parsing the delay table, false if parsing the output slew
 * capacitance: internal capacitance of the gate
 * cell_delay: 7x7 delay table
 * output_slew: 7x7 output slew rate table
 *
 */
struct gate_t {
    string name = "";
    bool table_indicator = true;
    double capacitance = 0.0;
    double cell_delay[7][7] = {0.0};
    double output_slew[7][7] = {0.0};
};

struct delimiters : std::ctype<char> {
    delimiters() : std::ctype<char>(get_table()) {
    }
    static mask const *get_table() {
        static mask rc[table_size];
        rc['('] = std::ctype_base::space;
        rc[')'] = std::ctype_base::space;
        rc[','] = std::ctype_base::space;
        rc['='] = std::ctype_base::space;
        rc[' '] = std::ctype_base::space;
        rc['\t'] = std::ctype_base::space;
        rc[':'] = std::ctype_base::space;
        rc['\"'] = std::ctype_base::space;
        rc[';'] = std::ctype_base::space;
        rc['\r'] = std::ctype_base::space;
        return &rc[0];
    }
};

/**
 * @brief returns the next character in a file without actually reading it
 *
 * @param stream - file stream
 * @return int
 */
int fpeek(FILE *stream);
/**
 * @brief parse input file with gate information
 *
 * @param fName - relative file path
 * @return int - -1 if failed to open, number of gates otherwise
 */
int parse_gate_library(char *fName);

int parse_circuit_file(char *fName);

int find_gate_lib(string gate_name);
void check_resize(int gate_num);

/**
 * @brief global array of gates
 * size of MAX_NUM_GATES
 *
 */
gate_t gate_lib[MAX_NUM_GATES];
int gate_count;

std::vector<std::list<int>> adj_list;
std::map<int, gate_t *> gate_type_map;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Format Parameters: Library Filename, Circuit Filename, node "
                "numbers"
             << endl;
        return -1;
    }

    // parse Library File
    gate_count = parse_gate_library(argv[1]);
    if (gate_count == -1) {
        return -1;
    }

    // parse circuit File

    adj_list.resize(1000);  // initial assumption of 1000 gates
    parse_circuit_file(argv[2]);

    // Output node info
    // outputFile(gate_count);
    return 0;
}

// Kia got this function from stackoverflow
// https://stackoverflow.com/questions/2082743/c-equivalent-to-fstreams-peek
int fpeek(FILE *stream) {
    int c;

    c = fgetc(stream);
    ungetc(c, stream);

    return c;
}

int parse_gate_library(char *fName) {
    char lineBuf[1024];
    cout << "Parsing input file " << fName << " using C++ syntax." << endl;
    ifstream ifs(fName);
    if (ifs.is_open() == 0) {  // or we could say if (!ifs)
        cout << "Error opening file " << fName << endl;
        return -1;
    }

    int gate_number = 0;  // iterator between gates
    int delay_table = 0;  // iterator between rows of the delay table
    int output_slew = 0;  // iterator between rows of the output slew table
    while (ifs.good()) {
        ifs.getline(lineBuf, 1023);  // read one line
        string lineStr(lineBuf);     // convert to C++ string
        if (lineStr.empty())         // is empty line?
            continue;

        istringstream iss(lineStr);
        // apply delimiters to stream
        iss.imbue(locale(cin.getloc(), new delimiters));

        string firstWord;
        iss >> firstWord;
        if (firstWord.find("cell") != string::npos)  // found the word cell
        {
            string cellName;
            iss >> cellName;  // read the next work in the line
            // in this case, it is not a cell name
            if (cellName.find("Timing") != string::npos) {
                continue;
            }
            cout << "Found Cell " << cellName << endl;
            gate_lib[gate_number].name = cellName;
        } else if (firstWord.compare("capacitance") ==
                   0)  // the first word is exactly capacitance, we have found
                       // that cells capacitance
        {
            string cap;
            iss >> cap;
            cout << "capacitance =" << cap << endl;
            gate_lib[gate_number].capacitance = stod(cap);
        } else if (firstWord.compare("values") ==
                   0)  // this is either the first line of the delay or skew
                       // table
        {
            if (gate_lib[gate_number]
                    .table_indicator) {  // we are in the delay_table
                // take the first row of the table
                string val;
                for (int i = 0; i < 7; i++) {
                    iss >> val;
                    cout << "delay table value: " << val << endl;
                    gate_lib[gate_number].cell_delay[delay_table][i] =
                        stod(val);
                }
                delay_table++;
            } else {  // in the output slew table
                // take the first row of the table
                string val;
                for (int i = 0; i < 7; i++) {
                    iss >> val;
                    cout << "output slew value: " << val << endl;
                    gate_lib[gate_number].output_slew[output_slew][i] =
                        stod(val);
                }
                output_slew++;
            }
        } else if (firstWord.find("0.") !=
                   string::npos)  // all other rows of either table
        {
            if (gate_lib[gate_number]
                    .table_indicator) {  // we are in the delay_table
                gate_lib[gate_number].cell_delay[delay_table][0] =
                    stod(firstWord);
                string val;
                for (int i = 1; i < 7; i++) {
                    iss >> val;
                    cout << "delay table value: " << val << endl;
                    gate_lib[gate_number].cell_delay[delay_table][i] =
                        stod(val);
                }
                delay_table++;
                // check if we've fully populated the delay table
                if (delay_table >= 7) {
                    delay_table = 0;
                    gate_lib[gate_number].table_indicator =
                        false;  // move onto the slew table
                }
            } else {  // in the output slew table
                gate_lib[gate_number].output_slew[output_slew][0] =
                    stod(firstWord);
                string val;
                for (int i = 1; i < 7; i++) {
                    iss >> val;
                    cout << "output slew value: " << val << endl;
                    gate_lib[gate_number].output_slew[output_slew][i] =
                        stod(val);
                }
                output_slew++;
                // check if we've fully populated the output slew table
                if (output_slew >= 7) {
                    output_slew = 0;
                    gate_number++;  // we're done with this gate
                }
            }
        }
    }

    ifs.close();
    return gate_number;
}

int parse_circuit_file(char *fName) {
    char lineBuf[1024];
    cout << "Parsing circuit file " << fName << " using C++ syntax." << endl;
    ifstream ifs(fName);
    if (ifs.is_open() == 0) {  // or we could say if (!ifs)
        cout << "Error opening file " << fName << endl;
        return -1;
    }

    while (ifs.good()) {
        ifs.getline(lineBuf, 1023);  // read one line
        string lineStr(lineBuf);     // convert to C++ string
        if (lineStr.empty())         // is empty line?
            continue;

        transform(lineStr.begin(), lineStr.end(), lineStr.begin(), ::toupper);
        istringstream iss(lineStr);
        // apply delimiters to stream
        iss.imbue(locale(cin.getloc(), new delimiters));

        string currentWord;
        iss >> currentWord;

        if (currentWord.compare("#") == 0 || currentWord.compare("") == 0) {
            continue;  // this line is a comment
        } else if (currentWord.compare("INPUT") == 0) {
            iss >> currentWord;
            int gate_num = stoi(currentWord);
            check_resize(gate_num);
            // list set to -1 if input node
            std::list<int> temp = {-1};
            adj_list.insert(adj_list.begin() + gate_num, temp);
        } else if (currentWord.compare("OUTPUT") == 0) {
            continue;  // not currently tracking outputs of nodes
        } else {
            // any other line would begin with a number
            int gate_num = stoi(currentWord);
            check_resize(gate_num);

            iss >> currentWord;  // now hold gate type
            int gate_lib_loc = find_gate_lib(currentWord);

            // assign the type of gate
            if (gate_lib_loc == -1) {
                // if we don't have this gate in library, assume 0 delay and
                // slew
                gate_type_map.insert(
                    std::pair<int, gate_t *>(gate_num, new gate_t));
                gate_type_map[gate_num]->name = currentWord;
            } else {
                // give the map a pointer to the gate type info
                gate_type_map.insert(std::pair<int, gate_t *>(
                    gate_num, &gate_lib[gate_lib_loc]));
            }

            // build adjacency list
            std::list<int> temp;

            while (iss.good()) {
                iss >> currentWord;
                if (currentWord.compare("#") == 0) {
                    break;
                }
                temp.push_back(stoi(currentWord));
            }

            adj_list.insert(adj_list.begin() + gate_num, temp);
        }
    }
    return 0;
}

int find_gate_lib(string gate_name) {
    for (int i = 0; i < gate_count; i++) {
        if (gate_name.compare(gate_lib[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void check_resize(int gate_num) {
    if (gate_num >= adj_list.size()) {
        // increment size to 1000 greater than order of number
        adj_list.resize(((gate_num / 1000) + 1) * 1000);
        // integer division truncates
    }
}