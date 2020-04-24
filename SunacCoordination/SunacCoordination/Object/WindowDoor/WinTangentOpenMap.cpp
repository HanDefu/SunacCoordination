#include "StdAfx.h"
#include "WinTangentOpenMap.h"


void CWinTangentOpenMap::AddWindow(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenId)
{
	if (p_winId==AcDbObjectId::kNull)
		return;

	m_idsMap[p_winId] = p_tangentOpenId;
}

AcDbObjectId CWinTangentOpenMap::GetTangentOpenId(AcDbObjectId p_winId)
{
	if (m_idsMap.find(p_winId)!=m_idsMap.end())
	{
		return m_idsMap[p_winId];
	}

	return AcDbObjectId::kNull;
}
void CWinTangentOpenMap::RemoveWindow(AcDbObjectId p_winId)
{
	map<AcDbObjectId, AcDbObjectId>::iterator it = m_idsMap.find(p_winId);
	if (it != m_idsMap.end())
	{
		m_idsMap.erase(it);
	}
}


Acad::ErrorStatus CWinTangentOpenMap::ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version)
{
	if (p_version<8) //FILE_VERSION 8
	{
		Acad::eOk;
	}

	Adesk::UInt32 nMapSize;
	pFiler->readItem(&nMapSize);

	m_idsMap.clear();
	for (UINT i = 0; i < nMapSize; i++)
	{
		AcDbHandle tempHandle1;
		pFiler->readItem(&tempHandle1);
		AcDbObjectId winId = AcDbObjectId::kNull;
		Acad::ErrorStatus es1 = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(winId, false, tempHandle1);

		AcDbHandle tempHandle2;
		pFiler->readItem(&tempHandle2);
		AcDbObjectId tangentOpenId = AcDbObjectId::kNull;
		Acad::ErrorStatus es2 = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(tangentOpenId, false, tempHandle2);

		AcDbObjectId retId = AcDbObjectId::kNull;
		if (es1 == Acad::eOk && es2==Acad::eOk)
		{
			m_idsMap[winId] = tangentOpenId;
		}
	}
	return Acad::eOk;
}

Acad::ErrorStatus CWinTangentOpenMap::WriteToDwg(AcDbDwgFiler* pFiler)
{
	//FILE_VERSION 8 °æ±¾ ÐÂÌí¼Ó
	pFiler->writeItem((Adesk::UInt32)m_idsMap.size());
	for (map<AcDbObjectId, AcDbObjectId>::iterator it = m_idsMap.begin(); it != m_idsMap.end(); it++)
	{
		pFiler->writeItem(it->first.handle());
		pFiler->writeItem(it->second.handle());
	}
	return Acad::eOk;
}
