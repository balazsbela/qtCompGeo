#include "slab.h"

Slab::Slab()
{
}

Slab :: ~Slab() {

}

QList<Line> Slab :: getSegments() {
    return segments;
}

void Slab :: addSegment(Line& l) {
    if(!segments.contains(l)) segments.append(l);
}

void Slab :: clearSegments() {
    segments.clear();
}

void Slab :: setLeft(Point& p) {
    left = p;
}

Point Slab :: getLeft() {
    return left;
}

void Slab :: setRight(Point& p) {
    right = p;
}

Point Slab :: getRight() {
    return right;
}



bool lessThanLinesY(const Line& l1,const Line& l2) {
    //use the upper point A

    //determine the upper point for both.

    Point A;
    Point B;

    if(l1.getA().y() > l1.getB().y()) {
        A = l1.getA();
    }
    else {
        A = l1.getB();
    }

    if(l2.getA().y() > l2.getB().y()) {
        B = l2.getA();
    }
    else {
        B = l2.getB();
    }

    if(A.y() == B.y()) {
        return A.x()<B.x();
    }
    else {
        return A.y() > B.y();
    }
}


void Slab :: sortSegments() {
    qSort(segments.begin(),segments.end(),lessThanLinesY);
}
