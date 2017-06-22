#include "DomItem.h"
#include "MediaResModel.h"
#include <QtXml>
#include <QColor>
#include <QFont>
#include <QPixmap>
#include <QFileInfo>
#include "../Document/Document.h"

const QStringList INVALID_NODE_LST = { "#cdata-section" ,"#comment" ,"#document" ,"#document-fragment" ,"#text" };
const char* const DOAG_DROP_TYPE = "LM-video-resource";
const int RES_ID = Qt::UserRole + 1;
//! [0]
MediaResModel::MediaResModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	m_rootItem = new DomItem(m_elem, 0);
}
//! [0]

//! [1]
MediaResModel::~MediaResModel()
{
    delete m_rootItem;
}
//! [1]

//! [2]
int MediaResModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 6;
}

QStringList MediaResModel::mimeTypes() const
{
	return{ DOAG_DROP_TYPE };
}

QMimeData* MediaResModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();

	mimeData->setData(DOAG_DROP_TYPE, data(indexes.first(), RES_ID).toString().toStdString().c_str());
	return mimeData;
}

bool MediaResModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (!data->hasFormat(DOAG_DROP_TYPE))
	{
		return false;
	}

	return true;
}

Qt::DropActions MediaResModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

Qt::DropActions MediaResModel::supportedDragActions() const
{
	return Qt::CopyAction | Qt::MoveAction;
}

void MediaResModel::addFiles(const QStringList& files)
{
	QDomElement resNode = m_elem;

	for (auto ite = files.cbegin(); ite != files.cend(); ++ite)
	{
		if (Document::instance()->isValidMediaFile(*ite) && !Document::instance()->exist(*ite))
		{
			QDomElement elem = Document::instance()->document().createElement("resource");
			elem.setAttribute("filePath", *ite);
			elem.setAttribute("id", Document::instance()->createId());
			elem.setAttribute("type", Document::instance()->mediaType(*ite));
			resNode.appendChild(elem);
		}
	}
}

//! [2]

//! [3]
QVariant MediaResModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

	DomItem *item = static_cast<DomItem*>(index.internalPointer());

	QDomNode node = item->node();
	QDomElement elem = node.toElement();

	//编辑和显示
	if (Qt::DisplayRole == role)
	{
		switch (index.column())
		{
		case 0:
			return QFileInfo(elem.attribute("filePath")).fileName();
		case 2:
			return QString::fromLocal8Bit("类型");
		case 3:
			return QString::fromLocal8Bit("分辨率");
		case 4:
			return QString::fromLocal8Bit("时间长度");
		case 5:
			return QFileInfo(elem.attribute("filePath")).absoluteFilePath();
		default:
			return "";
		}
	}
	//todo:添加资源接口，提供缩略图，分辨率，长度等的信息
	else if (Qt::DecorationRole == role)
	{
		switch (index.column())
		{
		case 1:
			return QPixmap(":/map15.png").scaled(60,30);
		default:
			return QVariant();
		}
	}
	else if (RES_ID == role)
	{
		return elem.attribute("id");
	}
// 	else if (Qt::TextColorRole == role)
// 	{
// 		return QVariant(QColor(Qt::white));
// 	}
// 	else if (Qt::FontRole == role)
// 	{
// 		return QVariant(QFont("微软雅黑", 10));
// 	}

	return QVariant();
}

//! [4]
bool MediaResModel::setData(const QModelIndex &index, const QVariant &val, int role = Qt::EditRole)
{
	return QAbstractItemModel::setData(index, val, role);
}

void MediaResModel::setData(const QDomElement& resLstElem)
{
	m_elem = resLstElem;
	delete m_rootItem;
	m_rootItem = new DomItem(m_elem, 0);
}

//! [5]
Qt::ItemFlags MediaResModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

	return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled;
}
//! [5]

//! [6]
QVariant MediaResModel::headerData(int section, Qt::Orientation orientation,
                              int role) const
{
    if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			switch (section)
			{
			case 0:
				return QString::fromLocal8Bit("名称");
			case 1:
				return QString::fromLocal8Bit("缩略图");
			case 2:
				return QString::fromLocal8Bit("类型");
			case 3:
				return QString::fromLocal8Bit("分辨率");
			case 4:
				return QString::fromLocal8Bit("时间长度");
			case 5:
				return QString::fromLocal8Bit("位置");
			default:
				return QVariant();
			}
		}
		else if (role == Qt::BackgroundColorRole || role == Qt::BackgroundRole || role == Qt::ForegroundRole || role == Qt::TextColorRole)
		{
			return QVariant(QColor(50, 50, 50));
		}
		//BackgroundColorRole = 8,
			//BackgroundRole = 8,
			//TextColorRole = 9,
			//ForegroundRole = 9,


    }

    return QVariant();
}
//! [6]

//! [7]
QModelIndex MediaResModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
	
    DomItem *parentItem;

	if (!parent.isValid())
	{
        parentItem = m_rootItem;
	}
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());
//! [7]

//! [8]
    DomItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [8]

//! [9]
QModelIndex MediaResModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
    DomItem *parentItem = childItem->parent();

    if (!parentItem || parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [9]

//! [10]
int MediaResModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    DomItem *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DomItem*>(parent.internalPointer());
	QString nodename = parentItem->node().nodeName();

	QDomNodeList nodeLst = parentItem->node().childNodes();

	int count = nodeLst.count();
	for (int i = 0; i < nodeLst.count(); ++i)
	{
		if (INVALID_NODE_LST.contains(nodeLst.at(i).nodeName()))
		{
			--count;
		}
	}
	return count;
}
//! [10]
