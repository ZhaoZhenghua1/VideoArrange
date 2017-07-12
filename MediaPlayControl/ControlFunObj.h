#pragma once
#include <QString>
#include <QVector>
#include <functional>
#include <QDebug>
#include <memory>
//���ſ��Ƶĺ�������
typedef std::function<void(void)> ControlFunObj;

class TimeObj
{
public:
	TimeObj(unsigned int time = 0, const ControlFunObj& control = []() {}) :m_time(time), m_fun(control) {}
	
	//ʱ������
	bool timeMeet(unsigned int time)	{return m_time <= time;}
	unsigned int time()const { return m_time; }
	//���ƺ���
	ControlFunObj controlFunction()	{return m_fun;}

protected:
	unsigned int m_time = 0;//ʱ��ms
	ControlFunObj m_fun = nullptr;//��Ҫִ�еĲ���
};

//��Դ����
class MovieLoadObj
{
public:
	MovieLoadObj(const QString& resId) {}
};

//��Դ�ͷ�
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
	//���ò��Žӿں󣬷��صĲ��ž��������ֹͣ�����õ�
	void* m_playMovieHandle = nullptr;
};
typedef std::shared_ptr<MoviePlayMsg> SharedMovieMsg;
typedef std::weak_ptr<MoviePlayMsg> WeakMovieMsg;

//������Ƶ
class MoviePlayObj
{
public:
	MoviePlayObj(const QString& resId, unsigned int offset):m_res(resId),m_offsetTime(offset)
	{
		m_movieMsg = std::make_shared<MoviePlayMsg>();
	}
	const SharedMovieMsg& moviePlayMsg() const { return m_movieMsg; }
public:
	//�м�Ҫ�Ѳ�����Ƶ�ľ�����ݸ�������Ƶ�����Ŀ���
	void operator()();
private:
	QString m_res;
	//��ӰƬ��ĳ��ʱ�俪ʼ���ţ����Ǵ�ͷ����
	unsigned int m_offsetTime = 0;
	SharedMovieMsg m_movieMsg;
};

//���Ʋ���
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
	//���ò��ž�������������������Ž���
	void setPlayMsg(const SharedMovieMsg& msg) { m_movieMsg = msg; }
protected:
	WeakMovieMsg m_movieMsg;
};

//������Ƶ����
class MovieEndObj : public SubPlayControl
{
public:
	MovieEndObj() {}
public:
	void operator()();
};

//����͸����
class TransparencyObj : public SubPlayControl
{
public:
	TransparencyObj(qreal transparency):m_transparency(transparency){}
public:
	void operator()();
private:
	qreal m_transparency = 1;
};

//����λ��
class PositionObj : public SubPlayControl
{
public:
	PositionObj(qreal x, qreal y):m_x(x),m_y(y){}
public:
	void operator()();
private:
	qreal m_x = 0, m_y = 0;
};

//��ת
class RotateObj : public SubPlayControl
{
public:
	RotateObj(int rotate) :m_rotate(rotate){}
public:
	void operator()();
private:
	int m_rotate = 0;
};

//����
class ScailObj : public SubPlayControl
{
public:
	ScailObj(qreal x, qreal y):m_scailX(x),m_scailY(y){}
public:
	void operator()();
private:
	qreal m_scailX = 0, m_scailY = 0;
};

//��������
class VoiceObj : public SubPlayControl
{
public:
	VoiceObj(qreal voice):m_voice(voice){}
public:
	void operator()();
private:
	qreal m_voice = 1;
};


