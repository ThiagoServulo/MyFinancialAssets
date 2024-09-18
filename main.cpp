#include "addwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AddWindow w;
    w.show();
    return a.exec();
}
