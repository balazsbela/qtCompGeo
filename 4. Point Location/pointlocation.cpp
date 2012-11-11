#include "pointlocation.h"
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QDebug>


PointLocation :: PointLocation()
{

}

PointLocation :: ~PointLocation() {
    if(p) delete p;
}

PointLocation :: PointLocation(QString& filename) {
    readFaces(filename);
}

void PointLocation :: readFaces(QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file!\n";
        return;
    }

    QTextStream stream( &file );
    QString fline,sline;

    fline = stream.readLine();
    p = new Point(fline);

    int faceIndex = 0;
    int lineIndex = 0;
    Face f;

    while (!stream.atEnd()) {
        sline = stream.readLine();
        //qDebug() << sline;
        if(sline.isEmpty()) {
            faceIndex++;
            lineIndex = 0;
            faces.append(f);
            f.clearEdges();
        }
        else {
            f.setIndex(faceIndex);
            Line l(sline);
            l.setId(lineIndex);
            l.setFaceIndex(faceIndex);
            f.addEdge(l);
            lineIndex++;

        }
    }
}

QList<Face> PointLocation :: getFaces() {
    return faces;
}

Point PointLocation :: getPoint() {
    return *p;
}


//determines if two lines intersect
Point* PointLocation :: intersects(Line& l1,Line& l2) {

    Point* intrs = new Point();
    float xa, xb, xc, xd, ya, yb, yc, yd, n, s = -1, t = -1, px, py;
    xa = l1.getA().x();
    xb = l1.getB().x();
    xc = l2.getA().x();
    xd = l2.getB().x();
    ya = l1.getA().y();
    yb = l1.getB().y();
    yc = l2.getA().y();
    yd = l2.getB().y();

    n = xa * (yd - yc) + xb * (yc - yd) + xd * (yb - ya) + xc * (ya - yb);


    if (n != 0) {
        s = xa * (yd - yc) + xc * (ya - yd) + xd * (yc - ya);
        s = s / n;
        t = xa * (yc - yb) + xb * (ya - yc) + xc * (yb - ya);
        t = -t / n;
        px = xa + s * (xb - xa);
        py = ya + s * (yb - ya);

        intrs->setX(px);
        intrs->setY(py);

        if ((s >= 0 && s <= 1) && (t >= 0 && t <= 1)) {
            return intrs;
        }

    }

    return NULL;
}



//determines the segments which go through the slab
void PointLocation :: fillSlabSegments(Slab& s) {
    s.clearSegments();
    //we simulate two lines passing through the left point and the right point

    Point al(s.getLeft().x(),999);
    Point bl(s.getLeft().x(),-999);

    Point ar(s.getRight().x(),999);
    Point br(s.getRight().x(),-999);

    Line mleft;
    mleft.setA(al);
    mleft.setB(bl);

    Line mright;
    mright.setA(ar);
    mright.setB(br);

    qDebug() << "\n";
    qDebug() << "Filling slab with limits:" << s.getLeft().x() << " " << s.getLeft().y() << " " << s.getRight().x() << " " << s.getRight().y();

    foreach(Face f,faces) {
        foreach(Line l,f.getEdges()) {
            //check if it intersects both lines
            qDebug() << "Checking:" << l.toString() << "on face" << l.getFaceIndex();
            if( intersects(l,mleft) && intersects(l,mright) ) {                
                if(!s.getSegments().contains(l)) {
                    qDebug() << "Adding segment:" << l.toString();
                    s.addSegment(l);
                }
            }
        }
    }

    s.sortSegments();

}

void PointLocation :: generateSlabs() {
    sortPoints();
    Slab s;
    for(int i=0;i<points.size()-1;i++) {
        s.setLeft(points[i]);
        s.setRight(points[i+1]);
        fillSlabSegments(s);
        slabs.append(s);
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

void PointLocation :: sortPoints() {
    //extract vertices and sort them by x coordinate
    foreach(Face f,faces) {
        foreach(Line l,f.getEdges()) {
            if(!points.contains(l.getA())) points.append(l.getA());
            if(!points.contains(l.getB())) points.append(l.getB());
        }
    }

    qSort(points.begin(),points.end(),lessThanX);
}

Face PointLocation :: getFace() {
    generateSlabs();
    int faceIndex = 0;
    //find slab which contains the point
    qDebug() << "Locating point:" << p->x() << " " << p->y();
    qDebug() << "Slabs are:";

    foreach(Slab s,slabs) {
        qDebug() << s.getLeft().x() << " " << s.getLeft().y() << " " << s.getRight().x() << " " << s.getRight().y();
    }
    qDebug() << "\n";

    foreach(Slab s,slabs) {
        if( (p->x() > s.getLeft().x()) && (p->x() <= s.getRight().x()) ) {
            qDebug() << "Found on slab having limits:";
            qDebug() << s.getLeft() << " " << s.getRight();
            faceIndex = determineRegion(s);
        }
    }
    if(faceIndex >= 0) {
        return faces[faceIndex];
    }
    else {
        Face f;
        f.setIndex(-1);
        return f;
    }
}

bool PointLocation :: isOnLine(Point &p,Line &line) {
    //checks if point p belongs to the line

    float slope,xa,xb,ya,yb,x,y;
    float lhs,rhs;

    xa = line.getA().x();
    xb = line.getB().x();
    ya = line.getA().y();
    yb = line.getB().y();

    x = p.x();
    y = p.y();

    float det = (xa * yb) + (xb * y) + (ya * x) - (x * yb) - (y * xa) - (xb * ya);

    //check endpoints
    if( p == line.getA() ) {
        return 0;
    }
    else {
        if( p == line.getB() ) {
            return 0;
        }
    }

    if ( det < 0.0001 ) {
        slope = (yb-ya)/(xb-xa);
        lhs = y - ya;
        rhs = slope * (x-xa);

        if(qAbs(lhs - rhs) < 0.001) {
            return 1;
        }

        if((xa==xb)&&(xb==x)) {
            return 1;
        }

        if((ya==yb)&&(yb==y)) {
            return 1;
        }
    }

    return 0;
}


QList<Face>* PointLocation :: searchFaceByEdge(QList<Face> faceList,Line& myLine) {
    QList<Face>* result = new QList<Face>();
    foreach(Face f,faceList) {
        foreach(Line l,f.getEdges()) {
            if(l.equals(myLine)) {
                result->append(f);
            }
        }
    }

    return result;
}


int PointLocation :: determineRegion(Slab& s) {
    Line auxl;
    auxl.setFaceIndex(-1);
    Point left;
    Point right;
    foreach(Line l, s.getSegments()) {
        qDebug() << "Current segment on the slab is:" << l.toString() << " on face " << l.getFaceIndex();
        //if is on the edge
        if(isOnLine(*p,l)) {
            return l.getFaceIndex();
        }
       /* else
            if(p->x() == s.getRight().x()) {
                return l.getFaceIndex();
            }*/

        else {
            //if under the line
            left = l.getA().x() < l.getB().x() ? l.getA() : l.getB();
            right = l.getA().x() < l.getB().x() ? l.getB() : l.getA();
            //if it makes a right turn then it's lower
            if(turn(left,right,*p) == 1) {
                qDebug() << "Point is lower";              
            }
            else {
                //point is above
                auxl = l;
                qDebug() << "The resulting line is:" << auxl.toString() << " on face " << auxl.getFaceIndex();
                break;
            }
        }

    }

    QList<Face>* potentialFaces = searchFaceByEdge(faces,auxl);


    qDebug() << "Potential faces:";
    foreach(Face f,*potentialFaces) {
        qDebug() << f.getIndex();
    }


    //now determine which face belongs to the slab
    Point al(s.getLeft().x(),999);
    Point bl(s.getLeft().x(),-999);

    Point ar(s.getRight().x(),999);
    Point br(s.getRight().x(),-999);

    Line mleft;
    mleft.setA(al);
    mleft.setB(bl);

    Line mright;
    mright.setA(ar);
    mright.setB(br);


    qDebug() << "Slab is:" << s.getLeft().x() << " " << s.getLeft().y() << " " << s.getRight().x() << " " << s.getRight().y();
    foreach(Face f,*potentialFaces) {
        foreach(Line l,f.getEdges()) {
            //check if it intersects both lines
            qDebug() << "Checking:" << l.toString() << "on face" << l.getFaceIndex();
            if( intersects(l,mleft) && intersects(l,mright) ) {
                qDebug() << "The solution is:" << f.getIndex();
                return f.getIndex();
            }
        }
    }


    return auxl.getFaceIndex();
}

int PointLocation :: turn(Point& a,Point& b,Point& c) {

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

QList<Slab> PointLocation :: getSlabs() {
    return slabs;
}
