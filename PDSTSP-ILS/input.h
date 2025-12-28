#ifndef INPUT_H
#define INPUT_H

#include "common.h"
#include "parameters.h"

class Instance {

public: 
    int cntVertices;
    std::vector<Vertex> vertices;
    std::vector<std::vector<double>> distance;
    Instance();
    Instance(const int &_cntVertices, const std::vector<Vertex> &_vertices); 
    void readFromFile(const std::string &fileName);
    void prepareDistanceMatrix();
    double getDistance(int from, int to) const;
    void operator=(const Instance &other) {
        cntVertices = other.cntVertices;
        vertices = other.vertices;
        distance = other.distance;
    }
};

#endif