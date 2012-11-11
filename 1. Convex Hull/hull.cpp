#include "hull.h"
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>
#include <cmath>


#define _USE_MATH_DEFINES

Hull::Hull(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;

    readPoints();
}

void Hull::initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    //glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
}

void Hull::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -2.0, +2.0, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
}

void Hull::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void Hull :: displayAxes() {

      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(1.0, 0.0, 0.0);
      glLineWidth(4.0);
      glBegin(GL_LINES);
        glVertex3f(-5.0, 0.0, 0.0);
        glVertex3f(4.5, 0.0, 0.0);
      glEnd();

      glColor3f(0.0, 1.0, 0.0);
      glBegin(GL_LINES);
        glVertex3f(0.0, 4.5, 0.0);
        glVertex3f(0.0, -5.0,0.0);
      glEnd();

      glColor3f(1.0, 1.0, 0.0);
      glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, -7.0);
        glVertex3f(0.0, 0.0, 5.0);
      glEnd();

      glLineWidth(1.0);
      glColor3f(1.0, 0.0, 0.0);
      glPushAttrib(GL_ENABLE_BIT);
      glDisable(GL_DEPTH_TEST);
      glMatrixMode(GL_PROJECTION);
      glPushMatrix();
      glLoadIdentity();
      gluOrtho2D(0, 2500, 0, 2500);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
      glLoadIdentity();

      glColor3f(0.0, 1.0, 0.0);

      glColor3f(1.0, 1.0, 0.0);

      glPopMatrix();
      glMatrixMode(GL_PROJECTION);
      glPopMatrix();
      glPopAttrib();
      glMatrixMode(GL_MODELVIEW);

      glFlush();
}


void Hull::draw()
{

    //displayAxes();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    glPointSize(2.2f);

    glBegin(GL_POINTS);
    foreach(QPointF p,points) {
        glVertex2f(p.x(),p.y());
        //qDebug() << p.x() << " " << p.y() <<"\n";
    }
    glEnd();

    QVector<QPointF> hlist = calculateHull();

    for(int i=0;i<hlist.size()-1;i++) {
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_LINES);
            glVertex2f(hlist[i].x(),hlist[i].y());
            glVertex2f(hlist[i+1].x(),hlist[i+1].y());
        glEnd();
    }

    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINES);
        glVertex2f(hlist[0].x(),hlist[0].y());
        glVertex2f(hlist[hlist.size()-1].x(),hlist[hlist.size()-1].y());
    glEnd();


}


Hull::~Hull()
{
}

void Hull :: readPoints() {
    QFile file("points.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file!\n";
        return;
    }

    QTextStream stream( &file );
    QString line;
    QStringList list;
    while (!stream.atEnd()) {
            line = stream.readLine();
            list = line.split(" ");
            points.push_back(QPointF(list[0].toFloat(),list[1].toFloat()));
    }


}

bool lessThan(const QPointF &p1,const QPointF &p2) {
    if(p1.x()<p2.x()) {
        return true;
    }
    else {
        if(p1.x()==p2.x()) {
            return p1.y() < p2.y();
        }
    }
    return false;
}

bool Hull :: isRightTurn(QVector<QPointF> &vect) {
    //check last 3 points.
    if(vect.size()<3) {
        return false;
    }

    QPointF a,b,c;
    a = vect[vect.size()-3];
    b = vect[vect.size()-2];
    c = vect[vect.size()-1];


    QPointF v,u; //the 2 vectors
    v.setX(b.x()-a.x());
    v.setY(b.y()-a.y());
    u.setX(c.x()-b.x());
    u.setY(c.y()-b.y());

    float crossProduct = (b.x()-a.x())*(c.y()-a.y())-(b.y()-a.y())*(c.x()-a.x());
    //qDebug() << crossProduct;
    return crossProduct<0;
}

QVector<QPointF> Hull :: calculateHull() {
    qSort(points.begin(),points.end(),lessThan);
    QVector<QPointF> lupper,llower;
    lupper.push_back(points[0]);
    lupper.push_back(points[1]);

    for(int i=2;i<points.size();i++) {
        lupper.push_back(points[i]);
        while ((lupper.size()>2) && (!isRightTurn(lupper))) {
            //delete the middle point
            lupper.remove(lupper.size()-2);
        }
    }

    llower.push_back(points[points.size()-1]);
    llower.push_back(points[points.size()-2]);

    for(int i=points.size()-3;i>=0;i--) {
        llower.push_back(points[i]);

        while ((llower.size()>2) && (!isRightTurn(llower))) {
            //delete the middle point
            llower.remove(llower.size()-2);
        }
    }

    llower.remove(0);
    llower.remove(llower.size()-1);

    lupper += llower;
    return lupper;
}


