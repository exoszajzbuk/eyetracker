#include "recorder.h"
#include "helper.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

Recorder::Recorder()
{
    state = None;
    session = NULL;
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

    // create new session
    session = new Session();
    session->setImage(window->getImage());

    // set state
    state = Recording;
}

void Recorder::recordPoint(Point2f p)
{
    session->addPoint(p);
}

void Recorder::stopRecording()
{
    // set state
    state = None;

    if (window != NULL)
    {
        delete window;
        window = NULL;
    }

    // save session
    qDebug("saving session");
    session->save(Helper::timeToTimestamp(time(0)));

    if (session != NULL)
    {
        delete session;
        session = NULL;
    }
}

Recorder::RecorderState Recorder::getState()
{
    return state;
}


