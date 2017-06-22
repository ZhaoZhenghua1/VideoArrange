#pragma once

#include "Layer.h"

class IEditor;
class IOriginator;
class RightLayer : public LayerBase
{
public:
	RightLayer(IEditor* editor = nullptr);
	~RightLayer();
	IEditor* editor() const{ return m_editor; }
	void setEditor(IEditor* editor);
	int selectedIndex(IOriginator* orig) { return m_selectedOriginator == orig ? m_selectedIndex : -1; }
	void setSelected(IOriginator* orig, int index);
private:
	int m_selectedIndex = -1;
	IOriginator* m_selectedOriginator;
	//Öµ±à¼­Æ÷
	IEditor* m_editor = nullptr;
};

