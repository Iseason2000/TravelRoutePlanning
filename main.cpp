#include "trpmainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TRPMainWindow w;
    w.show();
    return a.exec();
}
