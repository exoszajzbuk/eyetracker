#ifndef RECORDER_H
#define RECORDER_H

#include "recordingwindow.h"
#include "session.h"

class MainWindow;

class Recorder
{
public:
    Recorder();

    enum RecorderState
    {
        None,
        Recording
    };

    RecorderState getState();
    void recordPoint(Point2f);

    // getter/setter for mainwindow
    void setMainWindow(MainWindow*);
    MainWindow* getMainWindow();

    void startRecording();
    void stopRecording();

private:
    RecorderState state;

    RecordingWindow* window;
    MainWindow* mainWindow;

    Session* session;

    Point2f position;

};

#endif // RECORDER_H
