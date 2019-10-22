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
#include "../UI/DoorDlg.h"
#include "../UI/FacadeDlg.h"
#include "../UI/FillingDlg.h"
#include "../UI/MoldingsDlg.h"
#include "../UI/WaterproofDlg.h"
#include "../ui/MyPalette.h"
#include "../ui/MyPaletteSet.h"
#include "../ui/DlgLogin.h"
#include "Command.h"

#include "../Common/ComFun_Math.h"

//登录
void CMD_Login()
{
	CAcModuleResourceOverride resOverride;

	DlgLogin dlg;
	dlg.DoModal();
	CADPalette_AddP();
}

//窗
void CMD_SUNACWINDOW()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (g_windowDlg == NULL)
	{
		//预览控件必须在当前文档下加载
		if (acDocManager->documentCount() == 0)
		{
			AfxMessageBox(L"没有可以操作的文档");
			return;
		}
		g_windowDlg = new CWindowDlg(acedGetAcadFrame());
		g_windowDlg->Create(IDD_DIALOG_WINDOW);
	}
	g_windowDlg->ShowWindow(SW_SHOW);
}

//厨房
void CMD_SUNACKITCHEN()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (g_kitchenDlg == NULL)
	{
		//预览控件必须在当前文档下加载
		if (acDocManager->documentCount() == 0)
		{
			AfxMessageBox(L"没有可以操作的文档");
			return;
		}
		g_kitchenDlg = new CKitchenDlg(acedGetAcadFrame());
		g_kitchenDlg->Create(IDD_DIALOG_KITCHEN);
	}
	g_kitchenDlg->ShowWindow(SW_SHOW);
}

//卫生间
void CMD_SUNACBATHROOM()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (g_bathroomDlg == NULL)
	{
		//预览控件必须在当前文档下加载
		if (acDocManager->documentCount() == 0)
		{
			AfxMessageBox(L"没有可以操作的文档");
			return;
		}
		g_bathroomDlg = new CBathroomDlg(acedGetAcadFrame());
		g_bathroomDlg->Create(IDD_DIALOG_BATHROOM);
	}
	g_bathroomDlg->ShowWindow(SW_SHOW);
}

//门
void CMD_SUNACDOOR()
{
	CAcModuleResourceOverride resOverride;

	CDoorDlg dlg;
	dlg.DoModal();
}

//栏杆
void CMD_SUNACRAILING()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (g_railingDlg == NULL)
	{
		//预览控件必须在当前文档下加载
		if (acDocManager->documentCount() == 0)
		{
			AfxMessageBox(L"没有可以操作的文档");
			return;
		}
		g_railingDlg = new CRailingDlg(acedGetAcadFrame());
		g_railingDlg->Create(IDD_DIALOG_RAILING);
	}
	g_railingDlg->ShowWindow(SW_SHOW);
}

//线脚
void CMD_SUNACMOLDINGS()
{
	CAcModuleResourceOverride resOverride;

	CMoldingsDlg dlg;
	dlg.DoModal();
}

//填充材质
void CMD_SUNACFILLING()
{
	CAcModuleResourceOverride resOverride;

	CFillingDlg dlg;
	dlg.DoModal();
}

//空调
void CMD_SUNACAIRCONDITIONER()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	if (g_airconditionerDlg == NULL)
	{
		//预览控件必须在当前文档下加载
		if (acDocManager->documentCount() == 0)
		{
			AfxMessageBox(L"没有可以操作的文档");
			return;
		}
		g_airconditionerDlg = new CAirconditionerDlg(acedGetAcadFrame());
		g_airconditionerDlg->Create(IDD_DIALOG_AIRCONDITIONER);
	}
	g_airconditionerDlg->ShowWindow(SW_SHOW);
}

//标准立面
void CMD_SUNACFACADE()
{
	CAcModuleResourceOverride resOverride;

	CFacadeDlg dlg;
	dlg.DoModal();
}

//防水构造
void CMD_SUNACWATERPROOF()
{
	CAcModuleResourceOverride resOverride;

	CWaterproofDlg dlg;
	dlg.DoModal();
}

//统计算量
void CMD_SUNACSTATISTICS()
{

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
}
