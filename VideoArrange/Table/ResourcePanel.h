#pragma once
#include <QWidget>

class ResTableView;
class ResTaskView;
//��Դ���ڣ����������Դ��ť����Դ��ͼ�����Դ�����
class ResourcePanel : public QWidget
{
	Q_OBJECT
public:
	ResourcePanel();
	~ResourcePanel();
public slots:
//�����Դ
void onAddResource();
private:
	ResTableView* m_resTableView = nullptr;
	ResTaskView* m_resTaskView = nullptr;
};

