#include "parameters.h"

Vertex::Vertex() {
    id = -1;
    x = 0.0;
    y = 0.0;
}

Vertex::Vertex(int _id, double _x, double _y) {
    id = _id;
    x = _x;
    y = _y;
}

