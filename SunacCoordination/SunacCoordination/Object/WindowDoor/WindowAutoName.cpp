#include "StdAfx.h"
#include "WindowAutoName.h"


//////////////////////////////////////////////////////////////////////////
CWindowAutoName::CWindowAutoName()
{
}

CWindowAutoName::~CWindowAutoName()
{
}

CWindowAutoName* CWindowAutoName::GetInstance()
{
	static CWindowAutoName instance;
	return &instance;
}

CString CWindowAutoName::GetWindowName(const AttrWindow& p_att) const
{
	CString sWindowName;
	CString sWidth, sHeight;
	sWidth.Format(L"%02.0lf", p_att.GetW());
	sHeight.Format(L"%02.0lf", p_att.GetH());
	sWindowName.Format(L"%s_%s%s", p_att.m_prototypeCode.Mid(7), sWidth.Left(2), sHeight.Left(2));
	int count = 0;

	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].IsInstanceEqual(p_att))
		{
			sWindowName = m_allTypeWindows[i].m_instanceCode;
			sWindowName.TrimRight(L"Mm");
			if (p_att.m_isMirror && !p_att.m_isMirrorWindow)
				sWindowName += L"m";
			return sWindowName;
		}
		if (m_allTypeWindows[i].m_instanceCode.Find(sWindowName) == 0)
			count++;
	}

	if (count > 0)
	{
		CString sNum;
		sNum.Format(L"_%d", count);
		sWindowName += sNum;
	}

	if (p_att.m_isMirror && !p_att.m_isMirrorWindow)
		sWindowName += L"m";

	return sWindowName;
}

void CWindowAutoName::AddWindowType(const AttrWindow& p_att)
{
	m_allTypeWindows.push_back(p_att);
}

bool CWindowAutoName::IsUserNameValid(const AttrWindow& p_att, CString p_sName)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if ((p_att.m_instanceCode == m_allTypeWindows[i].m_instanceCode) && !m_allTypeWindows[i].IsInstanceEqual(p_att))
			return false;
	}
	return true;
}
