
# 前言

需求：

1、如何使用OpenCV捕抓Windows电脑上USB摄像头的流、

2、采用多线程

3、获知当前摄像头的帧率。

这个需求，之前就有做了，但是由于出现了一个问题，人家摄像头的帧率目前都可以达到60帧/s 了，而我的程序获取的却还只能获取20帧/s ，那么肯定就是存在问题了。所以，目前采用了一些办法进行解决，能充分发挥其60帧/s 的优势，并能打出帧率。下面开始详细阐述。

# 正文

## 一、环境

1、Win10

2、Qt 5.8.0 MSVC2015

3、OpenCV VS2015编译的

## 二、效果展示

![121111](Image/1_如何使用OpenCV进行抓图-多线程.assets/121111.gif)

## 三、关键代码

1、抓图线程：

```c
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
```

最关键的就是run里面的函数了。

## 四、遇到的问题

### 1、帧率怎么都上不去

解决方案：更改了相机打开时的设置

更改成这样：

```c
m_bInit = m_capture.open(0+cv::CAP_DSHOW, cv::CAP_ANY);  // 打开默认摄像头
m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
```

如果是这样的，帧率就会上不去：

```c
m_capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
m_bInit = m_capture.open(0+cv::CAP_DSHOW, cv::CAP_ANY);  // 打开默认摄像头
m_capture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
```

通过查询，问题的原因应该在：

> ### **属性设置的顺序如何影响帧率**
>
> 1. **属性的优先级问题**：
>    - 摄像头通常优先处理某些属性。例如：
>      - 分辨率 (`FRAME_WIDTH`, `FRAME_HEIGHT`) 是基础属性，影响图像传感器的输出格式。
>      - 编码格式 (`FOURCC`) 决定图像流的压缩方式。
>    - 如果先设置分辨率，然后设置编码格式，编码器会根据新分辨率重新调整帧率。
>    - 反之，如果先设置编码格式，随后更改分辨率，驱动可能无法实时适配，导致性能下降。
> 2. **驱动和硬件内部调整机制**：
>    - 摄像头驱动程序在每次设置参数时，可能会重新协商帧率。例如：
>      - 设置分辨率后，驱动需要查询硬件支持的最大帧率。
>      - 如果在分辨率设置之前先设置编码格式，驱动可能会选择一个默认分辨率（通常较低），随后改变分辨率会导致帧率调整。
> 3. **参数依赖性问题**：
>    - 某些参数依赖于其他参数。例如：
>      - 不同的编码格式支持的最大帧率和分辨率是不同的。
>      - MJPG（Motion JPEG）通常支持高分辨率和高帧率，但 YUV 格式可能性能较低。
> 4. **初始化次序影响驱动的行为**：
>    - 驱动可能在第一次设置后锁定某些参数。如果设置顺序错误，后续参数可能不会生效，或需要额外的时间重新初始化。
>    - 例如，先设置编码格式再设置分辨率，可能触发两次完整的驱动初始化。
>
> ------
>
> ### **正确的设置顺序建议**
>
> 为了确保帧率最大化，可以按照以下顺序设置参数：
>
> 1. **设置分辨率**：
>    - 首先设置分辨率 (`FRAME_WIDTH` 和 `FRAME_HEIGHT`)，确保摄像头硬件和驱动调整到正确的传感器输出。
> 2. **设置帧率**（如果需要）：
>    - 如果特定帧率是关键要求，可以尝试显式设置帧率（`cv::CAP_PROP_FPS`）。
> 3. **设置编码格式**：
>    - 最后设置编码格式 (`FOURCC`)，以确保基于之前设置的分辨率和帧率进行编码。

还有另一个原因，就是我抓图与处理的线程处于同一个，这样也会拉低整个线程处理的效率。

### 2、调整USB摄像头的亮度与曝光，出现意外情况

若不按照先调曝光再调增益的顺序，有可能出现调整曝光错乱的情况，原因未知。

目前按照的逻辑是，若想亮起来，则为：曝光：0 + 亮度：64

若想暗下来，则为：曝光：-13 + 亮度:-64

## 五、资源

https://github.com/calorsol/QtDemo/tree/master/OpenCVCaptureTest

# 参考
