#ifndef RECORDER_H
#define RECORDER_H

#include "recordingwindow.h"

class MainWindow;

class Recorder
{
public:
    Recorder();

    // getter/setter for mainwindow
    void setMainWindow(MainWindow*);
    MainWindow* getMainWindow();

    void startRecording();
    void stopRecording();

private:
    RecordingWindow* window;
    MainWindow* mainWindow;

};

#endif // RECORDER_H
