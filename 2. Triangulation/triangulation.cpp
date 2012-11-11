#include "triangulation.h"
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QDebug>
#include "sweeplines.h"

Triangulation :: Triangulation()
{

}

Triangulation :: Triangulation(QString& filename) {
    readLines(filename);
}

void Triangulation :: readLines(QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file!\n";
        return;
    }

    int index = 0;
    QTextStream stream( &file );
    QString fline,sline,fullLine,first;

    fline = stream.readLine();
    first = fline;
    while (!stream.atEnd()) {
        sline = stream.readLine();
        fullLine = fline+" "+sline;

        Point p(fline);
        vertices.push_back(p);

        fline = sline;
       // qDebug() << fullLine;

        Line l(fullLine);
        l.setId(index);
        Point aux = l.getA();
        aux.setSegmentIndex(l.getId());
        l.setA(aux);
        aux = l.getB();
        aux.setSegmentIndex(l.getId());
        l.setB(aux);
        //qDebug() << l.getA().getSegmentIndex();
        edges.append(l);
        index++;
        //qDebug() << edges.last().toString();
    }

    Point plast(sline);
    vertices.push_back(plast);

    fullLine=sline+" "+first;
    Line l(fullLine);
    l.setId(index);
    Point aux = l.getA();
    aux.setSegmentIndex(l.getId());
    l.setA(aux);
    aux = l.getB();
    aux.setSegmentIndex(l.getId());
    l.setB(aux);
    edges.append(l);

}


QList<Line> Triangulation::getEdges() {
    return edges;
}


QList<Point> Triangulation::getVertices() {
    return vertices;
}


Triangulation :: ~Triangulation() {

}

void Triangulation :: reportDiagonal(Point& p1,Point& p2) {
    QString lineStr = QString::number(p1.x())+" "+QString::number(p1.y())+" "+QString::number(p2.x())+" "+QString::number(p2.y());
    Line diag(lineStr);
    diag.setId(diagonals.size());
    if(!diagonals.contains(diag)) {
        diagonals.append(diag);
    }
}

int Triangulation :: findVertex(Point& v1) {
    int i=0;
    bool isFound = false;
    foreach(Point p,vertices) {
        if(p == v1) {
            isFound = true;
            break;
        }
        i++;
    }

    if(!isFound) i = -1;
    return i;
}

void Triangulation :: triangulate() {
    Point p1 = vertices[0];
    Point p2 = vertices[1];
    Point p3;
    int pos = 0;
    while(vertices.size()>3) {
        qDebug() << "\n";
        qDebug() << "nr of vertices:" << vertices.size();
        pos = findVertex(p2);
        qDebug() << "p2 has index:" << pos;
        qDebug() << "p2 is" << vertices[pos].x() << " " << vertices[pos].y();

        if(pos+1 >= vertices.size()) {
            p3 = vertices[0];
        }
        else {
            p3 = vertices[pos+1];
        }

        qDebug() << "p3 is" << p3.x() << " " << p3.y();
        qDebug() << "p1 is" << p1.x() << " " << p1.y();

        qDebug() << "Processing line:" <<". " << p1.x() << " " << p1.y() << " " << p3.x() << " " << p3.y();

        if (diagonal(p1,p3)) {
            qDebug() << "Line is a diagonal";
            reportDiagonal(p1,p3);
            //remove p2;
            qDebug() << "Removing p2 with index:" << pos;
            vertices.removeAt(pos);
            p2 = p3;
        }
        else {
            qDebug() << "Line is not a diagonal";
            p1 = p2;
            p2 = p3;
        }

    }
}

bool Triangulation :: intersects(Line& l1,Line& l2) {
    SweepLines *sw = new SweepLines();
    sw->addSegment(l1);
    sw->addSegment(l2);

   // qDebug() << "\n";
   // qDebug() << "APPLYING SWEEPLINE ALGORITHM TO CHECK IF "<<l1.toString()<<" and " << l2.toString() << " intersect";

    sw->findIntersections();

    qDebug() << "FOUND FOLLOWING INTERSECTION POINTS:";
    QList<Point> intrs = sw->getIntersections();
    int i=0;
    foreach(Point p,intrs) {
            qDebug() << p.x() << " " << p.y();
            if((p==l1.getA())||(p==l1.getB())||(p==l2.getA())||(p==l2.getB())) {
                qDebug() << "Is on edges";
                intrs.removeAt(i);
            }
            i++;
    }

    qDebug() << "\n";

    bool intersects = false;
    if(intrs.size()>0) {
        intersects = true;
    }

    if(sw) {
        delete sw;
    }

    return intersects;
}


bool Triangulation :: diagonal_IE(Point &p1,Point &p2) {

   QString lineStr = QString::number(p1.x())+" "+QString::number(p1.y())+" "+QString::number(p2.x())+" "+QString::number(p2.y());
   Line l(lineStr);
   l.setId(0);
   qDebug() << "Checking if is diagIE: " << l.toString();
   foreach(Line e,edges) {
        //for every edge not incident to p1 or p2
        if( ( (e.getA() != p1 ) && (e.getB() != p2) ) && ( (e.getA() != p2 ) && (e.getB() != p2) ) ) {
            qDebug() << "Edge:" << e.toString();
            if( intersects(e,l)) {
                return false;
            }
        }
    }

    return true;
}

bool Triangulation :: diagonal(Point& p1,Point& p2) {

    qDebug() << "Checking for valid diagonal";
    bool inter = interior(p1,p2);
    bool diag = diagonal_IE(p1,p2);

    qDebug() << "Interior:"<<inter;
    qDebug() << "DiagonalIE"<<diag;

    return inter && diag;
}

int Triangulation :: turn(Point& a,Point& b,Point& c) {

    Point v,u; //the 2 vectors
    v.setX(b.x()-a.x());
    v.setY(b.y()-a.y());
    u.setX(c.x()-b.x());
    u.setY(c.y()-b.y());

    float crossProduct = (b.x()-a.x())*(c.y()-a.y())-(b.y()-a.y())*(c.x()-a.x());
    //qDebug() << crossProduct;
    if(crossProduct < 0) {
        return 1; // right turn
    }
    else {
        if(crossProduct == 0) {
            return 0; //collinear
        }
        return -1; //left turn;
    }
}


bool Triangulation :: interior(Point& v1,Point& v2) {
    Point q1,q2;

    int pos = findVertex(v1);

    if(pos+1<vertices.size()) {
        q2 = vertices[pos+1];
    }
    else {
        q2 = vertices[0];
    }

    if(pos<=0) {
        q1 = vertices[vertices.size()-1];
    }
    else {
        q1 = vertices[pos-1];
    }

    //convex case
    if( turn(q1,v1,q2) == -1 ) {
        return (turn(v1,q2,v2)==-1) && (turn(v2,q1,v1)==-1);
    }
    //reflex case
    else {
        return ( (turn(q2,v1,v2)==1) || (turn(v1,q1,v2)==1) );
    }

    return false;

}

QList<Line> Triangulation :: getDiagonals() {
    return diagonals;
}
