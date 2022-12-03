#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QTimer>

class CServerWidget : public QWidget
{
    Q_OBJECT

public:
    CServerWidget(QWidget *parent = 0);
    ~CServerWidget();
public:
    QPushButton *buttonSend;
    qint64 sendSize; //已经发送文件的大小
    void getServerFile();
public slots:
    void serverNewConnection();
    void on_buttonSend_clicked();
    void slotClientReceiveOver();
protected:
    void initServerWidget();
    void sendData();
private:
    QWidget *ServerWidget;
    //QPushButton *buttonSend;
    QTcpServer *tcpServer; //监听套接字
    QTcpSocket *tcpSocket; //通信套接字
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    QTimer timer; //定时器
};

#endif // SERVERWIDGET_H
