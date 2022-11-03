#include "timing_analyzer.h"

int timing_analyzer::forward_traverse() {
    // max arrival time of inputs plus gate delay
    // each node needs to tell fanout it's ready
    // when a node is done waiting on all other nodes, inset to queue
    // when input is processed, decrease counter

    int degrees[parser->fanin_list.size()];
    std::list<int> queue = std::list<int>();

    // store degree of all nodes
    for (int i = 0; i < parser->fanin_list.size(); i++) {
        if (parser->fanin_list[i].front() == -1) {
            // is input
            degrees[i] = 0;
            queue.push_back(i);
        } else {
            degrees[i] = parser->fanin_list[i].size();
        }
    }

    // go through queue
    while (!queue.empty()) {
        int gate_num = queue.front();
        queue.pop_front();
        float input_slew = 0;
        float output_load = 0;
        std::list<int>::iterator j;
        for (j = parser->fanin_list[gate_num].begin();
             j != parser->fanin_list[gate_num].end(); j++) {
            // this needs to be the calculated value
            input_slew = std::max(input_slew, arrival_times[???].first());
        }
    }
}

int timing_analyzer::backward_traverse() {
}

int timing_analyzer::analyze(char* outFile) {
    forward_traverse();
    backward_traverse();
    // output
}