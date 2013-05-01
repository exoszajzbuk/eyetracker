#ifndef PLAYERWINDOW_H
#define PLAYERWINDOW_H


#include <QDialog>

namespace Ui {
class PlayerWindow;
}

class Player;

class PlayerWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit PlayerWindow(QWidget *parent = 0);
    ~PlayerWindow();
    void reject();

    void setPlayer(Player*);
    
private:
    Ui::PlayerWindow *ui;
    Player* player;
};

#endif // PLAYERWINDOW_H
