#include "input.h"

//Default constructor for Instance class
Instance::Instance() {
    cntVertices = 0;
    vertices = std::vector<Vertex>();
    distance = std::vector<double>();
}

//Parameterized constructor for Instance class
Instance::Instance(const int &_cntVertices, const std::vector<Vertex> &_vertices) {
    cntVertices = _cntVertices;
    vertices = _vertices;
    distance = std::vector<double>(cntVertices * cntVertices, 0.0);
}

//Function to get distance between two vertices
double Instance::getDistance(const int &from, const int &to) {
    if(from == to)
        return 0.0;
    return getEuclideDistance(vertices[from], vertices[to]);
}

//Function to prepare the distance matrix for all vertex pairs
void Instance::prepareDistanceMatrix() {
    for (int i = 0; i < cntVertices; ++i) {
        for (int j = i; j < cntVertices; ++j) {
            distance[j][i] = distance[i][j] = getDistance(i, j);
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

    for (int i = 0; i < cntVertices; ++i) {
        int id;
        double x, y;
        inputFile >> id >> x >> y;
        vertices[i] = Vertex(id, x, y);
    }

    inputFile.close();
    prepareDistanceMatrix();
}







