#ifndef RECORDINGWINDOW_H
#define RECORDINGWINDOW_H

#include <QDialog>

namespace Ui {
class RecordingWindow;
}

class Recorder;

class RecordingWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit RecordingWindow(QWidget *parent = 0);
    ~RecordingWindow();
    void reject();
    
    QPixmap getImage();
    void setRecorder(Recorder*);

private:
    Ui::RecordingWindow *ui;
    Recorder* recorder;

    QPixmap image;

};

#endif // RECORDINGWINDOW_H
