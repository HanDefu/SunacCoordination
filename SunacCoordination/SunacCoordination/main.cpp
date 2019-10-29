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
#include "main.h"
#include "AcExtensionModule.h"
#include "accmd.h"
#include "UI\menu\Menu_Def.h"
#include "object\WindowDoor\AttrWindow.h"
#include "object\AirCondition\AttrAirCon.h"
#include "object\Kitchen\AttrKitchen.h"
#include "object\Bathroom\AttrBathroom.h"
#include "object\Railing\AttrRailing.h"
#include "Command\CommandWindowTable.h"
#include "Command\Command.h"
#include "GlobalSetting.h"
#include "command\CommandWindowDetail.h"
#include "Common/ComFun_Sunac.h"
#include "webIO\WindowLocalData.h"
#include "WebIO\ConfigDictionary.h"
#include "Tool\DoubleClickBlockReference.h"
#include "Common\ComFun_Str.h"
#include "WebIO\WebIO.h"
#include "WebIO\KitchenBathroomWebData.h"
#include "WebIO\RailingWebData.h"
#include "WebIO\AirconditionWebData.h"
#include "WebIO\WindowWebData.h"
#include "WebIO\SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "Common\ComFun_Str.h"
#include "Tool\MarkupXml\Markup.h"
#include <string>
#include "WebIO/WebIO.h"
#include "Object/WindowStatistic/WindowMaterialUsage.h"
#include "Object/WindowStatistic/WindowFormula.h"
#include "Object/WindowStatistic/AluminumSeries.h"
#include "Object/WindowStatistic/DeductedSize.h"
#include "ProjectorFileMrg/ProjectFileMrg.h"
#include "Object/WindowStatistic/WindowStatictic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern "C" HWND adsw_acadMainWnd();
static HANDLE mThreadHandle = 0;

ACRX_DEFINE_MEMBERS(AcDbDoubleClickEdit);
enum Acad::ErrorStatus __stdcall LoadManagedDll(ACHAR const *path);
/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.
//和服务器同步数据
static void SyncDataWithService(void * ptr);

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);


void CMD_YTest()
{
	AttrWindow attrwindow;

	attrwindow.m_prototypeCode = _T("Door_TLM3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("推拉");
	attrwindow.m_openQty = 1;

	CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	CWindowsDimData dimdata3;

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/4");
	attrwindow.SetDimData(dimdata1);

	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);
	
	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(3600);
	attrwindow.SetH1(500);
	attrwindow.SetW1(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("ST105AM系列");
	attrwindow.SetInstanceCode(_T("TLM3"));

	vector<AttrWindow> winAtts;
	winAtts.push_back(attrwindow);
	winAtts.push_back(attrwindow);

	attrwindow.m_prototypeCode = _T("Window_NC3");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("内开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("(W-2a)/2");
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);
	
	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(1400);
	attrwindow.SetW(1300);
	attrwindow.SetH1(500);
	attrwindow.SetW1(700);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SN65A系列");
	attrwindow.SetInstanceCode(_T("NC3"));

	winAtts.push_back(attrwindow);

	CWindowStatictic winStatictic;

	CString filter = L"参数文件(*.xlsx)|*.xlsx|All Files(*.*)|*.*||";
	CFileDialog dlg(FALSE, L"xlsx", L"*.xlsx", NULL, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString pathName = dlg.GetPathName();
		winStatictic.Statictic(winAtts, pathName);
	}
}

void AddSubMenu(CAcadPopupMenu&IPopUpMenu, UINT MenuStartIndex)
{
	VARIANT index;

	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = MenuStartIndex++;
	IPopUpMenu.AddMenuItem(index, _T("登录"), _T("_SLOGIN ")); //命令的前面的下划线_ 和最后的空格都不能少
	
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = MenuStartIndex++;
	IPopUpMenu.AddMenuItem(index, _T("产品标准"), _T(" "));

}

void InitMenu()
{
	TRY
	{
		CAcadApplication IAcad(acedGetAcadWinApp()->GetIDispatch(TRUE));
		CAcadMenuBar IMenuBar(IAcad.get_MenuBar());

		long numberOfMenus = IMenuBar.get_Count();

		CAcadMenuGroups IMenuGroups(IAcad.get_MenuGroups());

		VARIANT index;
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;

		CAcadMenuGroup IMenuGroup(IMenuGroups.Item(index));
		CAcadPopupMenus IPopUpMenus(IMenuGroup.get_Menus());

		CString cstrMenuName = _T("融创产品标准");//自定义主菜单

		VariantInit(&index);
		V_VT(&index) = VT_BSTR;
		V_BSTR(&index) = cstrMenuName.AllocSysString();

		IDispatch* pDisp = NULL;
		TRY{ pDisp = IPopUpMenus.Item(index); pDisp->AddRef(); } CATCH(COleDispatchException, e){}END_CATCH;

		if (pDisp == NULL)
		{
			CAcadPopupMenu IPopUpMenu(IPopUpMenus.Add(cstrMenuName));
			UINT MenuIndex = 0;

			AddSubMenu(IPopUpMenu, MenuIndex);

			pDisp = IPopUpMenu.m_lpDispatch;
			pDisp->AddRef();
		}

		CAcadPopupMenu IPopUpMenu(pDisp);
		if (!IPopUpMenu.get_OnMenuBar())
		{
			VariantInit(&index);
			V_VT(&index) = VT_I4;
			V_I4(&index) = numberOfMenus;
			IPopUpMenu.InsertInMenuBar(index);
		}
		else
		{
			VariantInit(&index);
			V_VT(&index) = VT_BSTR;
			V_BSTR(&index) = cstrMenuName.AllocSysString();
			IPopUpMenus.RemoveMenuFromMenuBar(index);
			VariantClear(&index);
		}
		pDisp->Release();
	}
	CATCH(COleDispatchException, e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;
}

void ZJYTest()
{
	CString localWindowPath = TY_GetPrototypeFilePath();

	vector<AttrWindow> m_winAtts;
	AttrWindow attrwindow;

	attrwindow.m_prototypeCode = _T("Door_WM2");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);
	CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);
	CWindowsDimData dimdata3;
	/*dimdata3.sCodeName = _T("a");
	dimdata3.type = UNLIMIT;
	attrwindow.SetDimData(dimdata3);*/


	//CWindowsDimData dimdata1;
	dimdata1.sCodeName = _T("W1");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-W2");
	attrwindow.SetDimData(dimdata1);

	
	dimdata1.sCodeName = _T("W2");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("450-a");
	attrwindow.SetDimData(dimdata1);
	
	//dimdata1.sCodeName = _T("W3");
	//dimdata1.type = UNLIMIT;
	////dimdata1.sFomula = _T("");
	//attrwindow.SetDimData(dimdata1);
	//

	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a");
	attrwindow.SetDimData(dimdata2);

	//dimdata2.sCodeName = _T("H2");
	//dimdata2.type = UNLIMIT;
	//attrwindow.SetDimData(dimdata2);

	//dimdata2.sCodeName = _T("H3");
	//dimdata2.type = CALC;
	//dimdata2.sFomula = _T("(W-2a)/2");
	//attrwindow.SetDimData(dimdata2);

	//dimdata2.sCodeName = _T("R");
	//dimdata2.type = UNLIMIT;
	////dimdata2.sFomula = _T("(W-2a)/2");
	//attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(1200);
	attrwindow.SetH1(500);
	//attrwindow.SetH2(500);
	//attrwindow.SetH3(500);
	attrwindow.SetW1(700);
	attrwindow.SetW2(600);
	//attrwindow.SetW3(500);
	//attrwindow.SetR(0);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW60AM系列");
	attrwindow.SetInstanceCode(_T("111"));

	m_winAtts.push_back(attrwindow);

	attrwindow.m_prototypeCode = _T("Door_WLC1");
	attrwindow.m_quyuName = _T("全部");
	attrwindow.m_isJiTuan = true;
	attrwindow.m_isDynamic = true;

	attrwindow.m_gongNengquType = _T("全部");
	attrwindow.m_openType = _T("外开");
	attrwindow.m_openQty = 1;

	dimdata1.sCodeName = _T("W");
	dimdata1.type = UNLIMIT;
	attrwindow.SetDimData(dimdata1);

	dimdata2.sCodeName = _T("H");
	dimdata2.type = UNLIMIT;
	attrwindow.SetDimData(dimdata2);

	dimdata1.sCodeName = _T("W1");
	dimdata1.type = UNLIMIT;
	//dimdata1.sFomula = _T("(W-2a)/3");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W2");
	dimdata1.type = CALC;
	dimdata1.sFomula = _T("W-2a-1700");
	attrwindow.SetDimData(dimdata1);

	dimdata1.sCodeName = _T("W3");
	dimdata1.type = UNLIMIT;
	//dimdata1.sFomula = _T("");
	attrwindow.SetDimData(dimdata1);
	

	//CWindowsDimData dimdata2;
	dimdata2.sCodeName = _T("H1");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("H-2a-H2");
	attrwindow.SetDimData(dimdata2);

	dimdata2.sCodeName = _T("H2");
	dimdata2.type = CALC;
	dimdata2.sFomula = _T("1000-a");
	attrwindow.SetDimData(dimdata2);

	//dimdata2.sCodeName = _T("H3");
	//dimdata2.type = CALC;
	//dimdata2.sFomula = _T("(W-2a)/2");
	//attrwindow.SetDimData(dimdata2);

	//dimdata2.sCodeName = _T("R");
	//dimdata2.type = UNLIMIT;
	////dimdata2.sFomula = _T("(W-2a)/2");
	//attrwindow.SetDimData(dimdata2);

	attrwindow.CheckAndComplementDimeData();

	attrwindow.SetH(2400);
	attrwindow.SetW(2700);
	attrwindow.SetH1(500);
	attrwindow.SetH2(500);
	//attrwindow.SetH3(500);
	attrwindow.SetW1(1000);
	attrwindow.SetW2(600);
	attrwindow.SetW3(700);
	attrwindow.SetR(0);
	attrwindow.SetA(50);

	attrwindow.m_material.sAluminumSerial = _T("SW65M系列");
	attrwindow.SetInstanceCode(_T("222"));

	m_winAtts.push_back(attrwindow);


	CWindowStatictic winStatictic;

	CString filter=L"参数文件(*.xlsx)|*.xlsx|All Files(*.*)|*.*||";  
	CFileDialog dlg(FALSE, L"xlsx", L"*.xlsx", NULL, filter); 
	if(dlg.DoModal()==IDOK)
	{
		CString pathName = dlg.GetPathName();
		winStatictic.Statictic(m_winAtts, pathName);
	}
}

void CMD_test()
{
/*
	ads_name ename;
	ads_point pt;
	if (acedEntSel(L"\nSelect a dynamic block reference: ", ename, pt) != RTNORM)
	{
		acutPrintf(L"\nError selecting entity.");
		return;
	}
	AcDbObjectId eId;
	acdbGetObjectId(eId, ename);*/

	////SetDoorPos(eId, 3600);
	//SelectZaoTai(eId, L"900");
	//SelectShuiPen(eId, L"双盆900");
	//SetShuiPenPos(eId, 3000);
	////SelectShuiPen(eId, L"双盆900");
	////SelectZaoTai(eId, L"800");
	////acedRedraw(eId,);

	double width = 1800;
	double height = 1800;
	CString doorType = "";

	CWindowWebData WindowFromWeb;
	CWindowLocalData WindowFromLocal;

	CWindowWebData DoorFromWeb;
	CWindowLocalData DoorFromLocal;

	CKitchenBathroomWebData KitchenFromWeb;
	CKitchenBathroomWebData BathroomFromWeb;
	CRailingWebData RailingFromWeb;
	CAirConditionWebData AirConFromWeb;
	//vAttrWindow window = b.GetWindows(width, height, "", 0, "");
	std::vector<AttrWindow> WindowFromWebData = WindowFromWeb.GetAllWindows();
	std::vector<AttrWindow> WindowFromWebData2 = WindowFromWeb.GetWindows(1800, 1800, L"外开", 1, L"");
	std::vector<AttrWindow> WindowFromLocalData = WindowFromLocal.GetAllWindows();

	//比较windows1, windows2
/*
	for(int i = 0; i < WindowFromWebData.size(); i++ )
	{
		AttrWindow &curWebWin = WindowFromWebData[i];

		//从window1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < WindowFromLocalData.size(); j++)
		{
			if (curWebWin.m_prototypeCode == WindowFromLocalData[j].m_prototypeCode)
			{
				bFind = true;
				if(WindowFromLocalData[j].IsWindowPrototypeEqual(WindowFromWebData[i]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

		assert(bFind==true);
	}*/

	std::vector<AttrWindow> DoorFromWebData = DoorFromWeb.GetAllDoors();
	std::vector<AttrWindow> DoorFromLocalData = DoorFromLocal.GetAllDoors();
/*
	for(int i = 0; i < DoorFromWebData.size(); i++ )
	{
		AttrWindow &curWebWin = DoorFromWebData[i];

		//从window1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < DoorFromLocalData.size(); j++)
		{
			if (curWebWin.m_prototypeCode == DoorFromLocalData[j].m_prototypeCode)
			{
				bFind = true;
				if(DoorFromLocalData[j].IsDoorPrototypeEqual(DoorFromWebData[i]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

		assert(bFind==true);
	}*/

	std::vector<AttrKitchen> KitchenFromLocalData = WebIO::GetInstance()->GetAllKitchens();
	std::vector<AttrKitchen> KitchenFromWebData = KitchenFromWeb.GetAllKitchens();
	/*
	for(int i = 0; i < attrs2.size(); i++ )
	{
		AttrKitchen &curWebAttr = attrs2[i];

		//从attr1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < attrs1.size(); j++)
		{
			if (curWebAttr.m_prototypeCode == attrs1[j].m_prototypeCode)
			{
				bFind = true;
				if(attrs1[i].IsKitchenPrototypeEqual(attrs2[j]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

	}*/

	std::vector<AttrKitchen> KitchenFromWebData2 = KitchenFromWeb.GetKitchens(2500, 2000, L"门窗垂直开", L"U型", true);
	std::vector<AttrBathroom> BathroomFromWebData = BathroomFromWeb.GetBathrooms(1800, 2000, L"门窗垂直开", L"L型");
	std::vector<AttrRailing> RailingFromWebData = RailingFromWeb.GetRailings(E_RAILING_BOLI);
	std::vector<AttrAirCon> AirConFromWebData = AirConFromWeb.GetAirCons(1.5, "后面", 1, "侧面");
	return;
}

void CMD_TEST2()
{
	/*vector<CAluminumFormula> vAlFormula;
	vector<CGlassFormula> vGlassFormula;
	vector<CHardwareData> vHardwareFormula;
	vector<CString> vAlSeries;
	CAluminumData AlData;
	CString AlSeries;
	double DeductedSizeData;
	vAlFormula = CWindowFormula::Instance()->GetAluminumFormulas(L"Window_NC1");
	vGlassFormula = CWindowFormula::Instance()->GetGlassFormulas(L"Window_NC1");
	vHardwareFormula = CWindowFormula::Instance()->GetHardwareData(L"Window_NC1");
	CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(E_WindowDoor_NC, L"SN65A系列", L"M1", DeductedSizeData);
	CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(E_WindowDoor_WC, L"SN65A系列", L"M1", DeductedSizeData);
	CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(E_WindowDoor_NC, L"SN60系列", L"假中梃", AlData);
	CAluminumSeries::Instance()->GetAluminumSerialByCode(L"SN60T002", AlSeries);
	vAlSeries = CAluminumSeries::Instance()->GetAluminumSerialsByWindowType(E_WindowDoor_NC);*/
	CProjectData::UploadFile("F:\\soapcpp2.exe", "soapcpp2.exe");
	CProjectData::DownloadFile(L"http://fastsoft.onlinedown.net/down/idm_ald.exe", L"F:\\FTPServer\\Test.exe");
}

static void initApp()
{
	CAcModuleResourceOverride resOverride;

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("ytest"),
		_T("ytest"),
		ACRX_CMD_MODAL,
		CMD_YTest,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());
	acedRegCmds->addCommand(_T("SUNAC"),
		_T("tes"),
		_T("tes"),
		ACRX_CMD_MODAL,
		CMD_test,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("test2"),
		_T("test2"),
		ACRX_CMD_SESSION,
		CMD_TEST2,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("zjy"),
		_T("zjy"),
		ACRX_CMD_MODAL,
		ZJYTest,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	//////////////////////////////////////////////////////////////////////////

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SLOGIN"),
		_T("SLOGIN"),
		ACRX_CMD_MODAL,
		CMD_Login,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDOW"),
		_T("SWINDOW"),
		ACRX_CMD_MODAL,
		CMD_SunacWindow,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINADVANCE"),
		_T("SWINADVANCE"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowAdvanceDesign,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDOWSTATISTICS"),
		_T("SWINDOWSTATISTICS"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowsStatistics,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDOWTABLE"),
		_T("SWINDOWTABLE"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowsTable,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWINDOWDETAIL"),
		_T("SWINDOWDETAIL"),
		ACRX_CMD_MODAL,
		CMD_SunacWindowDetail,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SKITCHEN"),
		_T("SKITCHEN"),
		ACRX_CMD_MODAL,
		CMD_SunacKitchen,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SBATHROOM"),
		_T("SBATHROOM"),
		ACRX_CMD_MODAL,
		CMD_SunacBathroom,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SRAILING"),
		_T("SRAILING"),
		ACRX_CMD_MODAL,
		CMD_SunacRailing,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SAIRCONDITIONER"),
		_T("SAIRCONDITIONER"),
		ACRX_CMD_MODAL,
		CMD_SunacAirconditioner,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SMOLDINGS"),
		_T("SMOLDINGS"),
		ACRX_CMD_MODAL,
		CMD_SunacMoldings,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SFILLING"),
		_T("SFILLING"),
		ACRX_CMD_MODAL,
		CMD_SunacFilling,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());


	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SFACADE"),
		_T("SFACADE"),
		ACRX_CMD_MODAL,
		CMD_SunacFacade,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());

	acedRegCmds->addCommand(_T("SUNAC"),
		_T("SWATERPROOF"),
		_T("SWATERPROOF"),
		ACRX_CMD_MODAL,
		CMD_SunacWaterproof,
		NULL,
		-1,
		theArxDLL.ModuleResourceInstance());



	AttrObject::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT));

	AttrWindow::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW));

	AttrAirCon::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON));

	AttrKitchen::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN));

	AttrBathroom::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom));

	AttrRailing::rxInit();
	acrxBuildClassHierarchy();
	acrxRegisterService(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING));

	// 为AcDbCircle类添加协议扩展
	CDoubleClickBlockReference *pCircleEdit = new CDoubleClickBlockReference;
	AcDbBlockReference::desc()->addX(AcDbDoubleClickEdit::desc(), pCircleEdit);	

	LoadManagedDll(MD2010_GetAppPath() + L"\\support\\Sunac2019\\rcdc.dll");

	//WEBINST;

	//mThreadHandle = (HANDLE)_beginthread(&SyncDataWithService, 0, 0);
}


static void unloadApp()
{
	// Do other cleanup tasks here  
	acedRegCmds->removeGroup(_T("SUNAC"));  

	deleteAcRxClass(AttrWindow::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW));

	deleteAcRxClass(AttrAirCon::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON));

	deleteAcRxClass(AttrRailing::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING));
	
	deleteAcRxClass(AttrKitchen::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN));

	deleteAcRxClass(AttrBathroom::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom));

	deleteAcRxClass(AttrObject::desc());
	delete acrxServiceDictionary->remove(_T(ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT));

	AcDbBlockReference::desc()->delX(AcDbDoubleClickEdit::desc());

	//WaitForSingleObject(mThreadHandle, 1000);
	//TerminateThread(mThreadHandle,0);
}


//////////////////////////////////////////////////////////////
//
// Entry points
//
//////////////////////////////////////////////////////////////
extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		theArxDLL.AttachInstance(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		theArxDLL.DetachInstance();  
	}
	return 1;   // ok
}

extern "C" AcRx::AppRetCode acrxEntryPoint( AcRx::AppMsgCode msg, void* appId)
{
	switch( msg ) 
	{
	case AcRx::kInitAppMsg: 
		acrxDynamicLinker->unlockApplication(appId);
		acrxDynamicLinker->registerAppMDIAware(appId);
		initApp(); 
		InitMenu();
		break;
	case AcRx::kUnloadAppMsg: 
		CloseModelessDialogs();
		CADPalette_RemoveP();
		unloadApp();
		if (acDocManager->documentCount() > 0)
			InitMenu();
		break;
	case AcRx::kQuitMsg:
		break;
	case  AcRx::kLoadDwgMsg:
		break;
	case  AcRx::kUnloadDwgMsg:
		CloseModelessDialogs(); //预览控件依赖于当前文档，关闭文档时退出对话框以防止崩溃
		break;
	case  AcRx::kSaveMsg:
		break;
	case AcRx::kInitDialogMsg:
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}

static void SyncDataWithService(void * ptr)
{
	while (1)
	{
		Sleep(1000);
	}
	GSINST->m_syncOK = true;
}

