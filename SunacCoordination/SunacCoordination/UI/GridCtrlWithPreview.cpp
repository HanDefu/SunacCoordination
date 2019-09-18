#include "stdafx.h"
#include "GridCtrlWithPreview.h"

CGridCtrlWithPreview::CGridCtrlWithPreview()
{
}


CGridCtrlWithPreview::~CGridCtrlWithPreview()
{
	ClearAllPreviews();
}

void CGridCtrlWithPreview::LoadDefaltSettings()
{
	EnableDragAndDrop(FALSE);
	SetEditable(FALSE);
	EnableTitleTips(FALSE);
	SetGridLines(0);
}

BEGIN_MESSAGE_MAP(CGridCtrlWithPreview, CGridCtrlEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()

int CGridCtrlWithPreview::SetRowCount(int nRows)
{
	for (int i = nRows; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->DeletePreview();
		}
	return CGridCtrlEx::SetRowCount(nRows);
}

int CGridCtrlWithPreview::SetColumnCount(int nCols)
{
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = nCols; j < GetColumnCount(); j++)
		{
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->DeletePreview();
		}
	return CGridCtrlEx::SetColumnCount(nCols);
}

void CGridCtrlWithPreview::SetDisplayRows(int nRows)
{
	m_nDisplayRows = nRows;
	CRect clientRect;
	GetClientRect(clientRect);
	int height = clientRect.Height() / nRows;
	for (int i = 0; i < GetRowCount(); i++)
		SetRowHeight(i, height);
}

void CGridCtrlWithPreview::SetDisplayColumns(int nCols)
{
	m_nDisplayCols = nCols;
	CRect clientRect;
	GetClientRect(clientRect);
	int width = clientRect.Width() / nCols;
	for (int i = 0; i < GetColumnCount(); i++)
		SetColumnWidth(i, width);
}

bool CGridCtrlWithPreview::AddPreview(int nRow, int nCol, CString sPath, CString sText, PREVIEW_LAYOUT_DIR dir)
{
	if (nRow >= GetRowCount() || nCol >= GetColumnCount())
		return false;

	if (GetPreviewCell(nRow, nCol) == NULL)
		SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellForPreview));
	CGridCellForPreview* pCell = GetPreviewCell(nRow, nCol);

	CRect gridRect;
	GetCellRect(nRow, nCol, gridRect);
	CPreviewWithDetail* pPreview = new CPreviewWithDetail;
	pPreview->Create(_T(""), WS_CHILD, gridRect, this);
	pPreview->SetLayoutMode(dir);
	pPreview->SetPreview(sPath);
	pPreview->SetText(sText);

	pCell->SetPreview(pPreview);
	return true;
}

void CGridCtrlWithPreview::ClearAllPreviews()
{
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->DeletePreview();
		}
}

void CGridCtrlWithPreview::SelectPreview(int nRow, int nCol)
{
	UINT state;
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			state = GetContentItemState(i, j);
			SetContentItemState(i, j, state & ~GVIS_SELECTED & ~GVIS_FOCUSED);
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->SetSelected(false);
		}

		state = GetContentItemState(nRow, nCol);
		SetContentItemState(nRow, nCol, state | GVIS_SELECTED | GVIS_FOCUSED);
		CGridCellForPreview* pCell = GetPreviewCell(nRow, nCol);
		if (pCell != NULL)
			pCell->SetSelected(true);
}

CGridCellForPreview* CGridCtrlWithPreview::GetPreviewCell(int nRow, int nCol)
{
	CGridCellForPreview *pCell = dynamic_cast<CGridCellForPreview *>(GetCell(nRow, nCol));
	return pCell;
}

void CGridCtrlWithPreview::OnPaint()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	CPaintDC dc(this);
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			CRect cellRect;
			GetCellRect(i, j, &cellRect);
			if (!clientRect.PtInRect(cellRect.BottomRight()) || !clientRect.PtInRect(cellRect.TopLeft()))
				cellRect.SetRectEmpty();
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->Draw(&dc, i, j, cellRect, 0);
		}
}

BOOL CGridCtrlWithPreview::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		CPoint point(pMsg->pt);
		ScreenToClient(&point);
		CCellID cellID = GetCellFromPt(point);
		SelectPreview(cellID.row, cellID.col);
		return TRUE;
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		return TRUE;
	}
	return FALSE;
}

