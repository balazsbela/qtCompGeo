#ifndef SLAB_H
#define SLAB_H

#include <QList>
#include "line.h"

class Slab
{    
public:
    Slab();
    ~Slab();

    QList<Line> getSegments();
    void addSegment(Line& l);
    void clearSegments();

    void setLeft(Point& p);
    Point getLeft();
    void setRight(Point& p);
    Point getRight();
    void sortSegments();

private:
    Point left;
    Point right;
    QList<Line> segments;
};

#endif // SLAB_H
