#include "face.h"

Face::Face()
{
}

Face :: ~Face() {

}

QList<Line> Face :: getEdges() {
    return edges;
}

void Face :: addEdge(Line &l) {
    if(!edges.contains(l)) {
        edges.append(l);
    }
}

int Face :: getIndex() {
    return index;
}

void Face :: setIndex(int ind) {
    index = ind;
}

void Face :: clearEdges() {
    edges.clear();
}
