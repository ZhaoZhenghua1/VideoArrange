#pragma once
#include <QString>
#include <QVector>
#include <functional>
#include <QDebug>
#include <memory>
//播放控制的函数对象
typedef std::function<void(void)> ControlFunObj;

class TimeObj
{
public:
	TimeObj(unsigned int time = 0, const ControlFunObj& control = []() {}) :m_time(time), m_fun(control) {}
	
	//时间满足
	bool timeMeet(unsigned int time)	{return m_time <= time;}
	unsigned int time()const { return m_time; }
	//控制函数
	ControlFunObj controlFunction()	{return m_fun;}

protected:
	unsigned int m_time = 0;//时间ms
	ControlFunObj m_fun = nullptr;//需要执行的操作
};

//资源加载
class MovieLoadObj
{
public:
	MovieLoadObj(const QString& resId) {}
};

//资源释放
class MovieReleaseObj
{
public:
	MovieReleaseObj(const QString& resId) {}
};

class MoviePlayMsg
{
public:
	void setPlayHandle(void* playHandle) { m_playMovieHandle = playHandle; }
	void* playHandle() const { return m_playMovieHandle; }
private:
	//调用播放接口后，返回的播放句柄，用以停止，设置等
	void* m_playMovieHandle = nullptr;
};
typedef std::shared_ptr<MoviePlayMsg> SharedMovieMsg;
typedef std::weak_ptr<MoviePlayMsg> WeakMovieMsg;

//播放视频
class MoviePlayObj
{
public:
	MoviePlayObj(const QString& resId, unsigned int offset):m_res(resId),m_offsetTime(offset)
	{
		m_movieMsg = std::make_shared<MoviePlayMsg>();
	}
	const SharedMovieMsg& moviePlayMsg() const { return m_movieMsg; }
public:
	//中间要把播放视频的句柄传递给播放视频结束的控制
	void operator()();
private:
	QString m_res;
	//从影片的某个时间开始播放，不是从头播放
	unsigned int m_offsetTime = 0;
	SharedMovieMsg m_movieMsg;
};

//控制播放
class SubPlayControl
{
public:
	const MoviePlayMsg* playMsg()
	{
		if (!m_movieMsg.expired())
		{
			SharedMovieMsg sharedMsg = m_movieMsg.lock();
			if (sharedMsg)
			{
				return sharedMsg.get();
			}
		}
		return nullptr;
	}
	//设置播放句柄，用来控制视屏播放结束
	void setPlayMsg(const SharedMovieMsg& msg) { m_movieMsg = msg; }
protected:
	WeakMovieMsg m_movieMsg;
};

//播放视频结束
class MovieEndObj : public SubPlayControl
{
public:
	MovieEndObj() {}
public:
	void operator()();
};

//调节透明度
class TransparencyObj : public SubPlayControl
{
public:
	TransparencyObj(qreal transparency):m_transparency(transparency){}
public:
	void operator()();
private:
	qreal m_transparency = 1;
};

//调整位置
class PositionObj : public SubPlayControl
{
public:
	PositionObj(qreal x, qreal y):m_x(x),m_y(y){}
public:
	void operator()();
private:
	qreal m_x = 0, m_y = 0;
};

//旋转
class RotateObj : public SubPlayControl
{
public:
	RotateObj(int rotate) :m_rotate(rotate){}
public:
	void operator()();
private:
	int m_rotate = 0;
};

//缩放
class ScailObj : public SubPlayControl
{
public:
	ScailObj(qreal x, qreal y):m_scailX(x),m_scailY(y){}
public:
	void operator()();
private:
	qreal m_scailX = 0, m_scailY = 0;
};

//调节声音
class VoiceObj : public SubPlayControl
{
public:
	VoiceObj(qreal voice):m_voice(voice){}
public:
	void operator()();
private:
	qreal m_voice = 1;
};


