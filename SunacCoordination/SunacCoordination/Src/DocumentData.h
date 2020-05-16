#pragma once

#include "..\Object\WindowDoor\WindowAutoName.h"
#include "..\Object\WindowDoor\InstanceCodeTextMrg.h"
#include "..\Object\WindowDoor\WinTangentOpenMap.h"
#include "..\Object\Kitchen\KitchenAutoName.h"
#include "..\Object\Bathroom\BathroomAutoName.h"
#include "ObjHighlight.h"

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

	CInstanceCodeTextMrg m_instanceCodeMrg;

	CWinTangentOpenMap m_winTangentMap;

	CObjsHighlight  m_hightLight; //用于过程处理，不需要保存

	bool m_bLoad;
};

CWindowAutoName* GetWindowAutoName();
CKitchenAutoName* GetKitchenAutoName();
CBathroomAutoName* GetBathroomAutoName();
CInstanceCodeTextMrg* GetInstanceCodeMrg();
CWinTangentOpenMap* GetWinTangentOpenMap();
CObjsHighlight* GetHightLightTool();


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

protected:
	map<AcApDocument*, CDocumentData*> m_pElecMrgs;
};

