#include "ServerTreeModel.h"

CServerTreeModel::CServerTreeModel(QObject *parent) : QStandardItemModel(parent)
{

}
CServerTreeModel::~CServerTreeModel()
{

}
bool CServerTreeModel::readByte(const QByteArray &byteArray)
{
    QDomDocument doc;
    if(!doc.setContent(byteArray))
    {
        return false;
    }
    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a school file" << std::endl;
        return false;
    }
    else{

        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1 = new QStandardItem((value));
        this->appendRow(item1);
        qDebug()<<"root";
    }
    parseAllMembers(root,item1);
    //parseRootMembers(root);
    return true;
}
bool CServerTreeModel::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fileName)
                  << ": " << qPrintable(file.errorString())
                  << std::endl;
        return false;
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
        return false;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a school file" << std::endl;
        return false;
    }
    else{

        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1 = new QStandardItem((value));
        this->appendRow(item1);
        qDebug()<<"root";
    }
    parseAllMembers(root,item1);
    //parseRootMembers(root);
    return true;
}
void CServerTreeModel::parseAllMembers(const QDomElement &element,QStandardItem *ParentItem)
{
    QDomNode child = element.firstChild();
    while(!child.isNull())
    {

        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//文件名
        QString suffix = appInfo.suffix();
        QString readvalue = value + "."+suffix;
        ChildItem = new QStandardItem(readvalue);
        childItems.clear();
        childItems.push_back(ChildItem);
        ParentItem->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            parseAllMembers(child.toElement(),ChildItem);
        }
        child = child.nextSibling();

    }

}
/*
void CServerTreeModel::parseRootMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();

    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1_1 = new QStandardItem( value);
        childItems.clear();
        childItems.push_back(item1_1);
        item1->appendRows(childItems);
        childItems.clear();
        qDebug()<<"parseRootMembers Folder";
        if (child.toElement().tagName() == "Folder")
        {

            parseFolderMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
void CServerTreeModel::parseFolderMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1_1_1 = new QStandardItem(value);
        childItems.clear();
        childItems.push_back(item1_1_1);
        item1_1->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            qDebug()<<"parseFolderMembers Folder";
            parseFileMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
void CServerTreeModel::parseFileMembers(const QDomElement &element)
{
    QDomNode child = element.firstChild();
    while (!child.isNull())
    {
        QFileInfo appInfo(child.toElement().attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1_1_1_1 = new QStandardItem(value);
        childItems.clear();
        childItems.push_back(item1_1_1_1);
        item1_1_1->appendRows(childItems);
        childItems.clear();
        if (child.toElement().tagName() == "Folder")
        {

            qDebug()<<"parseFolderMembers Folder";
            parseFileMembers(child.toElement());
        }
        child = child.nextSibling();
    }
}
/*
/*
void CServerTreeModel::parseFile(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QByteArray array = file.readAll();//字节流读入
    file.close();
    //setContent此函数从字节数组数据中解析XML文档，并将其设置为文档的内容。 它尝试检测XML规范要求的文档编码
    if(!m_doc.setContent(array))
    {
        return;
    }
    m_fileName = fileName;
    QDomElement rootElt = m_doc.documentElement();
    //解析类型对应关系
    qDebug()<<"rootElt.tagName"<<rootElt.tagName();
    qDebug()<<rootElt.attribute("Path");


    QDomNamedNodeMap map = rootElt.attributes();//
    //QDomNamedNodeMap包含一个节点集合，节点可以通过名字来访问
    QDomNode node = rootElt.firstChild();//获取第一个子结点

    //    while(!node.isNull())  //如果节点不空
    //    {
    //        if(node.isElement()) //如果节点是元素
    //        {
    //            QDomElement e=node.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
    //            qDebug()<<e.tagName()<<"-A"<<e.attribute("Path"); //打印键值对，tagName和nodeName是一个东西
    //            QDomNodeList list=e.childNodes();
    //            for(int i=0;i<list.count();i++) //遍历子元素，count和size都可以用,可用于标签数计数
    //            {
    //                QDomNode n=list.at(i);
    //                qDebug()<<"n"<<n.nodeValue();
    //                if(node.isElement())
    //                    qDebug()<<n.nodeName()<<":A"<<n.toElement().text();
    //            }
    //        }
    //        node=node.nextSibling(); //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素，都差不多
    //    }

    //    while(!node.isNull())
    //    {
    //        qDebug()<<"-"<<node.toElement().tagName();
    //        node = node.nextSibling();	//获取同级的结点
    //    }
    int size = map.size();
    qDebug()<<"size"<<size;
    //设置列数
    this->setColumnCount(size);
    m_typeMap.clear();
    m_headerList.clear();
    for(int i = 0;i<size;i++)
    {
        QDomNode node = map.item(i);
        QString name  = node.nodeName();
        QString value = node.nodeValue();
        qDebug()<<"name"<<name<<value;
        m_typeMap.insert(name,value);
        m_headerList.append(name);
    }
    //设置表格标题
    this->setHorizontalHeaderLabels(m_headerList);
    m_dataVector.clear();
    QDomElement infoElt = rootElt.firstChildElement();
    qDebug()<<"m_headerList.size()"<<m_headerList.size();
    while(!infoElt.isNull())
    {
        QString tagName = infoElt.tagName();
        QString value = infoElt.attribute("Path");
        QString isDir = infoElt.attribute("IsDir");
        qDebug()<<"tagName"<<tagName<<value<<isDir;

        QDomElement infoElt2 = infoElt.firstChildElement();
        while(!infoElt2.isNull())
        {
            QString tagName = infoElt2.tagName();
            QString value = infoElt2.attribute("Path");
            QString isDir = infoElt2.attribute("IsDir");
            qDebug()<<"tagName"<<tagName<<value<<isDir;
            QDomElement infoElt3 = infoElt2.firstChildElement();
            while(!infoElt3.isNull())
            {
                QString tagName = infoElt3.tagName();
                QString value = infoElt3.attribute("Path");
                QString isDir = infoElt3.attribute("IsDir");
                qDebug()<<"tagName"<<tagName<<value<<isDir;
                infoElt3 = infoElt3.nextSiblingElement();
            }

            infoElt2 = infoElt2.nextSiblingElement();
        }
        //        QVector<CTreeValueElement> vector;
        //        for(int i=0;i<m_headerList.size();i++)
        //        {
        //            QString header = m_headerList.at(i);
        //            CTreeValueElement element;
        //            element.m_name  = header;
        //            element.m_value = infoElt.attribute(header,"null");
        //            element.m_element = infoElt;
        //            vector.append(element);
        //        }
        //        m_dataVector.append(vector);
        infoElt = infoElt.nextSiblingElement();

    }
    qDebug()<<"m_dataVector.size()"<<m_dataVector.size();
    this->setRowCount(m_dataVector.size());
}
*/
//QVariant CServerTreeModel::data(const QModelIndex &index, int role) const
//{
//    if(!index.isValid())
//        return QStandardItemModel::data(index,role);////第二问题：若index无效的话，为何返回的是这个？
//    if(role==Qt::DisplayRole)//数据以文本形式出现. (QString)
//    {
//        int row = index.row();//得到model的行索引
//        int col = index.column();//得到model的列索引
//        if(m_dataVector.size()>row)//数组可容纳的元素，是否大于这个索引
//        {
//            //若大于，则执行赋值给另一个vector的操作。
//            ////第三个问题，这里.at(row)是指的取第几行？然后，再取第几列吗？
//            QVector<CTreeValueElement> vector = m_dataVector.at(row);//v.at(n)和v[n]是一样的，
//            if(vector.size()>col)               ////把m_dataVector这个是看成一个二维数组吗
//            {
//                CTreeValueElement element = vector.at(col);
//                return element.m_value;
//            }
//        }
//    }//这里其实整个过程是相当于在绘制的一个过程，那么他需要什么元素，你只要给什么元素就可以了。这里的QColor就是相当于这样的一个操作。
//    //目前，它需要一个颜色的属性，那么你，给它就是了，不要想那么多。
//    else if(role==Qt::BackgroundRole)
//    {
//        QColor K(255,0,0);
//        return K;
//    }
//    else if(role==Qt::TextColorRole)
//    {
//        QColor c(0,255,0);
//        return c;
//    }
//    else if(role ==Qt::DecorationRole ){//数据以图标的形式呈现为装饰。
//        QColor c(0,200,0);
//        return c;
//    }
//    return QStandardItemModel::data(index,role);////第四个问题：这个函数所完成的功能是把model中的数据储存到vector中？
//    //答案：重写QStandardItemModel的data函数实现值替换。这样就实现整个数据的更新。
//}
////数据操作并保存
//bool CServerTreeModel::setData(const QModelIndex & index, const QVariant & value, int role )
//{
//    if(!index.isValid())
//        return QStandardItemModel::setData(index,value,role);//如果数据是无效的，那么直接返回这个data .
//    if(role==Qt::EditRole)//若表单中的数据适合在编辑器中编辑(QString)
//    {                       //这里的role其实算作一个角色，然后，不断判断当前单元格是否处于这个角色，若处于，则执行相应的操作。
//        int row = index.row();//接下来，得到目前操作的单元格的坐标
//        int col = index.column();
//        if(m_dataVector.size()>row)//如果取得的这个单元格的行数是小于m_dataVector的个数的话，那么就可以进行下面的操作了
//        {
//            QVector<CTreeValueElement> vector = m_dataVector.at(row);//这里再取得该行的所有元素。
//            if(vector.size()>col)//若该行所有元素的个数也大于这个列索引，执行下面的操作
//            {
//                CTreeValueElement element = vector.at(col);//取得这个值
//                element.m_value = value.toString();
//                element.setValue(element.m_name,element.m_value);//这里所做的操作，就已经是将新的值赋给element，并且这个操作会不断执行
//                vector.replace(col,element);//替换该值
//                m_dataVector.replace(row,vector);
//                //saveXml();//储存到xml文件里面去，基本就是一个单元格的修改，就可以实现这样的一个操作。
//                return true;
//            }
//        }
//    }
//    return QStandardItemModel::setData(index,value,role);//最后，也还是要返回这样的一个数据
//}
////基类实现返回标志的组合，该标志启用该项目（ItemIsEnabled）并允许对其进行选择（ItemIsSelectable）
////未找到调用的地方，意味着这个是直接执行的吗？
////并不是的，这里的执行时机是由基类所直接决定的，人为一般无法进行控制，
//Qt::ItemFlags  CServerTreeModel::flags(const QModelIndex& index) const
//{
//    if(!index.isValid())
//        return QStandardItemModel::flags(index);
//    int col = index.column();
//    if(m_headerList.size()>col)//在单元格被点到的时候，满足条件
//    {
//        QString header = m_headerList.at(col);
//        if(header == "Age")//这个地方的函数调用太过频繁，每秒函数执行的次数太多，每次都会在那边停，停的时机太短了，不适合观察。这个解释比较牵强，有比较好的解释，我再说。
//        {
//            Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);
//            quint32 flag = Qt::ItemIsEditable;//It can be edited.
//            flag = ~flag;
//            defaultFlags = defaultFlags&flag;
//            //defaultFlags = defaultFlags|Qt::ItemIsUserCheckable;
//            return defaultFlags;//设置该列不可编辑,接下来，就对某个单元格设置其是否能够进行编辑
//        }
//    }
//    return QStandardItemModel::flags(index);//处理完后，返回
//}
//文件解析
/*
void CServerTreeModel::parseFile(QString fileName)
{
    QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            QDomDocument dom("WCM");
            if (dom.setContent(&file))
            {
                //ui->treeWidget->clear();
                QDomElement docElem = dom.documentElement();
                listDom(docElem, NULL);
            }
        }
        file.close();


}
*/




//QString CServerTreeModel::getType(int col)
//{
//    if(m_headerList.size()>col)
//    {
//        QString name = m_headerList.at(col);
//        return m_typeMap.value(name);
//    }
//    return "String";
//}

//QString CServerTreeModel::getValue(int row,int col)
//{
//    if(m_dataVector.size()>row)
//    {
//        QVector<CTreeValueElement> vector = m_dataVector.at(row);
//        if(vector.size()>col)
//        {
//            CTreeValueElement element = vector.at(col);
//            return element.m_value;
//        }
//    }
//    return "";
//}
