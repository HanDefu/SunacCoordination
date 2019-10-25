// DoubleClickCircle.cpp: implementation of the CDoubleClickCircle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DoubleClickBlockReference.h"
#include <dbents.h>
#include <actrans.h>
#include "../Common/ComFun_Sunac.h"
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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDoubleClickBlockReference::CDoubleClickBlockReference()
{

}

CDoubleClickBlockReference::~CDoubleClickBlockReference()
{

}

void CDoubleClickBlockReference::startEdit( AcDbEntity *pEnt, AcGePoint3d pt )
{
	// 获得当前文档
	AcApDocument *pDoc = acDocManager->curDocument();
	
	// 将AcDbEntity指针转换为AcDbLine指针
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);

		// 锁定文档
		acDocManager->lockDocument(pDoc);
		// 将实体的打开状态升级为可写状态
		//pBlockReference->upgradeOpen();
		pBlockReference->close();
		// 解锁文档
		acDocManager->unlockDocument(pDoc);

		eRCType rcType = TY_GetType(pBlockReference);

		CAcModuleResourceOverride resOverride;
		switch (rcType)
		{
		case WINDOW:
			{
				if (g_windowDlg == NULL)
				{
					g_windowDlg = new CWindowDlg(acedGetAcadFrame());
					g_windowDlg->Create(IDD_DIALOG_WINDOW);
					g_windowDlg->ShowWindow(SW_SHOW);
				}
				break;
			}
			
		case KITCHEN:
			{
				if (g_kitchenDlg != NULL)
					break;
				g_kitchenDlg = new CKitchenDlg(acedGetAcadFrame());
				g_kitchenDlg->Create(IDD_DIALOG_KITCHEN);
				g_kitchenDlg->SetEditMode(pBlockReference);
				g_kitchenDlg->ShowWindow(SW_SHOW);
				break;
			}
			
		case Bathroom:
			{
				if (g_bathroomDlg != NULL)
					break;
				g_bathroomDlg = new CBathroomDlg(acedGetAcadFrame());
				g_bathroomDlg->Create(IDD_DIALOG_BATHROOM);
				g_bathroomDlg->SetEditMode(pBlockReference);
				g_bathroomDlg->ShowWindow(SW_SHOW);
				break;
			}
		}
	}
	else
	{
		acutPrintf(L"\n无效的AcDbBlockReference对象...");
		pEnt->close();
		return;
	}
}

void CDoubleClickBlockReference::finishEdit( void )
{
	// 此处不使用
}