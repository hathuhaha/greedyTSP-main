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
    Solution() {
        tour = std::vector<int>();
        totalLength = 0.0;
    }
    Solution(const std::vector<int> &_tour, double _totalLength) {
        tour = _tour;
        totalLength = _totalLength;
    }
    //define operators
    bool operator < (const Solution &other) const;
    bool operator > (const Solution &other) const;
    bool operator == (const Solution &other) const;
    void operator=(const Solution &other);

    //define helper functions
    void addVertex(int vertex, int position, const Instance &instance);
    void reverseTour(int from, int to);

    //print functions
    void printSolution();
    void printSolutionToFile(const std::string &fileName);
};

class Solver {
    class Config {
    public:
        long long timeLimitInSeconds;
        std::string algorithmName;
        double mutationRate;
        Config() {
            timeLimitInSeconds = 100000;
            algorithmName = "ILS";
            mutationRate = randomDouble(0.15, 0.3);
        }
    };
public: 
    Instance instance;
    Config config;
    long long runtime;

    //Constructors
    Solver() {
        instance = Instance();
        config = Config();
    }
    Solver(const Instance &_instance) {
        instance = _instance;
        config = Config();
    }

    //Set configuration parameters
    void setConfig_timeLimitInSeconds(long long _timeLimitInSeconds);
    void setConfig_algorithm(std::string _algorithmName);
    void setConfig_muatationRate(double _mutationRate);

    //Main solving functions
    Solution solve();
    Solution ILS(TimePoint &endTime);
    
    Solution generateInitialSolution();
    void localsearch(Solution &currentSolution);
    void perturbation(Solution &currentSolution, Solution &afterChangedSolution);
    void acceptanceCriterion(Solution &currentSolution, Solution &newSolution);

    // local search methods
    void localsearch_swap(Solution &currentSolution);
    void localsearch_2opt(Solution &currentSolution);
    void localsearch_relocate(Solution &currentSolution);

    //algorithms
    Solution NearestInsertion();


    //helpers
};

#endif