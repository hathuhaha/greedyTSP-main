#include "input.h"
#include "helpers.h"


Customer::Customer() {
    id = -1;
    x = 0.0;
    y = 0.0;
}

Customer::Customer(int _id, double _x, double _y) {
    id = _id;
    x = _x;
    y = _y;
}


//Default constructor for Instance class
Instance::Instance() {
    cntCustomers = 0;
    customers = std::vector<Vertex>();
    tDistance = std::vector<std::vector<double>>(cntCustomers, std::vector<double>(cntCustomers, 0.0));
    dDistance = std::vector<std::vector<double>>(cntCustomers, std::vector<double>(cntCustomers, 0.0));

}

//Function to get distance between two vertices
double Instance::getTDistance(int from, int to) const {
    return tDistance[from][to];
}
double Instance::getDDistance(int from, int to) const {
    return dDistance[from][to];
}

//Function to prepare the distance matrix for all vertex pairs
void Instance::prepareDistanceMatrix() {
    for(int i = 0 ; i <= cntCustomers; i++) {
        for(int j = i; j <= cntCustomers; j++) {
            tDistance[j][i] = tDistance[i][j]= getManhattanDistance(customers[i], customers[j]);
            dDistance[j][i] = dDistance[i][j] = getManhattanDistance(customer[i], customers[j]);
        }
    }

}

//Function to read instance data from a file
void Instance::readFromFile(const std::string &fileName) {
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    inputFile >> cntCustomers;
    customers.resize(cntCustomers + 1);
    tDistance.resize(cntCustomers + 1, std::vector<double>(cntCustomers + 1, 0.0));
    dDistance.resize(cntCustomers + 1, std::vector<double>(cntCustomers + 1, 0.0));

    for (int i = 0; i <= cntCustomers; ++i) {
        int id;
        double x, y;
        inputFile >> id >> x >> y;
        customers[i] = Customer(id, x, y);
    }
    inputFile.close();
    prepareDistanceMatrix();

}







