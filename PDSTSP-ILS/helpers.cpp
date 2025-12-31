#include "helpers.h"

// Function to calculate Euclidean distance between two vertices
double getEuclideDistance(const Customer &a, const Customer &b){
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
// Function to calculate Manhattan distance between two vertices
double getManhattanDistance(const Customer &a, const Customer &b){
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
// Function to calculate the total cost of a given tour
double calculateTourCost(const std::vector<int> &tTour, const std::vector<int> &dTour, const Instance &instance) {
    double totalCost = 0.0;
    for (int i = 0; i < tTour.size(); i++) {
        int from = tTour[i];
        int to = tTour[(i + 1) % int(tTour.size())]; // wrap around to the start
        totalCost += instance.getTDistance(from, to);
    }

    for (int i = 0; i < dTour.size() - 1; i++) {
        int from = dTour[i];
        int to = dTour[i];
        totalCost += instance.getDDistance(from, to);
    }
    
    return totalCost;
}
double randomDouble(double from, double to) {

    double rate = (double)rand() / RAND_MAX;
    return from + rate * (to - from);
}
double randomInt(int from, int to) {
    return from + rand() % (to - from + 1);
}

std::pair<int, int> randomDistinctPair(int from, int to) {
    int v1 = randomInt(from, to);
    int v2 = randomInt(from, to);
    if(v1 != v2)
        return std::make_pair(v1, v2);
    return randomDistinctPair(from, to);
}
