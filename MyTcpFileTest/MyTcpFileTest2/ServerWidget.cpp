#include "ServerWidget.h"

CServerWidget::CServerWidget(QWidget *parent)
    : QWidget(parent)
{
    initServerWidget();
    //这里是倘若建立了一个新的连接，执行槽函数serverNewConnection中的内容。
    connect(tcpServer, SIGNAL(newConnection()),this,SLOT(serverNewConnection()));
    //获取完服务端的文件，等待send按钮被按下,先只是发送头部信息而已，并开启一个小型定时器。等定时器执行完，执行下面有一个connect函数
    connect(buttonSend,SIGNAL(pressed()), this, SLOT(on_buttonSend_clicked()));
    //加入server可以从tcp中得到readyread的信号的话，证明估计client应该已经结束了。
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(slotClientReceiveOver()));
    connect(&timer, &QTimer::timeout,
            [=]()
    {
        qDebug()<<"ServerWidget timer";
        //关闭定时器
        timer.stop();
        //发送文件
        sendData();
    }
    );
}

CServerWidget::~CServerWidget()
{

}

//本小类用于初始化Server
void CServerWidget::initServerWidget()
{
    ServerWidget = new QWidget(this);
    ServerWidget->resize(574, 422);
    tcpServer = new QTcpServer(this);
    //监听
    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("Server Port：8888");
    buttonSend = new QPushButton(this);
    buttonSend->setText("Send File");
}

//本小类用客户端接收完信息后，会将file done写入socket，当服务端读取到file done时，则关闭socket连接
void CServerWidget::slotClientReceiveOver()
{
    QByteArray buf = tcpSocket->readAll();
    if(QString(buf) == "file done")
    {//文件接收完毕
        file.close();
        //断开客户端端口
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }

}

//这里的函数可获取客户端的ip和port
void CServerWidget::serverNewConnection()
{
    qDebug()<<"CServerWidget::serverNewConnection";
    //取出建立好连接的套接字
    tcpSocket = tcpServer->nextPendingConnection();
    //获取对方的ip和端口
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();
    QString str = QString("[%1:%2] success connect").arg(ip).arg(port);
    qDebug()<<"CServerWidget::serverNewConnection peer Connect ip and port is"<<ip<<port;
    getServerFile();//获取服务端文件
}

//本小类用于当点击send按钮时，则发送头部信息，并开启定时器，防止黏包。
void CServerWidget::on_buttonSend_clicked()
{

    //buttonSend->setEnabled(false);
    //qDebug()<<"buttonSend";
    //先发送文件头信息  文件名##文件大小
    qDebug()<<"CServerWidget::on_buttonSend_clicked"<<fileName<<fileSize;
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //发送头部信息
    qint64 len = tcpSocket->write( head.toUtf8() );
    qDebug()<<"CServerWidget::on_buttonSend_clicked"<<len;
    //qint64 len = 10;
    if(len > 0)//头部信息发送成功
    {
        qDebug()<<"CServerWidget::on_buttonSend_clicked sendhead success";
        //发送真正的文件信息
        //防止TCP黏包
        //需要通过定时器延时 20 ms
        timer.start(20);
    }
    else
    {
        qDebug() << "The head message send fail 142";
        file.close();

        //buttonSend->setEnabled(false);
    }
}

//本小类用于发送真正的数据，这里固定每次发送数据的大小，进行统一发送
void CServerWidget::sendData()
{
    qDebug()<<"exec server sendData";
    qint64 len = 0;
    //QByteArray array = file.readAll();
    //tcpSocket->write(array);
    //tcpSocket->readAll();
    sendSize =0;
    do
    {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;
        //往文件中读数据
        len = file.read(buf, sizeof(buf));
        //发送数据，读多少，发多少
        //len = tcpSocket->
        len = tcpSocket->write(buf, len);
        //发送的数据需要累积
        sendSize += len;
    }while(len > 0);
    qDebug()<<"CServerWidget::sendData"<<sendSize;
}

//这里用于获取要传输的文件
void CServerWidget::getServerFile()
{
    qDebug()<<"CServerWidget::getServerFile() exec";
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    qDebug()<<"filePath"<<filePath;
    if(false == filePath.isEmpty()) //如果选择文件路径有效
    {
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName(); //获取文件名字
        fileSize = info.size(); //获取文件大小
        sendSize = 0; //发送文件的大小

        //只读方式打开文件
        //指定文件的名字
        file.setFileName(filePath);
        //打开文件
        bool isOk = file.open(QIODevice::ReadOnly);
        if(false == isOk)
        {
            qDebug() << "open file fail with readonly 106";
        }
        else {
            qDebug()<<"the path is right";
        }
    }
    else
    {
        qDebug() << "Choose file path is wrong in ServerWidget getFile";
    }
}


