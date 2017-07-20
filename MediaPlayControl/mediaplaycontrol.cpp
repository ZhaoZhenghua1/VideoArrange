#include "mediaplaycontrol.h"
#include "PlayControlThread.h"
#include "ControlFunObj.h"
#include <QDomDocument>
#include "ControlFunObj.h"
#include <QEvent>
#include <QKeyEvent> 

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

void MediaPlayControl::locateTo(unsigned int time)
{
	d_func()->m_playControlThread->locateTo(time);
}

void MediaPlayControl::play()
{
	QDomDocument document = emit sigGetDocument();
	d_func()->m_playControlThread->setData(document.firstChildElement("project"));
	d_func()->m_playControlThread->play();
}

void MediaPlayControl::pause()
{
	d_func()->m_playControlThread->pause();
}

bool MediaPlayControl::isPlaying()
{
	return d_func()->m_playControlThread->isPlaying();
}

// bool MediaPlayControl::eventFilter(QObject *watched, QEvent *event)
// {
// 	if (event->type() == QEvent::KeyPress)
// 	{
// 		QKeyEvent* keyEvent = (QKeyEvent*)(event);
// 		if (keyEvent->key() == Qt::Key_Space)
// 		{
// 			if (d_func()->m_playControlThread->isPlaying())
// 			{
// 				d_func()->m_playControlThread->pause();
// 			}
// 			else
// 			{
// 				d_func()->m_playControlThread->play();
// 			}
// 		}
// 	}
// 	return QObject::eventFilter(watched, event);
// }
