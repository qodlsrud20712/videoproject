#ifndef FUNCTION_H
#define FUNCTION_H
#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::Move_Zoom_Home() {
    ui->graphicsView_9->move(130, 90);
    ui->graphicsView_9->resize(256,192);
}

void MainWindow::Move_Up() {
    ui->graphicsView_9->move(ui->graphicsView_9->x(), ui->graphicsView_9->y()-10);
}

void MainWindow::Move_L() {
    ui->graphicsView_9->move(ui->graphicsView_9->x()-10, ui->graphicsView_9->y());
}

void MainWindow::Move_Down() {
    ui->graphicsView_9->move(ui->graphicsView_9->x(), ui->graphicsView_9->y()+10);
}

void MainWindow::Move_R() {
    ui->graphicsView_9->move(ui->graphicsView_9->x()+10, ui->graphicsView_9->y());
}

void MainWindow::Zoom_Out() {
    ui->graphicsView_9->resize(ui->graphicsView_9->width()-10, ui->graphicsView_9->height()-10);
}

void MainWindow::Zoom_in() {
    ui->graphicsView_9->resize(ui->graphicsView_9->width()+10, ui->graphicsView_9->height()+10);
}

void MainWindow::Rotate_L() {
    ui->graphicsView_8->rotate(30);
}

void MainWindow::Rotate_Home() {
    ui->graphicsView_8->resetTransform();
}

void MainWindow::Rotate_R() {
    ui->graphicsView_8->rotate(-30);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(video.isOpened())
    {
        QMessageBox::warning(this,
                "Warning",
                "Stop the video before closing the application!");
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void MainWindow::Record_Start(){
    if(ui->record_Button->text()=="recording"){
        ui->record_Button->setText("Stop");
        digitalWrite(25, HIGH);
    }else{
        ui->record_Button->setText("recording");
        digitalWrite(25, LOW);
    }
}

void MainWindow::Detect_Start(){
    if(ui->detect_button->text()=="face_detect_start"){
        ui->detect_button->setText("Stop");
    }else{
        ui->detect_button->setText("face_detect_start");
    }
}


#endif // FUNCTION_H
