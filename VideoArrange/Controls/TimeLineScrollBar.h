#pragma once
#include <QScrollBar>

class TimeLineScrollBar : public QScrollBar
{
	Q_OBJECT
public:
	enum PressFlag
	{
		NotPressed, WhiteSpace, LBtn, RBtn,
	};
public:
	TimeLineScrollBar();
	~TimeLineScrollBar();
public:
	void setTimeLength(unsigned int ms);
	PressFlag mousePressBtn() { return m_fPressOnButton; }
	unsigned int maxSceneLength();
signals:
	void adjustWidth(const int width);
protected:
	virtual void paintEvent(QPaintEvent *)Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QMouseEvent *)Q_DECL_OVERRIDE;
	virtual void mousePressEvent(QMouseEvent *)Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QMouseEvent *)Q_DECL_OVERRIDE;
private:

	//mouse on left button or right button or no button at pos;
	PressFlag btnFlag(const QPoint& pos);
private:
	unsigned int m_timeLen = 30 * 60 * 1000;//最大时，时间间距200ms，像素间距100
	bool m_bOnButton = false;
	QPoint m_posPressed;
	PressFlag m_fPressOnButton = NotPressed;
};

