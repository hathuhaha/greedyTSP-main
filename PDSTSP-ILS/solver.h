#ifndef SOLVER_H
#define SOLVER_H

#include "common.h"
#include "parameters.h"
#include "input.h"
#include "helpers.h"


class Solution {
public: 
    std::vector<int> tTour; 
    std::vector<int> dTour;
    double tTour_cost, dTour_cost;
    double total_cost;
    Solution();

    //define operators
    bool operator < (const Solution &other) const;
    bool operator > (const Solution &other) const;
    void operator=(const Solution &other);

    //define helper functions

    //print functions
    // void printSolution();
    // void printSolutionToFile(const std::string &fileName);
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
    Solver();
    Solver(const Instance &_instance);

    //Set configuration parameters
    void setConfig_timeLimitInSeconds(long long _timeLimitInSeconds);
    void setConfig_algorithm(std::string _algorithmName); 
    void setConfig_muatationRate(double _mutationRate); 

    //helpers
    void addCustomertoTruck(Solution &solution, int customer, int position);
    double delta_moveDroneToTruck(Solution &solution, int id, int position);
    double delta_reverseTTour(Solution &solution, int from, int to);
    double delta_moveTruckToDrone(Solution &solution, int id);
    void moveDroneToTruck(Solution &solution, int id, int position); 
    void moveTruckToDrone(Solution &solution, int id); 
    void reverseTTour(Solution &solution, int from, int to); //

    //Main solving functions
    Solution solve(); //
    Solution ILS(TimePoint &endTime); //
    
    Solution generateInitialSolution(); //
    void localsearch(Solution &currentSolution); //
    void perturbation(Solution &currentSolution, Solution &afterChangedSolution); //
    void acceptanceCriterion(Solution &currentSolution, Solution &newSolution, double &coolingRate, double &threshold); //

    // local search methods
    void localsearch_swap(Solution &currentSolution); //
    void localsearch_2opt(Solution &currentSolution); //
    void localsearch_relocate(Solution &currentSolution); //

    //algorithms
    Solution NearestInsertion(); //



};

#endif