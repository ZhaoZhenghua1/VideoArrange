/*
 * SizeGripItem - A size grip QGraphicsItem for interactive resizing.
 *
 * Copyright (c) 2011 Cesar L. B. Silveira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QBrush>
#include <QPen>
#include "SizeGripItem.h"

SizeGripItem::HandleItem::HandleItem(int positionFlags, SizeGripItem* parent)
    : QGraphicsRectItem(-4, -1, 8, 25, parent),
      m_positionFlags(positionFlags),
      m_parent(parent)
{
 //   setBrush(QBrush(Qt::lightGray));
	setPen(Qt::NoPen);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
	setAcceptHoverEvents(true);
}

int SizeGripItem::HandleItem::positionFlags() const
{
    return m_positionFlags;
}

QVariant SizeGripItem::HandleItem::itemChange(GraphicsItemChange change,
                                              const QVariant &value)
{
    QVariant retVal = value;

    if (change == ItemPositionChange)
    {
        retVal = restrictPosition(value.toPointF());
    }
    else if (change == ItemPositionHasChanged)
    {
        QPointF pos = value.toPointF();

        switch (m_positionFlags)
        {
            case TopLeft:
                m_parent->setTopLeft(pos);
                break;
            case Top:
                m_parent->setTop(pos.y());
                break;
            case TopRight:
                m_parent->setTopRight(pos);
                break;
            case Right:
                m_parent->setRight(pos.x());
                break;
            case BottomRight:
                m_parent->setBottomRight(pos);
                break;
            case Bottom:
                m_parent->setBottom(pos.y());
                break;
            case BottomLeft:
                m_parent->setBottomLeft(pos);
                break;
            case Left:
                m_parent->setLeft(pos.x());
                break;
        }
    }

    return retVal;
}

QPointF SizeGripItem::HandleItem::restrictPosition(const QPointF& newPos)
{
    QPointF retVal = pos();

    if (m_positionFlags & Top || m_positionFlags & Bottom)
        retVal.setY(newPos.y());

    if (m_positionFlags & Left || m_positionFlags & Right)
        retVal.setX(newPos.x());

    if (m_positionFlags & Top && retVal.y() > m_parent->m_rect.bottom())
        retVal.setY(m_parent->m_rect.bottom());
    else if (m_positionFlags & Bottom && retVal.y() < m_parent->m_rect.top())
        retVal.setY(m_parent->m_rect.top());

    if (m_positionFlags & Left && retVal.x() > m_parent->m_rect.right())
        retVal.setX(m_parent->m_rect.right());
    else if (m_positionFlags & Right && retVal.x() < m_parent->m_rect.left())
        retVal.setX(m_parent->m_rect.left());

    return retVal;
}

SizeGripItem::SizeGripItem(Resizer* resizer, QGraphicsItem* parent)
    : QGraphicsItem(parent),
      m_resizer(resizer)
{


//     handleItems_.append(new HandleItem(TopLeft, this));
//     handleItems_.append(new HandleItem(Top, this));
//     handleItems_.append(new HandleItem(TopRight, this));
    m_handleItems.append(new HandleItem(Right, this));
//     handleItems_.append(new HandleItem(BottomRight, this));
//     handleItems_.append(new HandleItem(Bottom, this));
//     handleItems_.append(new HandleItem(BottomLeft, this));
    m_handleItems.append(new HandleItem(Left, this));
	fixToParent();
}

SizeGripItem::~SizeGripItem()
{
    if (m_resizer)
        delete m_resizer;
}

QRectF SizeGripItem::boundingRect() const
{
    return m_rect;
}

void SizeGripItem::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget* widget)
{
}


void SizeGripItem::fixToParent()
{
	if (parentItem())
		m_rect = ((QGraphicsRectItem*)parentItem())->rect();
	updateHandleItemPositions();
}

#define IMPL_SET_FN(TYPE, POS)                  \
    void SizeGripItem::set ## POS (TYPE v)      \
    {                                           \
        m_rect.set ## POS (v);                   \
        doResize();                             \
    }

IMPL_SET_FN(qreal, Top)
IMPL_SET_FN(qreal, Right)
IMPL_SET_FN(qreal, Bottom)
IMPL_SET_FN(qreal, Left)
IMPL_SET_FN(const QPointF&, TopLeft)
IMPL_SET_FN(const QPointF&, TopRight)
IMPL_SET_FN(const QPointF&, BottomRight)
IMPL_SET_FN(const QPointF&, BottomLeft)

void SizeGripItem::doResize()
{
    if (m_resizer)
    {
        (*m_resizer)(parentItem(), m_rect);
        updateHandleItemPositions();
    }
}

void SizeGripItem::updateHandleItemPositions()
{
    foreach (HandleItem* item, m_handleItems)
    {
        item->setFlag(ItemSendsGeometryChanges, false);

        switch (item->positionFlags())
        {
            case TopLeft:
                item->setPos(m_rect.topLeft());
                break;
            case Top:
                item->setPos(m_rect.left() + m_rect.width() / 2 - 1,
                             m_rect.top());
                break;
            case TopRight:
                item->setPos(m_rect.topRight());
                break;
            case Right:
			{

				item->setPos(m_rect.right(), m_rect.top());
				QRectF rect = item->rect();
				rect.setHeight(m_rect.height());
				item->setRect(rect);
			}
                break;
            case BottomRight:
                item->setPos(m_rect.bottomRight());
                break;
            case Bottom:
                item->setPos(m_rect.left() + m_rect.width() / 2 - 1,
                             m_rect.bottom());
                break;
            case BottomLeft:
                item->setPos(m_rect.bottomLeft());
                break;
            case Left:
			{
                item->setPos(m_rect.left(),m_rect.top());
				QRectF rect = item->rect();
				rect.setHeight(m_rect.height());
				item->setRect(rect);
			}
                break;
        }

        item->setFlag(ItemSendsGeometryChanges, true);
    }
}
