#include "solver.h"

//Solution class implementations

Solution::Solution() {
    tTour = std::vector<int>();
    dTour = std::vector<int>();
    tTour_cost = 0;
    dTour_cost = 0;
    total_cost = 0;
}

bool Solution::operator<(const Solution &other) const {
    return (total_cost - other.total_cost) < eps;
}

bool Solution::operator > (const Solution &other) const {
    return (total_cost - other.total_cost) > eps;
}

void Solution::operator=(const Solution &other) {
    tTour = other.tTour;
    dTour = other.dTour;
    tTour_cost = other.tTour_cost;
    dTour_cost = other.dTour_cost;
    total_cost = other.total_cost;
}


void Solution::reverseTTour(int from, int to) {
    if(to < from || to >= tour.size()) {
        std::cout << "Cannot reverse due to index error!";
        return;
    }
    double Delta = 0;

    while(from < to) {
        std::swap(TTour[from], TTour[to]);
        from += 1; 
        to -= 1;
    }
}

Solver::Solver() {
    config = Config();
    instance = Instance();
    runtime = 0;
}

Solver::Solver(const Instance &_instance) {
    config = Config();
    instance = Instance();
    runtime = 0;
}

// Set configuration parameters
void Solver::setConfig_timeLimitInSeconds(long long _timeLimitInSeconds) {
    config.timeLimitInSeconds = _timeLimitInSeconds;
}

void Solver::setConfig_algorithm(std::string _algorithmName) {
    config.algorithmName = _algorithmName;
}

void Solver::setConfig_muatationRate(double _mutationRate) {
    config.mutationRate = _mutationRate;
}


void Solver::addCustomertoTruck(Solution &solution, int customer, int position)
{
    solution.tTour.insert(solution.tTour.begin() + position, customer);
    if(solution.tTour.size() > 1) {
        int next_cus = (position == solution.tTour.size() - 1) ? solution.tTour[0] : solution.tTour[position + 1];
        int prev_cus = (position == 0) ? solution.tTour.back() : solution.tTour[position - 1];
        solution.tTour_cost += instance.getTDistance();
    }
}

double Solver::delta_moveDroneToTruck(Solution &solution, int id, int position) {
    double delta = 0.0;
    delta -= instance.getDDistance(0, solution.dtour[id]) * 2;

    if(solution.tTour.size() == 0) {
        return delta;
    }

    int next_id = (position == solution.tTour.size()) ? solution.tTour[0] : solution.tTour.[position];
    int prev_id = (position == 0) ? solution.tTour.back() : solution.tTour[position - 1];

    delta -= instance.getTDistance(next_id, prev_id);
    delta += instance.getTDistance(solution.dtour[id], next_id);
    delta += instance.getTDistance(solution.dTour[id], prev_id);

    return delta;
}

double Solver::delta_reverseTTour(Solution &solution, int from, int to) {
    double delta = 0.0;
    if(from > to)
        std::swap(from, to);
    if(from == 0 && to == solution.tTour.size() - 1)
        std::swap(from, to)
    int prev_from = (from == 0) ? solution.tTour.back() : solution.tTour[from - 1];
    int next_to = (to == solution.tTour.size() - 1) ? solution.tTour[0] : solution.tTour[to + 1];

    delta += instance.getTDistance()

    return delta;
}

double Solver::delta_moveTruckToDrone(Solution &solution, int id) {
    double delta = 0.0;
    if(solution.tTour.size() > 1) {
        int next_id = (id == solution.tTour.size() - 1) ? solution.tTour[0] : solution.tTour[id + 1];
        int prev_id = (id == 0) ? solution.tTour.back() : solution.tTour[id - 1];

        delta -= (instance.getTDistance(next_id, solution.tTour[id]) + instance.getTDistance(prev_id, solution.tTour[id]));
        delta += instance.getTDistance(next_id, prev_id);
    }
    delta += instance.getDDistance(0, solution.tTour[id]) * 2;
    return delta;
}
void Solver::moveDroneToTruck(Solution &solution, int id, int position)
{
    double delta = delta_moveDroneToTruck(solution, id);
    solution.total_cost += delta;
    solution.tTour.insert(solution.tTour.begin() + position, dTour[id]);
    solution.dTour.erase(solution.dTour.begin() + id);
}
void Solver::moveTruckToDrone(Solution &solution, int id) {
    double delta = delta_moveTruckToDrone(solution, id);
    solution.total_cost += delta;
    solution.dTour.push_back(solution.tTour[id]);
    solution.tTour.erase(solution.tTour.begin() + id);
}
void Solver::reverseTTour(Solution &solution, int from, int to) {
    if(from > to)
        swap(from, to);
    double delta = 0;

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

void Solver::acceptanceCriterion(Solution &currentSolution, Solution &newSolution, double &coolingRate, double &threshold) {
    if(newSolution)
    threshold *= coolingRate;
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
