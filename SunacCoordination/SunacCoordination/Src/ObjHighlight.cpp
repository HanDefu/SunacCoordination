#include "stdafx.h"
#include "ObjHighlight.h"
#include "../Common/ComFun_Sunac.h"
#include "../Common/ComFun_ACad.h"

CObjsHighlight::CObjsHighlight()
{
}

CObjsHighlight::~CObjsHighlight()
{
}

void CObjsHighlight::Highlight(vAcDbObjectId p_sunacIds)
{
	NoHighlight();

	for (UINT i = 0; i < p_sunacIds.size(); i++)
	{
		HighlightObject(p_sunacIds[i], true);
		m_sunacHighlights.push_back(p_sunacIds[i]);
	}
}

void CObjsHighlight::NoHighlight()
{
	for (UINT i = 0; i < m_sunacHighlights.size(); i++)
	{
		JHCOM_HilightObject(m_sunacHighlights[i], false);
	}
	m_sunacHighlights.clear();
}


void CObjsHighlight::HighlightObject(const AcDbObjectId& objId, bool highlight)
{
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if (pEnt == NULL)
		return;

	if (highlight)
		pEnt->highlight();
	else
		pEnt->unhighlight();

	pEnt->close();
}