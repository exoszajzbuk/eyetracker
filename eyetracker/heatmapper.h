#ifndef HEATMAPPER_H
#define HEATMAPPER_H

#include "heatmapperwindow.h"
#include "session.h"

#include <QProgressBar>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MainWindow;

class Heatmapper
{
public:
    Heatmapper();
    ~Heatmapper();

    void setMainWindow(MainWindow*);
    void generate(const Session&, QProgressBar*);

private:
    HeatmapperWindow* window;
    MainWindow* mainWindow;

    Session session;
    Mat background;
};

#endif // HEATMAPPER_H
