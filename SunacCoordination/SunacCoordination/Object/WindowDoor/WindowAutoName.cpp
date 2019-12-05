#include "StdAfx.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Sunac.h"

CWinClassify::CWinClassify()
{
}

CWinClassify::~CWinClassify()
{
}

void CWinClassify::AddObject(AcDbObjectId p_objId)
{
	if (p_objId == AcDbObjectId::kNull)
	{
		return;
	}

	if (IsObjectIn(p_objId))
	{
		return;
	}

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
	for (vector<AcDbObjectId>::iterator it = m_winsInCad.begin(); it < m_winsInCad.end(); )
	{
		if ((*it) == p_objId)
		{
			m_winsInCad.erase(it);
			return true;
		}
	}
	return false;
}

void CWinClassify::CheckObjectValid() //检查AcDbObjectId是否有效，针对用户在CAD中直接删除了门窗的情况
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
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(p_id, pDataEnt);
	if (pDataEnt==NULL)
		return false;
	
	AttrWindow * pWindow = AttrWindow::cast(pDataEnt);
	if (pWindow == NULL)
		return false;

	bool bSuc = pWindow->IsInstanceEqual(m_winAtt);
	pWindow->close();

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
	m_winAtt.SetInstanceCode(p_newName);

	for (UINT i = 0; i < m_winsInCad.size(); i++)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(m_winsInCad[i], pDataEnt);
		AttrWindow * pWindow = AttrWindow::cast(pDataEnt);
		if (pWindow == NULL)
			continue;

		pWindow->SetInstanceCode(p_newName);

		pDataEnt->close();
	}
}


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

void CWindowAutoName::AddWindowType(const AttrWindow& p_att, AcDbObjectId p_objId)
{
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		pWinClassify->AddObject(p_objId);
		return;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objId);
	m_allTypeWindows.push_back(newClassify);
}
void CWindowAutoName::AddWindowType(const AttrWindow& p_att, vector<AcDbObjectId> p_objIds)
{
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify != NULL)
	{
		pWinClassify->AddObject(p_objIds);
		return;
	}

	CWinClassify newClassify;
	newClassify.m_winAtt = p_att;
	newClassify.AddObject(p_objIds);
	m_allTypeWindows.push_back(newClassify);
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

void CWindowAutoName::AutoNameAllWindow()
{
	//TODO 尽量保持原来的编号，只有在编号不合理的情况下才重新编号 
	//vector<CWinClassify> temp = m_allTypeWindows;
	//m_allTypeWindows.clear();
	//for (UINT i = 0; i < temp.size(); i++)
	//{
	//	temp[i].m_winAtt.m_instanceCode = GetWindowName(temp[i].m_winAtt);
	//	AddWindowType(temp[i].m_winAtt);
	//}


	//TODO 遍历当前图上的所有门窗，对门窗进行重新编号
}

void CWindowAutoName::AutoNameWindows(const vector<AcDbObjectId>& p_ids)
{
	//TODO
}

bool CWindowAutoName::RenameWindow(const CString p_preName, const CString p_newName)
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

Acad::ErrorStatus CWindowAutoName::ReadFromDwg(AcDbDwgFiler* pFiler)
{
	Adesk::UInt32 size;
	pFiler->readItem(&size);
	m_allTypeWindows.resize(size);
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].m_winAtt.dwgInFields(pFiler);
	}
	return Acad::eOk;
}

Acad::ErrorStatus CWindowAutoName::WriteToDwg(AcDbDwgFiler* pFiler)
{
	pFiler->writeItem((Adesk::UInt32)m_allTypeWindows.size());
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].m_winAtt.dwgOutFields(pFiler);
	}
	return Acad::eOk;
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

void CWindowAutoName::CheckObjectValid() //对现有的检查有效性，用于移除变化后的实体
{
	for (UINT i = 0; i < m_allTypeWindows.size(); i++)
	{
		m_allTypeWindows[i].CheckObjectValid();
	}
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