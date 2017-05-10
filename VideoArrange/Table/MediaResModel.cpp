#include "DomItem.h"
#include "MediaResModel.h"
#include <QtXml>
#include <QColor>
#include <QFont>
#include <QPixmap>


const QStringList INVALID_NODE_LST = { "#cdata-section" ,"#comment" ,"#document" ,"#document-fragment" ,"#text" };
const char* const DOAG_DROP_TYPE = "LM-video-resource";
const int RES_ID = Qt::UserRole + 1;
//! [0]
MediaResModel::MediaResModel(QObject *parent)
	: QAbstractItemModel(parent)
{
	QFile file("project.xml");
	if (file.open(QIODevice::ReadOnly)) 
	{
		m_domDocConfig.setContent(&file);
		file.close();
	}
	QDomElement projectNode = m_domDocConfig.firstChildElement("project");
	QDomElement resNode = projectNode.firstChildElement("resourcelist");
	QString name = resNode.nodeName();
    rootItem = new DomItem(resNode, 0);
}
//! [0]

//! [1]
MediaResModel::~MediaResModel()
{
    delete rootItem;
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

//! [2]

//! [3]
QVariant MediaResModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

	DomItem *item = static_cast<DomItem*>(index.internalPointer());

	QDomNode node = item->node();
	QDomElement elem = node.toElement();

	//±‡º≠∫Õœ‘ æ
	if (Qt::DisplayRole == role)
	{
		switch (index.column())
		{
		case 0:
			return "col 0";
		case 2:
			return "2";
		case 3:
			return "3";
		case 4:
			return "4";
		case 5:
			return "5";
		default:
			return QVariant();
		}
	}
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
// 		return QVariant(QFont("Œ¢»Ì—≈∫⁄", 10));
// 	}

	return QVariant();
}

//! [4]
bool MediaResModel::setData(const QModelIndex &index, const QVariant &val, int role = Qt::EditRole)
{
	return QAbstractItemModel::setData(index, val, role);
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
				return tr("Name");
			case 1:
				return tr("Preview");
			case 2:
				return tr("Type");
			case 3:
				return tr("Resolution");
			case 4:
				return tr("Time Length");
			case 5:
				return tr("Location");
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
        parentItem = rootItem;
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

    if (!parentItem || parentItem == rootItem)
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
        parentItem = rootItem;
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
