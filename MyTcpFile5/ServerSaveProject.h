#ifndef SERVERSAVEPROJECT_H
#define SERVERSAVEPROJECT_H

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <iostream>
class CServerSaveProject : public QObject
{
    Q_OBJECT
public:
    explicit CServerSaveProject(QObject *parent = nullptr);
    ~CServerSaveProject();
signals:

public slots:

protected:


public:
     QDomDocument m_doc;
     void saveProject(QString fileName,QString path);
     void saveSelectedToDoc(QDomDocument& doc,QString path);
     void saveToElement(QDomElement &parentElt,QString path);
     void saveSelectedToFile(QString fileName,QString path);
};

#endif // SERVERSAVEPROJECT_H
