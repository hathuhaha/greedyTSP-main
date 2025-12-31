#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include "parameters.h"

class Customer {
public:
    int id;
    double x;
    double y;
    Customer();
    Customer(int _id, double _x, double _y);
};

class Instance {

public: 
    int cntCustomers;
    std::vector<Customer> customers;
    std::vector<std::vector<double>> tDistance; //distance for truck
    std::vector<std::vector<double>> dDistance; // distance for drones
    Instance();

    void readFromFile(const std::string &fileName);
    void prepareDistanceMatrix();
    double getTDistance(int from, int to) const;
    double getDDistance(int from, int to) const;
    void operator=(const Instance &other) {
        cntCustomers = other.cntCustomers;
        customers = other.customers;
        tDistance = other.tDistance;
        dDistance = other.dDistance;
    }
};

#endif