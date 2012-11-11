#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QGLWidget>
#include "pointlocation.h"

class LineWidget : public QGLWidget
{
    Q_OBJECT

public:
    LineWidget(QWidget *parent = 0);
    ~LineWidget();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    PointLocation *pl;
    Face face;

    void draw();
    void displayAxes();
};

#endif // LINEWIDGET_H
