#pragma once

class QString;
//ԭ����
class IOriginator
{
public:
	//��������
	virtual void setQsData(const QString& data) = 0;
	//��ȡ����
	virtual QString toQsData() = 0;
};

//�༭��
class IEditor
{
public:
	virtual void setOriginator(IOriginator* orig) { m_orig = orig; }
protected:
	IOriginator* m_orig = nullptr;
};

