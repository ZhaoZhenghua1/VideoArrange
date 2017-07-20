#ifndef MEDIAPLAYCONTROL_H
#define MEDIAPLAYCONTROL_H

#include "mediaplaycontrol_global.h"
#include <QObject>
#include <QDomDocument>

class MediaPlayControlPrivate;
class MEDIAPLAYCONTROL_EXPORT MediaPlayControl : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(MediaPlayControl)
public:
	MediaPlayControl();
	~MediaPlayControl(); 

	//定位到时间
	void locateTo(unsigned int time);
	//播放，如果没有定位，则从0开始
	void play();
	//暂停
	void pause();
	bool isPlaying();
signals:
	//刷新时间指针
	void updateTimePointer(unsigned int time);
	//获取编辑信息
	QDomDocument sigGetDocument();
};

#endif // MEDIAPLAYCONTROL_H
