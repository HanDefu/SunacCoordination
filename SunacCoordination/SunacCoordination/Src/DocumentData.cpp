#include "StdAfx.h"
#include "DocumentData.h"


CDocumentData::CDocumentData()
{
	m_bLoad = false;
}

CDocumentData::~CDocumentData()
{
}

Acad::ErrorStatus CDocumentData::ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version)
{
	m_winAutoName.ReadFromDwg(pFiler, p_version);
	m_kitchenAutoName.ReadFromDwg(pFiler, p_version);
	m_bathroomAutoName.ReadFromDwg(pFiler, p_version);

	m_bLoad = true; //YUAN DocSerialize

	return Acad::eOk;
}
Acad::ErrorStatus CDocumentData::WriteToDwg(AcDbDwgFiler* pFiler)
{
	m_winAutoName.WriteToDwg(pFiler);
	m_kitchenAutoName.WriteToDwg(pFiler);
	m_bathroomAutoName.WriteToDwg(pFiler);

	return Acad::eOk;
}
CWindowAutoName* GetWindowAutoName()
{
	return &(CDocumentFactory::Instance().GetCurDocData()->m_winAutoName);
}
CKitchenAutoName* GetKitchenAutoName()
{
	return &(CDocumentFactory::Instance().GetCurDocData()->m_kitchenAutoName);
}
CBathroomAutoName* GetBathroomAutoName()
{
	return &(CDocumentFactory::Instance().GetCurDocData()->m_bathroomAutoName);
}

CInstanceCodeTextMrg* GetInstanceCodeMrg()
{
	return &(CDocumentFactory::Instance().GetCurDocData()->m_instanceCodeMrg);
}


CWinTangentOpenMap* GetWinTangentOpenMap()
{
	return &(CDocumentFactory::Instance().GetCurDocData()->m_winTangentMap);
}

//////////////////////////////////////////////////////////////////////////
CDocumentFactory& CDocumentFactory::Instance()
{
	static CDocumentFactory s_eleFactory;
	return s_eleFactory;
}

CDocumentFactory::CDocumentFactory()
{
}
CDocumentFactory::~CDocumentFactory()
{
	RemoveAllDocment();
}

CDocumentData* CDocumentFactory::AddDocument(AcApDocument *pDoc)
{
	if (m_pElecMrgs.find(pDoc)!= m_pElecMrgs.end())
	{
		return m_pElecMrgs[pDoc];
	}
	else
	{
		CDocumentData* pNewElecMrg = new CDocumentData();
		m_pElecMrgs[pDoc] = pNewElecMrg;

		return pNewElecMrg;
	}
}

void CDocumentFactory::RemoveDocument(AcApDocument *pDoc)
{
	map<AcApDocument*, CDocumentData*>::iterator iter = m_pElecMrgs.find(pDoc);
	if (iter != m_pElecMrgs.end())
	{
		delete iter->second;
		m_pElecMrgs.erase(iter);
	}
}

CDocumentData* CDocumentFactory::GetCurDocData()
{
	AcApDocument* pDoc = acDocManager->curDocument();
	if (pDoc == NULL)
		return NULL;

	map<AcApDocument*, CDocumentData*>::iterator iter = m_pElecMrgs.find(pDoc);
	if (iter != m_pElecMrgs.end())
	{
		return iter->second;
	}
	else
	{
		return AddDocument(pDoc);
	}
}

void CDocumentFactory::RemoveCurDoc()
{
	AcApDocument* pDoc = acDocManager->curDocument();
	if (pDoc == NULL)
		return;

	RemoveDocument(pDoc);
}

void CDocumentFactory::RemoveAllDocment()
{
	for (map<AcApDocument*, CDocumentData*>::iterator it = m_pElecMrgs.begin(); it != m_pElecMrgs.end(); it++)
	{
		if (it->second != NULL)
		{
			delete (it->second);
			it->second = NULL;
		}
	}

	m_pElecMrgs.clear();
}

//////////////////////////////////////////////////////////////////////////
