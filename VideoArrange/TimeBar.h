#pragma once

#include "TimeView.h"

class Bar;
//ʱ����
class TimeBar : public TimeView
{
public:
	TimeBar();
	~TimeBar();
public:
	//����ʱ��
	void setTimeLength(unsigned int time);

	void timebarClicked(const QPointF& scenePos);
protected:

	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
private:
	void sceneWidthChanged();
private:
	Bar* m_rectBar = nullptr;
};

