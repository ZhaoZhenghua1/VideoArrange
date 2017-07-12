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

	//��������
	void setData(const QDomDocument& data);
	//��λ��ʱ��
	void locateTo(unsigned int time);
	//���ţ����û�ж�λ�����0��ʼ
	void play();
	//��ͣ
	void pause();
signals:
	//ˢ��ʱ��ָ��
	void updateTimePointer(unsigned int time);
};

#endif // MEDIAPLAYCONTROL_H
