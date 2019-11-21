//
//////////////////////////////////////////////////////////////////////////////
//
//  Copyright 2013 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//
//////////////////////////////////////////////////////////////////////////////
//
// AsdkAcUiSample.cpp : Defines the initialization routines for the DLL.
//
#include "StdAfx.h"

#if defined(_DEBUG) && !defined(AC_FULL_DEBUG)
#error _DEBUG should not be defined except in internal Adesk debug builds
#endif

#include <rxmfcapi.h>
#include <dbgroup.h>
#include <geassign.h>
#include "accmd.h"
#include "../Common/ComFun_Sunac.h"
#include "../Object/WindowDoor/RCWindow.h"
#include "../Common/ComFun_ACad.h"
#include "dbtable.h"
#include "../Tool/DocLock.h"
#include "../Common/ComFun_Str.h"
#include "../Object/RCBlock.h"

//改结构体代表插入进来的一个门窗详图的块
//Sunac2019\Data\Template\WindowDetail.dwg
typedef struct WindowDetailTemplate
{
	WindowDetailTemplate()
	{
		pnt = AcGePoint3d(0,0,0);//插入点
	}

    AcGePoint3d pnt;//外窗插入点

	//总宽度高度
	const static int width = 4000;
	const static int height = 4500;

	//原型编号
	const static int idX = 130;
	const static int idY = 650;

	//面积
	const static int areaX= 1760;
	const static int areaY= 690;

	//开启面积
	const static int openAreaX = 1960;
	const static int openAreaY = 190;

	//功能区
	const static int gongNengQuX = 3220;
	const static int gongNengQuY = 690;

	//窗下墙高度
	const static int heightUnderWindowX = 3310;
	const static int heightUnderWindowY = 190;


	//标注宽度
	const static int biaoZhuWid = 300;

	RCBlock thisBlock;

	//得到模板的插入点
	AcGePoint3d GetInsertPnt(AcGePoint3d basePnt, int rowIndex, int columnIndex)
	{
		return AcGePoint3d(basePnt.x + columnIndex*width, basePnt.y - (rowIndex+1) * height, 0);
	}

	static CString GetTemplateFileName()
	{
		return MD2010_GetAppPath() + L"\\Sunac2019\\Data\\Template\\WindowDetail.dwg";;
	}

	//计算门窗的插入点
	AcGePoint3d GetInsertPntWindow(double winwid, double winHei)
	{
		return AcGePoint3d(pnt.x + (width - winwid - biaoZhuWid * 2)/2,//宽度两边平分
			             pnt.y + 4500 - 900 - winHei,//高度顶端一致
						 0);
	}
}SWindowDetailTemplate;

typedef std::vector<SWindowDetailTemplate> vSWindowDetailTemplate;

static int InsertTemplates(vRCWindow &allWindowsTypes, AcGePoint3d pnt, vSWindowDetailTemplate &windowDetailTemplates)
{
	int columnNum = 3;

	int rowIndex = 0, columnIndex = 0;
	int size = (int)allWindowsTypes.size();
	for (int i = 0; i < size; i++)
	{
		rowIndex = (int)(i/3);
		columnIndex = i%3;

        SWindowDetailTemplate oneTemplate;
		oneTemplate.pnt = oneTemplate.GetInsertPnt(pnt,rowIndex,columnIndex);
		
		AcDbObjectId id = oneTemplate.thisBlock.Insert(WindowDetailTemplate::GetTemplateFileName(),
			oneTemplate.pnt,
			0,
			L"0",
			0);
		windowDetailTemplates.push_back(oneTemplate);
	}
    return 0;
}

//门窗详图
void CMD_SunacWindowDetail()
{
	CDocLock lockEnt;
	CString str;

	//第一步：选择需要统计的门窗
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
	{
		return;
	}
	vAcDbObjectId winIds = SelectWindows(viewDir);
	if (winIds.size() == 0)
		return;

	//第二步  选择门窗表插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择门窗详图插入点");
	if (bSuc == false)
		return;

	//第三步：读取门窗数据并且分类汇总
	vRCWindow allWindowsTypes;
	for (UINT i = 0; i < winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = winIds[i];
		oneWindow.InitParameters();
		oneWindow.GetAttribute();
		int index = vFind(oneWindow,allWindowsTypes);
		if (index == -1)
		{
			oneWindow.m_sameTypeIds.push_back(oneWindow.m_id);
			allWindowsTypes.push_back(oneWindow);
		}
		else
		{
			allWindowsTypes[index].m_sameTypeIds.push_back(oneWindow.m_id);
		}
	}

	//注：前面三步基本同门窗表
	//第四步：4.1首先插入门窗表模板
	vSWindowDetailTemplate windowDetailTemplates;
	InsertTemplates(allWindowsTypes, pnt, windowDetailTemplates);

	//4.2 插图门窗图块---写入文字
	//windowDetailTemplates 和 allWindowsTypes 的数量是一致的
	for (UINT i = 0; i < allWindowsTypes.size(); i++)
	{
		const AttrWindow * pWinAtt = allWindowsTypes[i].GetAttribute();

		AcGePoint3d winPnt = windowDetailTemplates[i].GetInsertPntWindow(allWindowsTypes[i].GetW(), allWindowsTypes[i].GetH());
		allWindowsTypes[i].Insert
			(
			    ACDB_MODEL_SPACE,
			    allWindowsTypes[i].GetBlockRecordName(),
				winPnt,
				0,
				L"0",
				0
			);
		allWindowsTypes[i].RunParameters();

		////功能区
		//JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].gongNengQuX,
		//	                      windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].gongNengQuY,0),
		//	AcGeVector3d(0,0,1), 
		//	120, 0, L"厨房"); 

		//门窗编号
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].idX,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].idY,0),
			AcGeVector3d(0,0,1), 
			120, 0, 
			allWindowsTypes[i].GetInstanceCode());

		//窗面积
		str.Format(L"%.2fm", pWinAtt->GetWindowArea());
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].areaX,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].areaY,0),
			AcGeVector3d(0,0,1), 
			120, 0, 
			str);
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].areaX+470,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].areaY+85, 0),
			AcGeVector3d(0, 0, 1),
			60, 0,
			_T("2")); //绘制㎡符号

		//开启面积
		str.Format(L"%.2fm", pWinAtt->GetTongFengQty());
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].openAreaX,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].openAreaY,0),
			AcGeVector3d(0,0,1), 
			120, 0, 
			str);
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].openAreaX + 470,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].openAreaY+85, 0),
			AcGeVector3d(0, 0, 1),
			60, 0,
			_T("2")); //绘制㎡符号

		//窗下墙高度
		str.Format(L"%dmm", (int)(pWinAtt->GetHeightUnderWindow()));
		JHCOM_CreateText(AcGePoint3d(windowDetailTemplates[i].pnt.x + windowDetailTemplates[i].heightUnderWindowX,
			windowDetailTemplates[i].pnt.y + windowDetailTemplates[i].heightUnderWindowY, 0),
			AcGeVector3d(0,0,1), 
			120, 0, 
			str);

	}

	//第四步 开始输出数据
	for (UINT i = 0; i < allWindowsTypes.size(); i++)
	{
		allWindowsTypes[i].CreateDims();
	}

	return;
}