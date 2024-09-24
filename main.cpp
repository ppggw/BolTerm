#include "mainwindow.h"

#include <QApplication>
QMutex m1;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(&m1);
    w.show();
    return a.exec();
}
