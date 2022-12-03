#include <QtWidgets>
#include "mainWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWidget w;
    //w.loadPath(qApp->applicationDirPath()+"/Image");//这里是直接将Image里面所有的图片都加载进行
    w.setFixedSize(800,600);
    w.show();

    int ret = a.exec();
    qDebug()<<"app closed";
    return ret;
}
