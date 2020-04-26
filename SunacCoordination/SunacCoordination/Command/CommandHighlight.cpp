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

void CCommandHighlight::SunacHighlight(vAcDbObjectId p_sunacIds)
{
	SunacNoHighlight();
	for (UINT i = 0; i < p_sunacIds.size(); i++)
	{
		JHCOM_HilightObject(p_sunacIds[i], true);
		m_sunacHighlights.push_back(p_sunacIds[i]);
	}
}

void CCommandHighlight::SunacNoHighlight()
{
	for (UINT i = 0; i < m_sunacHighlights.size(); i++)
	{
		JHCOM_HilightObject(m_sunacHighlights[i], false);
	}
	m_sunacHighlights.clear();
}
