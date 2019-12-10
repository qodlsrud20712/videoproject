#include "record.h"
#include "ui_mainwindow.h"
#include <QThread>

using namespace cv;
Mat MainWindow::frame;

Record::Record(Ui::MainWindow *ui, Size size1){
   size = size1;
   fourcc = VideoWriter::fourcc('D', 'X', '5', '0');
   fps = 10;
   delay = cvRound(1000.0/fps);

   writer.open("/home/pi/recordfile/video.avi", fourcc, fps, size);
}

void Record::Record_start2(Mat frame){
    frame = MainWindow::frame;
    writer << frame;
    waitKey(delay);
}
