#ifndef PLAYER_H
#define PLAYER_H

#include "session.h"
#include "playerwindow.h"

#include <QTimer>
#include <QObject>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class MainWindow;

class Player : public QObject
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent = 0);
    ~Player();

    void setMainWindow(MainWindow*);

    void startPlayback(const Session& session);
    void stopPlayback();

private:
    PlayerWindow* window;
    MainWindow* mainWindow;

    Session session;
    QTimer processTimer;

    Mat image;
    Mat background;
    unsigned int frame;

private slots:
    void process();
};

#endif // PLAYER_H
