#ifndef HELPERS_H
#define HELPERS_H

#include "common.h"
#include "parameters.h"
#include "input.h"

double getEuclideDistance(const Customer &a, const Customer &b); 
double getManhattanDistance(const Customer &a, const Customer &b);
double calculateTourCost(const std::vector<int> &tTour, const std::vector<int> &dTour, const Instance &instance);
double randomDouble(double from, double to);
double randomInt(int from, int to);
std::pair<int, int> randomDistinctPair(int from, int to);
#endif