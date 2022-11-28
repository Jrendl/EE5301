#pragma once
#include <stdio.h>  // needed to open files in C
#include <string.h>

#include <algorithm>
#include <fstream>   // needed to open files in C++
#include <iostream>  // basic C++ input/output (e.g., cin, cout)
#include <list>
#include <locale>
#include <map>
#include <sstream>  // needed if you are using sstream (C++)
#include <vector>

class sizer {
   private:
   public:
    sizer() {
    }
    std::list<std::pair<int, int>> *vert_size(
        std::list<std::pair<int, int>> *L, std::list<std::pair<int, int>> *R);

    std::list<std::pair<int, int>> *hor_size(std::list<std::pair<int, int>> *U,
                                             std::list<std::pair<int, int>> *L);

    std::map<int, std::pair<int, int>> *do_sizing(
        std::map<int, std::list<std::pair<int, int>>> *sizes, char *polish);
};