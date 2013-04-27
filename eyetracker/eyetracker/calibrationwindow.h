#ifndef CALIBRATIONWINDOW_H
#define CALIBRATIONWINDOW_H

#include <QDialog>
#include "calibrator.h"

namespace Ui
{
class CalibrationWindow;
}

class CalibrationWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit CalibrationWindow(QWidget *parent = 0);
    ~CalibrationWindow();
    void setCalibrator(Calibrator*);

private:
    Ui::CalibrationWindow *ui;
    Calibrator* calibrator;
};

#endif // CALIBRATIONWINDOW_H
