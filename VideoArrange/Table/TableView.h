#pragma once
#include <QTreeView>

class TableView : public QTreeView
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
};

