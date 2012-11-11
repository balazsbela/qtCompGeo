#include "voronoi.h"
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QList>
#include "triangle.h"
#include <math.h>


Voronoi::Voronoi(QString fn) {

    //Set external triangle
    Point a,b,c;
    a.setX(100);
    a.setY(100);

    b.setX(-100);
    b.setY(100);

    c.setX(0);
    c.setY(-100);

    points.append(a);
    points.append(b);
    points.append(c);

    Triangle t(a,b,c);
    triangles.append(t);

    ext1 = t.ab;
    ext2 = t.bc;
    ext3 = t.ca;

    extr1 = a;
    extr2 = b;
    extr3 = c;

    fileName = fn;
    readPoints();

}

Voronoi :: ~Voronoi() {

}

void Voronoi :: readPoints() {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file!\n";
        return;
    }

    QTextStream stream( &file );
    QString line;
    while (!stream.atEnd()) {
        line = stream.readLine();
        points.append(Point(line));
    }

   /* foreach(Point p,points) {
        qDebug() << p.x() << " " << p.y();
    }*/
}


void Voronoi :: insert(Point& p) {

    //Find the triangle cotaining the point
    int parentIndex = -1;
    int i=0;
    foreach(Triangle t,triangles) {
        if(t.contains(p)) {
            parentIndex = i;
        }
        i++;
    }

    if(parentIndex!=-1) {
        //Remove the triangle, split and insert 3 triangles

        Triangle parent = triangles[parentIndex];
        qDebug() << "Parent triangle is:";
        parent.print();

        triangles.removeAt(parentIndex);

     /* Point temp;

        Line pa,pb,pc;
        pa.setA(p);
        temp = parent.ab.getA();
        pa.setB(temp);

        pb.setA(p);
        temp = parent.bc.getA();
        pb.setB(temp);

        pc.setA(p);
        temp = parent.ca.getA();
        pc.setB(temp);

        Line ap,bp,cp;
        temp = parent.ab.getA();
        ap.setA(temp);
        ap.setB(p);

        temp = parent.bc.getA();
        bp.setA(temp);
        bp.setB(p);

        temp = parent.ca.getA();
        cp.setA(temp);
        cp.setB(p);*/


        Point temp1=parent.ab.getA();
        Point temp2=parent.ab.getB();

        Triangle t1 = Triangle :: makeTriangle(temp1,temp2,p);

        temp1 = parent.ab.getB();
        temp2 = parent.ca.getA();
        Triangle t2 = Triangle :: makeTriangle(temp1,temp2,p);

        temp1 = parent.ab.getA();
        temp2 = parent.ca.getA();
        Triangle t3 = Triangle :: makeTriangle(temp1,p,temp2);


        triangles.append(t1);
        triangles.append(t2);
        triangles.append(t3);

        swapTest(parent.ab,p);
        swapTest(parent.bc,p);
        swapTest(parent.ca,p);


    }
    else {
        qDebug() << "No triangle found for point: " << p.x() << " " << p.y();
    }
}

bool lessThanX(const Point &p1,const Point &p2) {
    if(p1.x() < p2.x()) {
        return true;
    }
    else {
        if(p1.x()==p2.x()) {
            return p1.y() < p2.y();
        }
    }
    return false;
}

/*void Voronoi :: sortPoints() {
    sortedPoints.clear();
    foreach(Point p,points) {
        sortedPoints.push_back(p);
    }

    qSort(points.begin(),points.end(),lessThanX);
}*/


void Voronoi :: swapTest(Line &e,Point &p) {

    qDebug() << "Edge: "<<e.toString() << " P:"<<p.x() << " " << p.y();

    if ( (e.equals(ext1)) || (e.equals(ext2)) || (e.equals(ext3)) )   {
        return;
    }
    else {

        bool exists = false;
        Point d = findVertexOnTheRight(e,p,exists);
        if (exists)  {

          qDebug() << "Vertex on the right" <<  d.x() << " " << d.y();
            Point eA = e.getA();
            Point eB = e.getB();
            if ( inCircle(p, eA, eB, d) ) {

                qDebug() << "Is in circle, flipping";
                flip(e,p,d);
                Line l1,l2;
                l1.setA(eA);
                l1.setB(d);

                l2.setA(d);
                l2.setB(eB);

                swapTest(l1, p);
                swapTest(l2, p);

            }

        }
        else {
            qDebug() << "No vertex on the right";
        }

    }
}


Point Voronoi :: findVertexOnTheRight(Line &e,Point &p,bool &valid) {
    qDebug() <<"Find vertex on the right:"<<e.toString() << " point:" << p.x() << " " << p.y();
    Point a;
    valid = false;
    foreach(Triangle t,triangles) {
        if(t.hasEdge(e) && (!t.hasVertex(p))) {
           a = t.getPointInFront(e);
           valid = true;
        }
    }

    return a;
}

void Voronoi :: flip(Line &e,Point &p,Point &d) {
    int ind1= -1;
    int ind2 = -1;
    int i = 0;

    //Find the two triangles that have the edge e
    foreach(Triangle t, triangles) {
        if(t.hasEdge(e)) {
            if(ind1== -1) {
                ind1 = i;
            }
            else {
                ind2 = i;
                break;
            }
        }
        i++;
    }


    if ((ind1==-1) || (ind2==-1)) {
        qDebug() << "No triangle with edge " << e.toString() << " found";
        return;
    }


    qDebug() <<"Removing triangles:";
    qDebug() << ind1 <<" :";
    triangles[ind1].print();
    qDebug() << ind2 <<" :";
    triangles[ind2].print();

    //Remove them;

    int ind=0;
    qDebug() << "Triangles before:";
    foreach(Triangle t,triangles) {
        qDebug() << ind << ":";
        t.print();;
        ind++;
    }

    triangles.removeAt(ind1);
    ind2--;
    triangles.removeAt(ind2);

    ind=0;
    qDebug() << "Triangles After:";
    foreach(Triangle t,triangles) {
        qDebug() << ind << ":";
        t.print();;
        ind++;
    }

    //Construct two new ones
    Point a = e.getA();
    Point b = e.getB();

    Triangle tr1 = Triangle :: makeTriangle(a, p, d);
    Triangle tr2 = Triangle :: makeTriangle(b, p, d);

    qDebug() << "Triangles: ";
    tr1.print();
    tr2.print();

    triangles.append(tr1);
    triangles.append(tr2);
}


 bool Voronoi :: inCircle(Point &a,Point &b,Point &c,Point &p)    {

     //Pretty much, magic as far as I'm concerned
     //http://www.ics.uci.edu/~eppstein/junkyard/circumcenter.html

     double d = 2*(a.y()*c.x() + b.y()*a.x() - b.y()*c.x() - a.y()*b.x() - c.y()*a.x() + c.y()*b.x() );

     double px = (b.y()*a.x()*a.x() - c.y()*a.x()*a.x() - b.y()*b.y()*a.y() + c.y()*c.y()*a.y()
                  +b.x()*b.x()*c.y() + a.y()*a.y()*b.y() + c.x()*c.x()*a.y() - c.y()*c.y()*b.y()
                  -c.x()*c.x()*b.y() - b.x()*b.x()*a.y() + b.y()*b.y()*c.y() - a.y()*a.y()*c.y()) / d;

     double py = (a.x()*a.x()*c.x() + a.y()*a.y()*c.x() + b.x()*b.x()*a.x() - b.x()*b.x()*c.x()
                  +b.y()*b.y()*a.x() - b.y()*b.y()*c.x() - a.x()*a.x()*b.x() - a.y()*a.y()*b.x()
                  -c.x()*c.x()*a.x() + c.x()*c.x()*b.x() - c.y()*c.y()*a.x() + c.y()*c.y()*b.x()) / d;

     double r = sqrt( (a.x() - px)*(a.x() - px) + (a.y() - py)*(a.y() - py) );
     double dist = sqrt( (p.x() - px)*(p.x() - px) + (p.y() - py)*(p.y() - py) );

     qDebug() << "px=" << px <<" Py=" << py << " r=" << r << " dist=" << dist;

     if (dist < r)
         return true;
     else
         return false;

}


void Voronoi :: getTriangulation() {
    foreach(Point p,points) {
        qDebug() << "Inserting point:" << p.x() << " " << p.y();
        insert(p);
    }

    foreach(Triangle t,triangles) {
        t.print();
    }
}

QList<Triangle> Voronoi :: getTriangles() {
    return triangles;
}

QList<Point> Voronoi :: getPoints() {
    return points;
}

Point Voronoi :: getCenter(Point &a,Point &b,Point &c) {
    Point center;

    double d = 2*(a.y()*c.x() + b.y()*a.x() - b.y()*c.x() - a.y()*b.x() - c.y()*a.x() + c.y()*b.x() );

    double px = (b.y()*a.x()*a.x() - c.y()*a.x()*a.x() - b.y()*b.y()*a.y() + c.y()*c.y()*a.y()
                 +b.x()*b.x()*c.y() + a.y()*a.y()*b.y() + c.x()*c.x()*a.y() - c.y()*c.y()*b.y()
                 -c.x()*c.x()*b.y() - b.x()*b.x()*a.y() + b.y()*b.y()*c.y() - a.y()*a.y()*c.y()) / d;

    double py = (a.x()*a.x()*c.x() + a.y()*a.y()*c.x() + b.x()*b.x()*a.x() - b.x()*b.x()*c.x()
                 +b.y()*b.y()*a.x() - b.y()*b.y()*c.x() - a.x()*a.x()*b.x() - a.y()*a.y()*b.x()
                 -c.x()*c.x()*a.x() + c.x()*c.x()*b.x() - c.y()*c.y()*a.x() + c.y()*c.y()*b.x()) / d;

    center.setX((float)px);
    center.setY((float)py);
    return center;
}


void Voronoi :: addCircumcenters()  {
   for(int i=0;i<triangles.size();i++) {

        Point a = triangles[i].ab.getA();
        Point b = triangles[i].bc.getA();
        Point c = triangles[i].ca.getA();
        Point cc = getCenter(a ,b ,c);
        qDebug() << "Circumcenter:" << cc.x() << cc.y();

        triangles[i].setCircumCenter(cc);
    }
}

void Voronoi :: addToVoronoi(Triangle &t) {

    qDebug() << "t is:";
    t.print();
    foreach(Triangle tr,triangles) {
        if(tr.adjacent(t)) {
            qDebug() << "Found adjacent triangle to t:";
            t.print();
            Line e;
            Point a = t.getCircumCenter();
            Point b = tr.getCircumCenter();
            qDebug() <<"Circumcenter of t:" << a.x() << " " << a.y();
            qDebug() <<"Circumcenter of tr:" << b.x() << " " << b.y();

            e.setA(a);
            e.setB(b);
            voronoi.append(e);
        }
    }

}

void Voronoi :: populateVoronoi() {
    addCircumcenters();
    voronoi.clear();
    foreach(Triangle t,triangles) {
        addToVoronoi(t);
    }

    qDebug() << "Voronoi lines:";
    foreach(Line l,voronoi) {
        qDebug() << l.toString();
    }
}

QList<Line> Voronoi :: getVoronoi() {
    return voronoi;
}
