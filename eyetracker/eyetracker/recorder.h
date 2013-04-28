#ifndef RECORDER_H
#define RECORDER_H

#include "recordingwindow.h"

class Recorder
{
public:
    Recorder();

    void startRecording();
    void stopRecording();

private:
    RecordingWindow* window;

};

#endif // RECORDER_H
