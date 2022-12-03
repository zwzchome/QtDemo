#ifndef CLIENTTREEMODEL_H
#define CLIENTTREEMODEL_H

#include <QObject>
#include <QtWidgets>
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <iostream>
#include <QApplication>
class CClientTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CClientTreeModel(QObject *parent = nullptr);
    ~CClientTreeModel();
    bool readByte(const QByteArray byteArray);
    bool readFile(const QString &fileName);
    QStandardItem *item1;
    QStandardItem *ChildItem;
    QDomDocument doc;
    QList<QStandardItem *> childItems;
protected:
    void parseAllMembers(const QDomElement &element,QStandardItem *ParentItem);

signals:

public slots:

};

#endif // CLIENTTREEMODEL_H
