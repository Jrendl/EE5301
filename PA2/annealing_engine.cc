#include "annealing_engine.h"

vector<string> annealing_engine::do_annealing() {
    vector<string> cur_sol = init_random_polish();

    float T = T_0;
    while (T > T_FREEZE) {
        for (int i = 0; i < NUM_MOVES_PER_TEMP_STEP; i++) {
            vector<string> next_sol = make_move(cur_sol);
            float delta_cost = cost(next_sol) - cost(cur_sol);
            if (accept_move(delta_cost, T)) {
                cur_sol = next_sol;
            }
        }
        T = cool_down(T);
    }

    return cur_sol;
}

bool annealing_engine::accept_move(float delta_cost, float T) {
    if (delta_cost < 0) {
        return true;
    } else {
        float boltz = exp((-delta_cost) / (K * T));
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return (r < boltz);
    }
}

float annealing_engine::cost(vector<string> solution) {
    sizing.do_sizing(shapes, solution);
    return ((alpha * sizing.get_area()) +
            ((1 - alpha) * wire_length(sizing.get_coords())));
}

float annealing_engine::cool_down(float T) {
    return COOLING_RATE * T;
}

vector<string> annealing_engine::init_random_polish() {
    // TODO: make this actually work
    int strlength = shapes->size();

    // make a list of available chars
    vector<string> avail_chars = vector<string>();
    for (auto it = shapes->begin(); it != shapes->end(); it++) {
        avail_chars.push_back(to_string((*it).first));
    }

    // we need spaces for n-1 operators
    strlength += (strlength - 1);
    vector<string> polish = vector<string>();

    for (int i = 0; i < strlength; i++) {
        if (i < 2) {
            // the first two chars must always be nodes for a valid polish
            // expression

            // choose a node and remove from available
            // rand int between 0 and avail_chars.length-1
            int loc = rand() % avail_chars.size();
            polish.push_back(avail_chars[loc]);
            avail_chars.erase(loc + avail_chars.begin());
        } else {
            // roll for op node, even odds
            int roll = rand() % 2;
            if (roll == 0 && avail_chars.size() > 0) {
                // if we rolled a 0 and there's characters available
                // choose a character
                // choose a node and remove from available
                // rand int between 0 and avail_chars.length-1
                int loc = rand() % avail_chars.size();
                polish.push_back(avail_chars[loc]);
                avail_chars.erase(loc + avail_chars.begin());
            } else if (roll == 0 && avail_chars.size() == 0) {
                // if we rolled a 0 and there's no characters available
                string r = polish[i - 1];
                if (is_op(r)) {
                    if (r.compare("|") == 0) {
                        polish.push_back("-");
                    } else {
                        polish.push_back("|");
                    }
                } else {
                    // we can place an op
                    int op = rand() % 2;
                    if (op == 0) {
                        polish.push_back("|");
                    } else {
                        polish.push_back("-");
                    }
                }

            } else {
                // if we rolled a 1
                // sample the two characters before the one you're placing
                string l = polish[i - 2];
                string r = polish[i - 1];

                if (is_op(l) && is_op(r)) {
                    // both are operators
                    // i must be at least 7
                    if (i >= 7) {
                        // place an op
                        // can't be the same as r
                        if (r.compare("|") == 0) {
                            polish.push_back("-");
                        } else {
                            polish.push_back("|");
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.size();
                        polish.push_back(avail_chars[loc]);
                        avail_chars.erase(loc + avail_chars.begin());
                    }
                } else if (is_op(l) && !is_op(r)) {
                    // the left child is an op
                    //  i must be at least 5
                    if (i >= 5) {
                        // place an op
                        // doesn't matter which one cause right child isn't op
                        int op = rand() % 2;
                        if (op == 0) {
                            polish.push_back("|");
                        } else {
                            polish.push_back("-");
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.size();
                        polish.push_back(avail_chars[loc]);
                        avail_chars.erase(loc + avail_chars.begin());
                    }
                } else if (!is_op(l) && is_op(r)) {
                    // right child is an op
                    //  i must be at least 5
                    // we can only place the op that is not in r
                    if (i >= 5) {
                        // place the op not in r
                        if (r.compare("|") == 0) {
                            polish.push_back("-");
                        } else {
                            polish.push_back("|");
                        }
                    } else {
                        // place a node
                        // choose a node and remove from available
                        // rand int between 0 and avail_chars.length-1
                        int loc = rand() % avail_chars.size();
                        polish.push_back(avail_chars[loc]);
                        avail_chars.erase(loc + avail_chars.begin());
                    }
                } else {
                    // neither are ops
                    // we can place an op
                    int op = rand() % 2;
                    if (op == 0) {
                        polish.push_back("|");
                    } else {
                        polish.push_back("-");
                    }
                }
            }
        }
    }
    // this should be fine cause you only have to do it once
    // should come up with a valid one eventually?
    // in theory the first one is valid? but this is a good check
    if (check_valid_polish(polish)) {
        return polish;
    } else {
        return init_random_polish();
    }
}

bool annealing_engine::is_op(string c) {
    return (c.compare("|") == 0 || c.compare("-") == 0);
}

vector<string> annealing_engine::make_move(vector<string> polish) {
    int r = rand() % 3;
    vector<string> out = polish;
    switch (r) {
        // TODO: implement moves
        case 0:
            // OP1: exchange two operands that have no other operands in between
            break;
        case 1:
            // OP2: Complement a series of operators between two operands
            break;
        case 2:
            // OP3: Exchange adjacent operance and operator if the resulting
            // expression still a normalized polish exp
            break;
    }
    return out;
}

float annealing_engine::wire_length(map<int, pair<int, int>> coords) {
    // TODO: implement wire-length calculation
    return 1;
}

bool annealing_engine::check_valid_polish(vector<string> polish) {
    int count = 0;
    for (auto it = polish.begin(); it != polish.end(); it++) {
        if (is_op(*it)) {
            count--;
        } else {
            count++;
        }
    }

    return (count == 1);
}
