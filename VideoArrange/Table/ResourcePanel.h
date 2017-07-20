#pragma once
#include <QWidget>

class ResTableView;
class ResTaskView;
//资源窗口，包含添加资源按钮，资源表和加载资源任务表
class ResourcePanel : public QWidget
{
	Q_OBJECT
public:
	ResourcePanel();
	~ResourcePanel();
public slots:
//添加资源
void onAddResource();
private:
	ResTableView* m_resTableView = nullptr;
	ResTaskView* m_resTaskView = nullptr;
};

