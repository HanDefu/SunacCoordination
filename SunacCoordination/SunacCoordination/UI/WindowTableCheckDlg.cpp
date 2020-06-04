// WindowTableCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowTableCheckDlg.h"
#include "afxdialogex.h"
#include "dbtable.h"
#include <dbobjptr.h>
#include "..\Common\ComFun_Interactive.h"
#include "..\Common\ComFun_Sunac.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_ACad.h"
#include "..\Src\DocumentData.h"
#include "..\Tool\DocLock.h"
#include "..\Object\WindowDoor\WindowAutoName.h"
#include "Src/DocumentData.h"

CWindowTableCheckDlg* g_winTableCheckDlg = NULL;

// CWindowTableCheckDlg 对话框

IMPLEMENT_DYNAMIC(CWindowTableCheckDlg, CDialogEx)

CWindowTableCheckDlg::CWindowTableCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWindowTableCheckDlg::IDD, pParent)
{

}

CWindowTableCheckDlg::~CWindowTableCheckDlg()
{
}

void CWindowTableCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWindowTableCheckDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CWindowTableCheckDlg::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON_WINTABLESELECT, &CWindowTableCheckDlg::OnBnClickedButtonWintableselect)
END_MESSAGE_MAP()

BOOL CloseWindowTableCheckDlg()
{
	if (g_winTableCheckDlg == NULL)
		return TRUE;

	BOOL bRet = g_winTableCheckDlg->DestroyWindow();
	if (bRet)
		g_winTableCheckDlg = NULL;

	return bRet;
}

void CWindowTableCheckDlg::OnCancel()
{
	CloseWindowTableCheckDlg();
	DeleteBrightBox();
}

void CWindowTableCheckDlg::OnBnClickedButtonWintableselect()
{
	CDocLock lock;

	DeleteBrightBox();

	g_winTableCheckDlg->ShowWindow(SW_HIDE);

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	for (UINT i = 0; i < ids.size(); i++)
	{
		vector<AcDbObjectId> vWinIds;
		GetWinIdFromWinTableXData(ids[i], vWinIds);
		CreateBrightBox(vWinIds);
	}

	g_winTableCheckDlg->ShowWindow(SW_SHOW);
}

void CWindowTableCheckDlg::GetWinIdFromWinTableXData(AcDbObjectId p_tableId, vector<AcDbObjectId> &vWinIds)
{
	AcDbObjectPointer<AcDbTable> pEnt(p_tableId, AcDb::kForRead);
	if (pEnt.openStatus() != Acad::eOk)
		return;

	struct resbuf* pRb = pEnt->xData(L"xData");
	if (pRb == NULL)
		return;
	
	struct resbuf *pTemp = pRb;
	pTemp = pTemp->rbnext;

	AcDbHandle handle = AcDbHandle(pTemp->resval.rstring);
	AcDbObjectId winId;
	JHCOM_GetObjectIDFromAcDbHandle(handle, winId);
	if (winId!=AcDbObjectId::kNull)
		vWinIds.push_back(winId);

	while (pTemp->rbnext != NULL)
	{
		handle = AcDbHandle(pTemp->rbnext->resval.rstring);
		JHCOM_GetObjectIDFromAcDbHandle(handle, winId);
		if (winId != AcDbObjectId::kNull)
			vWinIds.push_back(winId);
		pTemp = pTemp->rbnext;
	}

	acutRelRb(pRb);
}

void CWindowTableCheckDlg::CreateBrightBox(vector<AcDbObjectId> vWinIds)
{
	int offsetx = 100;
	int offsety = 300;

	bool bSuc = true;
	for (UINT i = 0; i < vWinIds.size(); i++)
	{
		if (IsObjectExsit(vWinIds[i])==false)
		{
			bSuc = false;
			continue;
		}
		
		AcGePoint3d minPt, maxPt;
		JHCOM_GetObjectMinMaxPoint(vWinIds[i], minPt, maxPt);

		AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;
		ptLeftBottom.set(minPt.x - offsetx, minPt.y - offsetx);
		ptRightBottom.set(maxPt.x + offsetx, minPt.y - offsetx);
		ptRightTop.set(maxPt.x + offsetx, maxPt.y + offsety);
		ptLeftTop.set(minPt.x - offsetx, maxPt.y + offsety);
		
		AcDbPolyline *pPoly = new AcDbPolyline(4);
		pPoly->addVertexAt(0, ptLeftTop, 0, 100, 100);
		pPoly->addVertexAt(1, ptLeftBottom, 0, 100, 100);
		pPoly->addVertexAt(2, ptRightBottom, 0, 100, 100);
		pPoly->addVertexAt(3, ptRightTop, 0, 100, 100);
		pPoly->setClosed(Adesk::kTrue);
		pPoly->setColorIndex(2);

		AcDbPolyline *pPoly1 = new AcDbPolyline(2);
		pPoly1->addVertexAt(0, ptLeftTop, 0, 100, 100);
		pPoly1->addVertexAt(1, ptRightBottom, 0, 100, 100);
		pPoly1->setColorIndex(2);

		AcDbPolyline *pPoly2 = new AcDbPolyline(2);
		pPoly2->addVertexAt(0, ptLeftBottom, 0, 100, 100);
		pPoly2->addVertexAt(1, ptRightTop, 0, 100, 100);
		pPoly2->setColorIndex(2);

		AcDbObjectId polyId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly);
		AcDbObjectId polyId1 = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly1);
		AcDbObjectId polyId2 = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly2);

		pPoly->close();
		pPoly1->close();
		pPoly2->close();

		m_polyBrightBoxId.push_back(polyId);
		m_polyBrightBoxId.push_back(polyId1);
		m_polyBrightBoxId.push_back(polyId2);
	}

	if (bSuc==false)
	{
		AfxMessageBox(L"部分门窗表统计的门窗未找到，可能已被删除");
	}
}

void CWindowTableCheckDlg::DeleteBrightBox()
{
	for (UINT i = 0; i < m_polyBrightBoxId.size(); i++)
	{
		JHCOM_DeleteCadObject(m_polyBrightBoxId[i]);
	}

	m_polyBrightBoxId.clear();
}