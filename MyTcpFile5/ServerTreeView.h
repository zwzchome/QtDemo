#ifndef SERVERTREEVIEW_H
#define SERVERTREEVIEW_H

#include <QWidget>
#include "ServerTreeModel.h"
#include <QtWidgets>
#include "ServerSaveProject.h"
class CServerTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CServerTreeView(QWidget *parent = nullptr);
    ~CServerTreeView();

signals:

public slots:
public:
    CServerTreeModel *p_model;
    void initTreeView();
    CServerSaveProject *p_saveProject;
    void receiveByte(QByteArray byteArray);

};

#endif // SERVERTREEVIEW_H
