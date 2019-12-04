#pragma once


//文档数据，都通过GetCurElecMrg获取，以便确保拿到的是当前文档的数据
class CDocumentData
{
public:
	CDocumentData();
	~CDocumentData();

protected:

};

//////////////////////////////////////////////////////////////////////////
class CDocumentFactory
{
	CDocumentFactory();
public:
	static CDocumentFactory& Instance();
	static CDocumentData* GetCurElecMrg();

	static void RemoveCurElecMrg();

	virtual ~CDocumentFactory();

	//是否为临时加载dwg，如预览其他dwg图、打开dwg作为块插入等情况，SetTempLoadDwg(true) SetTempLoadDwg(false) 配套使用
	static bool IsTempLoadDwg() { return Instance().m_bTempLoadDwg; }
	static void SetTempLoadDwg(bool bTemp);

protected:
	map<AcApDocument*, CDocumentData*> m_pElecMrgs;

	bool m_bTempLoadDwg;
};

