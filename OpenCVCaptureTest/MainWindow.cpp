#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CameraCaptureThd.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(gCameraCaptureThd::instance(), &CCameraCaptureThd::SIGNAL_UpdateRgbImage, this, &MainWindow::SLOT_UpdateRgbImage);
    connect(gCameraCaptureThd::instance(), &CCameraCaptureThd::SIGNAL_FrameRate,this, &MainWindow::SLOT_FrameRate);
    _Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_Init()
{
    gCameraCaptureThd::instance()->Init();
    gCameraCaptureThd::instance()->StartThd();
}

void MainWindow::on_btnStart_clicked()
{
    gCameraCaptureThd::instance()->StartCapture();
}

void MainWindow::on_btnStop_clicked()
{
    gCameraCaptureThd::instance()->StopCapture();
}

void MainWindow::SLOT_UpdateRgbImage(const QImage &_img)
{
    m_oImg = _img.copy();
    ui->wgtCamera->SLOT_NewFrame(m_oImg);
}

void MainWindow::SLOT_FrameRate(double _dRate)
{
    ui->lblFrameRate->setText(QString::number(_dRate));
}

void MainWindow::on_btnSetBrightness_clicked()
{
    gCameraCaptureThd::instance()->SetCameraBrightness(ui->edtBrightness->text().toInt());
}

void MainWindow::on_btnSetExposure_clicked()
{
    gCameraCaptureThd::instance()->SetCameraExposure(ui->edtExposure->text().toInt());
}
