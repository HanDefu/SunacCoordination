#include "StdAfx.h"
#include <dbobjptr.h>
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "..\..\Tool\DocLock.h"
#include "..\..\GlobalSetting.h"
#include "InstanceCodeTextMrg.h"



CInstanceCodeTextMrg::CInstanceCodeTextMrg()
{

}
CInstanceCodeTextMrg::~CInstanceCodeTextMrg()
{

}

AcDbObjectId CInstanceCodeTextMrg::FindTextId(const CSunacObjInCad &p_sunacObj)
{
	map<CSunacObjInCad, AcDbObjectId>::iterator iter = m_instanceMap.find(p_sunacObj);
	if (iter != m_instanceMap.end())
	{
		return iter->second;
	}
	else
	{
		return AcDbObjectId::kNull;
	}
}
bool CInstanceCodeTextMrg::SetNewInstanceCode(const CSunacObjInCad &p_sunacObj, CString p_newTextCode)//更新编号值
{
	AcDbObjectId textId = FindTextId(p_sunacObj);
	if (textId==AcDbObjectId::kNull)
	{
		return false;
	}

	AcDbObjectPointer<AcDbText> pTextEnt(textId, AcDb::kForWrite);
	if (pTextEnt.openStatus()==Acad::eOk)
	{
		pTextEnt->setTextString(p_newTextCode);
		return true;
	}

	return false;
}

void CInstanceCodeTextMrg::AddInstanceCode(const CSunacObjInCad &p_sunacObj, AcDbObjectId p_textId)
{
	map<CSunacObjInCad, AcDbObjectId>::iterator iter = m_instanceMap.find(p_sunacObj);
	if (iter != m_instanceMap.end())
	{
		iter->second = p_textId;
	}
	else
	{
		m_instanceMap[p_sunacObj] = p_textId;
	}
}
void CInstanceCodeTextMrg::RemoveInstanceCode(const CSunacObjInCad & p_sunacObj)
{
	map<CSunacObjInCad, AcDbObjectId>::iterator iter = m_instanceMap.find(p_sunacObj);
	if (iter != m_instanceMap.end())
	{
		AcDbObjectId textId = iter->second;
		JHCOM_DeleteCadObject(textId);

		m_instanceMap.erase(iter);
	}
}

void CInstanceCodeTextMrg::RemoveInstanceCodeByWinId(AcDbObjectId p_winId)
{
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); )
	{
		const CSunacObjInCad sunacObj = it->first;
		if (sunacObj.m_rootId==p_winId || sunacObj.m_winId==p_winId)
		{
			AcDbObjectId textId = it->second;
			JHCOM_DeleteCadObject(textId);

			it = m_instanceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void CInstanceCodeTextMrg::RemoveInstanceCodeText(AcDbObjectId p_textId)
{
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		AcDbObjectId textId = it->second;
		if (textId == p_textId)
		{
			m_instanceMap.erase(it);
			return;
		}
	}
}

bool CInstanceCodeTextMrg::IsTextIdIn(AcDbObjectId p_textId)
{
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		AcDbObjectId textId = it->second;
		if (textId == p_textId)
		{
			return true;
		}
	}

	return false;
}

void CInstanceCodeTextMrg::RemoveInvalidInstanceCode()
{
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); )
	{
		const CSunacObjInCad sunacObj = it->first;
		if (IsObjectExsit(sunacObj.m_rootId))
		{
			AcDbObjectId textId = it->second;
			JHCOM_DeleteCadObject(textId);

			it = m_instanceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void CInstanceCodeTextMrg::RemoveAll()
{
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		AcDbObjectId textId = it->second;
		JHCOM_DeleteCadObject(textId);
	}

	m_instanceMap.clear();
}



//得到当前图纸范围内的所有的门窗编号文字的id
vector<AcDbObjectId> CInstanceCodeTextMrg::GetAllInstanceCodeIds()
{
	vector<AcDbObjectId> ids;
	struct resbuf *rb;
	ads_name ssname;
	CString LayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;
	char* cLayerName = CT2A(LayerName);
	rb = acutBuildList(RTDXF0, TEXT("TEXT"), 8, LayerName, RTNONE);//获取实体类型为文字，图层名为LayerName的结果缓冲区链表

	acedSSGet(TEXT("X"), NULL, NULL, rb, ssname);

	Adesk::Int32 length;
	acedSSLength(ssname, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(ssname, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);
		ids.push_back(objId);
	}

	acutRelRb(rb);
	acedSSFree(ssname);
	return ids;
}
vector<AcDbObjectId> CInstanceCodeTextMrg::GetInstanceCodeIdsInRect(const TYRect p_rect)
{
	vector<AcDbObjectId> ids;
	struct resbuf *rb;
	ads_name ssname;
	CString LayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;
	char* cLayerName = CT2A(LayerName);
	rb = acutBuildList(RTDXF0, TEXT("TEXT"), 8, LayerName, RTNONE);

	ads_point pt1, pt2;
	pt1[X] = p_rect.GetLT().x;
	pt1[Y] = p_rect.GetLT().y;
	pt1[Z] = p_rect.GetLT().z;
	pt2[X] = p_rect.GetRB().x;
	pt2[Y] = p_rect.GetRB().y;
	pt2[Z] = p_rect.GetRB().z;
	acedSSGet(TEXT("W"), pt1, pt2, rb, ssname);//筛选在rect范围内的结果

	Adesk::Int32 length;
	acedSSLength(ssname, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(ssname, i, ent);
		AcDbObjectId objId;
		acdbGetObjectId(objId, ent);
		ids.push_back(objId);
	}

	acutRelRb(rb);
	acedSSFree(ssname);
	return ids;
}



Acad::ErrorStatus CInstanceCodeTextMrg::ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version)
{
	if (p_version<9) //FILE_VERSION 版本9才支持
	{
		return Acad::eOk;
	}

	Acad::ErrorStatus es;

	Adesk::UInt32 nSize;
	pFiler->readItem(&nSize);

	m_instanceMap.clear();
	for (UINT i = 0; i < nSize; i++)
	{
		CSunacObjInCad sunacObj;
		AcDbHandle tempHandle;
		pFiler->readItem(&tempHandle);
		es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(sunacObj.m_winId, false, tempHandle);
		pFiler->readItem(&tempHandle);
		es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(sunacObj.m_rootId, false, tempHandle);
		
		AcDbObjectId textId = AcDbObjectId::kNull;
		pFiler->readItem(&tempHandle);
		es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(textId, false, tempHandle);

		//////////////////////////////////////////////////////////////////////////
		AddInstanceCode(sunacObj, textId);
	}

	return Acad::eOk;
}

Acad::ErrorStatus CInstanceCodeTextMrg::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_instanceMap.size());
	for (map<CSunacObjInCad, AcDbObjectId>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		pFiler->writeItem(it->first.m_winId.handle());
		pFiler->writeItem(it->first.m_rootId.handle());

		pFiler->writeItem(it->second.handle());
	}

	return Acad::eOk;
}