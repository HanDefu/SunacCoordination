#include "StdAfx.h"
#include <dbeval.h>
#include <AcValue.h>

#if (!defined ARX_2010) && (!defined ARX_2011)
#include <AcDbAssocArrayRectangularParameters.h>
#include <AcDbAssocArrayActionBody.h>
#include <AcDbAssocArrayPathParameters.h>
#include <AcDbAssocArrayPolarParameters.h>
#endif

#include <AcDbAssocManager.h>
#include "dbobjptr2.h"
#include "WindowSelect.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Def.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_RectArray.h"
#include "..\..\Src\DocumentData.h"



CWinInCad::CWinInCad()
{
	m_winId = AcDbObjectId::kNull;
	m_rootId = AcDbObjectId::kNull;
	m_bMxMirror = false;
	m_rotateAngle = 0;
	m_mx = AcGeMatrix3d::kIdentity;
}


vector<CWinInCad> CWindowSelect::SelectWindows(eViewDir p_view, bool p_bAllWindow)
{
	Acad::ErrorStatus es;

	acutPrintf(L"\n请选择门窗");

	ads_name sset;
	if (p_bAllWindow)
	{
		int rt = acedSSGet(_T("A"), NULL, NULL, NULL, sset); // 提示用户选择对象
	}
	else
	{
		acedSSGet(NULL, NULL, NULL, NULL, sset);
	}


	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	vector<AcDbObjectId> ids;
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);

		AcDbObjectId objId = 0;
		es = acdbGetObjectId(objId, ent);
		if (es != Acad::eOk || objId == AcDbObjectId::kNull)
		{
			continue;
		}

		ids.push_back(objId);
	}
	acedSSFree(sset);
	//////////////////////////////////////////////////////////////////////////

	vector<CWinInCad> winsOut = GetWinsInObjectIds(ids ,p_view);//当前选择的ids

	if (winsOut.size() == 0)
	{
		acutPrintf(L"\n未选择到门窗\n");
	}
	else
	{
		CString info;
		info.Format(L"\n共选择了%d个门窗\n", winsOut.size());
		acutPrintf(info);
	}
	return winsOut;
}


vector<CWinInCad> CWindowSelect::GetWinsInObjectIds(const vector<AcDbObjectId>& p_ids, eViewDir p_view)
{
	vector<CWinInCad> winsOut;
	for (UINT i = 0; i < p_ids.size(); i++)
	{
		AcDbObjectId objId = p_ids[i];

		vector<CWinInCad> winsTemp;
		AcGeMatrix3d mxTemp = AcGeMatrix3d::kIdentity;
		FindWindowsDeep(objId, p_view, mxTemp, winsTemp);
		if (winsTemp.size() > 0)
		{
			for (UINT n = 0; n < winsTemp.size(); n++)
			{
				winsTemp[n].m_rootId = objId;
			}
			winsOut.insert(winsOut.end(), winsTemp.begin(), winsTemp.end());
		}
	}

	return winsOut;
}

vector<CWinInCad> CWindowSelect::SelectWindowsByRect(eViewDir p_view, TYRect p_rect)
{
	ads_point pt1, pt2;
	pt1[X] = p_rect.GetLT().x;
	pt1[Y] = p_rect.GetLT().y;
	pt1[Z] = p_rect.GetLT().z;
	pt2[X] = p_rect.GetRB().x;
	pt2[Y] = p_rect.GetRB().y;
	pt2[Z] = p_rect.GetRB().z;

	Acad::ErrorStatus es;

	acutPrintf(L"\n请选择门窗");

	ads_name sset;
	acedSSGet(TEXT("W"), pt1, pt2, NULL, sset);//筛选在rect范围内的结果

	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	vector<AcDbObjectId> ids;
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);

		AcDbObjectId objId = 0;
		es = acdbGetObjectId(objId, ent);
		if (es != Acad::eOk || objId == AcDbObjectId::kNull)
		{
			continue;
		}

		ids.push_back(objId);
	}
	acedSSFree(sset);
	//////////////////////////////////////////////////////////////////////////

	vector<CWinInCad> winsOut = GetWinsInObjectIds(ids, p_view);//当前选择的ids

	if (winsOut.size() == 0)
	{
		acutPrintf(L"\n未选择到门窗\n");
	}
	else
	{
		CString info;
		info.Format(L"\n共选择了%d个门窗\n", winsOut.size());
		acutPrintf(info);
	}
	return winsOut;
}

int CWindowSelect::FindWindowsDeep(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	if (inputId == AcDbObjectId::kNull)
		return 0;

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es!=Acad::eOk || pEnt==NULL)
		return 0;


	bool bArray = false;

	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
    const bool bBlock = (pBRef != NULL);
#if (!defined ARX_2010) && (!defined ARX_2011)
    bArray  = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);
#endif
	pEnt->close();

	if (bArray) 
	{
		FindWindowInArray(inputId, viewDir, p_parentMx, outputIds);
	}
	else if (bBlock)
	{
		FindWindowInBlock(inputId, viewDir, p_parentMx, outputIds);
	}
	//group情况 不用特定处理，在选择的时候还是多个单独的实体，只是一起选择
	 

	return 0;
}

int CWindowSelect::FindWindowInBlock(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return 0;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	const AcGeMatrix3d curMx = p_parentMx*curReferenceMx;

	if (TY_IsWindow(inputId, viewDir))
	{
		CWinInCad winInCad;
		winInCad.m_winId = inputId;
		winInCad.m_rootId = inputId;
		winInCad.m_bMxMirror = IsMxMirror(curMx);
		winInCad.m_mx = curMx;

		AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
		if (pBRef!=NULL)
		{
			winInCad.m_rotateAngle = pBRef->rotation();
		}

		////平面图时，由于原型文件和立面图方向有矛盾，平面图的镜像关系是相反的，参见IsInstanceNeedMirror  yuan 1124 Mirror
		//AcDbObject * pDataEnt = 0;
		//TY_GetAttributeData(inputId, pDataEnt);
		//AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
		//if (pWindow->m_viewDir==E_VIEW_TOP)
		//{
		//	winInCad.m_bMirror = !winInCad.m_bMirror;
		//}

		outputIds.push_back(winInCad);
		return 1;
	}


	ACHAR *blockName = NULL;
	vAcDbObjectId vidsToCheck;
	int ret = MD2010_GetBlockReference_Record_name(inputId, blockName);
	if (ret == 0)
	{
		MD2010_CycleBlockEntites(blockName, vidsToCheck);
		for (int i = 0; i < vidsToCheck.size(); i++)
		{
			FindWindowsDeep(vidsToCheck[i], viewDir, curMx, outputIds);
		}
	}

	if (blockName!=NULL)
	{
		delete blockName;
	}

	return 0;
}

int CWindowSelect::FindWindowInArray(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx, vector<CWinInCad> &outputIds)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, inputId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return 0;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	const AcGeMatrix3d curMx = p_parentMx*curReferenceMx;


	vAcDbObjectId ids;
#if (defined ARX_2010) || (defined ARX_2011)
	ids.push_back(inputId);
#else
	AcDbObjectId actionID = TYCOM_GetActionId(inputId);
	TYCOM_GetArrayObjects(actionID, ids);
#endif

	vAcDbObjectId ids2;
	for (int i = 0; i < ids.size(); i++)
	{
		FindWindowsDeep(ids[i], viewDir, curMx, outputIds);
	}

	return 0;
}

bool CWindowSelect::IsMxMirror(const AcGeMatrix3d p_parentMx)
{
	double value = p_parentMx.entry[0][0] * p_parentMx.entry[1][1] * p_parentMx.entry[2][2];
	
	return value<0;
}

bool CWindowSelect::IsReferenctMirror(const AcDbObjectId refId)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, refId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
	{
		return false;
	}

	AcGeMatrix3d curReferenceMx = AcGeMatrix3d::kIdentity;
	pEnt->getCompoundObjectTransform(curReferenceMx);
	pEnt->close();

	return IsMxMirror(curReferenceMx);
}

AcGeMatrix3d CWindowSelect::GetReferenctWorldMatrix(const AcDbObjectId refId) //得到实体的完整镜像
{
	AcGeMatrix3d mx = AcGeMatrix3d::kIdentity;

	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, refId, AcDb::kForRead);
	if (es != Acad::eOk || pEnt == NULL)
		return mx;

	pEnt->getCompoundObjectTransform(mx);

	AcDbObjectId parentId = pEnt->ownerId();
	while (parentId!=AcDbObjectId::kNull)
	{
		AcDbEntity * pEntParent = 0;
		es = acdbOpenObject(pEntParent, parentId, AcDb::kForRead);
		if (es != Acad::eOk || pEntParent == NULL)
			continue;

		AcGeMatrix3d mxParent = AcGeMatrix3d::kIdentity;
		pEntParent->getCompoundObjectTransform(mxParent);
		mx = mxParent*mx;

		pEntParent->close();

		parentId = pEntParent->ownerId();
	}

	pEnt->close();

	return mx;
}

//////////////////////////////////////////////////////////////////////////
CWindowAndCount::CWindowAndCount()
{
	nCount = 0;
}
bool CWindowCountArray::InitByWindowIds(const vector<CWinInCad>& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		AttrWindow* pAtt = AttrWindow::GetWinAtt(p_winIds[i].m_winId);
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			attTemp.SetMxMirror(p_winIds[i].m_bMxMirror);
			CString sInstanceCode = GetWindowAutoName()->GetWindowName(attTemp);
			attTemp.SetInstanceCode(sInstanceCode);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i].m_winId);
		}
	}

	return InitByWindowAtts(winAtts, winIds);
}

bool CWindowCountArray::InitByWindowIds(const vAcDbObjectId& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = p_winIds[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i]);
		}
	}

	return InitByWindowAtts(winAtts, winIds);
}


bool CWindowCountArray::InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids)
{
	bool bSuc = true;
	assert(p_ids.size() == p_winAtts.size());
	bool bUseIds = p_ids.size() == p_winAtts.size();
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //原型编号
		bool bFind = false;
		for (UINT n = 0; n < m_winCountArray.size(); n++)
		{
			if (m_winCountArray[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode) == 0)
			{
				bFind = true;
				if (p_winAtts[i].m_viewDir == E_VIEW_TOP) //平面图
				{
					if (0 == p_winAtts[i].m_floorInfo.GetFloorCount()) //若有未设置层数的情况，则返回false
					{
						bSuc = false;
					}

					m_winCountArray[n].nCount += p_winAtts[i].m_floorInfo.GetFloorCount();
				}
				else
				{
					m_winCountArray[n].nCount++;
				}

				m_winCountArray[n].objIds.append(p_ids[i]);
				break;
			}
		}

		if (bFind == false)
		{
			CWindowAndCount winNew;
			winNew.winAtt = p_winAtts[i];
			if (p_winAtts[i].m_viewDir == E_VIEW_TOP) //平面图
			{
				winNew.nCount = p_winAtts[i].m_floorInfo.GetFloorCount();
			}
			else
			{
				winNew.nCount = 1;
			}
			winNew.objIds.append(p_ids[i]);
			m_winCountArray.push_back(winNew);
		}
	}

	return bSuc;
}
