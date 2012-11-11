#include "sweeplines.h"
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QDebug>

#define DEBUGON 0

SweepLines::SweepLines()
{

}

SweepLines :: SweepLines(QString& filename) {
    //readLines(filename);
}

void SweepLines :: readLines(QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if(DEBUGON) qDebug() << "Error opening file!\n";
        return;
    }

    int index = 0;
    QTextStream stream( &file );
    QString fline;
    while (!stream.atEnd()) {
        fline = stream.readLine();
        Line l(fline);
        l.setId(index);
        Point aux = l.getA();
        aux.setSegmentIndex(l.getId());
        l.setA(aux);
        aux = l.getB();
        aux.setSegmentIndex(l.getId());
        l.setB(aux);
        //qDebug() << l.getA().getSegmentIndex();
        segments.append(l);
        index++;
      // if(DEBUGON) qDebug() << segments.last().toString();
    }
}

void SweepLines :: addSegment(Line& l) {
    int ind = segments.size();
    l.setId(ind);
    l.setReversed(false);
    if(!segments.contains(l)) {
        segments.append(l);
    }
}

SweepLines :: ~SweepLines() {

}


QList<Line> SweepLines :: getSegments() {
    return segments;
}

QList<Point> SweepLines :: getIntersections() {
    return intersections;
}

bool lessThan(const Point &p1,const Point &p2) {
    if(p1.y() > p2.y()) {
        return true;
    }
    else {
        if(p1.y()==p2.y()) {
            return p1.x() < p2.x();
        }
    }
    return false;
}

bool lessThanLines(const Line& l1,const Line& l2) {
    //use the upper point A
    Point A;
    Point B;

    if(l1.getReversed()) A = l1.getB();
    else A = l1.getA();

    if(l2.getReversed()) B = l2.getB();
    else B = l2.getB();


    if(A.x()==B.x()) return A.y()>B.y();

    return A.x() > B.x();
}

//adds segment endpoints to the event queue sorted by y
void SweepLines :: fillQueue() {

    QVector<Point> points;
    foreach(Line l,segments) {
        points.append(l.getA());
        points.append(l.getB());
    }

    qSort(points.begin(),points.end(),lessThan);

    foreach(Point p,points) {
        //qDebug() << p.x() << " " << p.y() << " " << p.getSegmentIndex();
        if(!eventQueue.contains(p)) eventQueue.append(p);
    }

    //qSort(eventQueue.begin(),eventQueue.end(),lessThan);

}


void SweepLines :: addSegmentToStatus(Line& l) {
    if(!status.contains(l)) {
        status.append(l);
        qSort(status.begin(),status.end(),lessThanLines);
    }
}


//checks if point p belongs to the line
bool SweepLines :: isInteriorPoint(Line& line,Point& p) {
   if(DEBUGON) qDebug() << "Checking if " << line.toString() << " contains " << p.x() << " " << p.y();
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

   if(DEBUGON) qDebug() << "Determinant:" << det;
   if(DEBUGON) qDebug() << "xa=" << xa << " xb="<<xb<<" ya="<<ya<<" yb="<<yb <<" x="<<x<<" y="<<y;
    if ( det < 0.0001 ) {
        slope = (yb-ya)/(xb-xa);
        lhs = y - ya;
        rhs = slope * (x-xa);

        //compare two floats
       if(DEBUGON) qDebug() << lhs << "=" << rhs;
       if(DEBUGON) qDebug() << slope << " " << x << " " <<  xa;
       if(DEBUGON) qDebug() << qAbs(lhs-rhs);
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

void SweepLines :: reportIntersection(Point& p) {
   if(DEBUGON) qDebug() << "Found intersection point:" << p.x() << " " << p.y();
    if(!intersections.contains(p)) {
        intersections.append(p);
    }
    //TODO:
}

void SweepLines :: handleEventPoint(Point& p) {
    U.clear();
    L.clear();
    C.clear();

   if(DEBUGON) qDebug() << "Dequeued point:"<< p.x() << " " << p.y();

    //Put the segments whose upper endpoint is p into U

    foreach(Line l,segments) {
        if(l.getA() == p) {
            if(!U.contains(l)) U.append(l);
        }
    }

   if(DEBUGON) qDebug() << "Populating C:";
    //Put midpoints into C and the rest in L
    foreach(Line line,status) {
        if(isInteriorPoint(line,p)) {

           if(DEBUGON) qDebug()<< p.x() << " " << p.y() << "is on line " << line.toString();
            if(!C.contains(line)) {
                C.append(line);
               if(DEBUGON) qDebug() << "Adding to C";
            }
            else {
               if(DEBUGON) qDebug() << "Already contained";
            }
        }
        else {
            if(line.getB() == p) {
                if(!L.contains(line)) {
                    L.append(line);
                }
            }
        }
    }


   if(DEBUGON) qDebug() << "Upper";
    printLineList(U);
   if(DEBUGON) qDebug() << "Lower:";
    printLineList(L);
   if(DEBUGON) qDebug() << "Mid:";
    printLineList(C);



    //report p as intersection
    if((U.size() + L.size()+C.size()) > 1 ) {
        reportIntersection(p);
    }

    //remove L and C from status
    int i=0;
    foreach(Line line,L) {
        if(status.contains(line)) {
            i=0;
            foreach(Line sl,status) {
                if(sl == line) {
                    status.removeAt(i);
                }
                i++;
            }
        }
    }

    foreach(Line line,C) {
        if(status.contains(line)) {
            i=0;
            foreach(Line sl,status) {
                if(sl == line) {
                    status.removeAt(i);
                }
                i++;
            }
        }
    }

    //add segments from U and C (reversed) into status
    foreach(Line line,U) {
       status.append(line);
    }

    foreach(Line line,C) {
       line.setReversed(true);
       status.append(line);
    }

    qSort(status.begin(),status.end(),lessThanLines);

    printStatus();


    if((U.isEmpty())&&(C.isEmpty())) {
        Line sl,sr;
        Point upper ;
        int i=0;
        bool hasRightNeighbour = false;
        bool hasLeftNeighbour = false;
        foreach(Line l,status) {
            if(l.getReversed()) upper = l.getB();
            else upper = l.getA();

            if(upper.x() > p.x()) {
                //we found the right neighbour
                hasRightNeighbour = true;
                sr = status[i];
                if(i>0) {
                    //TODO: IS IT i-2 ?

                    //set left neighbour
                    sl = status[i-1];
                    hasLeftNeighbour = true;
                }
                break;

            }

            i++;
        }               

        if(hasRightNeighbour&&hasLeftNeighbour) {            
            findNewEvent(sl,sr,p);
        }
    }
    else {

        //leftmost segment
        Line sl;
        foreach(Line l,status) {
            if(U.contains(l) || C.contains(l)) {
                sl = l;
                break;
            }
        }

        //left neighbour of sl
        Line left;
        int i=0;
        bool hasLeft = false;
        foreach(Line l,status) {
            if(l == sl) {
                if(i>0) {
                    left = status[i-1];
                    hasLeft = true;
                    break;
                }
            }
            i++;
        }

        if (hasLeft) findNewEvent(sl,left,p);


        Line sr;
        //get the rightmost segment
        foreach(Line l,status) {
            if(U.contains(l) || C.contains(l)) {
                sr = l;
            }
        }


        //find right neighbour
        Line right;
        i = 0;
        bool hasRight = false;
        foreach(Line l,status) {
            if(l == sl) {
                if(i+1<status.size()) {
                    right = status[i+1];
                    hasRight = true;
                    break;
                }
            }
            i++;
        }

        if(hasRight) findNewEvent(sr,right,p);

    }

}

void SweepLines :: findNewEvent(Line &l1,Line &l2,Point &p) {
   if(DEBUGON) qDebug() << "Called FindNewEvent() with:"<< l1.toString() << " " << l2.toString() << " and point "<<p.x()<<" "<<p.y();
    bool valid = false;
    Point *pnt = l1.getIntersection(l2,valid);
   if(DEBUGON) qDebug() << "Intersection point:" << pnt->x()<<" "<< pnt->y();
   if(DEBUGON) qDebug() << "Current event point:" << p.x() << " " << p.y();
    if(valid) {
          //if the intersection point is below the sweep line or on it and to the right
          //of the current event point
          if ( (pnt->y() < p.y()) || ( (pnt->y() == p.y()) && (pnt->x() > p.x())) ) {

               if(DEBUGON) qDebug() << "Checking if " << pnt->x() << " " << pnt->y() << "is contained in the event queue";
                //printQueue();
                if(!eventQueue.contains(*pnt)) {
                   if(DEBUGON) qDebug() <<"Not contained";
                   if(DEBUGON) qDebug() << "Adding " << pnt->x() << " " << pnt->y() << "to the event queue";
                    eventQueue.append(*pnt);
                    qSort(eventQueue.begin(),eventQueue.end(),lessThan);
                }
                else {
                   if(DEBUGON) qDebug() <<"ALREADY CONTAINED!";
                }
          }
    }
}


void SweepLines :: findIntersections() {
    fillQueue();
    //printQueue();
    while(!eventQueue.isEmpty()) {
        Point aux = eventQueue[0];
        eventQueue.removeFirst();
        printQueue();
        //addSegmentToStatus(segments[aux.getSegmentIndex()]);
        handleEventPoint(aux);
       if(DEBUGON) qDebug() << "----------";
    }


   if(DEBUGON) qDebug() << "Intersections:";
    foreach(Point p,intersections) {
     if(DEBUGON) qDebug() << QString("p:") << p.x() << " " << p.y();
    }
}


void SweepLines :: printStatus() {
   if(DEBUGON) qDebug() << "The status:";
    foreach(Line l,status) {
       if(DEBUGON) qDebug() << l.toString();
    }
   if(DEBUGON) qDebug() << "\n";
}

void SweepLines :: printQueue() {
   if(DEBUGON) qDebug() << "The event queue:";
    foreach(Point p,eventQueue) {
       if(DEBUGON) qDebug() << p.x() << p.y();
    }
   if(DEBUGON) qDebug() << "\n";
}

void SweepLines :: printLineList(QList<Line>& list) {
    foreach(Line l,list) {
       if(DEBUGON) qDebug() << l.toString();
    }
   if(DEBUGON) qDebug() << "\n";
}
