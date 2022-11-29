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

map<int, vector<pair<int, int>>> *sizer::do_sizing(
    map<int, vector<pair<int, int>>> *sizes, string polish) {
    this->polish = polish;
    // initialization
    for (int i = 0; i < polish.length(); i++) {
        used_by_parent[i] = false;
        coords[i] = pair<int, int>(0, 0);
        char x = polish.c_str()[i];
        if (x != '|' && x != '-') {
            sizes_by_loc[i] = (*sizes)[int(x - '0')];
        } else {
            // init the spaces for the operators to empty vectors
            sizes_by_loc[i] = vector<pair<int, int>>();
        }
    }

    int err_code = bottom_up_recursive(0);

    int head = polish.length() - 1;

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

    return &sizes_by_loc;
}

int sizer::bottom_up_recursive(int start) {
    char h = polish.c_str()[start + 2];

    if (h == '|') {
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

    } else if (h == '-') {
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

    if (start + 3 >= polish.length()) {
        // we've reached the end of the string since we're looking 2 ahead for
        // all of them
        return 0;
    } else {
        bottom_up_recursive(start + 1);
        return 0;
    }
}

int sizer::top_down_recursive(int node, int shape) {
    char n = polish.c_str()[node];
    if (n == '|' || n == '-') {
        // grab ij values
        pair<int, int> ij = ij_pairs[node][shape];
        int child_1 = children[node].first;
        int child_2 = children[node].second;
        pair<int, int> child_1_dims = sizes_by_loc[child_1][ij.first];
        pair<int, int> child_2_dims = sizes_by_loc[child_2][ij.second];

        if (n == '|') {
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