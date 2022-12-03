#include "ServerTreeView.h"
#include "ServerTreeModel.h"
CServerTreeView::CServerTreeView(QWidget *parent) : QTreeView(parent)
{
    p_saveProject = new CServerSaveProject();
    //在更改参数时，建议将复制过来的文件路径的斜杠更换一下方向，才能更好的识别
    QString fileName="E:/Test/test.xml";//这是要写入的目标文件。
    QString path = "E:/QT_Code/work_content/Day01";//这是要进行检测的目标文件夹
    p_saveProject->saveProject(fileName,path);
    initTreeView();
}
CServerTreeView::~CServerTreeView()
{
}
void CServerTreeView::initTreeView()
{
    QString xmlPath = "E:/Test/test.xml";
    p_model = new CServerTreeModel();
   // p_model->readByte(byteArray);
    this->setModel(p_model);//public void setModel(TreeModel newModel):根据传递的参数设置TreeModel的值
    //p_model->parseFile(xmlPath);//解析xml文件
    p_model->readFile(xmlPath);
    p_model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("文件名"));
    //this->setUpdatesEnabled(true);  //恢复界面刷新
}
void CServerTreeView::receiveByte(QByteArray byteArray)
{
    qDebug()<<"receiveByte";
    p_model->readByte(byteArray);
}
