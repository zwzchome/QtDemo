#ifndef THREADONE_H
#define THREADONE_H

#include <QObject>
#include <QtWidgets>
//#include "MyWidget.h"
class ThreadOne : public QThread
{
    Q_OBJECT
public:
    explicit ThreadOne(QObject *parent = nullptr);

signals:
     void over();
public slots:
protected:
    void run();//多线程执行的内容将通过重新该虚函数实现
};

#endif // THREADONE_H
