#include "sizer.h"

std::list<std::pair<int, int>> *sizer::vert_size(
    std::list<std::pair<int, int>> *L, std::list<std::pair<int, int>> *R) {
    // L is sorted with a_i<a_j, b_i>b_j for all i<j
    // R is sorted with x_i<x_j, y_i>y_j for all i<j

    std::list<std::pair<int, int>> H = std::list<std::pair<int, int>>();
    auto i = L->begin();
    auto j = R->begin();
    while (i != L->end() && j != R->end()) {
        std::pair<int, int> out = std::pair<int, int>(
            (*i).first + (*j).first, std::max((*i).second, (*j).second));
        H.push_back(out);
        if (std::max((*i).second, (*j).second) == (*i).second) {
            i++;
        }
        if (std::max((*i).second, (*j).second) == (*j).second) {
            j++;
        }
    }

    return &H;
}

std::list<std::pair<int, int>> *sizer::hor_size(
    std::list<std::pair<int, int>> *U, std::list<std::pair<int, int>> *L) {
    // everything stays the same except you traverse from back to front and
    // change the dimension used to check for dominance

    std::list<std::pair<int, int>> H = std::list<std::pair<int, int>>();
    auto i = U->rbegin();
    auto j = L->rbegin();
    while (i != U->rend() && j != L->rend()) {
        std::pair<int, int> out = std::pair<int, int>(
            std::max((*i).first, (*j).first), (*i).second + (*j).second);
        H.push_back(out);
        if (std::max((*i).first, (*j).first) == (*i).first) {
            i++;
        }
        if (std::max((*i).first, (*j).first) == (*j).first) {
            j++;
        }
    }

    return &H;
}

// std::map<int, std::pair<int, int>> *sizer::do_sizing(
//     std::map<int, std::list<std::pair<int, int>>> *sizes, std::string polish)
//     {
// }

std::map<int, std::list<std::pair<int, int>>> *sizer::bottom_up_recursive(
    std::map<int, std::list<std::pair<int, int>>> *sizes, std::string polish) {
    // this whole thing will break if it's not a normal expression
    //  assumes at least 3 chars for valid polish expression
    char top = polish.c_str()[polish.length() - 1];
    char j = polish.c_str()[polish.length() - 2];
    char i;
    // TODO: these need to be lists of pairs cause we haven't chosen the best
    // one yet use a global var to store all the shapes and change them as you
    // go
    // need to also store the shapes for the | and -
    std::list<std::pair<int, int>> *j_shapes;
    std::list<std::pair<int, int>> *i_shapes;

    if (j == '|' || j == '-') {
        // recursively call for the substring
        // there will always be 2 numbers in a row to terminate the branch

        int begin;
        for (int it = polish.length() - 3; it > 0; it--) {
            // we find 2 numbers in a row
            if ((polish.c_str()[it] != '|' || polish.c_str()[it] != '-') &&
                (polish.c_str()[it - 1] != '|' ||
                 polish.c_str()[it - 1] != '-')) {
                begin = it - 1;
                break;
            }
        }
        j_shapes = bottom_up_recursive(
            sizes, polish.substr(begin, polish.length() - 1));
        i = polish.c_str()[begin - 1];
        if (i == '|' || i == '-') {
            i_shapes = bottom_up_recursive(sizes, polish.substr(0, begin - 1));
        } else {
            i_shapes = sizes->[int(i - 0)];
        }
    } else {
        char i = polish.c_str()[polish.length() - 3];
        if (i == '|' || i == '-') {
            i_shapes = bottom_up_recursive(
                sizes, polish.substr(0, polish.length() - 3));
        } else {
            i_shapes = sizes->[int(i - 0)];
        }
    }
}