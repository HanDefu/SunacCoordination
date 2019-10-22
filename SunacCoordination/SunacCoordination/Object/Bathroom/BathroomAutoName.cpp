#include "StdAfx.h"
#include "BathroomAutoName.h"


//////////////////////////////////////////////////////////////////////////
CBathroomAutoName::CBathroomAutoName()
{
}

CBathroomAutoName::~CBathroomAutoName()
{
}

CBathroomAutoName* CBathroomAutoName::GetInstance()
{
	static CBathroomAutoName instance;
	return &instance;
}

CString CBathroomAutoName::GetBathroomName(const AttrBathroom& p_att) const
{
	CString sBathroomName;
	sBathroomName.Format(L"%s_%02.0lf%02.0lf", p_att.m_prototypeCode.Mid(7), p_att.m_width / 100, p_att.m_height / 100);
	int count = 0;

	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
	{
		if (m_allTypeBathrooms[i].IsInstanceEqual(p_att))
		{
			sBathroomName = m_allTypeBathrooms[i].m_instanceCode;
			sBathroomName.TrimRight(L"Mm");
			if (p_att.m_isMirror)
				sBathroomName += L"m";
			return sBathroomName;
		}
		if (m_allTypeBathrooms[i].m_instanceCode.Find(sBathroomName) == 0)
			count++;
	}

	if (count > 0)
	{
		CString sNum;
		sNum.Format(L"_%d", count);
		sBathroomName += sNum;
	}

	if (p_att.m_isMirror)
		sBathroomName += L"m";

	return sBathroomName;
}

void CBathroomAutoName::AddBathroomType(const AttrBathroom& p_att)
{
	m_allTypeBathrooms.push_back(p_att);
}

bool CBathroomAutoName::IsUserNameValid(const AttrBathroom& p_att, CString p_sName)
{
	if (p_sName.IsEmpty())
		return false;
	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
	{
		if ((p_sName == m_allTypeBathrooms[i].m_instanceCode) && !m_allTypeBathrooms[i].IsInstanceEqual(p_att))
			return false;
	}
	return true;
}
