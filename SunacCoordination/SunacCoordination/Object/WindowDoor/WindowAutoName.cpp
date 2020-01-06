#include "StdAfx.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "..\..\Tool\DocLock.h"

CWinClassify::CWinClassify()
{
}

CWinClassify::~CWinClassify()
{
}

void CWinClassify::AddObject(AcDbObjectId p_objId)
{
	//20200106注释，允许空id占位
	//if (p_objId == AcDbObjectId::kNull)
	//	return;

	if (IsObjectIn(p_objId))
		return;

	m_winsInCad.push_back(p_objId);
}
void CWinClassify::AddObject(vector<AcDbObjectId> p_objIds)
{
	for (UINT i = 0; i < p_objIds.size(); i++)
	{
		AddObject(p_objIds[i]);
	}
}

bool CWinClassify::RemoveObject(AcDbObjectId p_objId)
{
	for (vector<AcDbObjectId>::iterator it = m_winsInCad.begin(); it < m_winsInCad.end(); it++)
	{
		if ((*it) == p_objId)
		{
			m_winsInCad.erase(it);
			return true;
		}
	}
	return false;
}

void CWinClassify::CheckAndRemoveObjectNotBelong() //检查AcDbObjectId是否有效，针对用户在CAD中直接删除了门窗的情况
{
	for (vector<AcDbObjectId>::iterator it = m_winsInCad.begin(); it < m_winsInCad.end(); )
	{
		if (IsObjectBelongThisClassify(*it) == false)
		{
			it = m_winsInCad.erase(it);
		}
		else
		{
			it++;
		}
	}
}

bool CWinClassify::IsObjectBelongThisClassify(AcDbObjectId p_id) //判断是否和此门窗分类相同
{
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	bool bSuc = pWinAtt->IsInstanceEqual(m_winAtt);
	pWinAtt->close();

	if (bSuc)
	{
		assert(pWinAtt->IsInstanceEqual(m_winAtt));
	}

	return bSuc;
}

void CWinClassify::ClearObjsects() //清理所有的关联门窗
{
	m_winsInCad.clear();
}

bool CWinClassify::IsObjectIn(AcDbObjectId p_objId)const
{
	for (UINT i = 0; i < m_winsInCad.size(); i++)
	{
		if (m_winsInCad[i] == p_objId)
		{
			return true;
		}
	}
	return false;
}

void CWinClassify::Rename(const CString p_newName)
{
	m_winAtt.m_instanceCode = p_newName;
}

//////////////////////////////////////////////////////////////////////////
CWindowAutoName::CWindowAutoName()
{
}

CWindowAutoName::~CWindowAutoName()
{
}


CString CWindowAutoName::GetWindowName(const AttrWindow& p_att)
{
	//在示例库里查找与当前窗型相同且编号相同的，若找到则直接返回
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify!=NULL)
	{
		return pWinClassify->m_winAtt.GetInstanceCode();
	}


	//去除原型编号中的"Window_"前缀
	CString prototype = p_att.GetMainPrototypeCode();
	prototype.MakeUpper();
	prototype.Trim();
	prototype.Replace(L"WINDOW_", L"");
	prototype.Replace(L"DOOR_", L"");

	//根据"原型编号_尺寸编号"生成门窗编号
	CString sWindowName;
	sWindowName.Format(L"%s_%02d%02d", prototype, (int)p_att.GetW() / 100, (int)p_att.GetH() / 100);

	//镜像窗型增加"M"后缀
	CString sMirror;
	if (!p_att.m_isMirrorWindow && p_att.m_isMirror)
		sMirror = L"M";

	//查找一个未被占用的门窗编号
	CString sWindowFullName = sWindowName + sMirror;
	for (int i = 1; !IsNameValid(p_att, sWindowFullName); i++)
	{
		sWindowFullName.Format(L"%s_%d%s", sWindowName, i, sMirror);
	}

	return sWindowFullName;
}

CWinClassify* CWindowAutoName::FindWinClassifyByAtt(const AttrWindow& p_att)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].m_winAtt.IsInstanceEqual(p_att))
		{
			return &(m_allTypeWindows[i]);
		}
	}

	return NULL;
}

CWinClassify* CWindowAutoName::FindWinClassifyByInstantCode(const CString p_sCode)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].m_winAtt.GetInstanceCode().CompareNoCase(p_sCode)==0)
		{
			return &(m_allTypeWindows[i]);
		}
	}

	return NULL;
}

CWinClassify* CWindowAutoName::FindWinClassifyByObjectId(const AcDbObjectId& p_id)
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		if (m_allTypeWindows[i].IsObjectIn(p_id))
		{
			return &(m_allTypeWindows[i]);
		}
	}

	return NULL;
}

bool CWindowAutoName::AddWindowType(const CString p_sInstanceCode, AcDbObjectId p_objId)
{
	if (p_objId == AcDbObjectId::kNull || p_sInstanceCode.IsEmpty())
		return false;

	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_sInstanceCode);
	if (pWinClassify == NULL)
	{
		assert(false);
		return false;
	}
	
	pWinClassify->AddObject(p_objId);

	return true;
}
bool CWindowAutoName::AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId)
{
	//20200106注释，允许空id占位
	//if (p_objId == AcDbObjectId::kNull)
	//	return false;

	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		assert(pWinClassify->m_winAtt.GetInstanceCode().CompareNoCase(p_att.GetInstanceCode()) == 0);
		pWinClassify->AddObject(p_objId);
		return true;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objId);
	m_allTypeWindows.push_back(newClassify);
	return true;
}
bool CWindowAutoName::AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds)
{
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		pWinClassify->AddObject(p_objIds);
		return true;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objIds);
	m_allTypeWindows.push_back(newClassify);
	return true;
}

bool CWindowAutoName::IsNameValid(const AttrWindow& p_att, CString p_sName) 
{
	if (p_sName.IsEmpty())
		return false;

	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_sName);
	if (pWinClassify != NULL)
	{
		return pWinClassify->m_winAtt.IsInstanceEqual(p_att);
	}
	else
	{
		return  true;
	}
}

bool CWindowAutoName::RenameWindows(const CString p_preName, const CString p_newName)
{
	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_preName);
	if (pWinClassify == NULL)
	{
		return false;
	}

	CWinClassify* pWinClassify2 = FindWinClassifyByInstantCode(p_newName);
	if (pWinClassify2 == NULL) //新的编号已经被占用，不能使用
	{
		return false;
	}

	pWinClassify->Rename(p_newName);
	return true;
}


Acad::ErrorStatus CWindowAutoName::ReadFromDwg(AcDbDwgFiler* pFiler, Adesk::Int32 p_version)
{
	Adesk::UInt32 nTypeSize;
	pFiler->readItem(&nTypeSize);

	m_allTypeWindows.clear();
	for (UINT i = 0; i < nTypeSize; i++)
	{
		CWinClassify winClassify;
		winClassify.m_winAtt.dwgInFields(pFiler);
		if (p_version >= 4)//版本4
		{
			Adesk::UInt32 nIdSize;
			pFiler->readItem(&nIdSize);
			for (UINT j = 0; j < nIdSize; j++)
			{
				AcDbHandle tempHandle;
				pFiler->readItem(&tempHandle);

				AcDbObjectId retId = AcDbObjectId::kNull;
				acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(retId, false, tempHandle);
				winClassify.m_winsInCad.push_back(retId);
			}
		}

		m_allTypeWindows.push_back(winClassify);
	}
	return Acad::eOk;
}

Acad::ErrorStatus CWindowAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeWindows.size());
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].m_winAtt.dwgOutFields(pFiler);

		////版本4添加
		pFiler->writeItem((Adesk::UInt32)m_allTypeWindows[i].m_winsInCad.size());
		for (UINT j = 0; j < m_allTypeWindows[i].m_winsInCad.size(); j++)
		{
			pFiler->writeItem(m_allTypeWindows[i].m_winsInCad[j].handle());
		}
	}
	return Acad::eOk;
}

bool CWindowAutoName::IsInstanceCodeExist(CString p_code)
{
	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_code);
	if (pWinClassify == NULL)
	{
		return false;
	}

	return pWinClassify->m_winsInCad.size()>0;
}


vector<AcDbObjectId> CWindowAutoName::GetAllIdsByInstantCode(CString p_code)
{
	vector<AcDbObjectId> idsOut;
	CWinClassify* pWinClassify = FindWinClassifyByInstantCode(p_code);
	if (pWinClassify==NULL)
	{
		return idsOut;
	}

	return pWinClassify->m_winsInCad;
}

vector<AcDbObjectId> CWindowAutoName::GetAllIds()
{
	vector<AcDbObjectId> allIds;
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		vector<AcDbObjectId> ids = m_allTypeWindows[i].GetObjects();
		allIds.insert(allIds.end(), ids.begin(), ids.end());
	}
	return allIds;
}

void CWindowAutoName::CheckAndRemoveObjectNotBelong() //对现有的检查有效性，用于移除变化后的实体
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].CheckAndRemoveObjectNotBelong();
	}

	//移除为空的分类
	RemoveEmptyClassify();
}
void CWindowAutoName::RemoveAllObjects()//移除所有的object，但是保留原来的名称库
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].ClearObjsects();
	}
}
bool CWindowAutoName::AddObject(AcDbObjectId p_id)
{
	CWinClassify* pWinClassify = FindWinClassifyByObjectId(p_id);
	if (pWinClassify != NULL)
		return true; //已存在
	
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;
	
	AddWindowType(*pWinAtt, p_id);

	pWinAtt->close();
	return true;
}
void CWindowAutoName::RemoveObject(AcDbObjectId p_id) //门窗参数变化时调用此函数更新
{
	for (vector<CWinClassify>::iterator it = m_allTypeWindows.begin(); it < m_allTypeWindows.end(); it++)
	{
		if (it->RemoveObject(p_id))
		{
			if (it->IsEmpty())
			{
				m_allTypeWindows.erase(it);
			}
			return;
		}
	}
}
void CWindowAutoName::RemoveObjectsByInstantCode(CString p_instanceCode)//移除某个门窗编号的对象
{
	for (vector<CWinClassify>::iterator it = m_allTypeWindows.begin(); it < m_allTypeWindows.end(); it++)
	{
		if (it->m_winAtt.GetInstanceCode().CompareNoCase(p_instanceCode)==0)
		{
			m_allTypeWindows.erase(it);
			return;
		}
	}
}
void CWindowAutoName::RemoveEmptyClassify()
{
	for (vector<CWinClassify>::iterator it = m_allTypeWindows.begin(); it < m_allTypeWindows.end(); it++)
	{
		if (it->IsEmpty())
		{
			it = m_allTypeWindows.erase(it);
		}
		else
		{
			it++;
		}
	}
}
bool CWindowAutoName::UpdateObject(AcDbObjectId p_id)//门窗参数变化时调用此函数更新，包括名称变化，属性变化
{
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	RemoveObject(p_id);

	AddWindowType(*pWinAtt, p_id);
	pWinAtt->close();

	return true;
}
bool CWindowAutoName::UpdateObject(const AttrWindow& p_oldAtt, const AttrWindow& p_newAtt) //某个类型的门窗全部调整为新的类型
{
	CWinClassify* pOldClassify = FindWinClassifyByAtt(p_oldAtt);
	if (pOldClassify == NULL)
		return false;

	CWinClassify* pNewClassify = FindWinClassifyByAtt(p_newAtt);
	if (pNewClassify) //如果新的有，在原理的就的id都加到新的中
	{
		pNewClassify->AddObject(pOldClassify->GetObjects());
		pOldClassify->ClearObjsects();

		RemoveEmptyClassify();
	}
	else
	{
		pOldClassify->m_winAtt = p_newAtt;
	}	

	return true;
}

CProtypeInstanceCodeMrg::CProtypeInstanceCodeMrg()
{

}
CProtypeInstanceCodeMrg::~CProtypeInstanceCodeMrg()
{

}
vector<AcDbObjectId> CProtypeInstanceCodeMrg::FindTextIds(AcDbObjectId p_keyId)
{
	std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator iter = m_instanceMap.find(p_keyId);
	if (iter != m_instanceMap.end())
	{
		return iter->second;
	}
	else
	{
		return vector<AcDbObjectId> ();
	}
}
void CProtypeInstanceCodeMrg::AddInstanceCode(AcDbObjectId p_id, AcDbObjectId p_textId)
{
	std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator iter = m_instanceMap.find(p_id);
	if (iter != m_instanceMap.end())
	{
		iter->second.push_back(p_textId);
	}
	else
	{
		vector<AcDbObjectId> textIds;
		textIds.push_back(p_textId);

		m_instanceMap[p_id] = textIds;
	}
}
void CProtypeInstanceCodeMrg::RemoveInstanceCode(AcDbObjectId p_id)
{
	std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator iter = m_instanceMap.find(p_id);
	if (iter != m_instanceMap.end())
	{
		vector<AcDbObjectId> textIds = iter->second;
		for (UINT i = 0; i < textIds.size(); i++)
		{
			JHCOM_DeleteCadObject(textIds[i]);
		}

		m_instanceMap.erase(iter);
	}
}

void CProtypeInstanceCodeMrg::RemoveInvalidInstanceCode()
{
	for (std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); )
	{
		if (IsObjectExsit(it->first))
		{
			vector<AcDbObjectId> textIds = it->second;
			for (UINT i = 0; i < textIds.size(); i++)
			{
				JHCOM_DeleteCadObject(textIds[i]);
			}

			it = m_instanceMap.erase(it);
		}
		else
		{
			it++;
		}
	}
}
void CProtypeInstanceCodeMrg::RemoveAll()
{
	for (std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		vector<AcDbObjectId> textIds = it->second;
		for (UINT i = 0; i < textIds.size(); i++)
		{
			JHCOM_DeleteCadObject(textIds[i]);
		}
	}

	m_instanceMap.clear();
}