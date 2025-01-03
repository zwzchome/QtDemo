#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void _Init();
private slots:
    void on_btnStart_clicked();

    void on_btnStop_clicked();
    void SLOT_UpdateRgbImage(const QImage& _img);
    void SLOT_FrameRate(double _dRate);

    void on_btnSetBrightness_clicked();

    void on_btnSetExposure_clicked();

private:
    Ui::MainWindow *ui;

private:
     QImage m_oImg;
};

#endif // MAINWINDOW_H
