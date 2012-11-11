#include "triangle.h"
#include <QDebug>

Triangle::Triangle() {
}

Triangle :: ~Triangle() {

}

Triangle :: Triangle(Line ab1,Line bc1,Line ca1) {
    ab = ab1;
    bc = bc1;
    ca = ca1;
}

Triangle :: Triangle(Point &a, Point &b , Point &c) {
        ab.setA(a);
        ab.setB(b);

        bc.setA(b);
        bc.setB(c);

        ca.setA(c);
        ca.setB(a);
}

bool Triangle :: contains(Point p) {


    if ( ( Point :: turn(ab.getA(), ab.getB(),p) == -1 ) &&
            ( Point :: turn(bc.getA(), bc.getB(), p) == -1 ) &&
            ( Point :: turn(ca.getA(), ca.getB(), p) == -1 ) )
        return true;
    else
        return false;
}


void Triangle :: setCircumCenter(Point& c) {
    circumCenter.setX(c.x());
    circumCenter.setY(c.y());
}

Point Triangle :: getCircumCenter() {
       return circumCenter;
}

bool Triangle :: hasEdge(Line &e) {
    if ( (e.equals(ab)) || (e.equals(bc)) || (e.equals(ca)) ) {
        return true;
    }

    return false;
}

bool Triangle :: hasVertex(Point p)   {
    if ( (ab.getA() == p) || (bc.getA() == p) || (ca.getA() == p) ) {
        return true;
    }
    return  false;
}

Point Triangle :: getPointInFront(Line &e) {
    if (e.equals(ab)) {
        return ca.getA();
    }
    if (e.equals(bc)) {
        return ab.getA();
    }

    if (e.equals(ca)) {
        return bc.getA();
    }

    qDebug() << "getPointInFront went wrong";
    return bc.getA();
}

Triangle Triangle :: makeTriangle(Point &a,Point &b,Point &c){
    Triangle t;

    Line e1,e2,e3;

    qDebug() << "Creating triangle ";
    if ( Point :: turn(a, b, c) == -1 ) {// || Point :: turn(a, b, c) == 0 ) {

        qDebug() <<"Points: " << a.x() << " " << a.y();
        qDebug() <<"Points: " << b.x() << " " << b.y();
        qDebug() <<"Points: " << c.x() << " " << c.y();

        e1.setA(a);
        e1.setB(b);

        e2.setA(b);
        e2.setB(c);

        e3.setA(c);
        e3.setB(a);

        t.ab = e1;
        t.bc = e2;
        t.ca = e3;

        return t;
    }
    else {

        //the order is c b a

        qDebug() <<"Points: " << c.x() << " " << c.y();
        qDebug() <<"Points: " << b.x() << " " << b.y();
        qDebug() <<"Points: " << a.x() << " " << a.y();


        e1.setA(c);
        e1.setB(b);

        e2.setA(b);
        e2.setB(a);

        e3.setA(a);
        e3.setB(c);

        t.ab = e1;
        t.bc = e2;
        t.ca = e3;

        return t;
    }

    qDebug() << "Something went wrong in makeTriangle";
    return t;
}

void Triangle :: print() {
    qDebug() << ab.toString() << " " << bc.toString() << " " << ca.toString();
}

bool Triangle :: adjacent(Triangle &t) {
       if ( &t == this )
           return false;

       if ( t.ab.equals(ab) || t.ab.equals(bc) || t.ab.equals(ca) )
           return true;
       if ( t.bc.equals(ab) || t.bc.equals(bc) || t.bc.equals(ca) )
           return true;
       if ( t.ca.equals(ab) || t.ca.equals(bc) || t.ca.equals(ca) )
           return true;
       return false;
}



