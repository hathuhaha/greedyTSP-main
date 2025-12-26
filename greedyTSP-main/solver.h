#ifndef SOLVER_H
#define SOLVER_H

#include "common.h"
#include "parameters.h"
#include "input.h"
#include "helpers.h"


class Solution {
public: 
    std::vector<int> tour; 
    double totalLength;
    Solution();
    Solution(const std::vector<int> &_tour, const double &_totalLength);
}

class solver {
public: 
    Instance instance;
};

#endif