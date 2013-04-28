#include "recorder.h"

#include <QApplication>
#include <QDesktopWidget>

Recorder::Recorder()
{
}

void Recorder::setMainWindow(MainWindow *p_mainWindow)
{
    mainWindow = p_mainWindow;
}

MainWindow* Recorder::getMainWindow()
{
    return mainWindow;
}

void Recorder::startRecording()
{
    QRect screen = QApplication::desktop()->screenGeometry(1);

    window = new RecordingWindow();
    window->setRecorder(this);
    window->move(screen.center());
    window->showFullScreen();
}

void Recorder::stopRecording()
{
    if (window != NULL)
    {
        delete window;
        window = NULL;
    }
}
