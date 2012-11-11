#ifndef POINT_H
#define POINT_H

#include <QPointF>

class Point : public QPointF
{

public:
    Point();
    Point(QString& str);
    Point(qreal x,qreal y);
    Point(const Point& orig);
    ~Point();

    void setSegmentIndex(int ind);
    int getSegmentIndex() const;
    Point& operator=(const Point& orig);
    bool operator==(const Point& orig);
    static int turn(Point a,Point b,Point c);
private:
    int segmentIndex;

};

#endif // POINT_H
