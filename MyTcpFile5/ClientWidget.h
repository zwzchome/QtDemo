#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QTcpServer> //监听套接字
#include <QTcpSocket> //通信套接字
#include <QFile>
#include <QTimer>
#include "ServerTreeView.h"
class CClientWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CClientWidget(QWidget *parent = nullptr);
    ~CClientWidget();
signals:


private slots:
    void on_buttonConnect_clicked();
    void readFile();
protected:
    void createUi();
public:
    QWidget *ClientWidget;
    QGridLayout *gridLayout_2;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEditPort;
    QLineEdit *lineEditIP;
    QLabel *label_2;
    QPushButton *buttonConnect;
    QProgressBar *progressBar;
    QTextEdit *textEdit;
    CServerTreeView *p_treeView;

private:
    QTcpSocket *tcpSocket;
    QFile file; //文件对象
    QString fileName; //文件名字
    qint64 fileSize; //文件大小
    qint64 recvSize; //已经接收文件的大小
    bool isStart;   //标志位，是否为头部信息
};

#endif // CLIENTWIDGET_H
