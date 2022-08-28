#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sever = new QTcpServer(this);
    this->client = new QTcpSocket(this);
    ui->disconnect_button->setEnabled(false);
    connect(client, &QTcpSocket::connected, this, &MainWindow::client_connect);
    connect(client, &QTcpSocket::readyRead, this, &MainWindow::client_readyRead);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::check_status);
    key_clock0 = 1;
    key_door0 = 0;
    timer->start(100);

    vedio_sending = false;

    wiringPiSetup();
    pinMode(21, INPUT);
    pinMode(7, INPUT);
    pullUpDnControl(21, PUD_UP);
    pullUpDnControl(7, PUD_UP);

    pinMode(29, OUTPUT);
    digitalWrite(29, LOW);

    // rclient = new rtpClient("192.168.3.5", 8888, 6668);
    cam_status = 0;

//    cv_thread *cvThread = new cv_thread();
    camera = new cv_camera();
    cam_timer = new QTimer(this);
    connect(cam_timer, &QTimer::timeout, this, &MainWindow::get_picture);
//    cvThread->cam = camera;
//    cvThread->cam_status = &cam_status;
//    cvThread->start();

    sender = new QUdpSocket(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::client_connect()
{
    ui->log->append("connecting~~~");
}

void MainWindow::on_connect_button_clicked()
{
    client->connectToHost(ui->server_ip->text(), ui->server_port->text().toUInt());
    if(!client->waitForConnected())
    {
        qDebug() << "client connect error";
        ui->log->append("client connect error");
    }
    else
    {
        qDebug() << "connect success";
        ui->connect_button->setEnabled(false);
        ui->log->append("connect success");
        ui->disconnect_button->setEnabled(true);
    }
}

void MainWindow::client_readyRead()
{
    QString data = client->readAll();
    ui->log->append("recieve>>" + data);
    if(data.length() == 1 && data[0] == 'd')
    {
        ui->test_door->setText("open");
        key_door = 1;
        ui->log->append("door was opened");
        digitalWrite(29, HIGH);
    }
    else if(data.length() == 1 && data[0] == 'v')
    {
        if(vedio_sending)
        {
            vedio_sending = false;
            ui->log->append("stop send vedio~");
        }
        else
        {
            vedio_sending = true;
            ui->log->append("start send vedio");
        }
    }
    else if(data.length() == 1 && data[0] == 's')
    {
        ui->log->append("stop send vedio");
    }
}

void MainWindow::on_disconnect_button_clicked()
{
    ui->log->append("====================disconected==================");
    client->close();
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
}


void MainWindow::check_status()
{
    key_clock = digitalRead(21);
    key_door = digitalRead(7);
    if(key_clock == 0 && key_clock0 == 1)
    {
        QString data = "c";
        if(client->isWritable())
            client->write(data.toUtf8());
        qDebug() << "send >> clock request";
        ui->log->append("send >> clock request");

        vedio_sending = true;
        ui->log->append("vedio start send~");
    }
    if(key_door == 0 && key_door0 == 1)
    {
        door = 0;
        qDebug() << "door was closed";
        ui->log->append("door was closed");
        ui->test_door->setText("close");
        digitalWrite(29, LOW);
        client->write("d");
    }
    key_door0 = key_door;
    key_clock0 = key_clock;
}

void MainWindow::on_close_door_clicked()
{
    // key_door = 0;
}

void MainWindow::on_test_rtp_clicked()
{
    // int message = 123;
    // rclient->send_message(&message, rclient->destport, rclient->ip);
}

void MainWindow::on_take_picture_clicked()
{
//    camera->show_camera();
//    ui->log->append("123123123123");
//    cam_status = cam_status == 1? 0: 1;
//    ui->log->append("vedio set~");
//    ui->log->append(QString::number(cam_status));
    if(cam_status)
    {
        cam_timer->stop();
        cam_status = 0;
    }
    else
    {
        cam_timer->start(100);
        cam_status = 1;
    }
}

void MainWindow::get_picture()
{
    Mat frame;
    camera->cap >> frame;
    cvtColor(frame, frame, CV_BGR2RGB);
    QImage srcQImage;
    srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
    ui->show_camera->setPixmap(QPixmap::fromImage(srcQImage));
    ui->show_camera->resize(srcQImage.size());
    ui->show_camera->show();
    // rclient->send_message(&frame, rclient->destport, rclient->ip);
    QByteArray byte;
    QBuffer buff(&byte);
    srcQImage.save(&buff, "JPEG");
    QByteArray compressByte = qCompress(byte, 1);
    QByteArray base648Byte = compressByte.toBase64();
    sender->writeDatagram(base648Byte.data(), base648Byte.size(), QHostAddress(ui->server_ip->text()), 8888);
    qDebug() << base648Byte.size();
}
