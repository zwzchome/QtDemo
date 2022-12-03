#include "MyWidget.h"
#include "ui_MyWidget.h"
#include <ThreadOne.h>
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    p_thread = new ThreadOne(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MyWidget::dealclicked);//按下按钮后执行dealclicked()槽函数
    connect(timer,&QTimer::timeout,this,&MyWidget::dealtimeout);//根据定时器发出的信号更新LCD显示器的数字
    connect(p_thread,&ThreadOne::over,this,&MyWidget::dealover);//当开辟的线程内的复杂函数执行完后,发出over信号,接收到该信号后便停下计时器

}

MyWidget::~MyWidget()
{
    delete ui;
}
void MyWidget::dealclicked()
{
    timer->start(200);//启动计时器,每0.1秒发出一次信号
    p_thread->start();//QThread 的对象通过start()函数调用线程文件中的run()函数
}
void MyWidget::dealtimeout()//更新LCD显示器的数字
{
    static int time = 0;
    ui->lcdNumber->display(time);
    time++;
}
void MyWidget::dealover()//接收到信号后停下计时器
{
    timer->stop();
}
