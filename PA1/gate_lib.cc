#include "gate_lib.h"

int gate_lib::parse_gate_library(char* fName) {
    char lineBuf[1024];
    cout << "Parsing input file " << fName << " using C++ syntax." << endl;
    ifstream ifs(fName);
    if (ifs.is_open() == 0) {  // or we could say if (!ifs)
        cout << "Error opening file " << fName << endl;
        return -1;
    }

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
            gate_lib_array[gate_count].name = cellName;
        } else if (firstWord.compare("capacitance") ==
                   0)  // the first word is exactly capacitance, we have found
                       // that cells capacitance
        {
            string cap;
            iss >> cap;
            cout << "capacitance =" << cap << endl;
            gate_lib_array[gate_count].capacitance = stod(cap);
        } else if (firstWord.compare("index_1") == 0) {
            string val;
            for (int i = 0; i < 7; i++) {
                iss >> val;
                gate_lib_array[gate_count].index_1[i] = stof(val);
            }

        } else if (firstWord.compare("index_2") == 0) {
            string val;
            for (int i = 0; i < 7; i++) {
                iss >> val;
                gate_lib_array[gate_count].index_2[i] = stof(val);
            }
        } else if (firstWord.compare("values") ==
                   0)  // this is either the first line of the delay or skew
                       // table
        {
            if (gate_lib_array[gate_count]
                    .table_indicator) {  // we are in the delay_table
                // take the first row of the table
                string val;
                for (int i = 0; i < 7; i++) {
                    iss >> val;
                    cout << "delay table value: " << val << endl;
                    gate_lib_array[gate_count].cell_delay[delay_table][i] =
                        stod(val);
                }
                delay_table++;
            } else {  // in the output slew table
                // take the first row of the table
                string val;
                for (int i = 0; i < 7; i++) {
                    iss >> val;
                    cout << "output slew value: " << val << endl;
                    gate_lib_array[gate_count].output_slew[output_slew][i] =
                        stod(val);
                }
                output_slew++;
            }
        } else if (firstWord.find("0.") !=
                   string::npos)  // all other rows of either table
        {
            if (gate_lib_array[gate_count]
                    .table_indicator) {  // we are in the delay_table
                gate_lib_array[gate_count].cell_delay[delay_table][0] =
                    stod(firstWord);
                string val;
                for (int i = 1; i < 7; i++) {
                    iss >> val;
                    cout << "delay table value: " << val << endl;
                    gate_lib_array[gate_count].cell_delay[delay_table][i] =
                        stod(val);
                }
                delay_table++;
                // check if we've fully populated the delay table
                if (delay_table >= 7) {
                    delay_table = 0;
                    gate_lib_array[gate_count].table_indicator =
                        false;  // move onto the slew table
                }
            } else {  // in the output slew table
                gate_lib_array[gate_count].output_slew[output_slew][0] =
                    stod(firstWord);
                string val;
                for (int i = 1; i < 7; i++) {
                    iss >> val;
                    cout << "output slew value: " << val << endl;
                    gate_lib_array[gate_count].output_slew[output_slew][i] =
                        stod(val);
                }
                output_slew++;
                // check if we've fully populated the output slew table
                if (output_slew >= 7) {
                    output_slew = 0;
                    gate_count++;  // we're done with this gate
                }
            }
        }
    }

    ifs.close();
    return gate_count;
}

int gate_lib::find_gate_lib(string gate_name) {
    for (int i = 0; i < gate_count; i++) {
        if (gate_name.compare(gate_lib_array[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

int gate_lib::get_gate_count() {
    return gate_count;
}

int gate_lib::find_index_1(float value, gate_t* gate) {
    for (int i = 0; i < 7; i++) {
        if (gate->index_1[i] == value) {
            return i;
        }
    }
}

int gate_lib::find_index_2(float value, gate_t* gate) {
    for (int i = 0; i < 7; i++) {
        if (gate->index_2[i] == value) {
            return i;
        }
    }
}
