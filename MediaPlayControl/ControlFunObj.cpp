#include "ControlFunObj.h"

struct PlayMsg
{
	QString res;
	qreal transparency = 1;
	qreal x = 0, y = 0;
	int rotate = 0;
	qreal scailx = 0, scaily = 0;
	qreal voice = 1;
};

void* play(const QString& res, unsigned int offset)
{
	qDebug() << "play " << res << " at:"<< offset;
	PlayMsg* playMsg = new PlayMsg;
	playMsg->res = res;
	return playMsg;
}

void endPlay(void * control)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << "end play " << p->res;
	}
	else
		Q_ASSERT(false);
}

void setTransparency(void * control, qreal trans)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << p->res << "setTransparency: before: " << p->transparency << " after: " << trans;
		p->transparency = trans;
	}
	else
		Q_ASSERT(false);
}

void setPosition(void * control, qreal x, qreal y)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << p->res << "setPosition: before: x:" << p->x << " y:" << p->y << " after: x:" << x << "y:" << y;
		p->x = x;
		p->y = y;
	}
	else
		Q_ASSERT(false);
}

void rotate(void * control, int angle)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << p->res << "rotate: before: " << p->rotate << " after: " << angle;
		p->rotate = angle;
	}
	else
		Q_ASSERT(false);
}

void scail(void * control, qreal x, qreal y)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << p->res << "scail: before: x:" << p->scailx << " y:" << p->scaily << " after: x:" << x << "y:" << y;
		p->scailx = x;
		p->scaily = y;
	}
	else
		Q_ASSERT(false);
}

void setVoice(void * control, qreal voice)
{
	if (PlayMsg * p = (PlayMsg*)(control))
	{
		qDebug() << p->res << "setVoice: before: " << p->voice << " after: " << voice;
		p->voice = voice;
	}
	else
		Q_ASSERT(false);
}

void MoviePlayObj::operator()()
{
	void* playMsg =  play(m_res, m_offsetTime);
	m_movieMsg->setPlayHandle(playMsg);
}

void MovieEndObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		endPlay(playMsg->playHandle());
	}
	else
	{
		Q_ASSERT(false);
	}
}

void TransparencyObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		setTransparency(playMsg->playHandle(), m_transparency);;
	}
	else
	{
		Q_ASSERT(false);
	}
}

void PositionObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		setPosition(playMsg->playHandle(), m_x, m_y);
	}
	else
	{
		Q_ASSERT(false);
	}
}

void RotateObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		rotate(playMsg->playHandle(), m_rotate);
	}
	else
	{
		Q_ASSERT(false);
	}
}

void ScailObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		scail(playMsg->playHandle(), m_scailX, m_scailY);
	}
	else
	{
		Q_ASSERT(false);
	}
}

void VoiceObj::operator()()
{
	if (const MoviePlayMsg* playMsg = this->playMsg())
	{
		setVoice(playMsg->playHandle(), m_voice);
	}
	else
	{
		Q_ASSERT(false);
	}
}
