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
double Instance::getDistance(int from, int to) const {
    return distance[from][to];
}

//Function to prepare the distance matrix for all vertex pairs
void Instance::prepareDistanceMatrix() {
    //wait for data
}

//Function to read instance data from a file
void Instance::readFromFile(const std::string &fileName) {
    std::ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    inputFile >> cntCustomers;
    vertices.resize(cntCustomers);
    tDistance.resize(cntCustomers, std::vector<double>(cntCustomers, 0.0));
    dDistance.resize(cntCustomers, std::vector<double>(cntCustomers, 0.0));

    for (int i = 0; i < cntCustomers; ++i) {
        int id;
        double x, y;
        inputFile >> id >> x >> y;
        customers[i] = Customer(id, x, y);
    }
    inputFile.close();
    //prepareDistanceMatrix();

}







