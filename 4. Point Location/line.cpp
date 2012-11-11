#include "line.h"
#include <QStringList>
#include <QString>
#include <QDebug>

Line :: Line() {
    A = Point(0.0f,0.0f);
    B = Point(0.0f,0.0f);
    id = -1;
    isReversed = 0;
}

Line :: Line(QString& str) {
    QStringList points =  str.split(" ");
    if(points.size() > 3) {
        Point A = Point(points[0].toFloat(),points[1].toFloat());
        Point B = Point(points[2].toFloat(),points[3].toFloat());

        //if B upper then swap
        if(A.y()<B.y()) {
            //qDebug() << A.x() << " " << A.y() << " " << B.x() << " " << B.y();
            //qDebug() << "Swapping:";
            setA(B);
            setB(A);
        }
        else {
            setA(A);
            setB(B);
        }
    }
}

Line :: ~Line() {

}

void Line :: setA(Point& p) {
    A = p;
}

void Line :: setB(Point& p) {
    B = p;
}


Point Line :: getA() const {
    return A;
}

Point Line :: getB() const {
    return B;
}

void Line :: setReversed(bool r) {
    isReversed = r;
}

bool Line :: getReversed() const {
    return isReversed;
}

void Line :: setId(int myid) {
    id = myid;
}

int Line :: getId() const {
    return id;
}

bool Line :: operator==(const Line& l) const {
    return ((A == l.getA()) && (B == l.getB()) && (faceIndex == l.faceIndex)) ;
}

bool Line :: equals(const Line& l) const {
    return ((A == l.getA()) && (B == l.getB())) ;
}

bool Line :: contains(Point& p) const {
    return (A == p) || (B == p);
}

QString Line :: toString() {
    return QString(QString :: number(id) + ": " + QString::number(A.x()) + " " + QString::number(A.y()) + " " + QString::number(B.x()) + " " + QString::number(B.y()));
}

Point* Line :: getIntersection(Line& l,bool &valid) {

    Point *p = new Point();
    float xa, xb, xc, xd, ya, yb, yc, yd, n, s, px, py;

    xa = getA().x();
    xb = getB().x();
    xc = l.getA().x();
    xd = l.getB().x();
    ya = getA().y();
    yb = getB().y();
    yc = l.getA().y();
    yd = l.getB().y();

    n = xa * (yd - yc) + xb * (yc - yd) + xd * (yb - ya) + xc * (ya - yb);

    if (n != 0) {
        s = xa * (yd - yc) + xc * (ya - yd) + xd * (yc - ya);
        s = s / n;
        px = xa + s * (xb - xa);
        py = ya + s * (yb - ya);
        p->setX(px);
        p->setY(py);
        valid = true;
    }
    else {
        valid = false;
    }

    return p;
}


void Line :: setFaceIndex(int fi) {
    faceIndex = fi;
}

int Line :: getFaceIndex() {
    return faceIndex;
}
