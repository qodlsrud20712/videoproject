#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.h"
#include "record.h"
#include <vector>
#include <map>
#define CS_MCP3008 28
#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

int brightness = 0;
int adcChannel = 0;
int adcValue = 0;


using namespace cv;


std::vector<Rect> faces;

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    ui->graphicsView_2->setScene(new QGraphicsScene(this));
    ui->graphicsView_2->scene()->addItem(&pixmap_Red);
    ui->graphicsView_3->setScene(new QGraphicsScene(this));
    ui->graphicsView_3->scene()->addItem(&pixmap_Green);
    ui->graphicsView_4->setScene(new QGraphicsScene(this));
    ui->graphicsView_4->scene()->addItem(&pixmap_Blue);
    ui->graphicsView_5->setScene(new QGraphicsScene(this));
    ui->graphicsView_5->scene()->addItem(&pixmap_RG_Mix);
    ui->graphicsView_6->setScene(new QGraphicsScene(this));
    ui->graphicsView_6->scene()->addItem(&pixmap_GB_Mix);
    ui->graphicsView_8->setScene(new QGraphicsScene(this));
    ui->graphicsView_8->scene()->addItem(&pixmap_Rotate);
    ui->graphicsView_9->setScene(new QGraphicsScene(this));
    ui->graphicsView_9->scene()->addItem(&pixmap_Move);
    ui->graphicsView_11->setScene(new QGraphicsScene(this));
    ui->graphicsView_11->scene()->addItem(&pixmap_cds);
    ui->graphicsView_12->setScene(new QGraphicsScene(this));
    ui->graphicsView_12->scene()->addItem(&pixmap_detect);

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(Move_Up()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(Move_L()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(Move_Down()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(Move_R()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(Zoom_Out()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(Zoom_in()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(Rotate_L()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(Rotate_Home()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(Rotate_R()));
    connect(ui->pushButton_10, SIGNAL(clicked()), this, SLOT(Move_Zoom_Home()));
    connect(ui->record_Button, SIGNAL(clicked()), this, SLOT(Record_Start()));
    connect(ui->detect_button, SIGNAL(clicked()), this, SLOT(Detect_Start()));

}


MainWindow::~MainWindow()
{
    delete ui;
}

int read_mcp3008_adc(unsigned char adcChannel){
	unsigned char buff[3];
	int adcValue = 0;

	buff[0] = 0x01;
	buff[1] = 0x80|((adcChannel & 0x07)<<4);
	buff[2] = 0x00;

	digitalWrite(CS_MCP3008, 0);
	wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

	buff[1] = 0x03 & buff[1];
	adcValue = (buff[1] << 8) | buff[2];
	digitalWrite(CS_MCP3008, 1);
	return adcValue;
}


void MainWindow::on_startBtn_pressed()
{
    using namespace cv;


    if(video.isOpened())
    {
        ui->startBtn->setText("Start");
        video.release();
        return;
    }

    bool isCamera;
    int cameraIndex = ui->videoEdit->text().toInt(&isCamera);
    if(isCamera)
    {
        if(!video.open(cameraIndex))
        {
            QMessageBox::critical(this,
                    "Camera Error",
                    "Make sure you entered a correct camera index,"
                    "<br>or that the camera is not being accessed by another program!");
            return;
        }
    }
    else
    {
        if(!video.open(ui->videoEdit->text().trimmed().toStdString()))
        {
            QMessageBox::critical(this,
                    "Video Error",
                    "Make sure you entered a correct and supported video file path,"
                    "<br>or a correct RTSP feed URL!");
            return;
        }
    }

    ui->startBtn->setText("Stop");

    video.set(3, 640);
    video.set(4, 480);

     video >> frame;

    Size size(frame.cols, frame.rows);
    Record *Rec = new Record(ui, size);

    while(video.isOpened())
    {
        video >> frame>> frame2 >> frame3 >> frame4 >> frame5 >> frame6>>frame_cds;
        video >> frame1;

        if(!frame.empty())
        {
            QImage qimg(frame.data,
                    frame.cols,
                    frame.rows,
                    frame.step,
                    QImage::Format_RGB888);

            QImage qimg_Rot(frame.data,
                    frame.cols,
                    frame.rows,
                    frame.step,
                    QImage::Format_RGB888);

            QImage qimg_Move(frame.data,
                    frame.cols,
                    frame.rows,
                    frame.step,
                    QImage::Format_RGB888);

            if(ui->tabWidget->currentIndex() == 1){
                        wiringPiSetup();
                        pinMode(CS_MCP3008, OUTPUT);
                        wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);

                        video >> frame;

                        adcChannel = 3;
                        adcValue = read_mcp3008_adc(adcChannel);
                        QThread::msleep(30);
                        adcValue = adcValue-512 ;
                        frame.convertTo(frame_cds, -1, 1, adcValue);


                        QImage qimg_cds(frame_cds.data,
                                    frame_cds.cols,
                                    frame_cds.rows,
                                    frame_cds.step,
                                    QImage::Format_RGB888);

                        pixmap_cds.setPixmap(QPixmap::fromImage(qimg_cds.rgbSwapped()));
                        ui->graphicsView_11->fitInView(&pixmap_cds, Qt::KeepAspectRatio);

                    }

            QImage qimg_cds(frame_cds.data,
                    frame_cds.cols,
                    frame_cds.rows,
                    frame_cds.step,
                    QImage::Format_RGB888);

            CV_Assert(frame.data);


            if(ui->record_Button->text() == "Stop"){

		video >> frame;
                Rec->writer << frame;
		waitKey(Rec->delay);

            }

            if(ui->detect_button->text()=="Stop"){
                CascadeClassifier classifier("/home/pi/opencv/opencv-4.0.1/data/haarcascades/haarcascade_frontalface_default.xml");
                video >> frame1;
                classifier.detectMultiScale(frame1,faces);
                for(Rect rc: faces){
                    rectangle(frame1, rc, Scalar(255,0,255), 2);
                }
                waitKey();
            }

            Mat bgr[3];

            split(frame, bgr);

            cvtColor(bgr[2]/2 + bgr[1]/2, frame5, COLOR_BGR2RGB);//RG Mix
            cvtColor(bgr[1]/2 + bgr[0]/2, frame6, COLOR_BGR2RGB);//GB Mix
            cvtColor(bgr[2], frame2, COLOR_BGR2RGB);//red
            cvtColor(bgr[1], frame3, COLOR_BGR2RGB);//green
            cvtColor(bgr[0], frame4, COLOR_BGR2RGB);//blue

            QImage qimg2(frame2.data,
                    frame2.cols,
                    frame2.rows,
                    frame2.step,
                    QImage::Format_RGB888);
            QImage qimg3(frame3.data,
                    frame3.cols,
                    frame3.rows,
                    frame3.step,
                    QImage::Format_RGB888);
            QImage qimg4(frame4.data,
                    frame4.cols,
                    frame4.rows,
                    frame4.step,
                    QImage::Format_RGB888);
            QImage qimg5(frame5.data,
                    frame5.cols,
                    frame5.rows,
                    frame5.step,
                    QImage::Format_RGB888);
            QImage qimg6(frame6.data,
                    frame6.cols,
                    frame6.rows,
                    frame6.step,
                    QImage::Format_RGB888);
            QImage qimg_detect(frame1.data,
                    frame1.cols,
                    frame1.rows,
                    frame1.step,
                    QImage::Format_RGB888);

            pixmap.setPixmap( QPixmap::fromImage(qimg.rgbSwapped()) );
            pixmap_Red.setPixmap( QPixmap::fromImage(qimg2));
            pixmap_Green.setPixmap( QPixmap::fromImage(qimg3) );
            pixmap_Blue.setPixmap( QPixmap::fromImage(qimg4) );
            pixmap_RG_Mix.setPixmap( QPixmap::fromImage(qimg5) );
            pixmap_GB_Mix.setPixmap( QPixmap::fromImage(qimg6) );
            pixmap_Rotate.setPixmap( QPixmap::fromImage(qimg_Rot.rgbSwapped()));
            pixmap_Move.setPixmap(QPixmap::fromImage(qimg_Move.rgbSwapped()));
            pixmap_detect.setPixmap(QPixmap::fromImage(qimg_detect.rgbSwapped()));
            pixmap_cds.setPixmap(QPixmap::fromImage(qimg_cds.rgbSwapped()));


            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
            ui->graphicsView_2->fitInView(&pixmap_Red, Qt::KeepAspectRatio);
            ui->graphicsView_3->fitInView(&pixmap_Green, Qt::KeepAspectRatio);
            ui->graphicsView_4->fitInView(&pixmap_Blue, Qt::KeepAspectRatio);
            ui->graphicsView_5->fitInView(&pixmap_RG_Mix, Qt::KeepAspectRatio);
            ui->graphicsView_6->fitInView(&pixmap_GB_Mix, Qt::KeepAspectRatio);
            ui->graphicsView_8->fitInView(&pixmap, Qt::KeepAspectRatio);
            ui->graphicsView_9->fitInView(&pixmap, Qt::KeepAspectRatio);
            ui->graphicsView_11->fitInView(&pixmap_cds, Qt::KeepAspectRatio);
            ui->graphicsView_12->fitInView(&pixmap_detect, Qt::KeepAspectRatio);

        }
        qApp->processEvents();
    }

    ui->startBtn->setText("Start");
    delete Rec;
}



