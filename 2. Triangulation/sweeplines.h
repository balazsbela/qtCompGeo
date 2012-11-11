#ifndef SWEEPLINES_H
#define SWEEPLINES_H

#include <QQueue>
#include "line.h"
#include "point.h"


class SweepLines
{
public:
    SweepLines();
    SweepLines(QString& filename);
    ~SweepLines();

    QList<Line> getSegments();
    QList<Point> getIntersections();
    void addSegment(Line& l);
    void findIntersections();

private:
    QList<Point> eventQueue;
    QList<Point> intersections;
    QList<Line> segments;
    QList<Line> status;
    QList<Line> U;
    QList<Line> L;
    QList<Line> C;

    void readLines(QString& filename);
    void fillQueue();
    void addSegmentToStatus(Line& l);
    void handleEventPoint(Point& p);
    bool isInteriorPoint(Line& line,Point& p);
    void reportIntersection(Point& p);
    void findNewEvent(Line &l1,Line &l2,Point &p);
    void printQueue();
    void printStatus();
    void printLineList(QList<Line>&);        

};

#endif // SWEEPLINES_H
