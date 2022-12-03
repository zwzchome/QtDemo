#include "ServerSaveProject.h"

CServerSaveProject::CServerSaveProject(QObject *parent) : QObject(parent)
{


}
CServerSaveProject::~CServerSaveProject()
{

}
void CServerSaveProject::saveProject(QString fileName,QString path)
{

    QDomDocument doc;

    saveSelectedToFile(fileName,path);
}
//本小类用于将所得到的链接好的doc写入xml文件中
void CServerSaveProject::saveSelectedToFile(QString fileName,QString path)
{
    QDomDocument doc;
    //QString path = "E:/QT_Code/work_content";
    saveSelectedToDoc(doc,path);
    QFile file(fileName);

    bool bSuccess = file.open(QIODevice::WriteOnly);
    if(bSuccess == false)
    {
        file.close();
        return;
    }
    QTextStream out(&file);
    m_doc.save(out, 4);

    file.close();
}

//本小类用于将xml文件的一些头部写入xml文件中
void CServerSaveProject::saveSelectedToDoc(QDomDocument& doc,QString path)
{
    m_doc.clear();
    //写头部的操作
    QDomProcessingInstruction instruction;
    instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);

    //获取当前根节点，并作为参数传递给下面的处理函数
    QDomElement root = m_doc.createElement("root");
    root.setAttribute("Path",path);
    m_doc.appendChild(root);

    saveToElement(root,path);
    doc=m_doc;
}

//本小类用于递归将目标文件夹和文件的路径写入xml文件
void CServerSaveProject::saveToElement(QDomElement &parentElt,QString path)
{
    QDir dir(path);
    if(!dir.exists(path))
        dir.mkdir(path);

    //取到所有的文件和文件名，但是去掉.和..的文件夹（这是QT默认有的）
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    //文件夹优先
    dir.setSorting(QDir::DirsFirst);
    //转化成一个list
    QFileInfoList list = dir.entryInfoList();//取到当前文件夹的文件信息
    if(list.size()< 1 ) {
        return ;
    }
    for(int i=0;i<list.size();i++)
    {
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        QDomElement childElt=m_doc.createElement("tmp");//不断的以当前节点产生孩子节点，这里的tmp只是一个暂时的名称

        if(bisDir)//若是文件夹
        {
            childElt.setTagName("Folder");
            childElt.setAttribute("Path",fileInfo.filePath());
            childElt.setAttribute("IsDir","true");
            parentElt.appendChild(childElt);
            qDebug()<<fileInfo.filePath();
            saveToElement(childElt,fileInfo.filePath());
        }
        else//若是文件
        {
            childElt.setTagName("File");
            childElt.setAttribute("IsDir","false");
            childElt.setAttribute("Path",fileInfo.filePath());
            parentElt.appendChild(childElt);
        }
    }
}

