#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QRect>
#include <QList>

class CCameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCameraWidget(QWidget *parent = 0);
    ~CCameraWidget();

    void Reset();
    bool SnapPicture(const QString& _csSavePath);
    QList<QPoint> GetLinePoint();
    void SetVirtualPoint(QPoint _point);
    void ClearVirtualPoint();
    Q_INVOKABLE void SetPenColor(const QColor &_oColor){m_oPenColor = _oColor;}
    void SetCalibrateFlag(const bool &_bFlag);
    void ClearCalibratePoint();

protected:
    virtual void mousePressEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *);

protected:
    QImage m_imgCapture;
    QVector<QRect> m_vtRc;
    QColor m_oPenColor;

private:
    QRect _CalculateImageRect();

public slots:
    void SLOT_NewFrame(const QImage& _img);
    void SLOT_NewFrame(const QImage& _img, QVector<QRect> _cvtFacePos);

private:
    QList<QPoint> m_points;
    QList<QPoint> m_virtualPoints;
    bool m_bOpenCalibrate = false;

signals:
    void SIGNAL_MousePress(QMouseEvent *);

};

#endif // CAMERAWIDGET_H
