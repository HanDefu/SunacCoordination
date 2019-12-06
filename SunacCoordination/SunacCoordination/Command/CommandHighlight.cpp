#include "stdafx.h"
#include "CommandHighlight.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_ACad.h"

CCommandHighlight::CCommandHighlight()
{
}


CCommandHighlight::~CCommandHighlight()
{
}

void CCommandHighlight::WindowDoorHighlight(vAcDbObjectId p_windoorIds)
{
	WindowDoorNoHighlight();
	for (UINT i = 0; i < p_windoorIds.size(); i++)
	{
		JHCOM_HilightObject(p_windoorIds[i], true);
		m_winHighlights.push_back(p_windoorIds[i]);
	}
}

void CCommandHighlight::WindowDoorNoHighlight()
{
	for (UINT i = 0; i < m_winHighlights.size(); i++)
	{
		JHCOM_HilightObject(m_winHighlights[i], false);
	}
	m_winHighlights.clear();
}
