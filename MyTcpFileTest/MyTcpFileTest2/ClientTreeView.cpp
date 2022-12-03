#include "ClientTreeView.h"

CClientTreeView::CClientTreeView(QDialog *parent) : QTreeView(parent)
{
    qDebug()<<"CClientTreeView";
    p_model = new CClientTreeModel();
    this->setModel(p_model);
    QString xmlPath = qApp->applicationDirPath()+"/client/test2.xml";
    p_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件目录"));
    p_model->readFile(xmlPath);//解析xml文件
}

CClientTreeView::~CClientTreeView()
{

}


