#include "solver.h"

//Solution class implementations
Solution::Solution() {
    tour = std::vector<int>();
    totalLength = 0.0;
}
Solution::Solution(const std::vector<int> &_tour, double _totalLength) {
    tour = _tour;
    totalLength = _totalLength;
}

bool Solution::operator < (const Solution &other) const {
    return (totalLength - other.totalLength) <= eps;
}

bool Solution::operator > (const Solution &other) const {
    return (totalLength - other.totalLength) >= eps;
}

bool Solution::operator == (const Solution &other) const {
    return abs(totalLength - other.totalLength) <= eps && tour == other.tour;
}

void Solution::operator=(const Solution &other) {
    tour = other.tour;
    totalLength = other.totalLength;
}


//Solver class implementations
Solver::solver() {
    instance = Instance();
    config = Config();
}

Solver::solver(const Instance &_instance) {
    instance = _instance;
    config = Config();
}

//Set configuration parameters
void Solver::setAllConfig(long long _timeLimitInSeconds, std::string _algorithmName) {
    config.timeLimitInSeconds = _timeLimitInSeconds;
    config.algorithmName = _algorithmName;
}

void Solver::setConfig_timeLimitInSeconds(long long _timeLimitInSeconds) {
    config.timeLimitInSeconds = _timeLimitInSeconds;
}

void Solver::setConfig_algorithm(std::string _algorithmName) {
    config.algorithmName = _algorithmName;
}

//Main solving function
Solution Solver::solve() {
    
}
Solution Solver::generateInitialSolution() {
    
}
Solution Solver::localSearch_hillClimbing(const Solution &initialSolution) {
    
}
Solution Solver::perturbation(const Solution &currentSolution) {
    
}
Solution Solver::acceptanceCriterion(const Solution &currentSolution, const Solution &newSolution) {
}