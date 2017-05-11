#include "ResData.h"
#include "../Table/MediaResModel.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

const QStringList MAP_MEDIA = { "JPG","PNG","BMP","DIB","JPEG","JPE","JFIF","GIF","TIF","TIFF" };
const QStringList VIDEO_MEDIA = { "AVI","WMV","WMP","ASF","WMA","WAV","MID","MIDI","RM","RAM","RMVB","RA","RP","SMI","MP4" };//后面根据需求再追加

ResData::ResData()
{
	//temp
	load("project.xml");
}


ResData::~ResData()
{
}

ResData* ResData::instance()
{
	static ResData* ins = nullptr;
	if (nullptr == ins)
	{
		ins = new ResData;
	}
	return ins;
}

void ResData::load(const QString& path)
{
	QFile file(path);
	if (file.open(QIODevice::ReadOnly))
	{
		m_doc.setContent(&file);
		file.close();
	}
}

bool ResData::isValidMediaFile(const QString& path)
{
	QFileInfo fileInfo(path);
	if (!fileInfo.exists())
	{
		return false;
	}
	if (MAP_MEDIA.contains(fileInfo.suffix(),Qt::CaseInsensitive))
	{
		return true;
	}
	else if (VIDEO_MEDIA.contains(fileInfo.suffix(), Qt::CaseInsensitive))
	{
		return true;
	}
	return false;
}

bool ResData::exist(const QString& path)
{
	QFileInfo info(path);
	QDomElement projectNode = m_doc.firstChildElement("project");
	QDomElement resLstNode = projectNode.firstChildElement("resourcelist");
	QDomElement resNode = resLstNode.firstChildElement();

	for (QDomElement resNode = resLstNode.firstChildElement("resource"); !resNode.isNull(); resNode = resNode.nextSiblingElement("resource"))
	{
		QString path2 = resNode.attribute("filePath");
		if (info.absoluteFilePath() == QFileInfo(path2).absoluteFilePath())
		{
			return true;
		}
	}
	return false;
}

int ResData::mediaType(const QString& path)
{
	return 2;
}

QDomDocument ResData::document()
{
	return m_doc;
}

void ResData::addMediaResFiles(const QStringList& files)
{
	QDomElement projectNode = m_doc.firstChildElement("project");
	QDomElement resNode = projectNode.firstChildElement("resourcelist");

	bool changed = false;
	for (auto ite = files.cbegin(); ite != files.cend(); ++ite)
	{
		if (isValidMediaFile(*ite) && !exist(*ite))
		{
			QDomElement elem = ResData::instance()->document().createElement("resource");
			elem.setAttribute("filePath", *ite);
			elem.setAttribute("id", createId());
			elem.setAttribute("type", mediaType(*ite));
			resNode.appendChild(elem);
			changed = true;
		}
	}

	if (changed)
	{
		save();
	}
}

QString ResData::createId()
{
	QDomElement projectNode = m_doc.firstChildElement("project");
	QDomElement resLstNode = projectNode.firstChildElement("resourcelist");
	QDomElement resNode = resLstNode.firstChildElement();
	QList<QDomElement> lstElems;
	for (QDomElement resNode = resLstNode.firstChildElement("resource"); !resNode.isNull(); resNode = resNode.nextSiblingElement("resource"))
	{
		lstElems.push_back(resNode);
	}
	std::sort(lstElems.begin(), lstElems.end(), [](const QDomElement& l, const QDomElement& r) 
	{
		return l.attribute("id").replace("resourceID_","").toInt() < r.attribute("id").replace("resourceID_", "").toInt();
	});
	int index = 1;
	for ( auto ite = lstElems.cbegin(); ite != lstElems.cend(); ++ite, ++index)
	{
		if (index < ite->attribute("id").replace("resourceID_", "").toInt())
		{
			return QString("resourceID_%1").arg(index);
		}
	}
	return QString("resourceID_%1").arg(index);
}

void ResData::save()
{
	QFile file("project.xml");

	if (file.open(QIODevice::WriteOnly))
	{
		const int IndentSize = 4;
		QTextStream out(&file);
		m_doc.save(out, IndentSize);
		file.close();
	}
}

MediaResModel* ResData::createMediaResModel()
{
	QDomElement projectNode = m_doc.firstChildElement("project");
	QDomElement resNode = projectNode.firstChildElement("resourcelist");
	return new MediaResModel(resNode);
}
