#include "ImageViewer.h"

CDeepPictureViewer::CDeepPictureViewer(QWidget *parent)
    :QWidget(parent)
{
    m_col = 4;//设置显示的列数
    m_currentSelected = nullptr;
    m_disableDelete   = false;

    m_currentIndex = 0;//当前选中的图片索引号
    m_startIndex   = 0;//当前起始索引号
    m_endIndex     = 0;//当前结束索引号
    m_totalPageNum = 0;//当前总页数
    m_currentPage  = 0; //当前页
    m_isShowFileName = true;
    m_isAutoSelected = true;
    createUi();
    setUseControl(true);
    p_controlLabel->setVisible(false);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QPalette pal;//QPalette类包含每种小部件状态的颜色组
    pal.setColor(QPalette::Background,Qt::black);
    this->setPalette(pal);
    this->setAutoFillBackground(true);
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";//读取固定xmL文件中的数据进行显示
    QStringList allPictureList = readFile(filePath);//读取xml文件中的所有图片的信息进行显示
    addImageList(allPictureList);//将所有图片添加入addImageList,可成功实现显示
    connect(p_slider,SIGNAL(sliderReleasedAt(int)),this,SLOT(slotSliderValue(int)));//获取滑块的值并进行相应的操作

}

//根据ImageView中所选择的图片与所有的图片进行比对，可得到当前选择的图片的index
void CDeepPictureViewer::slotSelectedIndex(QString imageName)
{
    //qDebug()<<"CDeepPictureViewer::slotSelectedIndex pictureList"<<pictureList.size();
    for(int i =0;i<pictureList.size();i++)
    {
        if(imageName==pictureList.at(i))
        {
            m_currentIndex = i;//现在选择的图片的index
        }
    }
    update();
}

CDeepPictureViewer::~CDeepPictureViewer()
{

}

//获取滑块的值，传输到这边，控制图片的显示
void CDeepPictureViewer::slotSliderValue(int value)
{
    emit signalSilderValue(value,pictureList);
    gotoPage(value-1);
    updatePage();
}

//解析xml文件，将xml文件中的信息显示成图片(xml文件信息是图片)
QStringList CDeepPictureViewer::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error:"
                     "Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        std::cerr << "Error: Parse error at line " << errorLine << ", "
                  << "column " << errorColumn << ": "
                  << qPrintable(errorStr) << std::endl;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a root file" << std::endl;
    }
    else{
        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//文件名
    }

    QDomNode child = root.firstChild();
    while(!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString filepath = appInfo.filePath();
        pictureList.append(filepath);
        child = child.nextSibling();
    }
    return pictureList;
}

//设置取消可删除功能，应该是没有应用到这个类
void CDeepPictureViewer::setDisableDelete(bool flag)
{
    m_disableDelete = flag;
}

//控制部件的水平布局
void CDeepPictureViewer::createUi()
{
    p_layout = new QHBoxLayout();
    p_layout->setContentsMargins(0,0,0,0);
    this->setLayout(p_layout);
    createControlWidget();

}

//这里也有个类用来加载对应文件夹中的所有图片，以进行显示，但我们现在更换显示的方式了，使用读取xml文件信息进行显示了
void CDeepPictureViewer::loadPath(QString path)
{
    QDir dir = QDir(path);
    QStringList   fileList;
    QFileInfoList list = dir.entryInfoList(QDir::Files|QDir::NoDot|QDir::NoDotDot);
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        fileList.append(fileInfo.filePath());
    }
    addImageList(fileList);//这里的fileList是大个的List
}

//设置控制界面
void CDeepPictureViewer::createControlWidget()
{
    p_controlWidget = new QWidget(this);//控制界面
    p_controlLabel  = new QLabel(this); //控制界面label
    p_controlLabel->setText("0/0");
    p_controlLabel->setStyleSheet("color:white;");

    p_chooseButton = new QPushButton();//添加图片按钮
    p_chooseButton->setText(QString::fromLocal8Bit("添加图片"));
    p_chooseButton->setMinimumWidth(120);
    connect(p_chooseButton,SIGNAL(released()),this,SLOT(slotAddPicture()));

    p_prevButton = new QPushButton(); //前一页按钮
    p_prevButton->setText(QString::fromLocal8Bit("上一页"));
    p_prevButton->setMinimumWidth(80);
    connect(p_prevButton,SIGNAL(released()),this,SLOT(slotGotoPrevPage()));

    p_slider = new CImageSlider();
    p_slider->setOrientation(Qt::Horizontal);

    p_slider->setTickInterval(1);
    p_slider->setSingleStep(1);//设置在键盘上按下→键或←键时的步进值:

    p_nextButton = new QPushButton(); //后一页按钮
    p_nextButton->setText(QString::fromLocal8Bit("下一页"));
    p_nextButton->setMinimumWidth(80);
    connect(p_nextButton,SIGNAL(released()),this,SLOT(slotGotoNextPage()));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(p_chooseButton);
    layout->addStretch(1);
    layout->addWidget(p_prevButton);
    layout->addStretch(1);
    layout->addWidget(p_slider);
    layout->addStretch(1);
    layout->addWidget(p_controlLabel);
    layout->addStretch(1);
    layout->addWidget(p_nextButton);
    layout->setAlignment(Qt::AlignLeft);
    layout->setContentsMargins(1,1,1,1);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    p_controlWidget->setLayout(layout);
}

//更新页码
void CDeepPictureViewer::updatePage()
{
    int total = m_col;//m_col是每一页的列数
    int totalImageNum = m_totalImageList.size();//这里可以获得总的图片的页数
    if(totalImageNum>0)
    {
        if(totalImageNum%total==0)
            m_totalPageNum = (totalImageNum/total);
        else
            m_totalPageNum = (totalImageNum/total)+1;
    }
    else
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    QString str;
    if(totalImageNum>0)
    {
        str= QString("%1/%2").arg(m_currentPage+1).arg(m_totalPageNum);
        p_slider->returnTotalImgNum(m_totalPageNum);//将相应的数字传输p_slider进行更改
        p_slider->returnCurrentIndex(m_currentPage+1);
    }
    p_controlLabel->setText(str);
}

//取出没有被用过的item
CImgViewElement* CDeepPictureViewer::nextImgPic()
{
    CImgViewElement *item = nullptr;
    int size = m_totalVector.size();
    for(int i = 0;i<size;i++)
    {
        item = m_totalVector.at(i);//把所有图片都赋值给Item

        if(item->m_fileName.isEmpty())//看一下这个时候的item是否已经loadImage，如果没有就直接返回这个item
            return item;
    }
    item = new CImgViewElement();//如果有加载过item了，就新建一个CImgViewElement对象
    m_totalVector.append(item);//将对象连接在这个m_totalVector的下面。
    item->installEventFilter(this);//这个小控件安装过滤器
    item->setShowText(m_isShowFileName);
    connect(item,SIGNAL(signalDelete()),this,SLOT(slotDelete()));
    return item;
}

//得到对应name的Item,好像未使用
CImgViewElement* CDeepPictureViewer::getItemByName(QString name)
{
    foreach (CImgViewElement *item, m_totalVector)
    {
        if(item->m_fullName==name)
        {
            return item;
        }
    }
    return nullptr;
}

//这个类的参数是把imageList的所有内容加载进来
void CDeepPictureViewer::addImageList(QStringList imageList)
{
    int  total = m_col;//设置列数

    if(m_currentImageList.size()==0)//判断当前的显示列表是否大于0
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    QStringList addedList;
    QString     fileName;

    //检查文件是否存在
    clearAll();//我更改了整个addList的机制，让它每次执行这个函数，都是加载我给它的所有照片
    while(imageList.size()>0)
    {
        //QString roiFileName;
        fileName    = imageList.takeFirst();//返回这个第一个元素的值，并同时删除它·

        //如果文件不存在就跳过
        QFileInfo info(fileName);

        if(!info.exists())
            continue;//执行跳过操作
        if(!m_totalImageList.contains(fileName))//如果目前总的图片列表不包含这个文件名，就将其加入totalImage
        {
            addedList.append(fileName);
            m_totalImageList.append(fileName);

        }
    }
    //此时m_totalImageList就拥有所有的image
    //计算总页码数
    int totalImageNum = m_totalImageList.size();//这里的页数还是较为准确的页数的
    if(totalImageNum>0)
    {
        if(totalImageNum%total==0)
            m_totalPageNum = (totalImageNum/total);
        else
            m_totalPageNum = (totalImageNum/total)+1;
    }
    else
    {
        m_totalPageNum = 0;
        m_currentPage  = 0;
    }

    //如果当前界面的控件数量没有满就继续添加，如果满了就不用再添加了
    int tCurrentNum = 0;
    if(m_totalVector.size()<total)//如果当前所有的图片数量少于可放置的列数，则在新的图片要加进来的时候，进行增加新的控件
    {
        while(m_totalVector.size()<total)
        {
            if(addedList.size()<=0)//这里来一个判断，问是否你要加的元素是否有东西？
                break;
            fileName = addedList.takeFirst();//获得第一个fileName,然后，移除这个fileName
            CImgViewElement *item = nextImgPic();//这里获得一个装有内部数据的item，然后，这个item再加载相应的图片

            item->loadImage(fileName);//这里就把addedList中的file都加载进来
            //item->isSelected();
            if(m_size.isValid())//m_size是当前每个控件的大小
                item->setFixedSize(m_size.width(),m_size.height());
            p_layout->addWidget(item);//在布局中添加控件，删除的时候也是获取控件进行删除
        }
    }
    //m_totalVector是当前的容器数量
    //计算当前页显示的起始及终止序号
    m_startIndex = m_currentPage*total;
    tCurrentNum  = m_totalImageList.size() - m_startIndex;
    if(tCurrentNum>=total)
        tCurrentNum = total;
    m_endIndex   = m_startIndex + tCurrentNum;
    m_currentImageList = m_totalImageList.mid(m_startIndex,m_endIndex-m_startIndex);
    for(int i=0;i<m_currentImageList.size();i++)
    {
        fileName = m_currentImageList.at(i);
        if(i<m_totalVector.size())
        {
            CImgViewElement *item = m_totalVector.at(i);
            if(item->m_fullName!=fileName)
            {
                item->loadImage(fileName);
                if(m_size.isValid())
                    item->setFixedSize(m_size.width(),m_size.height());
            }
            if(item->isHidden())
            {
                item->show();
            }
        }
    }
    updatePage();
    //qDebug()<<"CDeepPictureViewer addPicture m_totalVector"<<m_totalVector.size();
}

void CDeepPictureViewer::clearAll()
{
    m_currentIndex = 0;//当前诉案中的图片索引号
    m_startIndex   = 0;//当前起始索引号
    m_endIndex     = 0;//当前结束索引号
    m_totalPageNum = 0;//当前总页数
    m_currentPage  = 0; //当前页

    foreach (CImgViewElement*item, m_totalVector)
    {
        item->hide();
        item->clearImg();
    }
    m_currentImageList.clear();
    m_totalImageList.clear();
    updatePage();
}

void CDeepPictureViewer::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    calculateWidget();
}

//计算每个控件应该有的大小
void CDeepPictureViewer::calculateWidget()
{
    int width  = this->size().width();
    int height = this->size().height();
    if(m_isShowPrevNextControl)
    {
        height-=p_controlWidget->height();
        int x;
        x = (width - p_controlWidget->width())/2;
        p_controlWidget->move(x,height);
    }

    int colSpacing = (m_col -1)*10+4;
    int rowSpacing = 40;

    qreal ww,wh;
    ww = qreal(width  - colSpacing)/m_col;
    wh = qreal(height - rowSpacing)/1;

    int w,h;
    w = qFloor(ww);
    h = qFloor(wh);
    m_size.setWidth(w);
    m_size.setHeight(h);

    foreach (CImgViewElement* item, m_totalVector)
    {
        if(item->size()!=m_size)
            item->setFixedSize(w,h);
    }
    p_layout->update();
}

void CDeepPictureViewer::showEvent(QShowEvent *event)
{
    setSeleceted(0);
    calculateWidget();
    QWidget::showEvent(event);
}

bool CDeepPictureViewer::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            clearAllSelected();
            CImgViewElement *t = dynamic_cast<CImgViewElement *>(watched);
            if(t)
            {
                m_currentSelected = t;
                emit signalCurrentFileName(t->m_fullName);
            }
        }
    }
    return QWidget::eventFilter(watched, event);
}

void CDeepPictureViewer::clearAllSelected()
{
    QList<CImgViewElement*> list = this->findChildren<CImgViewElement*>();
    foreach (CImgViewElement* item, list)
    {
        if(item->isSelected())
        {
            item->setSelected(false);
        }
    }
}

void CDeepPictureViewer::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        clearAllSelected();
        m_currentSelected = nullptr;
        emit signalNoSelected();
    }

    QWidget::mousePressEvent(event);
}

CImgViewElement* CDeepPictureViewer::getCurrent()
{
    return m_currentSelected;
}

void CDeepPictureViewer::slotGotoNextPage()
{
    if(m_currentPage>=m_totalPageNum-1)
    {
        return;
    }
    m_currentPage++;
    gotoPage(m_currentPage);
}

//在点击增加图片按钮所执行的槽函数
void CDeepPictureViewer::slotAddPicture()
{
    QString picturePath = QFileDialog::getOpenFileName(this, "open", "../");//这里得到的已经是正确的路径了
    QStringList fileList;
    if(false == picturePath.isEmpty())
    {

        QFileInfo info(picturePath);
        fileList.append(info.filePath());
    }
    else
    {
        qDebug() << "CDeepPictureViewer::slotAddPicture Chose file path is wrong! 118";
    }
    pictureList.append(fileList);
    addImageList(pictureList);//添加入显示的名单之中
    addXmlMes(picturePath);
}

//将添加的信息加入xml文件之中
void CDeepPictureViewer::addXmlMes(QString picturePath)
{
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    QDomDocument doc;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes open success";
    }

    QByteArray array = file.readAll();//字节流读入
    if(!doc.setContent(array))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes fail setContent";
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomElement childElt = doc.createElement("tmp");//创建一个临时根节点
    childElt.setTagName("Picture");
    childElt.setAttribute("Path",picturePath);
    root.appendChild(childElt);//连接在这个root的后面

    bool bSuccess = file.open(QIODevice::WriteOnly);//这里文件要再次打开的话，一定要保证，前面这个文件已经执行关闭操作了
    if(bSuccess == false)
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file could write";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    QTextStream out(&file);
    doc.save(out, 4);

    file.close();
}
void CDeepPictureViewer::slotGotoPrevPage()
{
    if(m_currentPage<=0)
    {
        return;
    }
    m_currentPage--;
    gotoPage(m_currentPage);
}

void CDeepPictureViewer::slotDelete()
{
    if(m_disableDelete)
    {
        qDebug()<<" CDeepPictureViewer::slotDelete";
        return;
    }
}

void CDeepPictureViewer::keyPressEvent(QKeyEvent * event)
{
    if(event->key()==Qt::Key_Delete)//记得PictureList也要进行删除
    {
        event->accept();
        deepDelete();//若按下按钮delete，则执行删除操作
    }
    QWidget::keyPressEvent(event);
}

//作为当delete按键按下所要执行的函数
void CDeepPictureViewer::deepDelete()
{
    //deleteXmlMes(pictureList.at(m_currentIndex));//先删除xml文件上当前选中的图片信息
    //pictureList.removeAt(m_currentIndex);//在pictureList上移除该图片信息
    //addImageList(pictureList);//在把经过更改的添加入addImageList之中，这是第一种方法，这种方法执行效率较低
    deepDeleteLayout();
    update();

}

//执行删除的函数
void CDeepPictureViewer::deepDeleteLayout()
{
    //deleteXmlMes(pictureList.at(m_currentIndex));//先删除xml文件上当前选中的图片信息，调试程序时会先注释掉。

    if(m_totalImageList.size()<=m_col&&m_totalVector.size()>0)//如果当前图片的数量减一后小于等于列数，就比如总共只有4张的情况，直接删除该控件即可
    {
        CImgViewElement *item2 = deleteImgPic();//获得要删除的那个控件
        //if(m_totalVector.size()==1)

        p_layout->removeWidget(item2);//将该控件进行删除
        delete item2;//斩草除根
        m_totalImageList.removeAt(m_currentIndex);
        m_totalVector.remove(m_currentIndex);
        pictureList.removeAt(m_currentIndex);
        if(m_totalVector.size()>0)
            setSeleceted(m_currentIndex);
    }
    else if(m_totalImageList.size()>m_col)
    {
        CImgViewElement *item2 = deleteImgPic();//获得要删除的那个控件
        p_layout->removeWidget(item2);//将该控件进行删除
        delete item2;//斩草除根
        m_totalVector.remove(m_currentIndex);
        m_totalImageList.removeAt(m_currentIndex);
        pictureList.removeAt(m_currentIndex);
        setSeleceted(m_currentIndex); 
        int m_replaceIndex = (m_currentPage+1)*m_col; 
        QString fileName=pictureList.at(m_replaceIndex-1);;//这个fileName的数值要注意
        CImgViewElement *item = nextImgPic();
        item->loadImage(fileName);
        if(m_size.isValid())
            item->setFixedSize(m_size.width(),m_size.height());
        p_layout->addWidget(item);
    }
    updatePage();
    this->update();
}


CImgViewElement* CDeepPictureViewer::deleteImgPic()
{
    CImgViewElement *item = nullptr;
    item = m_totalVector.at(m_currentIndex);
    return  item;
}

//删除对应图片路径的xml文件信息
void CDeepPictureViewer::deleteXmlMes(QString picturePath)
{
    QString filePath =  qApp->applicationDirPath()+"/xml/test.xml";
    QDomDocument doc;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes file open fail";
    }
    else{
        qDebug()<<"CDeepPictureViewer::addXmlMes open success";
    }
    QByteArray array = file.readAll();//字节流读入
    if(!doc.setContent(array))
    {
        qDebug()<<"CDeepPictureViewer::addXmlMes fail setContent";
        //return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList picture=doc.elementsByTagName("Picture"); //由标签名定位
    root.removeChild(picture.at(m_currentIndex));//移除该index的child位置
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {
        qDebug()<<"CDeepPictureViewer::deleteXmlMes open fail";
        return;
    }
    else
    {
        qDebug()<<"CDeepPictureViewer::deleteXmlMes open success";
    }

    QTextStream out_stream(&file);
    doc.save(out_stream,4); //缩进4格
    file.close();
}

void CDeepPictureViewer::gotoPage(int pageNum)
{
    m_currentPage = pageNum;
    int total = m_col;
    //计算当前页面需要几个显示的控件
    int tCurrentNum = 0;
    m_startIndex = m_currentPage*total;
    tCurrentNum = m_totalImageList.size() - m_startIndex;
    if(tCurrentNum>=total)
        tCurrentNum = total;
    m_endIndex   = m_startIndex + tCurrentNum;
    m_currentImageList = m_totalImageList.mid(m_startIndex,tCurrentNum);
    int size = m_totalVector.size();
    for(int i = 0;i<size;i++)
    {
        CImgViewElement *t = m_totalVector.at(i);
        t->clearFocus();
        if(i<m_currentImageList.size())
        {
            QString tFileName = m_currentImageList.at(i);
            t->loadImage(tFileName);
            t->setSelected(false);
            t->show();
        }
        else
        {
            t->clearImg();
            t->hide();
        }
    }
    m_currentSelected = nullptr;
    if(!m_isAutoSelected)
        emit signalNoSelected();
    updatePage();
    setSeleceted(0);
    this->update();
}

void CDeepPictureViewer::setUseControl(bool flag)
{
    m_isShowPrevNextControl = flag;
    p_controlLabel->setVisible(flag);
}

void CDeepPictureViewer::setShowImageName(bool flag)
{
    m_isShowFileName = flag;
    foreach (CImgViewElement *t, m_totalVector)
    {
        t->setShowText(flag);
    }
}

void CDeepPictureViewer::setAutoSelectedFirst(bool flag)
{
    m_isAutoSelected = flag;
}

//设置当前index为selecte
void CDeepPictureViewer::setSeleceted(int index)
{
    if(!m_isAutoSelected)
        return;
    //qDebug()<<"CDeepPictureViewer::setSeleceted"<<m_currentImageList.size()<<index;
    if(m_currentImageList.size()<0||m_currentImageList.size()<index)
    {
        qDebug()<<"1212121212";
        emit signalNoSelected();
        return;
    }
    CImgViewElement *item = m_totalVector.at(index);
    foreach (CImgViewElement *view, m_totalVector)
    {
        if(item!=view)
        {
            if(view->isSelected())
                view->setSelected(false);
        }
    }
    item->setSelected(true);
    m_currentSelected = item;
    //qDebug()<<"m_currentSelected->m_fullName"<<m_currentSelected->m_fullName;
    emit signalCurrentFileName(m_currentSelected->m_fullName);
    //    this->updatePage();
    //    this->update();
    //    p_layout->update();
}

void CDeepPictureViewer::gotoPrevImage()
{
    if(!m_currentSelected)
        return;
    if(m_totalVector.size()<=0)
        return;
    int index = m_totalVector.indexOf(m_currentSelected);
    qDebug()<<"CDeepPictureViewer::gotoPrevImage"<<index;
    if(index<0)
        return;
    if(index>0)
        setSeleceted(--index);
    else
    {
        if(m_currentPage==0)
            return;
        else
        {
            slotGotoPrevPage();
            setSeleceted(m_totalVector.size()-1);
        }
    }
}

void CDeepPictureViewer::gotoNextImage()
{
    if(!m_currentSelected)
        return;
    if(m_totalVector.size()<=0)
        return;
    int size = m_totalVector.size();
    int index = m_totalVector.indexOf(m_currentSelected);
    //qDebug()<<"CDeepPictureViewer::gotoNextImage"<<index;
    if(index<0)
        return;
    if(index>=0&&index<size-1)
    {
        if(m_endIndex!=m_currentPage*m_col+index+1)
            setSeleceted(++index);
    }
    else
    {
        if(m_currentPage==m_totalPageNum)
            return;
        else if(m_endIndex!=(m_totalPageNum-1)*m_col+index+1)
        {
            slotGotoNextPage();
            setSeleceted(0);
        }
    }
}

/****************************************
 *
 *           能读取ROI的控件
 *
 * *************************************/
CSingleImageView::CSingleImageView(QWidget *parent)
    :QWidget(parent)
{
    //设置鼠标穿透
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);

    //设置背景颜色
    QPalette palette = this->palette();
    QColor c(0,0,0,0);
    palette.setColor(QPalette::Base, c);
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

CSingleImageView::~CSingleImageView()
{

}

void CSingleImageView::slotSlideValue(int value,QStringList pictureList)
{
    loadImage(pictureList.at(value));// 这里设置完后在Img那边也要进行更新页面
    emit signalImgSlider(value,pictureList);
    this->update();
}

void CSingleImageView::clearImage()
{
    m_image = QImage();
    this->update();
}


//加载对应文件名的图片，大张的图片
void CSingleImageView::loadImage(QString fileName)
{
    m_image.load(fileName);
    emit signalSelectedImageName(fileName);
    //qDebug()<<"CSingleImageView1::loadImage"<<fileName;
    this->update();
}

void CSingleImageView::getWidthAndHeight(int &w,int &h)
{
    w = m_image.width();
    h = m_image.height();
}

void CSingleImageView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if(m_image.isNull())
    {
        painter.drawImage(this->rect(),m_image);
        return;
    }
    int widget_width =this->size().width();
    int widget_height =this->size().height();

    int image_width  = m_image.width();
    int image_height = m_image.height();

    qreal view_ratio =(qreal)widget_width/(qreal)widget_height;
    qreal img_ratio  =(qreal)image_width/(qreal)image_height;

    int rect_x     = 0;
    int rect_y     = 0;
    int rectWidth  = widget_width;
    int rectHeight = widget_height;

    //控件太宽了，高度太小，以高度为基准
    if(view_ratio>img_ratio)
    {
        rectWidth = widget_height*img_ratio;
        rect_x    = (widget_width - rectWidth)/2;
    }
    //控件太短了，高度太大，以宽度为基准
    else if(view_ratio<img_ratio)
    {
        rectHeight = widget_width/img_ratio;
        rect_y     = (widget_height -rectHeight)/2;
    }

    QRect rect = QRect(rect_x,rect_y,rectWidth,rectHeight);
    painter.drawImage(rect,m_image);
}

/**************************************
 *
 * 整个图片的控件
 *
 * ***************************************/
CImgViewElement::CImgViewElement(QWidget *parent)
    :QWidget(parent)
{
    m_fileName  = "";
    m_fullName  = "";
    m_selected = false;
    m_isEnter  = false;
    m_isShowText = true;
    setAttribute(Qt::WA_TranslucentBackground, true);
    createUi();
    this->setFocusPolicy(Qt::ClickFocus);


}

CImgViewElement::~CImgViewElement()
{

}

//void CImgViewElement::slotImgCur(QString currentPictureName)
//{
//    qDebug()<<"CImgViewElement::slotImgCu"<<currentPictureName;
//}

void CImgViewElement::createUi()
{
    QVBoxLayout *layout = new QVBoxLayout();
    p_singleImg = new CSingleImageView(this);
    //    bool flag = connect(p_singleImg,SIGNAL(signalImgSlider(int,QStringList)),this,SLOT(slotSlider(int,QStringList)));
    //    qDebug()<<"Img"<<flag;
    layout->addWidget(p_singleImg);
    layout->setContentsMargins(0,1,0,0);
    //居中上对齐
    layout->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    this->setLayout(layout);
}

void CImgViewElement::setFontSize(int size)
{
    m_font.setPixelSize(size);
    this->setFont(m_font);
    resetUi();
}

void CImgViewElement::slotSlider(int value,QStringList pictureList)
{
    loadImage(pictureList.at(value));
    //qDebug()<<"CImgViewElement::slotSlider";
    setSelected(true);
    update();
}
void CImgViewElement::loadImage(QString imageName)
{
    QFileInfo info(imageName);
    m_fullName  = imageName;
    m_fileName  = info.fileName();
    //qDebug()<<"CImgViewElement::loadImage"<<imageName;
    p_singleImg->loadImage(imageName);
}

void CImgViewElement::resetUi()
{
    int width      = this->width();
    int height     = this->height();

    int textHeight = fontMetrics().height()*2*1.1;
    int ww,wh;
    ww = width-2;
    if(m_isShowText)
        wh = height - textHeight;
    else
        wh = height-2;
    if(wh<=0)
        return;
    p_singleImg->setFixedSize(ww,wh);
    qreal x,y;
    x = 0;
    y = height - fontMetrics().height()*2;
    m_textRect.setRect(x,y,width,fontMetrics().height()*2);
}

void CImgViewElement::clearImg()
{
    m_fullName  = "";
    m_fileName  = "";
    QImage img;
    p_singleImg->clearImage();
    this->update();
}

void CImgViewElement::showEvent(QShowEvent *event)
{
    resetUi();
    QWidget::showEvent(event);
}

//这个是指下面那些小照片的绘图事件类
void CImgViewElement::paintEvent(QPaintEvent * event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    QStringList nameList;
    QString paintText;
    if(m_title.isEmpty())
        paintText = m_fileName;
    else
        paintText = m_title;
    caculateLines(m_textRect.width(),paintText,nameList);
    int nameListSize = nameList.size();
    //最多显示三行，如果第三行后面还有的话，就用。。。代替
    for(int i = 0; i < nameListSize; i++)
    {
        QString showTitle = nameList.at(i);
        if(i == 1 && nameListSize > 1&&showTitle.size()>3)
        {
            showTitle.chop(3);
            showTitle.append("...");
        }
        if(i >1)
        {
            break;
        }
        if(m_isShowText)
        {
            qreal x,y,w,h;
            x = m_textRect.x();
            y = m_textRect.y();
            w = m_textRect.width();
            h = m_textRect.height();
            y+=i*fontMetrics().height();
            QRect drawRect = QRect(x,y,w,h);
            QPen pen = painter.pen();
            pen.setColor(Qt::white);
            painter.save();
            painter.setPen(pen);
            painter.drawText(drawRect,Qt::AlignTop|Qt::AlignHCenter,showTitle);
            painter.restore();
        }
    }

    if(m_selected)
    {
        //        QPainterPath path;
        //        path.addRoundedRect(this->rect(),0,0);
        //        painter.fillPath(path,QColor(0,255,0,100));

        QColor c;
        c.setNamedColor("#00ff00");
        QPen pen;
        pen.setColor(c);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(this->rect());
    }
    //鼠标滑过
    else if(m_isEnter)
    {
        QColor c;
        c.setNamedColor("#ffff00");
        c.setAlpha(180);
        QPen pen;
        pen.setColor(c);
        pen.setStyle(Qt::SolidLine);

        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRect(this->rect());
    }
}

void CImgViewElement::resizeEvent(QResizeEvent * event)
{
    resetUi();
    QWidget::resizeEvent(event);
}

void CImgViewElement::caculateLines(int rectWidth, QString sourceStr, QStringList &destStrList)
{
    int soutceSize = sourceStr.size();
    QString lineStr;
    lineStr.clear();
    for(int i = 0 ; i < soutceSize; i++)
    {
        QString charStr = sourceStr.at(i);
        lineStr.append(charStr);
        int fontsize = fontMetrics().width(lineStr);
        //加入一个字符，如果加入后宽度大于最大宽度，则清除加入的字符
        if(fontsize > rectWidth)
        {
            lineStr.chop(1);
            destStrList<<lineStr;
            lineStr.clear();
            lineStr.append(charStr);
        }

        if( i == sourceStr.size() - 1)
        {
            destStrList<<lineStr;
        }
    }
}

void CImgViewElement::enterEvent(QEvent *event)
{
    m_isEnter = true;
    QWidget::enterEvent(event);
    update();
}

void CImgViewElement::leaveEvent(QEvent *event)
{
    m_isEnter = false;
    QWidget::leaveEvent(event);
    update();
}

void CImgViewElement::keyPressEvent(QKeyEvent * event)
{
    if(event->key()==Qt::Key_Delete)//记得PictureList也要进行删除
    {
        event->accept();
        qDebug()<<"CImgViewElement::keyPressEvent";
        emit signalDelete();
    }

    QWidget::keyPressEvent(event);
}

void CImgViewElement::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setSelected(true);
    }
    update();
}

void CImgViewElement::setSelected(bool flag)
{
    m_selected = flag;
    update();
}

bool CImgViewElement::isSelected()
{
    return m_selected;
}

void CImgViewElement::setShowText(bool flag)
{
    m_isShowText = flag;
    resetUi();
}
