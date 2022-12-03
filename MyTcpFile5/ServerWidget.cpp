#include "ServerWidget.h"

CServerWidget::CServerWidget(QWidget *parent)
    : QWidget(parent)
{
    createUi();
    //p_saveProject->saveProject();
    //监听套接字
    tcpServer = new QTcpServer(this);

    //监听
    tcpServer->listen(QHostAddress::Any, 8888);
    setWindowTitle("Server Port：8888");
    //两个按钮都不能按
    buttonFile->setEnabled(false);
    buttonSend->setEnabled(false);

    //如果客户端成功和服务器连接
    //tcpServer会自动触发 newConnection()
    connect(tcpServer, SIGNAL(newConnection()),this,SLOT(serverNewConnection()));
    connect(&timer, &QTimer::timeout,
            [=]()
    {
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
void CServerWidget::serverNewConnection()
{
    qDebug()<<"has a new Connnection";

    //取出建立好连接的套接字
    tcpSocket = tcpServer->nextPendingConnection();
    //获取对方的ip和端口
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();

    QString str = QString("[%1:%2] success connect").arg(ip).arg(port);
    textEdit->setText(str); //显示到编辑区

    //成功连接后，才能按选择文件
    buttonFile->setEnabled(true);
    connect(buttonFile,SIGNAL(pressed()), this, SLOT(on_buttonFile_clicked()));
    connect(buttonSend,SIGNAL(pressed()), this, SLOT(on_buttonSend_clicked()));
    connect(tcpSocket, &QTcpSocket::readyRead,
            [=]()
    {
        //取客户端的信息
        QByteArray buf = tcpSocket->readAll();
        if(QString(buf) == "file done")
        {//文件接收完毕
            textEdit->append("File send over");
            file.close();

            //断开客户端端口
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
        }
    }
    );

}
//选择文件的按钮
void CServerWidget::on_buttonFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
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

        //提示打开文件的路径
        textEdit->append(filePath);
        buttonFile->setEnabled(false);
        buttonSend->setEnabled(true);
    }
    else
    {
        qDebug() << "Chose file path is wrong! 118";
    }

}
//发送文件按钮
void CServerWidget::on_buttonSend_clicked()
{
    buttonSend->setEnabled(false);
    qDebug()<<"buttonSend";
    //先发送文件头信息  文件名##文件大小
    QString head = QString("%1##%2").arg(fileName).arg(fileSize);
    //发送头部信息
    qint64 len = tcpSocket->write( head.toUtf8() );
    if(len > 0)//头部信息发送成功
    {
        //发送真正的文件信息
        //防止TCP黏包
        //需要通过定时器延时 20 ms
        timer.start(20);
    }
    else
    {
        qDebug() << "The head message send fail 142";
        file.close();
        buttonFile->setEnabled(true);
        buttonSend->setEnabled(false);
    }
}

void CServerWidget::sendData()
{
    textEdit->append("is sending");
    qDebug()<<"exec server sendData";
    qint64 len = 0;
    //QByteArray array = file.readAll();
    //tcpSocket->write(array);
    //tcpSocket->readAll();
    do
    {
        //每次发送数据的大小
        char buf[4*1024] = {0};
        len = 0;
        //往文件中读数据
        len = file.read(buf, sizeof(buf));
        //发送数据，读多少，发多少

        len = tcpSocket->write(buf, len);
        //发送的数据需要累积
        sendSize += len;

    }while(len > 0);
    //     //是否发送文件完毕
    //     if(sendSize == fileSize)
    //     {
    //         ui->textEdit->append("文件发送完毕");
    //         file.close();

    //         //把客户端端口
    //         tcpSocket->disconnectFromHost();
    //         tcpSocket->close();
    //     }
}


void CServerWidget::createUi()
{
    ServerWidget = new QWidget(this);
    ServerWidget->resize(574, 422);
    buttonFile = new QPushButton();
    buttonSend = new QPushButton();
    ServerWidget->resize(574, 422);
    gridLayout = new QGridLayout(ServerWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    horizontalSpacer = new QSpacerItem(130, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer, 0, 0, 1, 2);
    label = new QLabel(ServerWidget);
    label->setObjectName(QString::fromUtf8("label"));
    QFont font;
    font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
    font.setPointSize(24);
    label->setFont(font);
    label->setText("Server");
    label->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(label, 0, 2, 1, 1);
    horizontalSpacer_2 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_2, 0, 3, 1, 2);
    textEdit = new QTextEdit(ServerWidget);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setReadOnly(true);
    gridLayout->addWidget(textEdit, 1, 0, 1, 5);
    buttonFile = new QPushButton(ServerWidget);
    buttonFile->setText("Chose File");
    buttonFile->setObjectName(QString::fromUtf8("buttonFile"));
    gridLayout->addWidget(buttonFile, 2, 0, 1, 1);
    horizontalSpacer_3 = new QSpacerItem(217, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(horizontalSpacer_3, 2, 1, 1, 3);
    buttonSend = new QPushButton(ServerWidget);
    buttonSend->setText("Send File");
    buttonSend->setObjectName(QString::fromUtf8("buttonSend"));
    gridLayout->addWidget(buttonSend, 2, 4, 1, 1);
    ServerWidget->setLayout(gridLayout);
    ServerWidget->setWindowTitle(QApplication::translate("ServerWidget", "ServerWidget", nullptr));
    label->setText(QApplication::translate("ServerWidget", "\346\234\215\345\212\241\345\231\250", nullptr));
    buttonFile->setText(QApplication::translate("ServerWidget", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
    buttonSend->setText(QApplication::translate("ServerWidget", "\345\217\221\351\200\201\346\226\207\344\273\266", nullptr));
}
