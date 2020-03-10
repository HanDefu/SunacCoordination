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
	if (pEnt->isKindOf(AcDbBlockReference::desc()))
	{
		AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);

		//AcApDocument *pDoc = acDocManager->curDocument();
		//acDocManager->lockDocument(pDoc);
		//// 将实体的打开状态升级为可写状态
		////pBlockReference->upgradeOpen();
		//acDocManager->unlockDocument(pDoc);

		eRCType rcType = TY_GetType(pBlockReference);
		pBlockReference->close();

		CAcModuleResourceOverride resOverride;
		switch (rcType)
		{
		case WINDOW:
			{
				OpenWindowDlg(pBlockReference->objectId());
				break;
			}
			
		case KITCHEN:
			{
				OpenKitchenDlg(pBlockReference);
				break;
			}
			
		case Bathroom:
			{
				OpenBathroomDlg(pBlockReference);
				break;
			}
		case RAILING:
			{
				OpenRailingDlg(pBlockReference);
				break;
			}
		case AIRCON:
			{
				OpenAirconditionerDlg(pBlockReference);
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