#include "linewidget.h"
#include <QGLWidget>
#include "line.h"
#include <QDebug>

LineWidget :: LineWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;

    QString filename("faces.txt");
    pl = new PointLocation(filename);
    face = pl->getFace();

}

LineWidget :: ~LineWidget()
{
    if(pl) delete pl;
}

void LineWidget :: initializeGL()
{
    qglClearColor(Qt::black);
    glShadeModel(GL_FLAT);
    //glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POINT_SMOOTH);
}

void LineWidget :: resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(width) / height;
    glFrustum(-x, +x, -x, +x, 0.8, 20.0);
    glMatrixMode(GL_MODELVIEW);
}


void LineWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}


void LineWidget :: displayAxes() {

      glClear(GL_COLOR_BUFFER_BIT);
      glColor3f(0.0, 0.3, 0.0);
      glBegin(GL_LINES);
        glVertex3f(-17.0, 0.0, 0.0);
        glVertex3f(17, 0.0, 0.0);
      glEnd();

      glBegin(GL_LINES);
        glVertex3f(0.0, 17.0, 0.0);
        glVertex3f(0.0, -17.0,0.0);
      glEnd();

     /* glColor3f(1.0, 1.0, 0.0);
      glBegin(GL_LINES);
        glVertex3f(0.0, 0.0, -7.0);
        glVertex3f(0.0, 0.0, 5.0);
      glEnd();*/

      glFlush();
}


void LineWidget :: draw()
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glDisable(GL_DEPTH_TEST);

    displayAxes();

    glColor4f(1.0,1.0,1.0,0.5);
    foreach(Face f,pl->getFaces()) {
        glBegin(GL_LINES);
        foreach(Line e,f.getEdges()) {
            glVertex2f(e.getA().x(),e.getA().y());
            glVertex2f(e.getB().x(),e.getB().y());
        }
        glEnd();
    }

    if(face.getIndex()!=-1) {
        glColor4f(1.0,0.0,0.0,1.0);
        glBegin(GL_LINES);
        foreach(Line e,face.getEdges()) {
            glVertex2f(e.getA().x(),e.getA().y());
            glVertex2f(e.getB().x(),e.getB().y());
        }
        glEnd();
    }

    Point p = pl->getPoint();
    glPointSize(3.0f);
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_POINTS);
        glVertex2f(p.x(),p.y());
    glEnd();

    /*//draw slabs
    glColor4f(0.0,0.0,0.3,0.1);
    glBegin(GL_LINES);
    foreach(Slab s,pl->getSlabs()) {
        Point al(s.getLeft().x(),20);
        Point bl(s.getLeft().x(),-20);
        glVertex2f(al.x(),al.y());
        glVertex2f(bl.x(),bl.y());
    }
    glEnd();*/
}

