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

#include "../UI/WindowDlg.h"
#include "../UI/KitchenDlg.h"
#include "../UI/BathroomDlg.h"
#include "../UI/RailingDlg.h"
#include "../UI/AirconditionerDlg.h"
//#include "../UI/FacadeDlg.h"
//#include "../UI/FillingDlg.h"
//#include "../UI/MoldingsDlg.h"
//#include "../UI/WaterproofDlg.h"
#include "../UI/MyPalette.h"
#include "../UI/MyPaletteSet.h"
#include "../UI/DlgLogin.h"
#include "../UI/WindowAdvanceDlg.h"
#include "../Common/ComFun_Math.h"
#include "../Common/ComFun_ACad.h"
#include "../Object/WindowStatistic/WindowStatictic.h"
#include "../Object/KitchenBathroom/KitchenBathroomStatistic.h"
#include "../Object/Railing/RailingStatistic.h"
#include "../Object/AirCondition/AirConStatistic.h"
#include "../Object/WindowDoor/WindowTop2Front.h"
#include "../UI/ProjectManagementDlg.h"
#include "../WebIO/WebIO.h"
#include "Command.h"
#include "CommandWindowDetail.h"


void SendCommandToCAD(CString cmd) //此函数尚未调通
{
#if 0
	acedCommand(RTSTR, cmd, 0);
#else
	cmd.Trim();
	cmd += _T(" "); //在末尾加一个空格

	acDocManager->sendStringToExecute(curDoc(), cmd);
	return;

	//COPYDATASTRUCT cmdMsg;
	//cmdMsg.dwData = (DWORD)1;
	//cmdMsg.cbData = (DWORD)_tcslen(cmd) + 1;
	//cmdMsg.lpData = cmd.GetBuffer(cmd.GetLength() + 1);
	//SendMessage(adsw_acadMainWnd(), WM_COPYDATA, (WPARAM)adsw_acadMainWnd(), (LPARAM)&cmdMsg);
#endif
}

//登录
void CMD_Login()
{
	CAcModuleResourceOverride resOverride;

	DlgLogin dlg;
	if(IDOK == dlg.DoModal())
	//dlg.DoModal();  测试版本下不论登录是否成功都进入
	{
		CADPalette_RemoveP();
		CADPalette_AddP();
	}
}

void CMD_ShowCADPalette()
{
	if (WebIO::GetInstance()->IsLogin())
	{
		if (g_pPaletteSet == NULL)
		{
			CADPalette_AddP();
		}
		else
		{
			CADPalette_RemoveP();
		}
	}
	else
	{
		AfxMessageBox(_T("请先登录"));
	}
}
//窗
void CMD_SunacWindow()
{
	if (WebIO::GetInstance()->IsLogin()==false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (acDocManager->documentCount() == 0)
	{
		AfxMessageBox(L"没有可以操作的文档");
		return;
	}
	OpenWindowDlg();
}

void CMD_SunacWindowAdvanceDesign() //门窗深化设计
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	OpenWindowAdvanceDlg();
}

void CMD_SunacWindowDetail()
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CWindowDetail::DrawWindowDetail();
}
void CMD_SunacWindowFloorSetting()//门窗楼层设置
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	//1.选择需要设置楼层的门窗
	vAcDbObjectId winIds = SelectWindows(E_VIEW_TOP);
	if (winIds.size() == 0)
	{
		return;
	}

	CFloorInfo floorInfo;

	//2. 楼层区间
	CString sFloors;
	bool bSuc = GetStringInput(_T("请输入楼层区间逗号分隔,(示例 2-5,7,8):"), sFloors);
	if (bSuc==false)
		return;

	while (floorInfo.SetFloors(sFloors) == false && bSuc)
	{
		bSuc = GetStringInput(_T("格式错误，请输入楼层区间逗号分隔,(示例 2-5,7,8):"), sFloors);
	}
	if (bSuc == false)
		return;

	//////////////////////////////////////////////////////////////////////////
	//3.层高
	double height = 2900;
	bSuc = GetRealInput(_T("请输入楼层高度:"), 2900, 0, height);
	if (bSuc == false)
		return;
	while (floorInfo.SetFloorHeight(height) == false && bSuc)
	{
		bSuc = GetRealInput(_T("楼层高度错误，请输入楼层高度:"), 2900, 0, height);
	}
	if (bSuc == false)
		return;


	//////////////////////////////////////////////////////////////////////////
	//设置到选中的门窗中
	for (UINT i = 0; i < winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = winIds[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt!=NULL)
		{
			pAtt->SetFloorInfo(floorInfo);
			pAtt->close();
		}
	}

	acutPrintf(_T("设置楼层信息成功\n"));
}

void CMD_SunacWindowTop2Front()//门窗平面到立面
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CWindowTop2Front::GenFrontFromTop();
}


void CMD_SunacWindowFront2Top()//门窗立面到平面
{
	//TODO
}

//厨房
void CMD_SunacKitchen()
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (acDocManager->documentCount() == 0)
	{
		AfxMessageBox(L"没有可以操作的文档");
		return;
	}
	OpenKitchenDlg();
}

//卫生间
void CMD_SunacBathroom()
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (acDocManager->documentCount() == 0)
	{
		AfxMessageBox(L"没有可以操作的文档");
		return;
	}
	OpenBathroomDlg();
}

void CMD_SunacKitchenBathroomStatistic()
{
	CKitchenBathroomStatistic instance;
	instance.SelectKitchenBathroom();
	AcGePoint3d insertPoint;
	bool bSuc = TY_GetPoint(insertPoint);
	if (bSuc == false)
		return;
	instance.InsertTableToCAD(insertPoint);
}

//栏杆
void CMD_SunacRailing()
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (acDocManager->documentCount() == 0)
	{
		AfxMessageBox(L"没有可以操作的文档");
		return;
	}
	OpenRailingDlg();
}

void CMD_SunacRailingStatistic()
{
	CRailingStatistic instance;
	instance.SelectRailings();
	AcGePoint3d insertPoint;
	bool bSuc = TY_GetPoint(insertPoint);
	if (bSuc == false)
		return;
	instance.InsertTableToCAD(insertPoint);
}

//线脚
void CMD_SunacMoldings()
{
	/*CAcModuleResourceOverride resOverride;

	CMoldingsDlg dlg;
	dlg.DoModal();*/
}

//填充材质
void CMD_SunacFilling()
{
	/*CAcModuleResourceOverride resOverride;

	CFillingDlg dlg;
	dlg.DoModal();*/
}

//空调
void CMD_SunacAirconditioner()
{
	if (WebIO::GetInstance()->IsLogin() == false)
	{
		acutPrintf(_T("请先登录\n"));
		return;
	}

	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (acDocManager->documentCount() == 0)
	{
		AfxMessageBox(L"没有可以操作的文档");
		return;
	}
	OpenAirconditionerDlg();
}

void CMD_SunacAirconditionerStatistic()
{
	CAirConStatistic instance;
	instance.SelectAirCons();
	AcGePoint3d insertPoint;
	bool bSuc = TY_GetPoint(insertPoint);
	if (bSuc == false)
		return;
	instance.InsertTableToCAD(insertPoint);
}

//标准立面
void CMD_SunacFacade()
{
	/*CAcModuleResourceOverride resOverride;

	CFacadeDlg dlg;
	dlg.DoModal();*/
}

//防水构造
void CMD_SunacWaterproof()
{
	/*CAcModuleResourceOverride resOverride;

	CWaterproofDlg dlg;
	dlg.DoModal();*/
}

//统计算量
void CMD_SunacWindowsStatistics()
{
	//第一步：选择需要统计的门窗
	eViewDir viewDir = E_VIEW_FRONT;
	bool bSuc1 = SelectViewDir(viewDir);
	if (bSuc1 == false)
	{
		return;
	}
	vAcDbObjectId winIds = SelectWindows(viewDir);
	if (winIds.size() == 0)
	{
		return;
	}

	vector<AcDbObjectId> idsNonAlserials; //未设置型材系列的门窗

	vector<AttrWindow>  winAtts;
	for (UINT i = 0; i < winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = winIds[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt= oneWindow.GetAttribute();
		if (pAtt!=NULL)
		{
			AttrWindow attTemp(*pAtt);
			winAtts.push_back(attTemp);

			if (attTemp.m_material.sAluminumSerial.IsEmpty())
			{
				idsNonAlserials.push_back(winIds[i]);
			}
		}
	}

	if (idsNonAlserials.size()>0)
	{
		AfxMessageBox(_T("部分窗户型材系列未设置"));

		for (UINT i = 0; i < idsNonAlserials.size(); i++)
		{
			JHCOM_HilightObject(idsNonAlserials[i], true);
		}
		return;
	}

	CString filter = L"算量报表文件(*.xlsx)|*.xlsx|All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, L"xlsx", L"*.xlsx", NULL, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();

		CWindowStatictic winStatic;
		winStatic.Statictic(winAtts, pathName);

		AfxMessageBox(_T("输出完成"));
	}
}

void CADPalette_AddP()
{
	if (g_pPaletteSet == NULL)
	{
		g_pPaletteSet = new CMyPaletteSet;
		CRect rect(0,30,160,300);
		g_pPaletteSet->Create(_T("标准产品"), WS_VISIBLE, rect, acedGetAcadFrame());

		CMyPalette *pPalette1 = new CMyPalette;
		//CAdUiPalette *pPalette2 = new CAdUiPalette;

		pPalette1->Create(WS_VISIBLE|WS_CHILD, _T("产品"), g_pPaletteSet);
		//pPalette2->Create(WS_VISIBLE|WS_CHILD,_T("算量"),pPaletteSet);

		g_pPaletteSet->AddPalette(pPalette1);
		//pPaletteSet->AddPalette(pPalette2);

		g_pPaletteSet->EnableDocking(CBRS_ALIGN_ANY);
		g_pPaletteSet->RestoreControlBar();
	}

	acedGetAcadFrame()->ShowControlBar(g_pPaletteSet,TRUE,FALSE);
}

void CADPalette_RemoveP()
{
	if (g_pPaletteSet == NULL)
		return;

	while (g_pPaletteSet->GetPaletteCount() > 0)
	{
		CAdUiPalette* pPalette = g_pPaletteSet->GetPalette(0);
		g_pPaletteSet->RemovePalette(pPalette);
		if (pPalette != NULL)
		{
			pPalette->DestroyWindow();
			delete pPalette;
			pPalette = NULL;
		}
	}
	g_pPaletteSet->DestroyWindow();
	delete g_pPaletteSet;
	g_pPaletteSet = NULL;
}

void CloseModelessDialogs()
{
	CloseWindowDlg();
	CloseKitchenDlg();
	CloseBathroomDlg();
	CloseRailingDlg();
	CloseAirconditionerDlg();
	CloseWindowAdvanceDlg();
	CloseProjectManagementDlg();
}
