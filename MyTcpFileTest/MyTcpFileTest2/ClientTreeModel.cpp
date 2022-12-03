#include "ClientTreeModel.h"

CClientTreeModel::CClientTreeModel(QObject *parent) : QStandardItemModel(parent)
{

}
CClientTreeModel::~CClientTreeModel()
{

}

//本小类用于读取字节流，但后面因程序更改，未使用该小类
bool CClientTreeModel::readByte(const QByteArray byteArray)
{

    qDebug()<<"CServerTreeModel::readByte"<<byteArray.size();
    if(!doc.setContent(byteArray))
    {
        qDebug()<<"could not doc";
        //return false;
    }
    qDebug()<<"readByte2";
    QDomElement root = doc.documentElement();
    if (root.tagName() != "root")
    {
        std::cerr << "Error: Not a root file" << std::endl;
        return false;
    }
    else{
        QFileInfo appInfo(root.attribute("Path"));
        QString value = appInfo.baseName();//文件名
        item1 = new QStandardItem((value));
        this->appendRow(item1);
        //qDebug()<<"root";
    }

    parseAllMembers(root,item1);
    qDebug()<<"parseAllMember is over";

    return true;
}

//本小类用于将doc树中的元素添加到Item之中，从而实现显示树形功能
void CClientTreeModel::parseAllMembers(const QDomElement &element,QStandardItem *ParentItem)
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

//解析xml,并添加item
bool CClientTreeModel::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cerr << "Error:"
                     "Cannot read file " << qPrintable(fileName)
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
    return true;
}

