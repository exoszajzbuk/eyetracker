#ifndef PLAYER_H
#define PLAYER_H

#include "session.h"
#include "playerwindow.h"

#include <QTimer>
#include <QObject>

class MainWindow;

class Player : public QObject
{
public:
    Player();
    ~Player();

    void setMainWindow(MainWindow*);

    void startPlayback(const Session& session);
    void stopPlayback();

private:
    PlayerWindow* window;
    MainWindow* mainWindow;

    Session session;
    QTimer processTimer;

private slots:
    void process();
};

#endif // PLAYER_H
