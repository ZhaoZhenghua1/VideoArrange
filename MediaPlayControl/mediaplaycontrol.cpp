#include "mediaplaycontrol.h"
#include "PlayControlThread.h"
#include "ControlFunObj.h"
#include <QDomDocument>
#include "ControlFunObj.h"

struct MediaPlayControlPrivate
{
	PlayControlThread* m_playControlThread = nullptr;
};

MediaPlayControl::MediaPlayControl()
{
	d_func()->m_playControlThread = new PlayControlThread;
	connect(d_func()->m_playControlThread, SIGNAL(timePointerChanged(unsigned int)), this, SIGNAL(updateTimePointer(unsigned int)));
}

MediaPlayControl::~MediaPlayControl()
{
	pause();
}

void MediaPlayControl::setData(const QDomDocument& data)
{
	d_func()->m_playControlThread->stop();
	d_func()->m_playControlThread->setData(data.firstChildElement("project"));
}

void MediaPlayControl::locateTo(unsigned int time)
{
	d_func()->m_playControlThread->locateTo(time);
}

void MediaPlayControl::play()
{
	d_func()->m_playControlThread->play();
}

void MediaPlayControl::pause()
{
	d_func()->m_playControlThread->pause();
}
