#-------------------------------------------------
#
# Project created by QtCreator 2011-04-05T14:56:40
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = PointLocation
TEMPLATE = app


SOURCES += main.cpp\
        linewidget.cpp \
    pointlocation.cpp \
    line.cpp \
    point.cpp \
    face.cpp \
    slab.cpp

HEADERS  += linewidget.h \
    pointlocation.h \
    line.h \
    point.h \
    face.h \
    slab.h

OTHER_FILES += \
    faces.txt
