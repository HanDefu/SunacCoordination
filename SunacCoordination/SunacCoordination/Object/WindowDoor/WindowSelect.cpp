#include "StdAfx.h"
#include <dbeval.h>
#include <AcValue.h>
#include <AcDbAssocArrayRectangularParameters.h>
#include <AcDbAssocArrayActionBody.h>
#include <AcDbAssocManager.h>
#include "dbobjptr2.h"
#include <AcDbAssocArrayPathParameters.h>
#include <AcDbAssocArrayPolarParameters.h>

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
	m_bMirror = false;
	m_mx = AcGeMatrix3d::kIdentity;
}


vector<CWinInCad> CWindowSelect::SelectWindows(eViewDir p_view, bool p_bAllWindow)
{
	Acad::ErrorStatus es;
	vector<CWinInCad> winsOut;//当前选择的ids

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
	acedSSFree(sset);

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


	AcDbBlockReference * pBRef = AcDbBlockReference::cast(pEnt);
	const bool bBlock = (pBRef != NULL);
	const bool bArray = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);
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
		winInCad.m_bMirror = IsMxMirror(curMx);
		winInCad.m_mx = curMx;
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
	AcDbObjectId actionID = TYCOM_GetActionId(inputId);
	TYCOM_GetArrayObjects(actionID, ids);

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

//////////////////////////////////////////////////////////////////////////
CWindowAndCount::CWindowAndCount()
{
	nCount = 0;
}
void CWindowCountArray::InitByWindowIds(const vector<CWinInCad>& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = p_winIds[i].m_winId;
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			attTemp.m_isMirror = p_winIds[i].m_bMirror;
			CString sInstanceCode = GetWindowAutoName()->GetWindowName(attTemp);
			attTemp.SetInstanceCode(sInstanceCode);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i].m_winId);
		}
	}

	InitByWindowAtts(winAtts, winIds);
}

void CWindowCountArray::InitByWindowIds(const vAcDbObjectId& p_winIds)
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

	InitByWindowAtts(winAtts, winIds);
}

//vRCWindow allWindowsTypes;
//for (UINT i = 0; i < winIds.size(); i++)
//{
//	RCWindow oneWindow;
//	oneWindow.m_id = winIds[i];
//	oneWindow.InitParameters();
//	oneWindow.GetAttribute();
//	int index = vFind(oneWindow, allWindowsTypes);
//	if (index == -1)
//	{
//		oneWindow.m_sameTypeIds.push_back(oneWindow.m_id);
//		allWindowsTypes.push_back(oneWindow);
//	}
//	else
//	{
//		allWindowsTypes[index].m_sameTypeIds.push_back(oneWindow.m_id);
//	}
//}

void CWindowCountArray::InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids)
{
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
}
