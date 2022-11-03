#include "timing_analyzer.h"

void timing_analyzer::forward_traverse() {
    int degrees[parser->fanin_list.size()];
    std::list<int> queue = std::list<int>();

    // store degree of all nodes
    for (int i = 0; i < parser->fanin_list.size(); i++) {
        if (parser->fanin_list[i].front() == -1) {
            // is input
            degrees[i] = 0;
            queue.push_back(i);
            // assign assumed values
            arrival_times[i] = std::tuple<float, float, float>(2.0, 0.0, 0.0);
        } else {
            degrees[i] = parser->fanin_list[i].size();
        }
    }

    // go through queue
    while (!queue.empty()) {
        int gate_num = queue.front();
        queue.pop_front();
        gate_t* gate = parser->gate_type_map[gate_num];
        float input_slew = 0;
        float output_load = gate->capacitance;
        float ready_time = 0;

        // find the max input slew
        std::list<int>::iterator j;
        for (j = parser->fanin_list[gate_num].begin();
             j != parser->fanin_list[gate_num].end(); j++) {
            input_slew = std::max(input_slew, std::get<0>(arrival_times[(*j)]));
            ready_time = std::max(ready_time, std::get<1>(arrival_times[(*j)]));
        }

        // find the max output capacitance
        for (j = parser->fanout_list[gate_num].begin();
             j != parser->fanout_list[gate_num].end(); j++) {
            output_load += (float)parser->gate_type_map[(*j)]->capacitance;
        }

        int i1 = lib->find_index_1(input_slew, gate);
        int i2 = lib->find_index_2(output_load, gate);

        if (parser->fanin_list[gate_num].size() > 1) {
            // assign output slew
            std::get<0>(arrival_times[gate_num]) =
                (parser->fanin_list[gate_num].size() / 2) *
                gate->output_slew[i1][i2];
            // assign gate arrival time
            std::get<1>(arrival_times[gate_num]) =
                ready_time + ((parser->fanin_list[gate_num].size() / 2) *
                              gate->cell_delay[i1][i2]);

            std::get<2>(arrival_times[gate_num]) =
                (parser->fanin_list[gate_num].size() / 2) *
                gate->cell_delay[i1][i2];
        } else {
            // assign output slew
            std::get<0>(arrival_times[gate_num]) = gate->output_slew[i1][i2];
            // assign gate arrival time
            std::get<1>(arrival_times[gate_num]) =
                ready_time + gate->cell_delay[i1][i2];
            std::get<2>(arrival_times[gate_num]) = gate->cell_delay[i1][i2];
        }

        // update latest arrival
        // should be critical node by end of pass
        latest_arrival =
            std::max(latest_arrival, std::get<1>(arrival_times[gate_num]));

        // decrement fanout counters
        for (j = parser->fanout_list[gate_num].begin();
             j != parser->fanout_list[gate_num].end(); j++) {
            degrees[*j]--;
            // ready to be processed
            if (degrees[*j] == 0) {
                queue.push_back(*j);
            }
        }

        // TODO: capactiance for output nodes (4x invertor capacitance)
    }
}

void timing_analyzer::backward_traverse() {
    // do reverse
    // node tells it's inputs if its done

    int degrees[parser->fanout_list.size()];
    std::list<int> queue = std::list<int>();

    // store degree of all nodes
    for (int i = 0; i < parser->fanout_list.size(); i++) {
        if (parser->fanout_list[i].front() == -2) {
            // is input
            degrees[i] = 0;
            queue.push_back(i);
            // assign assumed values
            required_times[i] = 1.1 * latest_arrival;
        } else {
            degrees[i] = parser->fanout_list[i].size();
        }
    }

    while (!queue.empty()) {
        int gate_num = queue.front();
        queue.pop_front();
        float required_output = 100000;
        std::list<int>::iterator j;

        // find earliest required output
        for (j = parser->fanout_list[gate_num].begin();
             j != parser->fanout_list[gate_num].end(); j++) {
            required_output = std::min(required_output, required_times[*j]);
        }

        // update required input time
        required_times[gate_num] =
            required_output - std::get<2>(arrival_times[gate_num]);

        slack[gate_num] =
            std::get<1>(arrival_times[gate_num]) - required_times[gate_num];

        // decrement the inputs and add to queue if ready
        for (j = parser->fanin_list[gate_num].begin();
             j != parser->fanin_list[gate_num].end(); j++) {
            degrees[*j]--;
            if (degrees[*j] == 0) {
                queue.push_back(*j);
            }
        }
    }
}

int timing_analyzer::analyze(char* outFile) {
    forward_traverse();
    backward_traverse();
    // output
}