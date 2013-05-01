#include "videohandler.h"

#include <QDebug>

VideoHandler::VideoHandler()
{

}

void VideoHandler::start(int cameraIdx)
{
    cap.open(cameraIdx);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    if(cap.isOpened())
    {
        qDebug() << "Video " <<
                ": width=" << cap.get(CV_CAP_PROP_FRAME_WIDTH) <<
                ", height=" << cap.get(CV_CAP_PROP_FRAME_HEIGHT) <<
                ", nframes=" << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
    }
    else
    {
        qDebug() << "Could not initialize capturing...\n";
        return;
    }
}

void VideoHandler::stop()
{
    //cap.release();
}

const Mat& VideoHandler::getFrame()
{
    cap >> frame;
    return frame;
}
