#include "RightLayer.h"
#include "Controls/IOriginatorEditor.h"
#include <QGraphicsAnchorLayout>

RightLayer::RightLayer(IEditor* editor):m_editor(editor)
{
	QGraphicsAnchorLayout* lay = new QGraphicsAnchorLayout;
	lay->setContentsMargins(0, 0, 0, 0);
	lay->setSpacing(0);
	setLayout(lay);
}


RightLayer::~RightLayer()
{
}

void RightLayer::setEditor(IEditor* editor)
{
	m_editor = editor;
}

void RightLayer::setSelected(IOriginator* orig, int index)
{
	m_selectedOriginator = orig; 
	m_selectedIndex = index;
	m_editor->setOriginator(orig);
	update();
}
