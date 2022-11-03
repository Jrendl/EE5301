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

        if (parser->fanin_list[gate_num].size() > 1) {
            // assign output slew
            std::get<0>(arrival_times[gate_num]) =
                (parser->fanin_list[gate_num].size() / 2) *
                lib->get_output_slew(input_slew, output_load, gate);
            // assign gate arrival time
            std::get<1>(arrival_times[gate_num]) =
                ready_time +
                ((parser->fanin_list[gate_num].size() / 2) *
                 lib->get_cell_delay(input_slew, output_load, gate));

            // assign cell delay
            std::get<2>(arrival_times[gate_num]) =
                (parser->fanin_list[gate_num].size() / 2) *
                lib->get_cell_delay(input_slew, output_load, gate);
        } else {
            // assign output slew
            std::get<0>(arrival_times[gate_num]) =
                lib->get_output_slew(input_slew, output_load, gate);
            // assign gate arrival time
            std::get<1>(arrival_times[gate_num]) =
                lib->get_cell_delay(input_slew, output_load, gate);
            // assign cell delay
            std::get<2>(arrival_times[gate_num]) =
                lib->get_cell_delay(input_slew, output_load, gate);
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
            // don't add it if it's an output
            if (degrees[*j] == 0 &&
                parser->gate_type_map[*j]->name.compare("OUTPUT") != 0) {
                queue.push_back(*j);
            }
        }
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
            // is output
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
            required_times[gate_num] - std::get<1>(arrival_times[gate_num]);

        // decrement the inputs and add to queue if ready
        for (j = parser->fanin_list[gate_num].begin();
             j != parser->fanin_list[gate_num].end(); j++) {
            degrees[*j]--;
            // ready to be processed
            // don't add it if it's an input
            if (degrees[*j] == 0 && parser->fanin_list[*j].front() != -1) {
                queue.push_back(*j);
            }
        }
    }
}

int timing_analyzer::output(const char* outFile) {
    char lineBuf[1024];
    cout << "outputting to file: " << outFile << endl;
    ofstream ofs(outFile);

    if (ofs.is_open() == 0) {
        cout << "Error opening " << outFile << endl;
        return -1;
    }

    ofs << "Circuit delay: " << latest_arrival << " ps\n\n";
    ofs << "Gate Slacks:" << endl;

    std::map<int, gate_t*>::iterator i;
    for (i = parser->gate_type_map.begin(); i != parser->gate_type_map.end();
         i++) {
        //<gate_type>-n<gate_number>: <slack_val> ps
        ofs << i->second->name << "-n" << i->first << ": " << slack[i->first]
            << " ps" << endl;
    }

    ofs << "\nCritical Path: " << endl;
    int min_slack_out_node = 0;
    float min_slack_out = 1000000.0;
    std::list<int> critical_path = std::list<int>();

    for (int i = 1; i <= parser->fanout_list.size(); i++) {
        if (parser->fanout_list[i].front() == -2 && slack[i] < min_slack_out) {
            min_slack_out = slack[i];
            min_slack_out_node = i;
        }
    }

    critical_path.push_front(min_slack_out_node);

    int node = min_slack_out_node;
    while (1) {
        int next_node = parser->fanin_list[node].front();
        std::list<int>::iterator it;
        for (it = parser->fanin_list[node].begin();
             it != parser->fanin_list[node].end(); it++) {
            if (slack[*it] <= slack[next_node]) {
                next_node = *it;
            }
        }
        critical_path.push_front(next_node);
        node = next_node;
        // we've gotten to the input
        if (parser->fanin_list[node].front() == -1) {
            break;
        }
    }

    std::list<int>::iterator it;

    for (it = critical_path.begin(); it != critical_path.end(); it++) {
        ofs << parser->gate_type_map[*it]->name << "-n" << *it << ", ";
    }
    ofs << endl;
    ofs.close();
    return 0;
}

int timing_analyzer::analyze(const char* outFile) {
    cout << "forward traversal" << endl;
    forward_traverse();
    cout << "backward Traversal" << endl;
    backward_traverse();
    return output(outFile);
}