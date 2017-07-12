#pragma once
#include <QThread>
#include <QMutex>
#include <QStack>
#include "ControlFunObj.h"
#include <windows.h>
#include <QDateTime>
class QDomElement;
//���ſ����̣߳�����ʱ�������������Ӧ�Ŀ��ƺ���
class PlayControlThread : public QObject
{
	Q_OBJECT
public:
	PlayControlThread();
	~PlayControlThread();
	
	void play();
	void pause();
	void stop();
	void locateTo(unsigned int time);
	
	void setData(const QDomElement& elem);
signals:
	void timePointerChanged(unsigned int time);
private:
	QVector<TimeObj> buildPlayControlObjs(const QDomElement& data);
	QVector<TimeObj> buildMediaControlObj(const QDomElement& mediaElem);
private:
	bool m_bRunning = false;
	//ʱ��ָ��
	unsigned int m_timePointer = 0;
	//����ջ
	QVector<TimeObj> m_controls;
	//��ǰִ�е������кţ��α깦��
	unsigned int m_curExeIndex = 0;
	QMutex m_mutex;
	//��ʼ����ʱ��
	QDateTime m_startTime;
private:
	HANDLE m_hTimer = NULL;
	HANDLE m_hTimerQueue = NULL;

	friend VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);
};
