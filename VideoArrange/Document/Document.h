#pragma once

#include <QDomDocument>
#include <QList>

class MediaResModel;
class Observer;
class Document
{
public:
	Document();
	~Document();
public:
	static Document* instance();
	void open(const QString& path);
	//判断是否是有效的媒体文件，1、文件存在。2、文件格式支持
	bool isValidMediaFile(const QString& path);
	bool exist(const QString& path);
	int mediaType(const QString& path);
	void addMediaResFiles(const QStringList& files);
	QString createId();
	void save();
	QDomDocument document();
	QDomElement data(const QStringList& path);
	void addObserver(Observer * observer);
private:
	QDomDocument m_doc;
	QString m_fileName;
	QList<Observer*> m_lstObservers;
};

