#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <string>
#include <QDebug>
#include <QTimer>
#include <QtNetwork/QUdpSocket>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <QBuffer>

#include "rtpclient.h"
#include "wiringPi.h"
#include "cv_thread.h"
#include "cv_camera.h"

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTcpServer *sever;
    QTcpSocket *client;
    void client_connect();
    void client_readyRead();
    QTimer *timer;
    void check_status();
    int key_clock, key_door, key_clock0, key_door0, door;
    bool vedio_sending;
    rtpClient *rclient;
    cv_camera* camera;
    Mat img;
    int cam_status;
    QTimer *cam_timer;
    void get_picture();
    QUdpSocket* sender;

private slots:
    void on_connect_button_clicked();

    void on_disconnect_button_clicked();

    void on_close_door_clicked();

    void on_test_rtp_clicked();

    void on_take_picture_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
