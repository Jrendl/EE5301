#include "sizer.h"

pair<vector<pair<int, int>>, vector<pair<int, int>>> sizer::vert_size(
    vector<pair<int, int>> *L, vector<pair<int, int>> *R) {
    // L is sorted with a_i<a_j, b_i>b_j for all i<j
    // R is sorted with x_i<x_j, y_i>y_j for all i<j

    // first vector in pair is H, the actual sizes
    vector<pair<int, int>> H = vector<pair<int, int>>();
    // second vector in pair is the corresponding <i, j>
    vector<pair<int, int>> ij = vector<pair<int, int>>();
    auto i = L->begin();
    auto j = R->begin();
    while (i != L->end() && j != R->end()) {
        pair<int, int> out = pair<int, int>((*i).first + (*j).first,
                                            max((*i).second, (*j).second));
        H.push_back(out);
        ij.push_back(pair<int, int>(i - L->begin(), j - R->begin()));
        if (max((*i).second, (*j).second) == (*i).second) {
            i++;
        }
        if (max((*i).second, (*j).second) == (*j).second) {
            j++;
        }
    }

    return pair<vector<pair<int, int>>, vector<pair<int, int>>>(H, ij);
}

pair<vector<pair<int, int>>, vector<pair<int, int>>> sizer::hor_size(
    vector<pair<int, int>> *U, vector<pair<int, int>> *L) {
    // everything stays the same except you traverse from back to front and
    // change the dimension used to check for dominance

    // first vector in pair is H, the actual sizes
    vector<pair<int, int>> H = vector<pair<int, int>>();
    // second vector in pair is the corresponding <i, j>
    vector<pair<int, int>> ij = vector<pair<int, int>>();
    auto i = U->rbegin();
    auto j = L->rbegin();
    while (i != U->rend() && j != L->rend()) {
        pair<int, int> out = pair<int, int>(max((*i).first, (*j).first),
                                            (*i).second + (*j).second);
        H.push_back(out);
        ij.push_back(pair<int, int>(i - U->rbegin(), j - L->rbegin()));
        if (max((*i).first, (*j).first) == (*i).first) {
            i++;
        }
        if (max((*i).first, (*j).first) == (*j).first) {
            j++;
        }
    }

    return pair<vector<pair<int, int>>, vector<pair<int, int>>>(H, ij);
}

int sizer::do_sizing(map<int, vector<pair<int, int>>> *sizes,
                     vector<string> polish) {
    // clear internal variables
    this->polish = polish;
    sizes_by_loc = map<int, vector<pair<int, int>>>();
    final_shapes = map<int, pair<int, int>>();
    ij_pairs = map<int, vector<pair<int, int>>>();
    used_by_parent = map<int, bool>();
    coords = map<int, pair<int, int>>();
    children = map<int, pair<int, int>>();
    // initialization
    for (int i = 0; i < polish.size(); i++) {
        used_by_parent[i] = false;
        coords[i] = pair<int, int>(0, 0);
        string x = polish[i];
        if (!is_op(x)) {
            sizes_by_loc[i] = (*sizes)[stoi(x)];
        } else {
            // init the spaces for the operators to empty vectors
            sizes_by_loc[i] = vector<pair<int, int>>();
        }
    }

    int err_code = bottom_up_recursive(0);
    if (err_code == -1) {
        return -1;
    }

    int head = polish.size() - 1;

    auto it = sizes_by_loc[head].begin();
    int min_area_space = 0;
    int current_min_area = sizes_by_loc[head][min_area_space].first *
                           sizes_by_loc[head][min_area_space].second;
    while (it != sizes_by_loc[head].end()) {
        int area = (*it).first * (*it).second;

        if (area <= current_min_area) {
            current_min_area = area;
            min_area_space = it - sizes_by_loc[head].begin();
        }
        it++;
    }

    top_down_recursive(head, min_area_space);

    return 0;
}

int sizer::bottom_up_recursive(int start) {
    string h = polish[start + 2];

    if (h.compare("|") == 0) {
        if (!used_by_parent[start]) {
            // we only care about the left child
            pair<vector<pair<int, int>>, vector<pair<int, int>>> split_info =
                vert_size(&sizes_by_loc[start], &sizes_by_loc[start + 1]);

            // housekeeping
            sizes_by_loc[start + 2] = split_info.first;
            ij_pairs[start + 2] = split_info.second;
            used_by_parent[start] = true;
            used_by_parent[start + 1] = true;
            children[start + 2] = pair<int, int>(start, start + 1);
        } else {
            // the right child will never have been used by a parent so the only
            // other case is that the left has
            // we need to find the most recent child that hasn't been used by a
            // parent
            int direct_child = start;
            while (used_by_parent[direct_child]) {
                direct_child--;
            }

            pair<vector<pair<int, int>>, vector<pair<int, int>>> split_info =
                vert_size(&sizes_by_loc[direct_child],
                          &sizes_by_loc[start + 1]);

            // housekeeping
            sizes_by_loc[start + 2] = split_info.first;
            ij_pairs[start + 2] = split_info.second;

            used_by_parent[direct_child] = true;
            used_by_parent[start + 1] = true;
            children[start + 2] = pair<int, int>(direct_child, start + 1);
        }

    } else if (h.compare("-") == 0) {
        if (!used_by_parent[start]) {
            // we only care about the left child
            pair<vector<pair<int, int>>, vector<pair<int, int>>> split_info =
                hor_size(&sizes_by_loc[start], &sizes_by_loc[start + 1]);

            // housekeeping
            sizes_by_loc[start + 2] = split_info.first;
            ij_pairs[start + 2] = split_info.second;

            used_by_parent[start] = true;
            used_by_parent[start + 1] = true;
            children[start + 2] = pair<int, int>(start, start + 1);
        } else {
            // the right child will never have been used by a parent so the only
            // other case is that the left has
            // we need to find the most recent child that hasn't been used by a
            // parent
            int direct_child = start;
            while (used_by_parent[direct_child]) {
                direct_child--;
            }

            pair<vector<pair<int, int>>, vector<pair<int, int>>> split_info =
                hor_size(&sizes_by_loc[direct_child], &sizes_by_loc[start + 1]);

            // housekeeping
            sizes_by_loc[start + 2] = split_info.first;
            ij_pairs[start + 2] = split_info.second;

            used_by_parent[direct_child] = true;
            used_by_parent[start + 1] = true;
            children[start + 2] = pair<int, int>(direct_child, start + 1);
        }
    }

    if (start + 3 >= polish.size()) {
        // we've reached the end of the string since we're looking 2 ahead for
        // all of them
        return 0;
    } else {
        bottom_up_recursive(start + 1);
        return 0;
    }
}

int sizer::top_down_recursive(int node, int shape) {
    // grab the string at that location
    string n = polish[node];
    // store the final shape
    final_shapes[node] = sizes_by_loc[node][shape];
    if (is_op(n)) {
        // grab ij values
        pair<int, int> ij = ij_pairs[node][shape];
        // grab the children
        int child_1 = children[node].first;
        int child_2 = children[node].second;
        // find the children's final dimensions
        // don't store it yet, we'll do that when we recurse to them
        pair<int, int> child_1_dims = sizes_by_loc[child_1][ij.first];
        pair<int, int> child_2_dims = sizes_by_loc[child_2][ij.second];

        if (n.compare("|") == 0) {
            // set left child's coords to ours
            coords[child_1] = coords[node];

            // set x coord of right child to our x + width of left
            // same y value
            int right_x = coords[node].first + child_1_dims.first;
            coords[child_2] = pair<int, int>(right_x, coords[node].second);
        } else {
            // set bottom child's coords to be the same as ours
            coords[child_2] = coords[node];

            // set y coord of top child to our y + height of bottom
            // same x value
            int top_y = coords[node].second + child_2_dims.second;
            coords[child_1] = pair<int, int>(coords[node].first, top_y);
        }

        // recursively call with first child and i value
        top_down_recursive(child_1, ij.first);

        // recursively call with second child and j value
        top_down_recursive(child_2, ij.second);
    } else {
        // we've reached a child block which doesn't have any children and it's
        // coords are set by the parent
        // end of recursion
        return 0;
    }

    return 0;
}

int sizer::output_sizing(string fout) {
    // open in append mode
    ofstream ofs(fout.c_str(), ios_base::app);

    if (ofs.is_open() == 0) {
        cout << "Error opening " << fout << endl;
        return -1;
    }
    // output polish
    for (int i = 0; i < polish.size(); i++) {
        if (is_op(polish[i])) {
            ofs << polish[i];
        } else {
            ofs << '[' << polish[i] << ']';
        }
    }
    ofs << endl;

    // output total shape
    ofs << "width/height: ";
    ofs << final_shapes[polish.size() - 1].first << " "
        << final_shapes[polish.size() - 1].second << endl;

    // output total area
    ofs << "Area: ";
    ofs << final_shapes[polish.size() - 1].first *
               final_shapes[polish.size() - 1].second
        << endl;

    ofs << "x/y/width/height" << endl;
    for (int i = 0; i < polish.size() - 1; i++) {
        if (!is_op(polish[i])) {
            ofs << polish[i] << ": ";
            ofs << coords[i].first << " " << coords[i].second << " "
                << final_shapes[i].first << " " << final_shapes[i].second
                << endl;
        }
    }

    return 0;
}

bool sizer::is_op(string c) {
    return (c.compare("|") == 0 || c.compare("-") == 0);
}

map<int, pair<float, float>> sizer::get_coords() {
    map<int, pair<float, float>> out_coords = map<int, pair<float, float>>();

    // should find the center of the shapes
    for (auto it = coords.begin(); it != coords.end(); it++) {
        if (!is_op(polish[(*it).first])) {
            out_coords[stoi(polish[(*it).first])] = pair<float, float>(
                (*it).second.first +
                    (float)final_shapes[(*it).first].first / 2.0,
                (*it).second.second +
                    (float)final_shapes[(*it).first].second / 2.0);
        }
    }

    return out_coords;
}

float sizer::get_area() {
    return final_shapes[polish.size() - 1].first *
           final_shapes[polish.size() - 1].second;
}