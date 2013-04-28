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

QImage VideoHandler::convert(const Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
