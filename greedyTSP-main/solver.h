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

    bool operator < (const Solution &other) const;
    bool operator > (const Solution &other) const;
    bool operator == (const Solution &other) const;
    void operator=(const Solution &other);
    void addVertex(int vertex, int position, const Instance &instance);
    void printSolution();
    void printSolutionToFile(const std::string &fileName);

};

class Solver {
    class Config {
    public:
        long long timeLimitInSeconds;
        std::string algorithmName;
        Config() {
            timeLimitInSeconds = 100000;
            algorithmName = "ILS";
        }
        Config(long long _timeLimitInSeconds, std::string _algorithmName) {
            timeLimitInSeconds = _timeLimitInSeconds;
            algorithmName = _algorithmName;
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

    //Main solving functions
    Solution solve();
    Solution ILS(TimePoint &endTime);
    
    Solution generateInitialSolution();
    void localsearch(Solution &currentSolution);
    Solution perturbation(const Solution &currentSolution);
    Solution acceptanceCriterion(const Solution &currentSolution, const Solution &newSolution);

    // local search methods
    void localsearch_swap(Solution &currentSolution);
    void localsearch_2opt(const Solution &currentSolution);

    //algorithms
    Solution NearestInsertion();

};

#endif