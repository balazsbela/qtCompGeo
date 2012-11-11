#-------------------------------------------------
#
# Project created by QtCreator 2011-04-05T14:56:40
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Triangulation
TEMPLATE = app


SOURCES += main.cpp\
        linewidget.cpp \
    sweeplines.cpp \
    triangulation.cpp \
    line.cpp \
    point.cpp

HEADERS  += linewidget.h \
    sweeplines.h \
    triangulation.h \
    line.h \
    point.h

OTHER_FILES += \
    lines.txt
