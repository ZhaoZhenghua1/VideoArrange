#pragma once

#include <QGraphicsView>
#include <QGraphicsWidget>
//ʱ�������
class TimeView;

//ʱ�����򣬿���ʱ��̶�
class TimeZone : public QGraphicsWidget
{
public:
	//��ǰ�̶��£�ʱ�䵽����λ�õĻ���
	qreal timeToPosition(qreal timeMS) const;
	//��ǰ�̶��£�����λ�õ�ʱ��Ļ���
	qreal positionToTime(qreal pos) const;
	//�̶ȼ�����ؼ��
	qreal pixSpace() const{ return m_dPixSpace; }
	//�̶ȼ��ʱ����
	unsigned int timeSpace()const { return m_uiTimeSpace; }
private:
	unsigned int m_iTimeLength = 30*60*1000;//ʱ�䳤��
	unsigned int m_uiTimeSpace = 1;//ʱ��̶ȼ��
	qreal m_dPixSpace = 1;//���ؿ̶ȼ��

	friend class TimeView;
};

class TimeView : public QGraphicsView
{
	Q_OBJECT
public:
	TimeView();
	~TimeView();
	//����ʱ��
	void setTimeLength(unsigned int time);
public slots:
	void zoomIn();//�Ŵ�
	void zoomOut();//��С
	void onAdjustWidth(int width);
public:
	bool isTimeMinimized();
	bool isTimeMaximized();
protected:
	void setSceneRect(const QRectF& rect);
	virtual void resizeEvent(QResizeEvent *event)Q_DECL_OVERRIDE;
	//ʱ����ͼ�������ͼ�������߶����������
	virtual qreal sceneHeight();
	virtual TimeZone* timeZone() = 0;
private:
	bool m_bIsMinimized = true;
};

