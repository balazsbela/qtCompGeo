#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include <QQueue>
#include "line.h"
#include "point.h"


class Triangulation
{
public:
    Triangulation();
    Triangulation(QString& filename);
    ~Triangulation();

    QList<Line> getEdges();
    QList<Point> getVertices();
    void triangulate();
    QList<Line> getDiagonals();

private:
    QList<Line> edges;
    QList<Point> vertices;
    QList<Line> diagonals;

    void readLines(QString& filename);
    bool diagonal(Point &p1,Point &p2);
    bool diagonal_IE(Point &p1,Point &p2);
    bool interior(Point& p1,Point& p2);
    void reportDiagonal(Point& p1,Point& p2);
    bool intersects(Line& l1,Line& l2);
    int turn(Point& a,Point& b,Point& c);
    int findVertex(Point& p);

};

#endif // SWEEPLINES_H
