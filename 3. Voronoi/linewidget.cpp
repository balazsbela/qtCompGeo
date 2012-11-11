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

    QString filename("points.in");
    vr = new Voronoi(filename);
    vr->getTriangulation();
    vr->populateVoronoi();
}

LineWidget :: ~LineWidget()
{
    if(vr) delete vr;
}

void LineWidget :: initializeGL()
{
    qglClearColor(Qt::white);
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

   // vr->getTriangulation();
   //displayAxes();

    glColor4f(0.0,1.0,0.0,0.5);
    foreach(Triangle tr,vr->getTriangles()) {
        glBegin(GL_LINES);
            glVertex2f(tr.ab.getA().x(),tr.ab.getA().y());
            glVertex2f(tr.ab.getB().x(),tr.ab.getB().y());
        glEnd();

        glBegin(GL_LINES);
            glVertex2f(tr.bc.getA().x(),tr.bc.getA().y());
            glVertex2f(tr.bc.getB().x(),tr.bc.getB().y());
        glEnd();

        glBegin(GL_LINES);
            glVertex2f(tr.ca.getA().x(),tr.ca.getA().y());
            glVertex2f(tr.ca.getB().x(),tr.ca.getB().y());
        glEnd();


    }

    //Draw Voronoi

    glColor4f(0.0,0.0,0.0,0.8);
    foreach(Line l,vr->getVoronoi()) {
        glBegin(GL_LINES);
            glVertex2f(l.getA().x(),l.getA().y());
            glVertex2f(l.getB().x(),l.getB().y());
        glEnd();

    }

    //Draw points
    glPointSize(3.0f);
    glColor4f(1.0,0.0,0.0,1.0);
    foreach(Point p,vr->getPoints()) {
        glBegin(GL_POINTS);
            glVertex2f(p.x(),p.y());
        glEnd();
    }

}

