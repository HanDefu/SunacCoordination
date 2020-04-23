// WindowTableCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WindowTableCheckDlg.h"
#include "afxdialogex.h"
#include "dbtable.h"
#include "..\Common\ComFun_Interactive.h"
#include "..\Common\ComFun_Sunac.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_ACad.h"
#include "..\Src\DocumentData.h"
#include "..\Tool\DocLock.h"

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

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	for (UINT i = 0; i < ids.size(); i++)
	{
		vector<AcDbObjectId> vWinIds;
		GetWinIdFromWinTableXData(ids[i], vWinIds);
		CreateBrightBox(vWinIds);
	}
}

//void CWindowTableCheckDlg::GetWinIdFromWinTableXData(AcDbObjectId p_tableId, vector<AcDbObjectId> &vWinIds)
//{
//	AcDbObject *pEnt = NULL;
//	Acad::ErrorStatus es = acdbOpenAcDbObject(pEnt, p_tableId, AcDb::kForRead);
//	AcDbTable* pTable = AcDbTable::cast(pEnt);
//	if (pTable == NULL)
//		return;
//
//	struct resbuf* pRb = pTable->xData(L"xData");
//	if (pRb == NULL)
//		return;
//
//	pEnt->close();
//	pTable->close();
//
//	struct resbuf *pTemp = pRb;
//	pTemp = pTemp->rbnext;
//
//	vCString vStr;
//	CString sValue = pTemp->resval.rstring;
//	vStr.push_back(sValue);
//	while (pTemp->rbnext != NULL)
//	{
//		sValue = pTemp->rbnext->resval.rstring;
//		vStr.push_back(sValue);
//		pTemp = pTemp->rbnext;
//	}
//
//	for (int i = 0; i < vStr.size(); i++)
//	{
//		AcDbHandle handle;
//		AcDbObjectId winId;
//		vCString vStrHandle = YT_SplitCString(vStr[i], L'-');
//		handle.setHigh(_ttoi(vStrHandle[0]));
//		handle.setLow(_ttoi(vStrHandle[1]));
//		JHCOM_GetObjectIDFromAcDbHandle(handle, winId);
//		vWinIds.push_back(winId);
//	}
//
//	acutRelRb(pRb);
//}

void CWindowTableCheckDlg::GetWinIdFromWinTableXData(AcDbObjectId p_tableId, vector<AcDbObjectId> &vWinIds)
{
	AcDbObject *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbObject(pEnt, p_tableId, AcDb::kForRead);
	AcDbTable* pTable = AcDbTable::cast(pEnt);
	if (pTable == NULL)
		return;

	struct resbuf* pRb = pTable->xData(L"xData");
	if (pRb == NULL)
		return;

	pEnt->close();
	pTable->close();

	struct resbuf *pTemp = pRb;
	pTemp = pTemp->rbnext;
	CString sValue = pTemp->resval.rstring;
	vCString vStr = YT_SplitCString(sValue, L',');

	for (int i = 0; i < vStr.size(); i++)
	{
		AcDbHandle handle;
		AcDbObjectId winId;
		vCString vStrHandle = YT_SplitCString(vStr[i], L'-');
		handle.setHigh(_ttoi(vStrHandle[0]));
		handle.setLow(_ttoi(vStrHandle[1]));
		JHCOM_GetObjectIDFromAcDbHandle(handle, winId);
		vWinIds.push_back(winId);
	}

	acutRelRb(pRb);
}

void CWindowTableCheckDlg::CreateBrightBox(vector<AcDbObjectId> vWinIds)
{
	int offset = 20;

	for (int i = 0; i < vWinIds.size(); i++)
	{
		//vector<AcDbObjectId> vTextIds = GetInstanceCodeMrg()->FindTextIds(vWinIds[i]);
		//for (int i = 0; i < vTextIds.size(); i++)
		//{
			AcGePoint3d minPt, maxPt;
			JHCOM_GetObjectMinMaxPoint(vWinIds[i], minPt, maxPt);

			AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;
			ptLeftBottom.set(minPt.x - offset, minPt.y - offset);
			ptRightBottom.set(maxPt.x + offset, minPt.y - offset);
			ptRightTop.set(maxPt.x + offset, maxPt.y + offset);
			ptLeftTop.set(minPt.x - offset, maxPt.y + offset);

			AcDbPolyline *pPoly = new AcDbPolyline(4);
			pPoly->addVertexAt(0, ptLeftTop, 0, 1, 1);
			pPoly->addVertexAt(1, ptLeftBottom, 0, 1, 1);
			pPoly->addVertexAt(2, ptRightBottom, 0, 1, 1);
			pPoly->addVertexAt(3, ptRightTop, 0, 1, 1);
			pPoly->setClosed(Adesk::kTrue);
			pPoly->setColorIndex(2);

			AcDbObjectId polyId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly);
			pPoly->close();

			m_polyBrightBoxId.push_back(polyId);
	//	}
	}
}

void CWindowTableCheckDlg::DeleteBrightBox()
{
	for (int i = 0; i < m_polyBrightBoxId.size(); i++)
	{
		JHCOM_DeleteCadObject(m_polyBrightBoxId[i]);
	}

	m_polyBrightBoxId.clear();
}