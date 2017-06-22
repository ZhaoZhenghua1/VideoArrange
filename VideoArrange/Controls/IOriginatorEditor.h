#pragma once

class QString;
//原发器
class IOriginator
{
public:
	//设置数据
	virtual void setQsData(const QString& data) = 0;
	//获取数据
	virtual QString toQsData() = 0;
};

//编辑器
class IEditor
{
public:
	virtual void setOriginator(IOriginator* orig) { m_orig = orig; }
protected:
	IOriginator* m_orig = nullptr;
};

