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

    QString filename("lines.txt");
    triangulation = new Triangulation(filename);
    triangulation->triangulate();
}

LineWidget :: ~LineWidget()
{
    if(triangulation) delete triangulation;
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
    glFrustum(-x, +x, -x, +x, 0.50, 20.0);
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
        glVertex3f(-12.0, 0.0, 0.0);
        glVertex3f(12, 0.0, 0.0);
      glEnd();

      glBegin(GL_LINES);
        glVertex3f(0.0, 12.0, 0.0);
        glVertex3f(0.0, -12.0,0.0);
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

    displayAxes();

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINES);
    foreach(Line e,triangulation->getEdges()) {
        //qDebug() << e.toString();
        glVertex2f(e.getA().x(),e.getA().y());
        glVertex2f(e.getB().x(),e.getB().y());
    }    
    glEnd();

    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
    foreach(Line e,triangulation->getDiagonals()) {
        //qDebug() << e.toString();
        glVertex2f(e.getA().x(),e.getA().y());
        glVertex2f(e.getB().x(),e.getB().y());
    }
    glEnd();

}

