#ifndef HULL_H
#define HULL_H

#include <QtGui/QWidget>
#include <QGLWidget>


#include <QGLWidget>
#include <QMouseEvent>
#include <QVector>
#include <QPoint>


class Hull : public QGLWidget
{
    Q_OBJECT

public:
    Hull(QWidget *parent = 0);
    ~Hull();

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private:
    void draw();
    void readPoints();
    void displayAxes();
    QVector<QPointF> calculateHull();
    bool isRightTurn(QVector<QPointF> &vect);
    QVector<QPointF> points;
    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    QColor faceColors[6];

};

#endif // HULL_H
