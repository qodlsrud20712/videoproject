#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>
#include <QThread>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "opencv2/opencv.hpp"


using namespace cv;



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    static Mat frame;
    Ui::MainWindow *ui;
    VideoCapture video;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_startBtn_pressed();

    void Move_Up();

    void Move_Down();

    void Move_L();

    void Move_R();

    void Zoom_in();

    void Zoom_Out();

    void Rotate_L();

    void Rotate_Home();

    void Rotate_R();

    void Move_Zoom_Home();

    void Detect_Start();

    void Record_Start();


private:

    QGraphicsPixmapItem pixmap;
    QGraphicsPixmapItem pixmap_Red;
    QGraphicsPixmapItem pixmap_Green;
    QGraphicsPixmapItem pixmap_Blue;
    QGraphicsPixmapItem pixmap_RG_Mix;
    QGraphicsPixmapItem pixmap_GB_Mix;
    QGraphicsPixmapItem pixmap_Rotate;
    QGraphicsPixmapItem pixmap_Move;
    QGraphicsPixmapItem pixmap_detect;
    QGraphicsPixmapItem pixmap_cds;



    Mat frame1 ,frame2, frame3, frame4, frame5, frame6, frame_cds;


};


#endif // MAINWINDOW_H
