#include "mainwindow.h"
#include <QApplication>
#include <QDir>

#include "easylogging++.h"
_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // _START_EASYLOGGINGPP(argc, argv);
    QString exePath = QCoreApplication::applicationDirPath();
    exePath.append("/log.cfg");
    easyloggingpp::Configurations confFromFile(exePath.toUtf8().data());
    easyloggingpp::Loggers::reconfigureAllLoggers(confFromFile);

    LINFO << QThread::currentThreadId() << "-start CSSHP";

    MainWindow w;
    w.show();
    return a.exec();
}
