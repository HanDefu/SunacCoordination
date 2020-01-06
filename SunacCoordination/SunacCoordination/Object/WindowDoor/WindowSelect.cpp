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
#include "..\..\Common\ComFun_Def.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_RectArray.h"



CWinInCad::CWinInCad()
{
	m_winId = AcDbObjectId::kNull;
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
	//TODO group情况
	 

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
		winInCad.m_bMirror = IsWindowMirror(inputId, viewDir, curMx);
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

bool CWindowSelect::IsWindowMirror(const AcDbObjectId inputId, const eViewDir viewDir, const AcGeMatrix3d p_parentMx)
{
	double value = p_parentMx.entry[0][0] * p_parentMx.entry[1][1] * p_parentMx.entry[2][2];
	
	return value<0;
}