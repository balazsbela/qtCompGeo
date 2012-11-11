#include <QtGui/QApplication>
#include "hull.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Hull w;
    w.show();

    return a.exec();
}
