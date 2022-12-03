#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QTimer>
#include <QtCore>
#include "ClientTreeView.h"
class CClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CClientWidget(QWidget *parent = nullptr);

signals:
    void signalBufEmit(QByteArray byteArray);

public slots:
    void on_buttonConnect_clicked();
    void clientReadFile();
    void slotTreeDisplay();

private:
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小
protected:
    void initClientWidget();

public:
    QWidget *ClientWidget;
    //QPushButton *buttonConnect;
    bool isStart;   //标志位，是否为头部信息
    QByteArray bufAll;
    QTcpSocket *tcpSocket;
    CClientTreeView *p_treeView;
    QPushButton *buttonConnect;
    QPushButton *buttonDisplayTree;

};

#endif // CLIENTWIDGET_H
