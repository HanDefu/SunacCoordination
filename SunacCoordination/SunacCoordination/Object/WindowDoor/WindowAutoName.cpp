#include "StdAfx.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "..\..\Tool\DocLock.h"
#include "..\..\GlobalSetting.h"

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
	const AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt == NULL)
		return false;

	bool bSuc = pWinAtt->IsInstanceEqual(m_winAtt);

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
	//1. 在编号库里查找与当前窗型相同且编号相同的，若找到则直接返回
	CWinClassify* pWinClassify = FindWinClassifyByAtt(p_att);
	if (pWinClassify!=NULL)
	{
		return pWinClassify->m_winAtt.GetInstanceCode();
	}

	if (p_att.m_isMirrorWindow==false)
	{
		//镜像的门窗若找到非镜像的门窗的编号，再反求当前镜像门窗编号
		AttrWindow attMirror = p_att;
		attMirror.m_isMirror = !(p_att.m_isMirror);

		pWinClassify = FindWinClassifyByAtt(attMirror);
		if (pWinClassify != NULL)
		{
			CString nameTemp = pWinClassify->m_winAtt.GetInstanceCode();
			//取镜像相反的名字：若找到名字最后一个字母是M，则去除M；反之加上M
			if (nameTemp.ReverseFind('M')==nameTemp.GetLength()-1)
			{
				return nameTemp.Left(nameTemp.GetLength() - 1);
			}
			else
			{
				return nameTemp + _T("M");
			}
		}
	}


	//2. 编号库中没找到编号，则根据编号规则
	//去除原型编号中的"Window_"前缀
	CString prototype = p_att.GetMainPrototypeCode();
	prototype.MakeUpper();
	prototype.Trim();
	prototype.Replace(L"WINDOW_", L"");
	prototype.Replace(L"DOOR_", L"");

	prototype=prototype.SpanExcluding(_T("0123456789")); //移除后面的数字  YUAN 20200222
	if (p_att.m_isFireproofWindow)
	{
		prototype = _T("FC");//若是防火窗，则变为FC开头
	}

	//根据"原型编号_尺寸编号"生成门窗编号
	CString sWindowName;
	//sWindowName.Format(L"%s_%02d%02d", prototype, (int)p_att.GetW() / 100, (int)p_att.GetH() / 100);
	sWindowName.Format(L"%s%02d%02d", prototype, (int)p_att.GetW() / 100, (int)p_att.GetH() / 100);//移除后面的数字  YUAN 20200222
	
	//查找一个未被占用的门窗编号
	CString sWindowFullName = sWindowName;
	for (int i = 1; !IsNameValid(p_att, sWindowFullName); i++)
	{
		sWindowFullName.Format(L"%s_%d", sWindowName, i);
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
	Acad::ErrorStatus es;
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
				es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(retId, false, tempHandle);
				if (es == Acad::eOk)
				{
					winClassify.m_winsInCad.push_back(retId);
				}
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
	
	const AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt == NULL)
		return false;
	
	AddWindowType(*pWinAtt, p_id);

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
	const AttrWindow * pWinAtt = AttrWindow::GetWinAtt(p_id, true);
	if (pWinAtt == NULL)
		return false;

	RemoveObject(p_id);

	AddWindowType(*pWinAtt, p_id);

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

CString CWindowAutoName::GetMirrorInstanceCode(CString p_name)
{
	if (p_name.ReverseFind(L'M')==p_name.GetLength()-1)
	{
		return p_name.Left(p_name.GetLength() - 1);
	}
	else
	{
		return p_name + _T("M");
	}
}
bool CWindowAutoName::IsSamePrototypeAndSize(CString p_instanceCode1, CString p_instanceCode2)
{
	int nPos1 = p_instanceCode1.Find(L"_");
	if (nPos1 <= 0 && p_instanceCode1.GetLength() < nPos1 + 5)
		return false;

	int nPos2 = p_instanceCode1.Find(L"_");
	if (nPos2 <= 0 && p_instanceCode1.GetLength() < nPos2 + 5)
		return false;

	CString sCode1 = p_instanceCode1.Left(nPos1 + 5);
	CString sCode2 = p_instanceCode2.Left(nPos2 + 5);
	return sCode1.CompareNoCase(sCode2)==0;
}

//////////////////////////////////////////////////////////////////////////

CInstanceCodeTextMrg::CInstanceCodeTextMrg()
{

}
CInstanceCodeTextMrg::~CInstanceCodeTextMrg()
{

}
vector<AcDbObjectId> CInstanceCodeTextMrg::FindTextIds(AcDbObjectId p_keyId)
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
void CInstanceCodeTextMrg::AddInstanceCode(AcDbObjectId p_id, AcDbObjectId p_textId)
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
void CInstanceCodeTextMrg::RemoveInstanceCode(AcDbObjectId p_winId)
{
	std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator iter = m_instanceMap.find(p_winId);
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
void CInstanceCodeTextMrg::RemoveInstanceCodeText(AcDbObjectId p_textId)
{
	for (std::map<AcDbObjectId, vector<AcDbObjectId>>::iterator it = m_instanceMap.begin(); it != m_instanceMap.end(); it++)
	{
		vector<AcDbObjectId>& textIds = it->second;
		for (vector<AcDbObjectId>::iterator itFind = textIds.begin(); itFind < textIds.end(); itFind++)
		{
			if ((*itFind) == p_textId)
			{
				textIds.erase(itFind);
				return;
			}
		}
	}
}

void CInstanceCodeTextMrg::RemoveInvalidInstanceCode()
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
void CInstanceCodeTextMrg::RemoveAll()
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
	for (int i = 0; i< length; i++)
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

	ads_point pt1,pt2;
	pt1[X] = p_rect.GetLT().x;
	pt1[Y] = p_rect.GetLT().y;
	pt1[Z] = p_rect.GetLT().z;
	pt2[X] = p_rect.GetRB().x;
	pt2[Y] = p_rect.GetRB().y;
	pt2[Z] = p_rect.GetRB().z;
	acedSSGet(TEXT("W"), pt1, pt2, rb, ssname);//筛选在rect范围内的结果

	Adesk::Int32 length;
	acedSSLength(ssname, &length);
	for (int i = 0; i< length; i++)
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