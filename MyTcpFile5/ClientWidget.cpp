#include "ClientWidget.h"
CClientWidget::CClientWidget(QWidget *parent) : QWidget(parent)
{
    createUi();
    connect(buttonConnect,SIGNAL(clicked()), this, SLOT(on_buttonConnect_clicked()));

    tcpSocket = new QTcpSocket(this);
    isStart = true;
    progressBar->setValue(0); //当前值
    setWindowTitle("Server");
    connect(tcpSocket, &QTcpSocket::connected,
            [=]()
    {
        //提示连接成功
        textEdit->clear();
        textEdit->append("connect success with server,waiting for sending……");
    }
    );
    connect(tcpSocket, SIGNAL(readyRead()),this,SLOT(readFile()));
    qDebug()<<"CServerTreeView w2";

}

//本小类用于当Socket中readyRead()时，执行读取socket中文件
void CClientWidget::readFile()
{
    //取出接收的内容
    QByteArray buf = tcpSocket->readAll();//把这个buf传递给ServerTreeView

    //p_treeView = new CServerTreeView();
    //p_treeView->initTreeView();
    qDebug()<<"buf.length"<<buf.length();

    if(true == isStart)
    {//接收头
        isStart = false;
        //解析头部信息 QString buf = "hello##1024"
        //                    QString str = "hello##1024#mike";
        //                            str.section("##", 0, 0)
        //初始化
        //文件名
        fileName = QString(buf).section("##", 0, 0);
        //文件大小
        fileSize = QString(buf).section("##", 1, 1).toInt();
        recvSize = 0;   //已经接收文件大小

        //打开文件
        //关联文件名字
        file.setFileName(fileName);

        //只写方式方式，打开文件
        bool isOk = file.open(QIODevice::WriteOnly);
        if(false == isOk)
        {
            qDebug() << "WriteOnly error 49";
            tcpSocket->disconnectFromHost(); //断开连接
            tcpSocket->close(); //关闭套接字
            return; //如果打开文件失败，中断函数
        }

        //弹出对话框，显示接收文件的信息
        qDebug()<<"filesize"<<fileSize;
        QString str;
        if(fileSize/1024<1)
        {
            str = QString("Receive file: [%1:1kb]").arg(fileName);
        }
        else
        {
            str = QString("Receive file: [%1: %2kb]").arg(fileName).arg(fileSize/1024);

        }

        //QMessageBox::information(this, "文件信息", str);
        textEdit->append(str);
        textEdit->append("is receiving..");

        //设置进度条
        progressBar->setMinimum(0); //最小值
        if(fileSize/1024<1)
        {
            qDebug()<<"fileSize is how big?"<<fileSize;
            progressBar->setMaximum(fileSize);//(value() - minimum()) / (maximum() - minimum())。
        }
        else {
            qDebug()<<"fileSize is how big?"<<fileSize/1024;
            progressBar->setMaximum(fileSize/1024); //最大值
        }

        progressBar->setValue(0); //当前值
    }
    else //文件信息
    {
        qint64 len = file.write(buf);
        if(len >0) //接收数据大于0
        {
            recvSize += len; //累计接收大小
            qDebug() << len;
        }

        //更新进度条
        qDebug()<<"client progressbar";
        if(fileSize/1024<1)
        {
            progressBar->setValue(recvSize);
        }
        else {
            progressBar->setValue(recvSize/1024);
        }

        if(recvSize == fileSize) //文件接收完毕
        {
            //先给服务发送(接收文件完成的信息)
            tcpSocket->write("file done");
            textEdit->append("file receive over");
            QMessageBox::information(this, "over", "file receive over");
            file.close(); //关闭文件
            //断开连接
            tcpSocket->disconnectFromHost();
            tcpSocket->close();
        }
    }
}
void CClientWidget::on_buttonConnect_clicked()
{
    qDebug()<<"buttonConnect";
    //获取服务器的ip和端口
    QString ip = lineEditIP->text();
    quint16 port = lineEditPort->text().toInt();

    //主动和服务器连接
    tcpSocket->connectToHost(QHostAddress(ip), port);
    isStart = true;
    //设置进度条
    progressBar->setValue(0);
}

CClientWidget::~CClientWidget()
{

}
void CClientWidget::createUi()
{
    ClientWidget = new QWidget(this);
    ClientWidget->resize(400, 300);
    if (ClientWidget->objectName().isEmpty())
        ClientWidget->setObjectName(QString::fromUtf8("ClientWidget"));
    ClientWidget->resize(400, 300);
    gridLayout_2 = new QGridLayout(ClientWidget);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    widget_2 = new QWidget(ClientWidget);
    widget_2->setObjectName(QString::fromUtf8("widget_2"));
    horizontalLayout = new QHBoxLayout(widget_2);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(125, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    label_3 = new QLabel(widget_2);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    QFont font;
    font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
    font.setPointSize(24);
    label_3->setFont(font);
    label_3->setAlignment(Qt::AlignCenter);
    horizontalLayout->addWidget(label_3);
    horizontalSpacer_2 = new QSpacerItem(125, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer_2);
    gridLayout_2->addWidget(widget_2, 0, 0, 1, 1);
    widget = new QWidget(ClientWidget);
    widget->setObjectName(QString::fromUtf8("widget"));
    gridLayout = new QGridLayout(widget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(widget);
    label->setObjectName(QString::fromUtf8("label"));
    gridLayout->addWidget(label, 0, 0, 1, 1);
    lineEditPort = new QLineEdit(widget);
    lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));
    gridLayout->addWidget(lineEditPort, 2, 1, 1, 1);
    lineEditIP = new QLineEdit(widget);
    lineEditIP->setObjectName(QString::fromUtf8("lineEditIP"));
    gridLayout->addWidget(lineEditIP, 0, 1, 1, 1);
    label_2 = new QLabel(widget);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    gridLayout->addWidget(label_2, 1, 0, 2, 1);
    buttonConnect = new QPushButton(widget);
    buttonConnect->setObjectName(QString::fromUtf8("buttonConnect"));
    gridLayout->addWidget(buttonConnect, 0, 2, 3, 1);
    gridLayout_2->addWidget(widget, 1, 0, 1, 1);
    progressBar = new QProgressBar(ClientWidget);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setValue(24);
    gridLayout_2->addWidget(progressBar, 2, 0, 1, 1);
    textEdit = new QTextEdit(ClientWidget);
    textEdit->setObjectName(QString::fromUtf8("textEdit"));
    textEdit->setReadOnly(true);
    gridLayout_2->addWidget(textEdit, 3, 0, 1, 1);
    // retranslateUi(ClientWidget);
    QMetaObject::connectSlotsByName(ClientWidget);

    ClientWidget->setWindowTitle(QApplication::translate("ClientWidget", "Form", nullptr));
    label_3->setText(QApplication::translate("ClientWidget", "\345\256\242\346\210\267\347\253\257", nullptr));
    label->setText(QApplication::translate("ClientWidget", "\346\234\215\345\212\241\345\231\250\347\232\204IP:", nullptr));
    lineEditPort->setText(QApplication::translate("ClientWidget", "8888", nullptr));
    lineEditIP->setText(QApplication::translate("ClientWidget", "127.0.0.1", nullptr));
    label_2->setText(QApplication::translate("ClientWidget", "\346\234\215\345\212\241\345\231\250\347\232\204\347\253\257\345\217\243:", nullptr));
    buttonConnect->setText(QApplication::translate("ClientWidget", "connect", nullptr));
}
