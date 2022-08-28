#ifndef CV_CAMERA_H
#define CV_CAMERA_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <QLabel>
#include <pthread.h>

using namespace std;
using namespace cv;

void* get_vedio(void* args);

class cv_camera
{
public:
    cv_camera();
    Mat get_picture();
    VideoCapture cap;
    Mat readImage;
    QLabel *label;
    pthread_t tid;
    void show_camera();
};

#endif // CV_CAMERA_H
