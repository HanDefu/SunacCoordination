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
	//去除原型编号中的后缀
	CString prototype = p_att.m_prototypeCode;
	int pos = prototype.Find(L'_');
	if (pos != -1)
		prototype = prototype.Left(pos);
	//根据"原型编号_尺寸编号"生成门窗编号
	sBathroomName.Format(L"%s-%.0lf×%.0lf", prototype, p_att.m_width, p_att.m_height);

	if (prototype.Find(L"_g"))
		sBathroomName += L"/g";

	//镜像卫生间增加"_m"后缀
	CString sMirror;
	if (p_att.m_isMirror)
		sMirror = L"_m";

	CString sBathroomFullName = sBathroomName + sMirror;

	//查找已存在的卫生间
	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
	{
		if (m_allTypeBathrooms[i].IsInstanceEqual(p_att))
			return m_allTypeBathrooms[i].m_instanceCode;
	}

	//查找一个未被占用的门窗编号
	for (int i = 1; !IsNameValid(p_att, sBathroomFullName); i++)
	{
		sBathroomFullName.Format(L"%s_%d%s", sBathroomName, i, sMirror);
	}

	return sBathroomFullName;
}

void CBathroomAutoName::AddBathroomType(const AttrBathroom& p_att)
{
	//插入前确保不重名
	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
	{
		if (m_allTypeBathrooms[i].m_instanceCode == p_att.m_instanceCode)
			return;
	}
	m_allTypeBathrooms.push_back(p_att);
}

bool CBathroomAutoName::IsNameValid(const AttrBathroom& p_att, CString p_sName) const
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

void CBathroomAutoName::AutoNameAllBathroom()
{
	vector<AttrBathroom> temp = m_allTypeBathrooms;
	m_allTypeBathrooms.clear();
	for (UINT i = 0; i < temp.size(); i++)
	{
		temp[i].m_instanceCode = GetBathroomName(temp[i]);
		AddBathroomType(temp[i]);
	}
}

bool CBathroomAutoName::RenameBathroom(const AttrBathroom& p_att)
{
	if (!IsNameValid(p_att, p_att.m_instanceCode))
		return false;

	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
	{
		if (m_allTypeBathrooms[i].IsInstanceEqual(p_att))
		{
			m_allTypeBathrooms[i].m_instanceCode = p_att.m_instanceCode;
			return true;
		}
	}

	return false;
}

Acad::ErrorStatus CBathroomAutoName::ReadFromDwg(AcDbDwgFiler* pFiler)
{
	Adesk::UInt32 size;
	pFiler->readItem(&size);
	m_allTypeBathrooms.resize(size);
	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
		m_allTypeBathrooms[i].dwgInFields(pFiler);
	return Acad::eOk;
}

Acad::ErrorStatus CBathroomAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeBathrooms.size());
	for (UINT i = 0; i < m_allTypeBathrooms.size(); i++)
		m_allTypeBathrooms[i].dwgOutFields(pFiler);
	return Acad::eOk;
}
