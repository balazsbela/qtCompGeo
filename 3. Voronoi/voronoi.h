#ifndef VORONOI_H
#define VORONOI_H

#include "point.h"
#include <QString>
#include <QList>
#include "line.h"
#include "triangle.h"

class Voronoi
{
public:
    Voronoi(QString fileName);
    ~Voronoi();
    void readPoints();
    void getTriangulation();
    QList<Triangle> getTriangles();
    QList<Point> getPoints();
    void addCircumcenters();
    void populateVoronoi();
    QList<Line> getVoronoi();
private:
    QString fileName;
    QList<Point> points;
    QList<Triangle> triangles;
    QList<Line> voronoi;
    //QList<Point> sortedPoints;

    Line ext1,ext2,ext3;
    Point extr1,extr2,extr3;

    Point findVertexOnTheRight(Line &e,Point &p,bool &valid);
    void swapTest(Line &e,Point &p);
    void insert(Point& p);
    void flip(Line &e,Point &p,Point &d);
    bool inCircle(Point &a,Point &b,Point &c,Point &p);
 //   void sortPoints();
    Point getCenter(Point &a,Point &b,Point &c);
    void addToVoronoi(Triangle &t);

};
#endif // VORONOI_H
