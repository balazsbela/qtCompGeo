#-------------------------------------------------
#
# Project created by QtCreator 2011-04-05T14:56:40
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Voronoi
TEMPLATE = app


SOURCES += main.cpp\
        linewidget.cpp \
    line.cpp \
    point.cpp \
    voronoi.cpp \
    triangle.cpp

HEADERS  += linewidget.h \
    line.h \
    point.h \
    voronoi.h \
    triangle.h

OTHER_FILES += \
    points.txt
