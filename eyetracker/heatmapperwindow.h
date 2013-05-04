#ifndef HEATMAPPERWINDOW_H
#define HEATMAPPERWINDOW_H

#include <QDialog>

namespace Ui {
class HeatmapperWindow;
}

class HeatmapperWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit HeatmapperWindow(QWidget *parent = 0);
    ~HeatmapperWindow();

    void setImage(const QImage& image);
    
private:
    Ui::HeatmapperWindow *ui;
};

#endif // HEATMAPPERWINDOW_H
