#include "PlayControlThread.h"
#include <QTime>
#include <QDebug>
#include <QDomDocument>

namespace
{

	template<class T>
	void AddToControl1(const QDomElement& parent, QVector<TimeObj>& ret, unsigned int startTime, unsigned int endtime, unsigned int timePointer, const MoviePlayObj& playObj)
	{
		for (QDomElement adjustmentpos = parent.firstChildElement("adjustmentpos"); !adjustmentpos.isNull(); adjustmentpos = adjustmentpos.nextSiblingElement("adjustmentpos"))
		{
			unsigned int time = startTime + adjustmentpos.attribute("name").toUInt();
			if (time >= timePointer && time <= endtime)
			{
				T control(adjustmentpos.attribute("value").remove('%').toDouble());
				control.setPlayMsg(playObj.moviePlayMsg());
				TimeObj transp(time, control);
				ret.push_back(transp);
			}
		}
	}

	template<class T>
	void AddToControl2(const QDomElement& parent, QVector<TimeObj>& ret, unsigned int startTime, unsigned int endtime,unsigned int timePointer, const MoviePlayObj& playObj)
	{
		for (QDomElement adjustmentpos = parent.firstChildElement("adjustmentpos"); !adjustmentpos.isNull(); adjustmentpos = adjustmentpos.nextSiblingElement("adjustmentpos"))
		{
			QString value = adjustmentpos.attribute("value");
			QStringList values = value.split(',');
			if (values.size() != 2)
			{
				continue;
			}
			unsigned int time = startTime + adjustmentpos.attribute("name").toUInt();
			if (time >= timePointer && time <= endtime)
			{
				T control(values[0].remove('%').toDouble(), values[1].remove('%').toDouble());
				control.setPlayMsg(playObj.moviePlayMsg());
				TimeObj transp(time, control);
				ret.push_back(transp);
			}
		}
	}
}

VOID CALLBACK TimerRoutine(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
	PlayControlThread* playControl = (PlayControlThread*)lpParam;
	QVector<ControlFunObj> timeToRun;
	//经过的时间
	ULARGE_INTEGER unsignedlargeinteger;
	FILETIME ft;
	GetSystemTimePreciseAsFileTime(&ft);//单位100ns
	unsignedlargeinteger.HighPart = ft.dwHighDateTime;
	unsignedlargeinteger.LowPart = ft.dwLowDateTime;
	
	quint64 timeLine = (unsignedlargeinteger.QuadPart/10000 - playControl->m_startTime);//单位ms
	if (timeLine - playControl->m_timePointer >= 200)//过了0.2秒钟
	{
		playControl->m_timePointer = timeLine;
		emit playControl->timePointerChanged(timeLine);
	}
	//只在此范围内加锁
	{
		QMutexLocker locker(&playControl->m_mutex);
		//将满足时间条件的函数放到执行队列,同时移动游标
		while (playControl->m_controls.size() > playControl->m_curExeIndex && playControl->m_controls[playControl->m_curExeIndex].timeMeet(timeLine))
		{
			timeToRun.push_back(playControl->m_controls[playControl->m_curExeIndex].controlFunction());
			++playControl->m_curExeIndex;
		}
	}

	//执行满足条件的函数
	for (ControlFunObj& obj : timeToRun)
	{
		obj();
	}
}

PlayControlThread::PlayControlThread()
{
	
}


PlayControlThread::~PlayControlThread()
{
	DeleteTimerQueue(m_hTimerQueue);
}

bool PlayControlThread::isPlaying()
{
	return m_hTimerQueue != nullptr;
}

void PlayControlThread::play()
{
	//计算开始时间
	FILETIME ft;
	ULARGE_INTEGER unsignedlargeinteger;
	GetSystemTimePreciseAsFileTime(&ft);
	unsignedlargeinteger.HighPart = ft.dwHighDateTime;
	unsignedlargeinteger.LowPart = ft.dwLowDateTime;
	m_startTime = unsignedlargeinteger.QuadPart/10000 - m_timePointer;
	//计算开始游标
	for (m_curExeIndex = 0; m_curExeIndex < m_controls.size(); ++m_curExeIndex)
	{
		if (m_controls[m_curExeIndex].time() >= m_timePointer)
		{
			break;
		}
	}

	if (!m_hTimerQueue)
	{
		m_hTimerQueue = CreateTimerQueue();
		bool b = CreateTimerQueueTimer(&m_hTimer, m_hTimerQueue, (WAITORTIMERCALLBACK)TimerRoutine, this, 0, 1, 0);
	}
}

void PlayControlThread::pause()
{
	DeleteTimerQueue(m_hTimerQueue);
	m_hTimerQueue = nullptr;
}

void PlayControlThread::stop()
{
	pause();
}

void PlayControlThread::locateTo(unsigned int time)
{
	m_timePointer = time;
}

void PlayControlThread::setData(const QDomElement& elem)
{
	stop();
	m_controls.clear();
	m_curExeIndex = 0;
	//生成控制信息
	m_controls = buildPlayControlObjs(elem);
	//按时间排序
	std::sort(m_controls.begin(), m_controls.end(), [](const TimeObj& l, const TimeObj& r) {return l.time() < r.time();});
}
QVector<TimeObj> PlayControlThread::buildMediaControlObj(const QDomElement& mediaElem)
{
	QVector<TimeObj> ret;
	//媒体配置开始时间
	unsigned int startTime = mediaElem.attribute("timeStart").toUInt();
	//媒体配置结束时间
	unsigned int endTime = startTime + mediaElem.attribute("timeLength").toUInt();
	//结束时间在时间指针之前
	if (endTime <= m_timePointer)
	{
		return ret;
	}

	//影片开始播放时跳过开头的时间
	unsigned int offset = mediaElem.attribute("offset").toUInt() + ((startTime < m_timePointer) ? (m_timePointer - startTime) : 0);
	//播放操作
	MoviePlayObj playObj(mediaElem.attribute("resourceId"), offset);
	ret.push_back(TimeObj(startTime < m_timePointer ? m_timePointer : startTime, playObj));

	QDomElement adjustmentlistElem = mediaElem.firstChildElement("adjustmentlist");
	for (QDomElement adjustment = adjustmentlistElem.firstChildElement("adjustment"); !adjustment.isNull(); adjustment = adjustment.nextSiblingElement("adjustment"))
	{
		if (adjustment.attribute("type") == "Transparency")
		{
			AddToControl1<TransparencyObj>(adjustment, ret, startTime, endTime, m_timePointer, playObj);
		}
		else if (adjustment.attribute("type") == "Position")
		{
			AddToControl2<PositionObj>(adjustment, ret, startTime, endTime, m_timePointer, playObj);
		}
		else if (adjustment.attribute("type") == "Rotate")
		{
			AddToControl1<RotateObj>(adjustment, ret, startTime, endTime, m_timePointer, playObj);
		}
		else if (adjustment.attribute("type") == "Scail")
		{
			AddToControl2<ScailObj>(adjustment, ret, startTime, endTime, m_timePointer, playObj);
		}
		else if (adjustment.attribute("type") == "Voice")
		{
			AddToControl1<VoiceObj>(adjustment, ret, startTime, endTime, m_timePointer, playObj);
		}
	}
	MovieEndObj endControl;
	endControl.setPlayMsg(playObj.moviePlayMsg());
	ret.push_back(TimeObj(endTime, endControl));

	return ret;
}

QVector<TimeObj> PlayControlThread::buildPlayControlObjs(const QDomElement& data)
{
	QVector<TimeObj> ret;
	QDomElement trackElem = data.firstChildElement("track");
	QDomElement layerlistElem = trackElem.firstChildElement("layerlist");
	for (QDomElement layerElem = layerlistElem.firstChildElement("layer"); !layerElem.isNull(); layerElem = layerElem.nextSiblingElement("layer"))
	{
		QDomElement child = layerElem.firstChildElement();
		if ("medialist" == child.nodeName())
		{
			for (QDomElement mediaElem = child.firstChildElement("media"); !mediaElem.isNull(); mediaElem = mediaElem.nextSiblingElement("media"))
			{
				ret.append(buildMediaControlObj(mediaElem));
			}
		}
		else if ("marklist" == child.nodeName())
		{
		}
	}
	return ret;
}

// 	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
// 	LARGE_INTEGER Frequency;
// 
// 	QueryPerformanceFrequency(&Frequency);
// 	QueryPerformanceCounter(&StartingTime);
// 
// 	// Activity to be timed
// 
// 	QueryPerformanceCounter(&EndingTime);
// 	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
// 
// 
// 	//
// 	// We now have the elapsed number of ticks, along with the
// 	// number of ticks-per-second. We use these values
// 	// to convert to the number of elapsed microseconds.
// 	// To guard against loss-of-precision, we convert
// 	// to microseconds *before* dividing by ticks-per-second.
// 	//
// 
// 	ElapsedMicroseconds.QuadPart *= 1000000;
// 	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;