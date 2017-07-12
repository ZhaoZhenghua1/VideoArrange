#pragma once
#include <QThread>
#include <QMutex>
#include <QStack>
#include "ControlFunObj.h"
#include <windows.h>
#include <QDateTime>
class QDomElement;
//播放控制线程，满足时间条件后产生相应的控制函数
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
	//时间指针
	unsigned int m_timePointer = 0;
	//控制栈
	QVector<TimeObj> m_controls;
	//当前执行到的序列号，游标功能
	unsigned int m_curExeIndex = 0;
	QMutex m_mutex;
	//开始播放时间
	QDateTime m_startTime;
private:
	HANDLE m_hTimer = NULL;
	HANDLE m_hTimerQueue = NULL;

	friend VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired);
};
