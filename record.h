#ifndef RECORD_H
#define RECORD_H

#include "mainwindow.h"

using namespace cv;

class Record
{
public:
    Size size;
    VideoWriter writer;
    Record(Ui::MainWindow *ui , Size size);
    int fourcc;
    double fps;
    int delay;
    void Record_start2(Mat frame);
};

#endif // RECORD_H
