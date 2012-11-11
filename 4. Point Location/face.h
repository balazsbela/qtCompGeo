#ifndef FACE_H
#define FACE_H

#include "line.h"
#include <QList>


class Face
{
public:
    Face();
    ~Face();

    QList<Line> getEdges();
    void addEdge(Line &l);
    int getIndex();
    void setIndex(int ind);
    void clearEdges();
private:
    QList<Line> edges;
    int index;

};

#endif // FACE_H
