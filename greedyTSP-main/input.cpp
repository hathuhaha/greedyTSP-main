#include "input.h"
#include "helpers.h"

//Default constructor for Instance class
Instance::Instance() {
    cntVertices = 0;
    vertices = std::vector<Vertex>();
    distance = std::vector<std::vector<double>>(cntVertices, std::vector<double>(cntVertices, 0.0));
}

//Parameterized constructor for Instance class
Instance::Instance(const int &_cntVertices, const std::vector<Vertex> &_vertices) {
    cntVertices = _cntVertices;
    vertices = _vertices;
    distance = std::vector<std::vector<double>>(cntVertices, std::vector<double>(cntVertices, 0.0));
}

//Function to get distance between two vertices
double Instance::getDistance(int from, int to) const {
    return distance[from][to];
}

//Function to prepare the distance matrix for all vertex pairs
void Instance::prepareDistanceMatrix() {
    for (int i = 0; i < cntVertices; ++i) {
        for (int j = i; j < cntVertices; ++j) {
            distance[j][i] = distance[i][j] = getEuclideDistance(vertices[i], vertices[j]);
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

    inputFile >> cntVertices;
    vertices.resize(cntVertices);
    distance.resize(cntVertices, std::vector<double>(cntVertices, 0.0));

    for (int i = 0; i < cntVertices; ++i) {
        int id;
        double x, y;
        inputFile >> id >> x >> y;
        vertices[i] = Vertex(id, x, y);
    }
    inputFile.close();

    prepareDistanceMatrix();
}







