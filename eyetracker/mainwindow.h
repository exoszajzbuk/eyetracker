#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "videohandler.h"
#include "imageprocessor.h"
#include "calibrator.h"
#include "recorder.h"
#include "session.h"

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

    void toggleCalibrate(bool state);
    void toggleRecord(bool state);

    // display
    void equalizedClicked();
    void thresholdedClicked();
    void blobsClicked();
    void pupilClicked();

    // playback
    void listItemSelected();
    void playClicked();
    void heatMapClicked();
    void deleteClicked();

private:
    Ui::MainWindow* ui;

    QLabel* localStatus;
    QLabel* screenStatus;

    QTimer timer;
    VideoHandler videoHandler;
    ImageProcessor imageProcessor;
    Calibrator calibrator;
    Recorder recorder;

    void findSessions();
    void refreshSessionList();
    vector<Session> sessions;

private slots:
    void timeout();
    void startRecording();
    void stopRecording();

};

#endif // MAINWINDOW_H
