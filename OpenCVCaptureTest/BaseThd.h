#ifndef BASETHD_H
#define BASETHD_H

#include <QThread>

class CBaseThd : public QThread
{
    Q_OBJECT
public:
    explicit CBaseThd(QObject *parent = 0) :
        QThread(parent),
        m_bExit(false)
    {

    }

public:
    virtual void ExitThd()
    {
        m_bExit = true;
        wait(1000);
    }

    virtual void StartThd()
    {
        m_bExit = false;
        start();
    }

signals:
    
public slots:
    
protected:
    volatile bool m_bExit;
};

#endif // BASETHD_H
