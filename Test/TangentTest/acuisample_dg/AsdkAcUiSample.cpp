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

#include <accmd.h>
#include <acedads.h>
#include "AsdkAcUiSample.h"
#include "AcExtensionModule.h"
#include "../Common/ComFun_ACAD_Common.h"

//////////////////////////////////////////////////////////////////////////
#include <afxwin.h>

// ATL includes for sqareCmd.h
#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>

#include <acadi.h>
#include <axtempl.h>
#include <axpnt3d.h>
#include <acdocman.h>
#include <aced.h>
#include <acdb.h>
#include <adslib.h>
#include <rxmfcapi.h>

#include "..\Tangent\TangentCom\tch10_com19_opening.tlh"
using namespace TCH10_COM9_T20V5X64;
//////////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern "C" HWND adsw_acadMainWnd();

/////////////////////////////////////////////////////////////////////////////
// Define the sole extension module object.

AC_IMPLEMENT_EXTENSION_MODULE(theArxDLL);


typedef std::vector<AcDbObjectId> vAcDbObjectId;
AcDbObjectId AppendEntity(AcDbEntity *pEnt, const WCHAR * entry)
{
	AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBt;
	pDb->getBlockTable(pBt, AcDb::kForRead);
	AcDbBlockTableRecord *pBtr;
	pBt->getAt(entry, pBtr, AcDb::kForWrite);
	AcDbObjectId entId;
	pBtr->appendAcDbEntity(entId, pEnt);
	pBtr->close();
	pBt->close();
	pEnt->close();
	return entId;
}

vAcDbObjectId YT_Explode(AcDbObjectId entId, const WCHAR * entry)
{
	// Add your code for command ahlzlARX._test here
	vAcDbObjectId ids;
	AcDbEntity *pEnt = NULL;
	acdbOpenObject(pEnt, entId, AcDb::kForWrite);
	if(pEnt == 0)
		return ids;
	AcDbVoidPtrArray pExps;
	if (pEnt->explode(pExps) == Acad::eOk)

	{
		for (int i = 0; i < pExps.length(); i++)
		{
			AcDbEntity *pExpEnt = (AcDbEntity*)pExps[i];
			ids.push_back(AppendEntity(pExpEnt,entry));
		}
		pEnt->erase(true);
	}
	else
	{
		acutPrintf(_T("\n该对象不能被分解！"));
	}
	pEnt->close();

	return ids;
}


void Test()
{
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,_T("请选择插入点"),pt)!=RTNORM) //第一角点选择
	{
		return ;
	}
	AcGePoint3d ptOut = AcGePoint3d(pt[0], pt[1], pt[2]);


	AcDbObjectId blockid = AcDbObjectId::kNull;
	CString sPath = _T("D:\\test\\Sunac_test\\T门洞.dwg");
	CString sBlockDefName = _T("T门洞");
	int nRet = MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, sPath, sBlockDefName, blockid, ptOut, 0, AcGeScale3d(1, 1, 1));


	YT_Explode(blockid, ACDB_MODEL_SPACE);
}

//////////////////////////////////////////////////////////////////////////

AcDbObjectId SelectOne()
{
	ads_name ssname;
	AcDbObjectId objId = 0;

	int rt = acedSSGet(L":S",NULL,NULL,NULL,ssname); 
	if (rt == RTNORM)
	{
		ads_name ent;
		acedSSName(ssname, 0, ent);
		acdbGetObjectId(objId, ent);
		acedSSFree(ssname);
	}
	return objId;
}

void TangentOpenWindowTest()
{
	HRESULT hr = S_OK;
	CComPtr<IComOpening> pWin;
	CComQIPtr<IAcadBaseObject> pSquareBase;
	CComQIPtr<IAcadBaseObject2> pSquareBase2;

	//CComQIPtr<IConnectionPointContainer> pConnectionPts;
	//CComPtr<IConnectionPoint> pConnection;
	//CComPtr<IUnknown> pCmd;

	try
	{
		if (FAILED(hr = pWin.CoCreateInstance(__uuidof(ComOpening))))
			throw hr;

		pSquareBase = pWin;
		pSquareBase2 = pWin;

		if (pSquareBase!=NULL)
		{
			AcDbObjectId winId = SelectOne();
			pSquareBase->SetObjectId(winId);
		}

		pWin->PutWidth(2200);
		pWin->PutHeight(2100);

	}
	catch (HRESULT)
	{

	}
}



static void initApp()
{

  CAcModuleResourceOverride resOverride;


  acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
	  _T("YTEST"),
	  _T("YTEST"),
	  ACRX_CMD_MODAL,
	  Test,
	  NULL,
	  -1,
	  theArxDLL.ModuleResourceInstance());

  acedRegCmds->addCommand(_T("ASDK_ACUI_SAMPLE"),
	  _T("TWIN"),
	  _T("TWIN"),
	  ACRX_CMD_MODAL,
	  TangentOpenWindowTest,
	  NULL,
	  -1,
	  theArxDLL.ModuleResourceInstance());
}


static void unloadApp()
{

  // Do other cleanup tasks here  
  acedRegCmds->removeGroup(_T("ASDK_ACUI_SAMPLE"));  
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
        break;
    case AcRx::kUnloadAppMsg: 
        unloadApp(); 
        break;
    case AcRx::kInitDialogMsg:
        
        break;
    default:
        break;
    }
    return AcRx::kRetOK;
}

