#ifndef VIDEOHANDLER_H
#define VIDEOHANDLER_H

#include <QtGui/QImage>
#include <opencv2/opencv.hpp>

using namespace cv;

class VideoHandler
{
public:
    VideoHandler();

    void startVideo(int cameraIdx);
    void stopVideo();

    Mat getFrame();
    QImage convert(const Mat& mat);

private:
    VideoCapture cap;
    Mat frame;

};

#endif // VIDEOHANDLER_H
