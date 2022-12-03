#include "ServerWidget.h"
#include <QApplication>
#include "ClientWidget.h"
#include "ClientTreeView.h"
#include <QTimer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CServerWidget w;
    w.setWindowTitle("Server");
    w.show();
    CClientWidget w1;
    w1.setWindowTitle("Client");
    w1.show();
    return a.exec();
}
