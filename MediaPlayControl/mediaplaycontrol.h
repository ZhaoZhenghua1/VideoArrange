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

	//��λ��ʱ��
	void locateTo(unsigned int time);
	//���ţ����û�ж�λ�����0��ʼ
	void play();
	//��ͣ
	void pause();
	bool isPlaying();
signals:
	//ˢ��ʱ��ָ��
	void updateTimePointer(unsigned int time);
	//��ȡ�༭��Ϣ
	QDomDocument sigGetDocument();
};

#endif // MEDIAPLAYCONTROL_H
