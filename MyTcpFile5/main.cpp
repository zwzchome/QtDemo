#include "ServerWidget.h"
#include <QApplication>
#include "ClientWidget.h"
#include "ServerTreeView.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CServerWidget w;
    w.setWindowTitle("Server");
    w.show();

    CClientWidget w1;
    w1.show();
    w1.setWindowTitle("Client");

    CServerTreeView w2;
    w2.show();
    w2.setWindowTitle("Tree");
    return a.exec();
}
