#include "CameraWidget.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

CCameraWidget::CCameraWidget(QWidget *parent) : QWidget(parent)
{
    qRegisterMetaType<QVector<QRect>>("QVector<QRect>");
    m_oPenColor = QColor(243, 156, 17);
}

CCameraWidget::~CCameraWidget()
{

}

void CCameraWidget::Reset()
{
    m_imgCapture = QImage();
    update();
}

bool CCameraWidget::SnapPicture(const QString& _csSavePath)
{
    return m_imgCapture.save(_csSavePath, "JPEG");
}

QList<QPoint> CCameraWidget::GetLinePoint()
{
    return m_points;
}

void CCameraWidget::SetVirtualPoint(QPoint _point)
{
    m_virtualPoints.append(_point);
    update();
}

void CCameraWidget::ClearVirtualPoint()
{
    m_virtualPoints.clear();
    update();
}

void CCameraWidget::SetCalibrateFlag(const bool &_bFlag)
{
    m_bOpenCalibrate = _bFlag;
}

void CCameraWidget::ClearCalibratePoint()
{
    m_points.clear();
}

void CCameraWidget::mousePressEvent(QMouseEvent *event)
{
    emit SIGNAL_MousePress(event);
    if (event->buttons() & Qt::LeftButton)
    {
        qDebug() << "CCameraWidget::mousePressEvent: x : "<<event->globalX()<<", y : "<<event->globalY();
        if (m_bOpenCalibrate)
        {
            m_points.append(event->pos());
            update();
        }
    }
}

void CCameraWidget::SLOT_NewFrame(const QImage& _img)
{
    if (_img.isNull())
        return;
    m_imgCapture = _img;

    update();
}

void CCameraWidget::SLOT_NewFrame(const QImage &_img, QVector<QRect> _cvtFacePos)
{
    m_vtRc = _cvtFacePos;
    SLOT_NewFrame(_img);
}

void CCameraWidget::paintEvent(QPaintEvent *)
{
    if (m_imgCapture.isNull())
        return;

    QPainter painter(this);
    QRect rectImage = _CalculateImageRect();
    painter.drawImage(rectImage, m_imgCapture);

    painter.setPen(QPen(m_oPenColor, 2));

    QRectF rcF;
    int iImageWidth = m_imgCapture.width();
    int iImageHeight = m_imgCapture.height();
    float fRadioW = width() * 1.0 / iImageWidth;
    float fRadioH = height() * 1.0 / iImageHeight;
    for (QRect rc : m_vtRc)
    {
        if (rc.isNull() || rc.isEmpty() || !rc.isValid())
        {
            continue;
        }
        rcF.setX(rc.x() * fRadioW);
        rcF.setY(rc.y() * fRadioH);
        rcF.setWidth(rc.width() * fRadioW);
        rcF.setHeight(rc.height() * fRadioH);
        painter.drawRect(rcF);
    }

    QPainter redPainter(this);
    redPainter.setPen(Qt::red);

    for (int i = 1; i < m_points.size(); i++)
    {
        redPainter.drawLine(m_points[i-1], m_points[i]);
    }

    QPen pen(Qt::green, 5);
    redPainter.setPen(pen);
    for (int i = 0; i < m_virtualPoints.size(); i++)
    {
        redPainter.drawEllipse(m_virtualPoints[i], 2, 2);
    }
}

QRect CCameraWidget::_CalculateImageRect()
{
    QRect rectImage;
    int iImageX;
    int iImageY;
    int iImageW = m_imgCapture.width();
    int iImageH = m_imgCapture.height();
    float fRadioW = QWidget::width() * 1.0 / iImageW;
    float fRadioH = QWidget::height() * 1.0 / iImageH;


    iImageW *= fRadioW;
    iImageH *= fRadioH;

    iImageX = (QWidget::width() - iImageW) / 2;
    iImageY = (QWidget::height() - iImageH) / 2;

    rectImage.setX(iImageX);
    rectImage.setY(iImageY);
    rectImage.setWidth(iImageW);
    rectImage.setHeight(iImageH);
    return rectImage;
}
