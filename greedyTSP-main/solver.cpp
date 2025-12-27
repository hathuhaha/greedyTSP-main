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

void Solution::printSolution() {
    std::cout << "Tour: ";
    for(int v : tour) {
        std::cout << v << " ";
    }
    std::cout << "\nTotal Length: " << totalLength << "\n";
}

void Solution::printSolutionToFile(const std::string &fileName) {
    std::ofstream outFile(fileName);
    if(!outFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << "\n";
        return;
    }
    outFile << "Tour: ";
    for(int v : tour) {
        outFile << v << " ";
    }
    outFile << "\nTotal Length: " << totalLength << "\n";
    outFile.close();
}

//Set configuration parameters
void Solver::setConfig_timeLimitInSeconds(long long _timeLimitInSeconds) {
    config.timeLimitInSeconds = _timeLimitInSeconds;
}

void Solver::setConfig_algorithm(std::string _algorithmName) {
    config.algorithmName = _algorithmName;
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

    // int maxIterations = 1000;
    // for(int i = 0; i < maxIterations; i++) {
    //     auto currentTime = std::chrono::high_resolution_clock::now();
    //     if(currentTime >= endTime) {
    //         break;
    //     }
        
    //     Solution newSolution = perturbation(currentSolution);
    //     newSolution = localSearch_hillClimbing(newSolution);
    //     currentSolution = acceptanceCriterion(currentSolution, newSolution);
        
    //     if(currentSolution.totalLength < bestSolution.totalLength) {
    //         bestSolution = currentSolution;
    //     }
    // }
    return bestSolution;
}
Solution Solver::generateInitialSolution() {

    Solution initialSolution = NearestInsertion();
    Solution &currentSolution = initialSolution;
    localsearch(currentSolution);
    return currentSolution;
}

void Solver::localsearch(Solution &currentSolution) {
    localsearch_swap(currentSolution);
}

Solution Solver::perturbation(const Solution &currentSolution) {
    return Solution();
}

Solution Solver::acceptanceCriterion(const Solution &currentSolution, const Solution &newSolution) {

    return Solution();
}

//local search methods
void Solver::localsearch_swap(Solution &currentSolution) {

    std::ofstream printlog("C:/Users/ORLab/OneDrive/Documents/Lab/greedyTSP-main/greedyTSP-main/debug.log");

    bool stop = false;
    while(!stop) {
        stop = true;
        for(int i = 0; i < currentSolution.tour.size() - 1 && stop; i++) {

            int prev_i = (i == 0) ? currentSolution.tour.back() : currentSolution.tour[i - 1];
            int next_i = (i == currentSolution.tour.size() - 1) ? currentSolution.tour[0] : currentSolution.tour[i + 1];

            for(int j = i + 1; j < currentSolution.tour.size() && stop; j++) {
                
                int prev_j = (j == 0) ? currentSolution.tour.back() : currentSolution.tour[j - 1];
                int next_j = (j == currentSolution.tour.size() - 1) ? currentSolution.tour[0] : currentSolution.tour[j + 1];
                
                double delta = - ((prev_i != currentSolution.tour[j]) ? instance.getDistance(prev_i, currentSolution.tour[i]) : 0.0)
                               - ((next_i != currentSolution.tour[j]) ? instance.getDistance(currentSolution.tour[i], next_i) : 0.0)
                               - ((prev_j != currentSolution.tour[i]) ? instance.getDistance(prev_j, currentSolution.tour[j]) : 0.0)
                               - ((next_j != currentSolution.tour[i]) ? instance.getDistance(currentSolution.tour[j], next_j) : 0.0)
                               + instance.getDistance(prev_i, currentSolution.tour[j])
                               + instance.getDistance(currentSolution.tour[j], next_i)
                               + instance.getDistance(prev_j, currentSolution.tour[i])
                               + instance.getDistance(currentSolution.tour[i], next_j);

                if(delta < -eps) {
                    std::swap(currentSolution.tour[i], currentSolution.tour[j]);
                    currentSolution.totalLength += delta;
                    stop = false;
                }
                printlog << delta << '\n';

            }
        }

    }
    printlog.close();
}

void Solver::localsearch_2opt(const Solution &currentSolution) {
    bool stop = false;
    while(!stop) {
        for(int i = 0; i < currentSolution.tour.size() - 1; i++) {
            for(int j = i + 1; j < currentSolution.tour.size(); j++) {

            }
        }
    }
}


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
        solution.addVertex(bestVertex, bestPosition, instance);
    }
    return solution;
}
