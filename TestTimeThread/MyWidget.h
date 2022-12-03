#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "ThreadOne.h"
#include <QtWidgets>
namespace Ui {
    class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
protected:
    void dealclicked();
    void dealtimeout();
private:
    Ui::MyWidget *ui;
    QTimer *timer;
    ThreadOne *p_thread;

private slots:
    void dealover();//处理新线程返回的结束信号
};

#endif // MYWIDGET_H
