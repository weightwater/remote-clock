#include "cv_thread.h"

cv_thread::cv_thread()
{
    ;
}

void cv_thread::run()
{
    Mat frame;
    int tmp;
//        while(true)
//        {
//            if(*cam_status == 0)
//            {
//                cout << *cam_status << endl;
//                continue;
//            }
//            cout << *cam_status << endl;
//        }
    // namedWindow("show camera", 1);
    for(;;)
    {
        cout << 1 << endl;
        cam->cap >> frame;
        if(!frame.empty())
        {
            // imshow("show camera",  frame);
            cout << "get picture" << endl;
        }
        if((tmp = waitKey(30)) >= 0)
        {
            cout << tmp << endl;
            destroyAllWindows();
            break;
        }
    }
}
