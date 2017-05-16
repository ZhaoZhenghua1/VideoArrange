#pragma once
#include <QDomElement>

class Observer
{
public:
	virtual ~Observer() = 0 {};
	//≥ı ºªØ
	virtual void init() = 0;
	QDomElement data(const QStringList& path);
};

