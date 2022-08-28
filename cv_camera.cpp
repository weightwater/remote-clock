#include "cv_camera.h"

cv_camera::cv_camera()
{
    cap.open(0);
//    cap.set(CAP_PROP_FRAME_HEIGHT, 600);
//    cap.set(CAP_PROP_FRAME_WIDTH, 800);
//    cap.set(CV_CAP_PROP_FPS, 30);

    if(!cap.isOpened())
    {
        std::cout << "can't open camera" << std::endl;
    }
}

Mat cv_camera::get_picture()
{
    cap >> readImage;
    if(!cap.read(readImage))
    {
        std::cout << "not get picture" << endl;
    }
    return readImage;
}

void* get_vedio(void* args)
{
    Mat frame;
//    QImage srcQImage;
    while(1)
    {
        cout << 1 << endl;
    }
//    cv_camera *cap = (cv_camera*)args;
//    namedWindow("show camera", 1);
//    for(;;)
//    {
//        cap->cap >> frame;
//        if(!frame.empty())
//        {
//            imshow("show camera",  frame);
//            cout << 1 << endl;
//        }
//        else
//            return nullptr;
//        if(waitKey(0) >= 0)
//        {
//            cout << waitKey(0) << endl;
//        }
//    }
//    while(1)
//    {
//        cap->cap >> frame;
//        cvtColor(frame, frame, CV_BGR2RGB);
//        srcQImage = QImage((uchar*)(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
//        cap->label->setPixmap(QPixmap::fromImage(srcQImage));
//        cap->label->resize(srcQImage.size());
//        cap->label->show();
//    }
}

void cv_camera::show_camera()
{
    pthread_create(&this->tid, nullptr, get_vedio, this);
}
