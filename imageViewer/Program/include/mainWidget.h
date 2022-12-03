#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "ImageViewer.h"

class CMainWidget:public QWidget
{
    Q_OBJECT
public:
    CMainWidget(QWidget *parent = Q_NULLPTR);
    ~CMainWidget();
    void loadPath(QString path);//用于加载固定地址的所有图片，现在这个功能已丢弃，现在使用的是：从xml文件中进行读取
public slots:
    void slotItemChanged(QString);//需要加载的图片对象发生变化
protected:
    void createUi();
protected:
    CDeepPictureViewer *p_viewer;
    CSingleImageView   *p_imageLabel;

};

#endif // MAINWIDGET_H
