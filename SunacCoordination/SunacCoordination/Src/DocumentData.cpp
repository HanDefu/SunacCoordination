#include "StdAfx.h"
#include "DocumentData.h"


CDocumentData::CDocumentData()
{
}

CDocumentData::~CDocumentData()
{
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

//////////////////////////////////////////////////////////////////////////
CDocumentFactory& CDocumentFactory::Instance()
{
	static CDocumentFactory s_eleFactory;
	return s_eleFactory;
}

CDocumentFactory::CDocumentFactory()
{
	m_bTempLoadDwg = false;
}
CDocumentFactory::~CDocumentFactory()
{
	RemoveAllDocment();
}

CDocumentData* CDocumentFactory::AddDocument(AcApDocument *pDoc)
{
	CDocumentData* pNewElecMrg = new CDocumentData();
	m_pElecMrgs[pDoc] = pNewElecMrg;

	return pNewElecMrg;
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
