#include "solver.h"

//Solution class implementations


bool Solution::operator < (const Solution &other) const {
    return (totalLength - other.totalLength) < eps;
}

bool Solution::operator > (const Solution &other) const {
    return (totalLength - other.totalLength) > eps;
}

bool Solution::operator == (const Solution &other) const {
    return abs(totalLength - other.totalLength) < eps && tour == other.tour;
}

void Solution::operator=(const Solution &other) {
    tour = other.tour;
    totalLength = other.totalLength;
}

void Solution::addVertex(int vertex, int position, const Instance &instance) {
    if(tour.size()) {
        int prevVertex = (position == 0) ? tour.back() : tour[position - 1];
        int nextVertex = (position == tour.size()) ? tour[0] : tour[position];

        totalLength -= instance.getDistance(prevVertex, nextVertex);
        totalLength += instance.getDistance(prevVertex, vertex);
        totalLength += instance.getDistance(vertex, nextVertex);
    }
    tour.insert(tour.begin() + position, vertex);
}

void Solution::reverseTour(int from, int to) {
    if(to < from || to >= tour.size()) {
        std::cout << "Cannot reverse due to index error!";
        return;
    }
    while(from < to) {
        std::swap(tour[from], tour[to]);
        from += 1; 
        to -= 1;
    }
}


//Set configuration parameters
void Solver::setConfig_timeLimitInSeconds(long long _timeLimitInSeconds) {
    config.timeLimitInSeconds = _timeLimitInSeconds;
}

void Solver::setConfig_algorithm(std::string _algorithmName) {
    config.algorithmName = _algorithmName;
}

void Solver::setConfig_muatationRate(double _mutationRate) {
    config.mutationRate = _mutationRate;
}

void Solver::addCustomertoTruck(int customer, int position) {

}

//Main solving function
Solution Solver::solve() {
    auto startTime = std::chrono::high_resolution_clock::now();
    auto endTime = startTime + std::chrono::seconds(config.timeLimitInSeconds);

    return ILS(endTime);
}
Solution Solver::ILS(TimePoint &endTime) {

    Solution initialSolution = generateInitialSolution();
    Solution currentSolution = initialSolution;
    Solution bestSolution = initialSolution;  // Initialize with initial solution

    double threshold = 0.2; 
    double cooling_rate = 0.996;
    int cnt = 0;
    int maxIterations = 0;
    for(int i = 0; i < maxIterations; i++) {
        Solution newSolution = currentSolution;
        perturbation(currentSolution, newSolution);
        localsearch(newSolution);
        if((newSolution.totalLength - currentSolution.totalLength) / currentSolution.totalLength - threshold < -eps) {
            currentSolution = newSolution;
            threshold *= cooling_rate;

        }
        if(currentSolution.totalLength < bestSolution.totalLength) {
            bestSolution = currentSolution;
            cnt += 1;
        }
    }
    std::cerr << cnt << '\n';
    return bestSolution;
}
Solution Solver::generateInitialSolution() {

    Solution initialSolution = NearestInsertion();
    return currentSolution;
}

void Solver::localsearch(Solution &currentSolution) {
    localsearch_swap(currentSolution);
    localsearch_2opt(currentSolution);
    localsearch_relocate(currentSolution);
}

void Solver::perturbation(Solution &currentSolution, Solution &afterChangedSolution) {
    int changedPairs = config.mutationRate * int(currentSolution.tour.size() / 2);
    afterChangedSolution = currentSolution;

    for(int i = 0; i < changedPairs; i++) {
        std::pair<int, int> randPair = randomDistinctPair(0, currentSolution.tour.size() - 1);
        int x = randPair.first;
        int y = randPair.second;

    }
}

void Solver::acceptanceCriterion(Solution &currentSolution, Solution &newSolution) {
    
}

//local search methods
void Solver::localsearch_swap(Solution &currentSolution) {
}

void Solver::localsearch_2opt(Solution &currentSolution) {
    

}
void Solver::localsearch_relocate(Solution &currentSolution) {
    
}

//construct new tour for truck
Solution Solver::NearestInsertion() {
    Solution solution;
    std::vector<bool> inTour(instance.cntVertices, false);
    std::vector<double> minDistToTour(instance.cntVertices, std::numeric_limits<double>::max());

    inTour[0] = true;
    solution.addVertex(0, 0, instance);
    for(int v = 0; v < instance.cntVertices; v++) {
        if(v != 0) {
            double dist = instance.getDistance(0, v);
            minDistToTour[v] = dist;
        }
    }
    while(solution.tour.size() < instance.cntVertices) {
        int bestVertex = -1;
        double bestDist = std::numeric_limits<double>::max();
        for(int v = 0; v < instance.cntVertices; v++) {
            if(!inTour[v]) {
                if(bestDist > minDistToTour[v]) {
                    bestDist = minDistToTour[v];
                    bestVertex = v;
                }
            }
        }

        int bestPosition = -1;
        if(solution.tour.size() == 1) {
            bestPosition = 1;
        }
        double bestIncrease = std::numeric_limits<double>::max();

        for(int pos = 0; pos < solution.tour.size(); pos++) { 
            int prevVertex = (pos == 0) ? solution.tour.back() : solution.tour[pos - 1];
            int nextVertex = (pos == solution.tour.size()) ? solution.tour[0] : solution.tour[pos];
            double increase = instance.getDistance(prevVertex, bestVertex) + instance.getDistance(bestVertex, nextVertex) - instance.getDistance(prevVertex, nextVertex);
            if(increase < bestIncrease) {
                bestIncrease = increase;
                bestPosition = pos;
            }
        }
        for(int v = 0; v < instance.cntVertices; v++) {
            if(!inTour[v]) {
                double dist = instance.getDistance(v, bestVertex);
                if(minDistToTour[v] > dist) {
                    minDistToTour[v] = dist;
                }
            }
        }
        inTour[bestVertex] = true;
        solution.addCustomertoTruck(bestVertex, bestPosition);
    }
    return solution;
}
