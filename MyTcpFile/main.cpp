#include "ServerWidget.h"
#include <QApplication>
#include "ClientWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CServerWidget w;
    w.setWindowTitle("Server");
    w.show();

    CClientWidget w1;
    w1.show();
    w1.setWindowTitle("Client");
    return a.exec();
}
