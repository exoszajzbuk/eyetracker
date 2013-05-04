#include "heatmapper.h"
#include "mainwindow.h"
#include "helper.h"
#include "session.h"

#include <QApplication>
#include <QDesktopWidget>

Heatmapper::Heatmapper()
{
}

Heatmapper::~Heatmapper()
{
}

void Heatmapper::setMainWindow(MainWindow *p_mainWindow)
{
    mainWindow = p_mainWindow;
}

void Heatmapper::generate(const Session &p_session, QProgressBar* progressBar)
{
    session = p_session;

    // initial background
    background = Helper::qimage2mat(session.getImage().toImage());

    // PROCESS
    int HUE_MAX = 120;
    double MAP_TR = 0.5;

    Mat tempImage = Mat(background.rows, background.cols, CV_8UC3);
    Mat accImage = Mat(background.rows, background.cols, CV_8UC3);

    // for every point
    for (unsigned int i = 0; i<session.getPoints().size(); ++i)
    {
        // update progress bar
        double percent = (double)i/(double)session.getPoints().size();
        int percentInt = static_cast<int>(percent*100);
        progressBar->setValue(percentInt+1);
        QCoreApplication::processEvents();

        // draw lines for the heatmap
        tempImage.setTo(Scalar(0,0,0));

        if (i < session.getPoints().size()-1)
        {
            cv::line(tempImage, session.getPoints().at(i), session.getPoints().at(i+1), Scalar(0, 0, 255), 8, 2);
        }

        accImage = accImage + tempImage;
    }

    cvtColor(accImage, accImage, CV_BGR2GRAY);

    // blur effect
    GaussianBlur(accImage, accImage, Size(35,35), 10, 10);

    // calculate maximum value
    unsigned char max = 0;

    for (int i=0; i<accImage.rows; i++)
    {
        for (int j=0; j<accImage.cols; j++)
        {
            if (accImage.at<unsigned char>(i,j) > max)
            {
                max = accImage.at<unsigned char>(i,j);
            }
        }
    }

    // normalize to correct range
    double ratio = (double)HUE_MAX/(double)max;
    accImage *= ratio;

    // invert
    for (int i=0; i<accImage.rows; i++)
    {
        for (int j=0; j<accImage.cols; j++)
        {
            unsigned char value = accImage.at<unsigned char>(i,j);
            //if (value != 0)
            //{
                accImage.at<unsigned char>(i,j) = HUE_MAX-value;
            //}
        }
    }

    // combine channels
    Mat satImage = Mat(background.rows, background.cols, CV_8UC3);
    Mat ones = Mat(background.rows, background.cols, CV_8UC3);

    // create mask
    inRange(accImage, Scalar(1,1,1), Scalar(255,255,255), ones);

    // combine channels
    Mat in[] = { accImage, ones };
    Mat out[] = { satImage };
    int from_to[] = { 0,0, 1,1, 1,2 };

    mixChannels(in, 2, out, 1, from_to, 3);

    // back to RGB color space
    cvtColor(satImage, satImage, CV_HSV2BGR);

    // refine edges
    GaussianBlur(satImage, satImage, Size(13,13), 1);

    // show saturation only
    //satImage.copyTo(displayImage);

    // show full image
    addWeighted(background, 1.0, satImage, MAP_TR, 0.0, background);

    progressBar->setHidden(true);
    progressBar->setValue(0);

    // set background
    QRect screen = QApplication::desktop()->screenGeometry(1);
    window = new HeatmapperWindow();
    window->move(screen.center());
    window->showFullScreen();
    window->setImage(Helper::mat2qimage(background));
}
