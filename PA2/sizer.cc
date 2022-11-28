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