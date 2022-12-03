#ifndef CLIENTTREEVIEW_H
#define CLIENTTREEVIEW_H

#include <QWidget>
#include <QtWidgets>
#include "ClientTreeModel.h"
class CClientTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CClientTreeView(QDialog *parent = nullptr);
    ~CClientTreeView();
    CClientTreeModel *p_model;
    QWidget *treeViewWidget;

signals:

public slots:


protected:

};

#endif // CLIENTTREEVIEW_H
