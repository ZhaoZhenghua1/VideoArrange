#pragma once
#include <QString>
#include <QPixmap>

class ResourceInterface
{
public:
	enum FileType
	{

	};
public:
	virtual QString path() = 0;
	virtual FileType type();
	virtual QString id() = 0;
	virtual QPixmap pixmap(int time = 0);
	virtual unsigned int timeLength() = 0;
};