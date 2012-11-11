#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "line.h"
#include "point.h"

class Triangle
{
public:
    Triangle();
    ~Triangle();
    Triangle(Line ab1,Line bc1,Line ca1);
    Triangle(Point &a, Point &b , Point &c);

    bool contains(Point p);
    void setCircumCenter(Point& c);
    Point getCircumCenter();
    bool hasEdge(Line &e);
    Point getPointInFront(Line &e);
    bool hasVertex(Point p);
    static Triangle makeTriangle(Point &a,Point &b,Point &c);
    void print();
    bool adjacent(Triangle &t);

    Line ab,bc,ca;

private:
    Point circumCenter;

};

#endif // TRIANGLE_H
