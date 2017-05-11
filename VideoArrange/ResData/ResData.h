#pragma once

#include <QDomDocument>

class MediaResModel;
class ResData
{
public:
	ResData();
	~ResData();
public:
	static ResData* instance();
	void load(const QString& path);
	//判断是否是有效的媒体文件，1、文件存在。2、文件格式支持
	bool isValidMediaFile(const QString& path);
	bool exist(const QString& path);
	int mediaType(const QString& path);
	void addMediaResFiles(const QStringList& files);
	QString createId();
	void save();
	QDomDocument document();
	MediaResModel* createMediaResModel();
private:
	QDomDocument m_doc;
};

