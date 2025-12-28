#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "common.h"

class Vertex {
public:
    int id;
    double x;
    double y;
    Vertex();
    Vertex(int _id, double _x, double _y);
};

#endif