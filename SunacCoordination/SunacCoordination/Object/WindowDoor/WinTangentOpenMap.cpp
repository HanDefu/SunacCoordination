#include "StdAfx.h"
#include "WinTangentOpenMap.h"


void CWinTangentOpenMap::AddWindow(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenId)
{
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