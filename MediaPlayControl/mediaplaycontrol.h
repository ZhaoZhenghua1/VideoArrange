#ifndef MEDIAPLAYCONTROL_H
#define MEDIAPLAYCONTROL_H

#include "mediaplaycontrol_global.h"
#include <QObject>

class QDomDocument;
class MediaPlayControlPrivate;
class MEDIAPLAYCONTROL_EXPORT MediaPlayControl : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(MediaPlayControl)
public:
	MediaPlayControl();
	~MediaPlayControl();

	//设置数据
	void setData(const QDomDocument& data);
	//定位到时间
	void locateTo(unsigned int time);
	//播放，如果没有定位，则从0开始
	void play();
	//暂停
	void pause();
signals:
	//刷新时间指针
	void updateTimePointer(unsigned int time);
};

#endif // MEDIAPLAYCONTROL_H
