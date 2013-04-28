#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "videohandler.h"
#include "imageprocessor.h"
#include "calibrator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // general
    void exitClicked();
    void aboutClicked();

    // record
    void startToggled(bool state);
    void calibrateToggled(bool state);
    void recordToggled(bool state);

    // display
    void equalizedClicked();
    void thresholdedClicked();
    void blobsClicked();
    void pupilClicked();

    // playback
    void replayClicked();
    void heatMapClicked();

private:
    QString getPositionString(Point);

    Ui::MainWindow* ui;

    QLabel* localStatus;
    QLabel* screenStatus;

    QTimer timer;
    VideoHandler videoHandler;
    ImageProcessor imageProcessor;
    Calibrator calibrator;

private slots:
    void timeout();

};

#endif // MAINWINDOW_H
