#include "mainwindow.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    using namespace cv;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    wiringPiSetup();
    pinMode(25, OUTPUT);

    return a.exec();
}
