#ifndef POINTLOCATION_H
#define POINTLOCATION_H

#include <QQueue>
#include "line.h"
#include "point.h"
#include "face.h"
#include "slab.h"


class PointLocation
{
public:
    PointLocation();
    PointLocation(QString& filename);
    ~PointLocation();

    QList<Face> getFaces();
    Point getPoint();
    Face getFace();
    QList<Slab> getSlabs();

private:

    QList<Face> faces;
    Point* p;
    QList<Point> points;
    QList<Slab> slabs;

    void readFaces(QString& filename);
    void generateSlabs();
    void sortPoints();
    void fillSlabSegments(Slab& s);
    Point* intersects(Line& l1,Line& l2);
    int determineRegion(Slab& s);
    bool isOnLine(Point &p,Line &line);
    int turn(Point& a,Point& b,Point& c);
    QList<Face>* searchFaceByEdge(QList<Face> faceList,Line& myLine);
};

#endif
