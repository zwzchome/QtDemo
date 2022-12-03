#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QDomDocument>
class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

protected:
    void saveProject();
    void saveSelectedToDoc(QDomDocument& doc,QString path);
    void saveToElement(QDomElement &parentElt,QString path);
    void saveSelectedToFile(QString fileName);
public:
    QDomDocument m_doc;
};

#endif // MAINWINDOW_H
