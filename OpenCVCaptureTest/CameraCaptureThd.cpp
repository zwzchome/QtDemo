#include "CameraCaptureThd.h"

#include <QDebug>
#include <QDateTime>

CCameraCaptureThd::CCameraCaptureThd(QObject* parent) : CBaseThd(parent)
{

}

CCameraCaptureThd::~CCameraCaptureThd()
{
    StopCapture();
    if (m_capture.isOpened())
    {
        m_capture.release();
    }
}

bool CCameraCaptureThd::Init()
{
    int index = 5;

    while (true)
    {
        m_bInit = m_capture.open(0+cv::CAP_DSHOW, cv::CAP_ANY);  // 打开默认摄像头
        m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
        m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
        m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        if (m_bInit)
        {

            return true;
        }
        else
        {
            index--;
            QThread::sleep(1);
            if (index < 0)
            {
                qDebug() << "--> Failed to initialize camera!";
                return false;
            }
        }

    }
    return true;
}

void CCameraCaptureThd::StartCapture()
{
    m_bCapture = true;
    double dExpore = m_capture.get(cv::CAP_PROP_EXPOSURE);
    double dBrightness = m_capture.get(cv::CAP_PROP_BRIGHTNESS);
    qDebug() << "---> dExpore:"<<dExpore<<"||"<<dBrightness;
}

void CCameraCaptureThd::StopCapture()
{
    m_bCapture = false;
}

bool CCameraCaptureThd::SetCameraBrightness(const int &_iBrightness)
{
    bool bRet = false;
    if (false == m_capture.isOpened())
    {
        return bRet;
    }
    qDebug() << "---> CCameraCaptureThd::SetCameraBrightness _iBrightness:"<<_iBrightness;
    bRet = m_capture.set(cv::CAP_PROP_BRIGHTNESS, _iBrightness);
    double dBrightness = m_capture.get(cv::CAP_PROP_BRIGHTNESS);
    qDebug() << "---> CCameraCaptureThd dBrightness:"<<dBrightness;
    return bRet;
}

bool CCameraCaptureThd::SetCameraExposure(const int &_iExposure)
{
    bool bRet = false;
    if (false == m_capture.isOpened())
    {
        return bRet;
    }
    qDebug() << "---> CCameraCaptureThd::SetCameraExposure _iExposure:"<<_iExposure;
    bRet = m_capture.set(cv::CAP_PROP_EXPOSURE, _iExposure);

    double dExpore = m_capture.get(cv::CAP_PROP_EXPOSURE);
    qDebug() << "---> CCameraCaptureThd dExpore:"<<dExpore;
    return bRet;
}

void CCameraCaptureThd::run()
{
    std::thread captureThread(&CCameraCaptureThd::captureThreadFunc, this);
    captureThread.detach();  // 捕获线程独立运行

    while (!m_bExit)
    {
        cv::Mat frame;

        {
            std::unique_lock<std::mutex> lock(m_mtxQueue);
            if (m_frameQueue.empty())
            {
                m_cvQueue.wait(lock, [this] { return !m_frameQueue.empty() || m_bExit; });
            }

            if (m_bExit) break;

            frame = m_frameQueue.front();
            m_frameQueue.pop();
        }

        if (!frame.empty())
        {
            processFrame(frame);
            calculateFps(); // 每处理一帧计算一次 FPS
        }
    }
}

void CCameraCaptureThd::captureThreadFunc()
{
    while (!m_bExit)
    {
        cv::Mat frame;

        if (m_capture.isOpened() && m_bCapture)
        {
            m_capture.read(frame);

            if (frame.empty() || !_CheckMat(frame))
            {
                if (!m_bWrongCamera)
                {
                    emit SIGNAL_CameraWrong();
                    m_bWrongCamera = true;
                }
                continue;
            }

            std::lock_guard<std::mutex> lock(m_mtxQueue);
            if (m_frameQueue.size() >= m_maxQueueSize)
            {
                m_frameQueue.pop();  // 丢弃旧帧
            }
            m_frameQueue.push(frame);

            m_cvQueue.notify_one();
        }

        QThread::msleep(m_iImageCaptureSleepTime);
    }
}

void CCameraCaptureThd::processFrame(const cv::Mat& frame)
{
    cv::Mat processedFrame;
    frame.copyTo(processedFrame);

    cv::cvtColor(processedFrame, processedFrame, cv::COLOR_BGR2RGB);
    cv::flip(processedFrame, processedFrame, 1);  // Y 轴翻转

    QImage img = QImage((uint8_t*)processedFrame.data, processedFrame.cols, processedFrame.rows, QImage::Format_RGB888).copy();
    //qDebug() << "---> processFrame:"<<QDateTime::currentMSecsSinceEpoch();
    emit SIGNAL_UpdateRgbImage(img);
}

void CCameraCaptureThd::calculateFps()
{
    ++m_frameCount;

    auto now = std::chrono::steady_clock::now();
    double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime).count();

    if (elapsedTime >= 1000.0) // 每秒更新一次 FPS
    {
        m_currentFps = m_frameCount * (1000.0 / elapsedTime);
        m_frameCount = 0;
        m_startTime = now;

        qDebug() << "Current FPS:" << m_currentFps;
        emit SIGNAL_FrameRate(m_currentFps);
    }
}

bool CCameraCaptureThd::_CheckMat(const cv::Mat& mat)
{
    return !mat.empty() && mat.cols > 0 && mat.rows > 0;
}
