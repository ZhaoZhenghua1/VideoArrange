#include "Document.h"
#include "Observer.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>

const QStringList MAP_MEDIA = { "JPG","PNG","BMP","DIB","JPEG","JPE","JFIF","GIF","TIF","TIFF" };
const QStringList VIDEO_MEDIA = { "AVI","WMV","WMP","ASF","WMA","WAV","MID","MIDI","RM","RAM","RMVB","RA","RP","SMI","MP4" };//后面根据需求再追加

Document::Document()
{
	
}


Document::~Document()
{
}

Document* Document::instance()
{
	static Document* ins = nullptr;
	if (nullptr == ins)
	{
		ins = new Document;
	}
	return ins;
}

void Document::open(const QString& path)
{
	if (m_fileName == path)
	{
		return;
	}
	m_fileName = path;
 
	bool succeed = false;
	QFile file(path);
	if (file.open(QIODevice::ReadOnly))
	{
		QString errorMsg;
		int errorLine, errorColumn;
		if (m_doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
		{
			succeed = true;
		}
		else
		{
			QMessageBox::information(nullptr, "Error", errorMsg + QString(" errorLine:%1, errorColumn:%2").arg(errorLine).arg(errorColumn));
		}
		file.close();
	}

	if (succeed)
	{
		for (auto ite = m_lstObservers.begin(); ite != m_lstObservers.end(); ++ite)
		{
			(*ite)->init();
		}
	}
}

bool Document::isValidMediaFile(const QString& path)
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

bool Document::exist(const QString& path)
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

int Document::mediaType(const QString& path)
{
	return 2;
}

QDomDocument Document::document()
{
	return m_doc;
}

void Document::addMediaResFiles(const QStringList& files)
{
	QDomElement projectNode = m_doc.firstChildElement("project");
	QDomElement resNode = projectNode.firstChildElement("resourcelist");

	bool changed = false;
	for (auto ite = files.cbegin(); ite != files.cend(); ++ite)
	{
		if (isValidMediaFile(*ite) && !exist(*ite))
		{
			QDomElement elem = Document::instance()->document().createElement("resource");
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

QString Document::createId()
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

void Document::save()
{
	if (m_fileName.isEmpty())
	{
		return;
	}
	QFile file(m_fileName);

	if (file.open(QIODevice::WriteOnly))
	{
		const int IndentSize = 4;
		QTextStream out(&file);
		m_doc.save(out, IndentSize);
		file.close();
	}
}

// MediaResModel* Document::createMediaResModel()
// {
// 	QDomElement projectNode = m_doc.firstChildElement("project");
// 	QDomElement resNode = projectNode.firstChildElement("resourcelist");
// 	return new MediaResModel(resNode);
// }

QDomElement Document::data(const QStringList& path)
{
	QDomElement elem = m_doc.firstChildElement(path.first());
	for (auto ite = path.cbegin() + 1; ite != path.cend() && !elem.isNull(); ++ite)
	{
		elem = elem.firstChildElement(*ite);
	}
	return elem;
}

void Document::addObserver(Observer * observer)
{
	m_lstObservers.push_back(observer);
}
