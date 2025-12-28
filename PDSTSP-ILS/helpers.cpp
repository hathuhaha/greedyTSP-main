#include "helpers.h"

// Function to calculate Euclidean distance between two vertices
double getEuclideDistance(const Vertex &a, const Vertex &b){
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}
// Function to calculate Manhattan distance between two vertices
double getManhattanDistance(const Vertex &a, const Vertex &b){
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}
// Function to calculate the total cost of a given tour
double calculateTourCost(const std::vector<int> &tour, const Instance &instance) {
    double totalCost = 0.0;
    int n = tour.size();
    for (int i = 0; i < n; ++i) {
        int from = tour[i];
        int to = tour[(i + 1) % n]; // wrap around to the start
        totalCost += instance.getDistance(from, to);
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
