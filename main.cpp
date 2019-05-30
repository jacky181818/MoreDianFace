#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    log4cplus::Initializer initializer;
    qInstallMessageHandler(MainWindow::myMessageHandler);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
