#ifndef HELPERS_H
#define HELPERS_H

#include "common.h"
#include "parameters.h"
#include "input.h"

double getEuclideDistance(const Vertex &a, const Vertex &b);
double getManhattanDistance(const Vertex &a, const Vertex &b);
double calculateTourCost(const std::vector<int> &tour, const Instance &instance);


#endif