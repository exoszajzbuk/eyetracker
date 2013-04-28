#include "recorder.h"

#include <QApplication>
#include <QDesktopWidget>

Recorder::Recorder()
{
}

void Recorder::startRecording()
{
    QRect screen = QApplication::desktop()->screenGeometry(1);

    window = new RecordingWindow();
    window->move(screen.center());
    window->showFullScreen();
}

void Recorder::stopRecording()
{
    window->close();
    delete window;
}
