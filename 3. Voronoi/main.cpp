#include <QtGui/QApplication>
#include "linewidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LineWidget w;
    w.show();

    return a.exec();
}
