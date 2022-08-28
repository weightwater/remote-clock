#ifndef CV_THREAD_H
#define CV_THREAD_H

#include <QThread>
#include "cv_camera.h"
#include <iostream>

using namespace std;

class cv_thread : public QThread
{
public:
    cv_thread();
    cv_camera* cam;
    int* cam_status;

protected:
    void run();
};

#endif // CV_THREAD_H
