#include "circuit_parser.h"

int circuit_parser::parse_circuit_file(char *fName) {
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
            int gate_lib_loc = library->find_gate_lib(currentWord);

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
                    gate_num, &(library->gate_lib_array[gate_lib_loc])));
            }

            // build adjacency list
            std::list<int> temp = *(new std::list<int>);

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

void circuit_parser::check_resize(int gate_num) {
    if (gate_num >= adj_list.size()) {
        // increment size to 1000 greater than order of number
        adj_list.resize(((gate_num / 1000) + 1) * 1000);
        // integer division truncates
    }
}