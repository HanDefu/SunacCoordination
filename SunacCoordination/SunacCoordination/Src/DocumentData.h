#pragma once

#include "..\Object\WindowDoor\WindowAutoName.h"
#include "..\Object\Kitchen\KitchenAutoName.h"
#include "..\Object\Bathroom\BathroomAutoName.h"

//文档数据，都通过GetCurDocData获取，以便确保拿到的是当前文档的数据
class CDocumentData
{
public:
	CDocumentData();
	~CDocumentData();

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

public:
	CWindowAutoName m_winAutoName;
	CKitchenAutoName m_kitchenAutoName;
	CBathroomAutoName m_bathroomAutoName;

	CProtypeInstanceCodeMrg m_instanceCodeMrg;

	bool m_bLoad;
};

CWindowAutoName* GetWindowAutoName();
CKitchenAutoName* GetKitchenAutoName();
CBathroomAutoName* GetBathroomAutoName();
CProtypeInstanceCodeMrg* GetInstanceCodeMrg();


//////////////////////////////////////////////////////////////////////////
class CDocumentFactory
{
	CDocumentFactory();
public:
	virtual ~CDocumentFactory();
	static CDocumentFactory& Instance();

	CDocumentData* GetCurDocData();

	//在acrxEntryPoint函数kLoadDwgMsg消息时移除
	CDocumentData* AddDocument(AcApDocument *pDoc);

	//在acrxEntryPoint函数kUnloadDwgMsg消息时移除
	void RemoveDocument(AcApDocument *pDoc);
	void RemoveCurDoc();

	//在acrxEntryPoint函数kUnloadAppMsg消息时清除所有document
	void RemoveAllDocment();


	////是否为临时加载dwg，如预览其他dwg图、打开dwg作为块插入等情况，SetTempLoadDwg(true) SetTempLoadDwg(false) 配套使用
	//bool IsTempLoadDwg() { return Instance().m_bTempLoadDwg; }
	//void SetTempLoadDwg(bool bTemp){m_bTempLoadDwg = bTemp;}

protected:
	map<AcApDocument*, CDocumentData*> m_pElecMrgs;


	bool m_bTempLoadDwg;
};

