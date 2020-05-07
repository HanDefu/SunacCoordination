#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrRailing.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_Math.h"
#include "../../Common/TYFormula.h"
#include "RCRailing.h"
//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrRailing, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALERAILING, ZffCustomObjectDBRAILING);
AttrRailing::AttrRailing()
{
	m_height = 1200;		//栏杆高度
	m_fanKanHeight = 200;	//反坎高度
	m_length = 3000;		//栏杆长
	m_railingType = E_RAILING_TIEYI; //栏杆类型

	m_viewDir = E_VIEW_FRONT;
}

AttrRailing::~AttrRailing()
{

}

//AttrRailing::AttrRailing(const AttrRailing &other) : AttrObject(other)
//{
//
//}
//
//AttrRailing & AttrRailing::operator=(const AttrRailing &rhs)
//{
//	AttrObject::operator=(rhs);
//
//	return *this;
//}

Acad::ErrorStatus AttrRailing::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->readItem(&m_height);
	filer->readItem(&m_fanKanHeight);
	filer->readItem(&m_length);
	filer->readItem((Adesk::UInt32*)&m_railingType);

	return filer->filerStatus();
}

Acad::ErrorStatus AttrRailing::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->writeItem(m_height);
	filer->writeItem(m_fanKanHeight);
	filer->writeItem(m_length);
	filer->writeItem((Adesk::UInt32)m_railingType);
	
	return filer->filerStatus();
}

bool AttrRailing::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrRailing * pRealObj = dynamic_cast<AttrRailing *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}

bool AttrRailing::IsPrototypeEqual_test(const AttrRailing& p_att)
{
	if (p_att.m_height != m_height)
	{
		return false;
	}
	else if (p_att.m_fanKanHeight != m_fanKanHeight)
	{
		return false;
	}
	else if (p_att.m_length != m_length)
	{
		return false;
	}
	else if (p_att.m_railingType != m_railingType)
	{
		return false;
	}

	else if (p_att.m_prototypeCode != m_prototypeCode)
	{
		return false;
	}
	//else if (p_att.m_type != m_type)
	//{
	//	return false;
	//}
	else if (p_att.m_isDynamic != m_isDynamic)
	{
		return false;
	}
	else if (p_att.m_isJiTuan != m_isJiTuan)
	{
		return false;
	}
	else if (p_att.m_quyuId != m_quyuId)
	{
		return false;
	}
	else if (p_att.m_quyuName != m_quyuName)
	{
		return false;
	}
	else if (p_att.m_instanceCode != m_instanceCode)
	{
		return false;
	}
	else return true;
}

CString AttrRailing::AutoInstanceCode()
{
	CString sInstanceCode;
	sInstanceCode.Format(_T("%s_%d_%d"), m_prototypeCode, (int)(m_length), (int)(m_height));
	if (sInstanceCode.Find(_T("Railing_"))==0)
	{
		sInstanceCode = sInstanceCode.Mid(8);
	}

	SetInstanceCode(sInstanceCode);

	return sInstanceCode;
}
void AttrRailing::SetViewDir(eViewDir p_view)
{
	if (p_view == E_VIEW_TOP || p_view == E_VIEW_FRONT)
	{
		m_viewDir = p_view;
	}
}
//////////////////////////////////////////////////////////////////////////
void CRailingCountArray::InitByRailingIds(const vAcDbObjectId& p_railingIds)
{
	vector<AttrRailing> railAttrs;
	vector<AcDbObjectId>  railIds;
	for (UINT i = 0; i < p_railingIds.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(p_railingIds[i], pAttr, true);
		AttrRailing* pAttrRailing = AttrRailing::cast(pAttr);
		if (pAttrRailing != NULL)
		{
			AttrRailing attTemp(*pAttrRailing);
			railAttrs.push_back(attTemp);
			railIds.push_back(p_railingIds[i]);
		}
	}
	InitByRailingAtts(railAttrs, railIds);
}

void CRailingCountArray::InitByRailingAtts(const vector<AttrRailing>& p_railingAtts, const vector<AcDbObjectId>& p_ids)
{
	assert(p_ids.size() == p_railingAtts.size());
	bool bUseIds = p_ids.size() == p_railingAtts.size();
	for (UINT i = 0; i < p_railingAtts.size(); i++)
	{
		CString sInstanceCode = p_railingAtts[i].GetInstanceCode(); //原型编号
		bool bFind = false;
		for (UINT n = 0; n < m_railCountArray.size(); n++)
		{
			if (m_railCountArray[n].railAtt.GetInstanceCode().CompareNoCase(sInstanceCode) == 0)
			{
				bFind = true;
				m_railCountArray[n].nCount++;
				m_railCountArray[n].objIds.append(p_ids[i]);
				break;
			}
		}

		if (bFind == false)
		{
			CRailingAndCount railNew;
			railNew.railAtt = p_railingAtts[i];
			railNew.nCount = 1;
			railNew.objIds.append(p_ids[i]);
			m_railCountArray.push_back(railNew);
		}
	}
}
