#include "mainWidget.h"

CMainWidget::CMainWidget(QWidget *parent)
    :QWidget(parent)
{
    createUi();
    connect(p_imageLabel,SIGNAL(signalSelectedImageName(QString)),p_viewer,SLOT(slotSelectedIndex(QString)));//当选择的图片的名字发生更改时，发送信号通知CDeepPictureViewer那个类
    connect(p_viewer,SIGNAL(signalSilderValue(int,QStringList)),p_imageLabel,SLOT(slotSlideValue(int,QStringList)));//通过类CDeepPictureViewer的滑块控制要放大显示的图像

}

CMainWidget::~CMainWidget()
{

}

//这个类是调用CDeepPictureViewer这个类中的加载固定地址的所有图片的方法
void CMainWidget::loadPath(QString path)
{
    p_viewer->loadPath(path);
}

//设计整个Ui界面
void CMainWidget::createUi()
{
    QVBoxLayout *layout = new QVBoxLayout();//这是垂直布局
    p_imageLabel = new CSingleImageView();//单一图片显示的类
    p_viewer = new CDeepPictureViewer();//多个图片进行显示的视图
    connect(p_viewer,SIGNAL(signalCurrentFileName(QString)),
            this,SLOT(slotItemChanged(QString)));//这个connect未使用了，没有跟现有功能进行关联
    p_viewer->setFixedHeight(250);//这是底下显示多张图片的那部分
    layout->addWidget(p_imageLabel);
    layout->addWidget(p_viewer);
    this->setLayout(layout);
}

//给它一个具体的文件名，就可以显示出该图片
void CMainWidget::slotItemChanged(QString fileName)
{
    p_imageLabel->loadImage(fileName);
}
