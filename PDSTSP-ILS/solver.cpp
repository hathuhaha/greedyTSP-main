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

void Solver::setConfig_muatationRate(double _mutationRate) {
    config.mutationRate = _mutationRate;
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
    int maxIterations = 1000;
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
    Solution &currentSolution = initialSolution;
    localsearch(currentSolution);
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

        //swap two vertices
        int prev_x = (x == 0) ? afterChangedSolution.tour.back() : afterChangedSolution.tour[x - 1];
        int next_x = (x == afterChangedSolution.tour.size() - 1) ? afterChangedSolution.tour[0] : afterChangedSolution.tour[x + 1];
        int prev_y = (y == 0) ? afterChangedSolution.tour.back() : afterChangedSolution.tour[y - 1];
        int next_y = (y == afterChangedSolution.tour.size() - 1) ? afterChangedSolution.tour[0] : afterChangedSolution.tour[y + 1];

        double delta = -((prev_x == afterChangedSolution.tour[y]) ? 0.0 : instance.getDistance(prev_x, afterChangedSolution.tour[x]))
                       -((next_x == afterChangedSolution.tour[y]) ? 0.0 : instance.getDistance(next_x, afterChangedSolution.tour[x]))
                       -((prev_y == afterChangedSolution.tour[x]) ? 0.0 : instance.getDistance(prev_y, afterChangedSolution.tour[y]))
                       -((next_y == afterChangedSolution.tour[x]) ? 0.0 : instance.getDistance(next_y, afterChangedSolution.tour[y]))
                       +((prev_x == afterChangedSolution.tour[y]) ? 0.0 : instance.getDistance(prev_x, afterChangedSolution.tour[y]))
                       +((next_x == afterChangedSolution.tour[y]) ? 0.0 : instance.getDistance(next_x, afterChangedSolution.tour[y]))
                       +((prev_y == afterChangedSolution.tour[x]) ? 0.0 : instance.getDistance(prev_y, afterChangedSolution.tour[x]))
                       +((next_y == afterChangedSolution.tour[x]) ? 0.0 : instance.getDistance(next_y, afterChangedSolution.tour[x]));

        //change solution
        std::swap(afterChangedSolution.tour[x], afterChangedSolution.tour[y]);
        afterChangedSolution.totalLength += delta;
    }
}

void Solver::acceptanceCriterion(Solution &currentSolution, Solution &newSolution) {
    
}

//local search methods
void Solver::localsearch_swap(Solution &currentSolution) {

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

            }
        }

    }
}

void Solver::localsearch_2opt(Solution &currentSolution) {
    std::ofstream printlog("debug.log");

    bool stop = false;
    while(!stop) {
        stop = true;
        for(int i = 0; i < currentSolution.tour.size() - 1 && stop; i++) {
            for(int j = i + 1; j < currentSolution.tour.size() && stop; j++) {
                double delta = 0.0;
                if(i == 0 && j == currentSolution.tour.size() - 1) {
                    continue;
                }

                int prev_i = (i == 0) ? currentSolution.tour.back() : currentSolution.tour[i - 1];
                int next_j = (j == currentSolution.tour.size() - 1) ? currentSolution.tour[0] : currentSolution.tour[j + 1];

                delta -= instance.getDistance(prev_i, currentSolution.tour[i]);
                delta -= instance.getDistance(next_j, currentSolution.tour[j]);
                delta += instance.getDistance(prev_i, currentSolution.tour[j]);
                delta += instance.getDistance(next_j, currentSolution.tour[i]);


                if(delta < -eps) {
                    stop = false;
                    currentSolution.totalLength += delta;
                    currentSolution.reverseTour(i, j);
                }
                printlog << delta << '\n';
            }
        }
    }
    printlog.close();

}
void Solver::localsearch_relocate(Solution &currentSolution) {
    bool stop = false;
    while(!stop) {
        stop = true;
        for(int i = 0; i < currentSolution.tour.size(); i++) {
            for(int j = 0; j < currentSolution.tour.size(); j++) {
                if(i == j) {
                    continue;
                }

                double delta = 0.0;
                int tour_i = currentSolution.tour[i];

                int prev_i = (i == 0) ? currentSolution.tour.back() : currentSolution.tour[i - 1];
                int next_i = (i == currentSolution.tour.size() - 1) ? currentSolution.tour[0] : currentSolution.tour[i + 1];

                delta -= instance.getDistance(prev_i, tour_i);
                delta -= instance.getDistance(next_i, tour_i);
                delta += instance.getDistance(prev_i, next_i);

                int id_prev_j = (j == 0) ? currentSolution.tour.size() - 1 : (j - 1);
                if(id_prev_j == i) {
                    id_prev_j = (id_prev_j == 0) ? currentSolution.tour.size() - 1 : (id_prev_j - 1);
                }
                int prev_j = currentSolution.tour[id_prev_j];
                delta += instance.getDistance(prev_j, currentSolution.tour[i]);
                delta += instance.getDistance(currentSolution.tour[i], currentSolution.tour[j]);
                delta -= instance.getDistance(prev_j, currentSolution.tour[j]);

                if(delta < -eps) {
                    currentSolution.tour.erase(currentSolution.tour.begin() + i);
                    int er = 0;
                    if(j > i)
                        er += 1;
                    currentSolution.tour.insert(currentSolution.tour.begin() + j - er, tour_i);
                    currentSolution.totalLength += delta;
                    stop = false;
                }
                
            }
        }
    }
}
//construct new solution
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

