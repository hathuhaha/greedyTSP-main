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
    Solution();
    Solution(const std::vector<int> &_tour, double _totalLength);

    bool operator < (const Solution &other) const;
    bool operator > (const Solution &other) const;
    bool operator == (const Solution &other) const;
    void operator=(const Solution &other);
    
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

    //Constructors
    solver();
    solver(const Instance &_instance);

    //Set configuration parameters
    void setAllConfig(long long _timeLimitInSeconds, std::string _algorithmName);
    void setConfig_timeLimitInSeconds(long long _timeLimitInSeconds);
    void setConfig_algorithm(std::string _algorithmName);

    //Main solving functions
    Solution solve();
    
    Solution generateInitialSolution();
    Solution localSearch_hillClimbing(const Solution &initialSolution);
    Solution perturbation(const Solution &currentSolution);
    Solution acceptanceCriterion(const Solution &currentSolution, const Solution &newSolution);

    

};

#endif