#include "StdAfx.h"
#include "KitchenAutoName.h"


//////////////////////////////////////////////////////////////////////////
CKitchenAutoName::CKitchenAutoName()
{
}

CKitchenAutoName::~CKitchenAutoName()
{
}

CKitchenAutoName* CKitchenAutoName::GetInstance()
{
	static CKitchenAutoName instance;
	return &instance;
}

CString CKitchenAutoName::GetKitchenName(const AttrKitchen& p_att) const
{
	CString sKitchenName;
	sKitchenName.Format(L"%s_%02.0lf%02.0lf", p_att.m_prototypeCode.Mid(7), p_att.m_width / 100, p_att.m_height / 100);
	int count = 0;

	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].IsInstanceEqual(p_att))
		{
			sKitchenName = m_allTypeKitchens[i].m_instanceCode;
			sKitchenName.TrimRight(L"Mm");
			if (p_att.m_isMirror)
				sKitchenName += L"m";
			return sKitchenName;
		}
		if (m_allTypeKitchens[i].m_instanceCode.Find(sKitchenName) == 0)
			count++;
	}

	if (count > 0)
	{
		CString sNum;
		sNum.Format(L"_%d", count);
		sKitchenName += sNum;
	}

	if (p_att.m_isMirror)
		sKitchenName += L"m";

	return sKitchenName;
}

void CKitchenAutoName::AddKitchenType(const AttrKitchen& p_att)
{
	m_allTypeKitchens.push_back(p_att);
}

bool CKitchenAutoName::IsUserNameValid(const AttrKitchen& p_att, CString p_sName)
{
	if (p_sName.IsEmpty())
		return false;
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if ((p_sName == m_allTypeKitchens[i].m_instanceCode) && !m_allTypeKitchens[i].IsInstanceEqual(p_att))
			return false;
	}
	return true;
}
