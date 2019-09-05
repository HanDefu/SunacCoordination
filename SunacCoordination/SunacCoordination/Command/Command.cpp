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
#include "../Common/ComFun_Sunac.h"
#include "../Object/RCWindow.h"
#include "../Common/ComFun_ACad.h"
#include "dbtable.h"

static void CADPalette_AddP(void);

//登录
void CMD_Login()
{
	DlgLogin dlg;
	dlg.DoModal();
	CADPalette_AddP();
}

//窗
void CMD_SUNACWINDOW()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	CWindowDlg * pDlg = new CWindowDlg(acedGetAcadFrame());
	pDlg->Create(IDD_DIALOG_WINDOW);
	pDlg->ShowWindow(SW_SHOW);
}

//厨房
void CMD_SUNACKITCHEN()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	CKitchenDlg * pDlg = new CKitchenDlg(acedGetAcadFrame());
	pDlg->Create(IDD_DIALOG_KITCHEN);
	pDlg->ShowWindow(SW_SHOW);
}

//卫生间
void CMD_SUNACBATHROOM()
{
	CBathroomDlg dlg;
	dlg.DoModal();
}

//门
void CMD_SUNACDOOR()
{
	CDoorDlg dlg;
	dlg.DoModal();
}

//栏杆
void CMD_SUNACRAILING()
{
	CAcModuleResourceOverride resOverride;

	//Memory freed on PostNcDestroy(call delete this;) or cancel function.
	CRailingDlg * pDlg = new CRailingDlg(acedGetAcadFrame());
	pDlg->Create(IDD_DIALOG_RAILING);
	pDlg->ShowWindow(SW_SHOW);
}

//线脚
void CMD_SUNACMOLDINGS()
{
	CMoldingsDlg dlg;
	dlg.DoModal();
}

//填充材质
void CMD_SUNACFILLING()
{
	CFillingDlg dlg;
	dlg.DoModal();
}

//空调
void CMD_SUNACAIRCONDITIONER()
{
	CAirconditionerDlg dlg;
	dlg.DoModal();
}

//标准立面
void CMD_SUNACFACADE()
{
	CFacadeDlg dlg;
	dlg.DoModal();
}

//防水构造
void CMD_SUNACWATERPROOF()
{
	CWaterproofDlg dlg;
	dlg.DoModal();
}

//统计算量
void CMD_SUNACSTATISTICS()
{

}


static void CADPalette_AddP(void)
{
	CMyPaletteSet *pPaletteSet = new CMyPaletteSet;
	CRect rect(0,30,160,300);
	pPaletteSet->Create(_T("标准产品"),WS_VISIBLE,rect,acedGetAcadFrame());
	CMyPalette *pPalette1 = new CMyPalette;
	CAdUiPalette *pPalette2 = new CAdUiPalette;
	pPalette1->Create(WS_VISIBLE|WS_CHILD,_T("产品"),pPaletteSet);
	//pPalette2->Create(WS_VISIBLE|WS_CHILD,_T("算量"),pPaletteSet);

	pPaletteSet->AddPalette(pPalette1);
	//pPaletteSet->AddPalette(pPalette2);
	pPaletteSet->EnableDocking(CBRS_ALIGN_ANY);
	pPaletteSet->RestoreControlBar();

	acedGetAcadFrame()->ShowControlBar(pPaletteSet,TRUE,FALSE);
}

