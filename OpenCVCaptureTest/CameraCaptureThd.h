#ifndef CAMERACAPTURETHD_H
#define CAMERACAPTURETHD_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "Singleton.h"
#include "BaseThd.h"

class CCameraCaptureThd : public CBaseThd
{
    Q_OBJECT

public:
    explicit CCameraCaptureThd(QObject* parent = nullptr);
    ~CCameraCaptureThd();

    bool Init();
    void StartCapture();
    void StopCapture();
    bool SetCameraBrightness(const int &_iBrightness);
    bool SetCameraExposure(const int &_iExposure);
signals:
    void SIGNAL_UpdateRgbImage(const QImage& img);
    void SIGNAL_CameraWrong();
    void SIGNAL_FrameRate(double _iRate);
protected:
    void run() override;
private:
    void calculateFps();
private:
    bool m_bInit = false;
    bool m_bExit = false;
    bool m_bCapture = false;
    bool m_bWrongCamera = false;

    cv::VideoCapture m_capture;
    std::queue<cv::Mat> m_frameQueue;
    std::mutex m_mtxQueue;
    std::condition_variable m_cvQueue;

    int m_iImageWaitTime = 33;  // 默认帧间隔时间 (ms)
    int m_iImageCaptureSleepTime = 10; // 捕获线程睡眠时间 (ms)
    const size_t m_maxQueueSize = 5; // 最大队列长度

    // FPS 相关
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    int m_frameCount = 0;
    double m_currentFps = 0.0;

    void captureThreadFunc();
    void processFrame(const cv::Mat& frame);
    bool _CheckMat(const cv::Mat& mat);

private:
    DECLARE_SINGLETON_CLASS(CCameraCaptureThd)
};

typedef CSingleton<CCameraCaptureThd> gCameraCaptureThd;

#endif // CAMERACAPTURETHD_H
