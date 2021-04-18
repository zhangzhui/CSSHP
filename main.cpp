#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "easylogging++.h"
_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    _START_EASYLOGGINGPP(argc, argv);
    LINFO << "start CSSHP";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
