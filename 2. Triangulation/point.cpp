#include "point.h"
#include <QDebug>
#include <QStringList>

Point::Point() : QPointF() {
    segmentIndex = -1;
}

Point::Point(QString& str) : QPointF() {
    QStringList list = str.split(" ");
    setX(list[0].toFloat());
    setY(list[1].toFloat());
    segmentIndex = -1;
}

Point :: Point(qreal x,qreal y) : QPointF(x,y) {
    segmentIndex = -1;
}

Point :: Point(const Point& orig) : QPointF(orig) {
    segmentIndex = orig.getSegmentIndex();
}

Point& Point :: operator=(const Point& orig) {
    QPointF::operator=(orig);
    segmentIndex = orig.segmentIndex;
    return *this;
}

Point :: ~Point() {

}

void Point :: setSegmentIndex(int ind) {
    segmentIndex = ind;
}

int Point :: getSegmentIndex() const {
    return segmentIndex;
}

bool Point :: operator==(const Point& orig) {
    //qDebug() << "Comparing " << this->x() << " " << this->y() <<" with "<<orig.x() << " " << orig.y();

    float A = x()-orig.x();
    float B = y()-orig.y();

    //qDebug() << "abs(A) =" << qAbs(A);
    //qDebug() << "abs(B) =" << qAbs(B);

    bool isEqual = (qAbs(A) < 0.001) && (qAbs(B) < 0.001);
    //qDebug() << isEqual;
    return isEqual;
    //return (x() == orig.x()) && (y() == orig.y());
}
