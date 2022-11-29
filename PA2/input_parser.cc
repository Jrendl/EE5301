#include "input_parser.h"

int input_parser::parse_file(char *input) {
    char lineBuf[1024];
    ifstream ifs(input);

    if (ifs.is_open() == 0) {  // or we could say if (!ifs)
        cout << "Error opening file " << input << endl;
        return -1;
    }

    // get first line
    ifs.getline(lineBuf, 1023);  // read one line
    string lineStr(lineBuf);     // convert to C++ string

    // this is the number of blocks
    int num_blocks = stoi(lineStr);

    // parse block sizes
    for (int i = 0; i < num_blocks; i++) {
        ifs.getline(lineBuf, 1023);  // read one line
        string lineStr(lineBuf);     // convert to C++ string

        // this depends heavily on a correctly formatted file
        istringstream iss(lineStr);

        string block;
        iss >> block;

        string width;
        iss >> width;

        string height;
        iss >> height;

        sizes[stoi(block)].push_back(pair<int, int>(stoi(width), stoi(height)));
        sizes[stoi(block)].push_back(pair<int, int>(stoi(height), stoi(width)));
    }

    // TODO: parse hyper-edges

    ifs.close();
}

map<int, vector<pair<int, int>>> *input_parser::get_sizes() {
    return &sizes;
}