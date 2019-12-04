#include "StdAfx.h"
#include "DocumentData.h"


CDocumentData::CDocumentData()
{
}

CDocumentData::~CDocumentData()
{
}



CDocumentFactory::CDocumentFactory()
{
	m_bTempLoadDwg = false;
}
CDocumentFactory::~CDocumentFactory()
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

void CDocumentFactory::SetTempLoadDwg(bool bTemp)
{
	Instance().m_bTempLoadDwg = bTemp;
}

CDocumentFactory& CDocumentFactory::Instance()
{
	static CDocumentFactory s_eleFactory;
	return s_eleFactory;
}

CDocumentData* CDocumentFactory::GetCurElecMrg()
{
	AcApDocument* npDoc = acDocManager->curDocument();
	if (npDoc == NULL)
	{
		return NULL;
	}

	map<AcApDocument*, CDocumentData*>& elecMrgs = CDocumentFactory::Instance().m_pElecMrgs;

	map<AcApDocument*, CDocumentData*>::iterator iter = elecMrgs.find(npDoc);

	if (iter != elecMrgs.end())
	{
		return iter->second;
	}
	else
	{
		//加入一个新的
		CDocumentData* pNewElecMrg = new CDocumentData();
		elecMrgs[npDoc] = pNewElecMrg;

		return pNewElecMrg;
	}
}

void CDocumentFactory::RemoveCurElecMrg()
{
	AcApDocument* npDoc = acDocManager->curDocument();
	if (npDoc == NULL)
	{
		return;
	}

	map<AcApDocument*, CDocumentData*>& elecMrgs = CDocumentFactory::Instance().m_pElecMrgs;

	map<AcApDocument*, CDocumentData*>::iterator iter = elecMrgs.find(npDoc);
	if (iter != elecMrgs.end())
	{
		delete iter->second;
		elecMrgs.erase(iter);
	}
}
