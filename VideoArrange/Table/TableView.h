#pragma once
#include <QTreeView>
#include "../Document/Observer.h"

class TableView : public QTreeView , public Observer
{
public:
	TableView();
	~TableView();
protected:
	virtual void startDrag(Qt::DropActions supportedActions)Q_DECL_OVERRIDE;
	virtual void dragEnterEvent(QDragEnterEvent *event) override;
	virtual void dragMoveEvent(QDragMoveEvent *event) override;
	virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
	virtual void dropEvent(QDropEvent *event) override;
protected:
	virtual void init() override;
};

