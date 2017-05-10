#pragma once
#include <QTreeView>

class TableView : public QTreeView
{
public:
	TableView();
	~TableView();
protected:
	virtual void startDrag(Qt::DropActions supportedActions)Q_DECL_OVERRIDE;
};

