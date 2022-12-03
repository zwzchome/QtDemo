#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QtWidgets>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <iostream>
#include <QApplication>
#include <QDomDocument>
#include "ImageSlider.h"

class CImgViewElement;
class CSingleImageView;
class CDeepPictureViewer:public QWidget
{
    Q_OBJECT
public:
    CDeepPictureViewer(QWidget *parent = Q_NULLPTR);
    ~CDeepPictureViewer();
    void setDisableDelete(bool flag);
    void addImageList(QStringList imageList);
    void clearAll();
    void resizeEvent(QResizeEvent * event);
    void calculateWidget();
    bool eventFilter(QObject *watched, QEvent *event);
    void showEvent(QShowEvent *event);
    void clearAllSelected();
    void mousePressEvent(QMouseEvent *event);
    CImgViewElement* getCurrent();
    //CImgViewElement* currentPicture;
    void setUseControl(bool flag);
    void setShowImageName(bool flag);
    void setAutoSelectedFirst(bool flag);
    void setSeleceted(int index);
    void gotoPrevImage();
    void gotoNextImage();
    void loadPath(QString path);
    QStringList pictureList;
    void keyPressEvent(QKeyEvent * event);

signals:
    void signalNoSelected();
    void signalCurrentSelecedInfo(int currentIndex,int totalIndex,CImgViewElement* imgPic);
    void signalCurrentFileName(QString);
    void signalSilderValue(int value,QStringList pictureList);
    void signalDeepDelete();
protected slots:
    void slotDelete();
    void slotGotoNextPage();
    void slotGotoPrevPage();
    void slotAddPicture();
    void slotSliderValue(int value);

    void slotSelectedIndex(QString imageName);
    void deepDelete();
    void deepDeleteLayout();
protected:
    void createUi();
    void createControlWidget();
    void updatePage();
    void gotoPage(int pageNum);
    void addXmlMes(QString picturePath);//将添加的照片的路径写入xml文件
    void deleteXmlMes(QString picturePath);//将需要删除的文件路径删除
    //CImgViewElement* layoutNextPic();
    CImgViewElement* deleteImgPic();
    QStringList readFile(const QString &fileName);
    CImgViewElement* nextImgPic();
    CImgViewElement* getItemByName(QString name);
protected:
    QHBoxLayout                   *p_layout;
    CImgViewElement*               m_currentSelected;
    QVector<CImgViewElement*>      m_totalVector;//每个照片的集合容器
    //CSavePicXml                   *p_saveXml;
    int                            m_col;//设置显示的列数
    QSize                          m_size;    //当前每个控件的大小
    QStringList                    m_currentImageList;//当前的显示列表
    QStringList                    m_totalImageList;//总的图片列表
    int                            m_currentIndex;//当前选中的图片索引号
    int                            m_startIndex;  //当前起始索引号
    int                            m_endIndex;    //当前结束索引号
    int                            m_totalPageNum;//当前总页数
    int                            m_currentPage; //当前页
protected:
    bool                           m_isAutoSelected;
    bool                           m_isShowFileName;//每个img底下要展示的名字
    bool                           m_isShowPrevNextControl;//是否显示控制按钮
    QWidget                        *p_controlWidget;//控制界面
    QLabel                         *p_controlLabel; //控制界面label
    QPushButton                    *p_chooseButton;//选择某一个图片
    QPushButton                    *p_prevButton; //前一页按钮
    CImageSlider                   *p_slider;//界面上的滑块
    QPushButton                    *p_nextButton; //后一页按钮
    bool                            m_disableDelete;
};

class CSingleImageView : public QWidget
{
    Q_OBJECT
public:
    CSingleImageView(QWidget *parent = 0);
    ~CSingleImageView();
    void clearImage();
    void loadImage(QString fileName);
    void getWidthAndHeight(int &w,int &h);
    void paintEvent(QPaintEvent *event);
protected:
    QImage m_image;
signals:
    void signalSelectedImageName(QString selectedImageName);
    void signalImgSlider(int value,QStringList pictureList);
public slots:
    void slotSlideValue(int value,QStringList pictureList);
};

class CImgViewElement : public QWidget
{
    Q_OBJECT
public:
    CImgViewElement(QWidget *parent = 0);
    ~CImgViewElement();
    void clearImg();
    void setFontSize(int size);
    void loadImage(QString imageName);
    void setSelected(bool flag);
    bool isSelected();
    void setShowText(bool flag);

public:
    void showEvent(QShowEvent *event);
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent * event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void signalDelete();
    void signalSelectedImageName(QString pictureName);
protected:
    void createUi();
    void resetUi();
    void caculateLines(int rectWidth, QString sourceStr, QStringList &destStrList);
public:
    QString           m_fileName;
    QString           m_fullName;
    QString           m_title;
protected:
    CSingleImageView *p_singleImg;
    QFont             m_font;
    QRectF            m_textRect;
    bool              m_selected;
    bool              m_isEnter;
    bool              m_isShowText;
protected slots:
    void slotSlider(int value,QStringList pictureList);
    //void slotSlideValue(int value,QStringList pictureList);
   // void slotImgCur(QString currentPictureName);

};

#endif // IMAGEVIEWER_H
