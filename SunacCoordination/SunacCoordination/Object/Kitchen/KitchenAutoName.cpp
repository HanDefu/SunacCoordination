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
	CString sKitchenName = p_att.GetPrototypeCode();

	//镜像厨房增加"M"后缀
	CString sMirror;
	if (p_att.m_isMirror)
		sMirror = L"M";

	CString sKitchenFullName = sKitchenName + sMirror;

	//查找已存在的厨房
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].IsInstanceEqual(p_att))
			return m_allTypeKitchens[i].m_instanceCode;
	}

	//查找一个未被占用的门窗编号
	for (int i = 1; !IsNameValid(p_att, sKitchenFullName); i++)
	{
		sKitchenFullName.Format(L"%s_%d%s", sKitchenName, i, sMirror);
	}

	return sKitchenFullName;
}

void CKitchenAutoName::AddKitchenType(const AttrKitchen& p_att)
{
	//插入前确保不重名
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].m_instanceCode == p_att.m_instanceCode)
			return;
	}
	m_allTypeKitchens.push_back(p_att);
}

bool CKitchenAutoName::IsNameValid(const AttrKitchen& p_att, CString p_sName) const
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

void CKitchenAutoName::AutoNameAllKitchen()
{
	vector<AttrKitchen> temp = m_allTypeKitchens;
	m_allTypeKitchens.clear();
	for (UINT i = 0; i < temp.size(); i++)
	{
		temp[i].m_instanceCode = GetKitchenName(temp[i]);
		AddKitchenType(temp[i]);
	}
}

bool CKitchenAutoName::RenameKitchen(const AttrKitchen& p_att)
{
	if (!IsNameValid(p_att, p_att.m_instanceCode))
		return false;

	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
	{
		if (m_allTypeKitchens[i].IsInstanceEqual(p_att))
		{
			m_allTypeKitchens[i].m_instanceCode = p_att.m_instanceCode;
			return true;
		}
	}

	return false;
}

Acad::ErrorStatus CKitchenAutoName::ReadFromDwg(AcDbDwgFiler* pFiler)
{
	Adesk::UInt32 size;
	pFiler->readItem(&size);
	m_allTypeKitchens.resize(size);
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
		m_allTypeKitchens[i].dwgInFields(pFiler);
	return Acad::eOk;
}

Acad::ErrorStatus CKitchenAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeKitchens.size());
	for (UINT i = 0; i < m_allTypeKitchens.size(); i++)
		m_allTypeKitchens[i].dwgOutFields(pFiler);
	return Acad::eOk;
}
