#ifndef LINE_H
#define LINE_H

#include "point.h"

class Line
{
public:   
    Line();
    Line(QString& str);
    ~Line();

    void setA(Point&);
    void setB(Point&);
    Point getA() const;
    Point getB() const;
    void setReversed(bool);
    bool getReversed() const;
    void setId(int);
    int getId() const;
    bool operator==(const Line& l) const;
    bool contains(Point&) const;
    QString toString();
    Point* getIntersection(Line& l,bool &valid);

private:
    Point A;
    Point B;
    int id;
    bool isReversed;

};

#endif // LINE_H
