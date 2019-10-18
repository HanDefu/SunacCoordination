#include "stdafx.h"
#include "GridCtrlWithPreview.h"

CGridCtrlWithPreview::CGridCtrlWithPreview()
{
	m_nDisplayRows = 1;
	m_nDisplayCols = 1;
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

void CGridCtrlWithPreview::SetDisplayRows(int nRows)
{
	m_nDisplayRows = nRows;
	CRect clientRect;
	GetClientRect(clientRect);
	int height = clientRect.Height() / nRows;
	SetDefCellHeight(height);
	for (int i = 0; i < GetRowCount(); i++)
		SetRowHeight(i, height);
}

void CGridCtrlWithPreview::SetDisplayColumns(int nCols)
{
	m_nDisplayCols = nCols;
	CRect clientRect;
	GetClientRect(clientRect);
	int width = clientRect.Width() / nCols;
	SetDefCellWidth(width);
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
CGridCellForPreview* CGridCtrlWithPreview::AddPreviewPng(int nRow, int nCol, CString sPath, CString sText, PREVIEW_LAYOUT_DIR dir)
{
	if (nRow >= GetRowCount() || nCol >= GetColumnCount())
		return NULL;

	if (GetPreviewCell(nRow, nCol) == NULL)
		SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellForPreview));
	CGridCellForPreview* pCell = GetPreviewCell(nRow, nCol);

	CRect gridRect;
	GetCellRect(nRow, nCol, gridRect);
	CPreviewWithDetail* pPreview = new CPreviewWithDetail;
	pPreview->Create(_T(""), WS_CHILD, gridRect, this);
	pPreview->SetLayoutMode(dir);
	pPreview->SetPreviewImage(sPath);
	pPreview->SetText(sText);

	pCell->SetPreview(pPreview);
	return pCell;
}

void CGridCtrlWithPreview::ClearAllPreviews()
{
	SetRowCount(0);
}

void CGridCtrlWithPreview::SelectPreview(int nRow, int nCol)
{
	if (nRow < 0 || nRow >= GetRowCount())
		return;
	if (nCol < 0 || nCol >= GetColumnCount())
		return;

	EnsureVisible(nRow, nCol);
	CPoint origin;
	GetCellOrigin(nRow, nCol, &origin);
	OnLButtonDown(MK_LBUTTON, origin);
	OnLButtonUp(MK_LBUTTON, origin);
}

CGridCellForPreview* CGridCtrlWithPreview::GetPreviewCell(int nRow, int nCol)
{
	CGridCellForPreview *pCell = dynamic_cast<CGridCellForPreview *>(GetCell(nRow, nCol));
	return pCell;
}

void CGridCtrlWithPreview::OnPaint()
{
	CPaintDC dc(this);

	CCellID topLeftCell = GetTopleftNonFixedCell();
	int stRow = topLeftCell.row;
	int stCol = topLeftCell.col;
	int edRow = GetRowCount() - stRow;

	if (edRow < m_nDisplayRows)
	{
		CRect clientRect;
		CPoint cellOrigin;
		GetClientRect(&clientRect);
		GetCellOrigin(edRow, 0, &cellOrigin);
		clientRect.top = cellOrigin.y;
		dc.FillSolidRect(clientRect, GetBkColor());
	}
	
	for (int i = 0; i < GetRowCount(); i++)
	{
		for (int j = 0; j < GetColumnCount(); j++)
		{
			CRect cellRect;
			if ((i >= stRow) && (i < stRow + m_nDisplayRows) && (j >= stCol) && (j < stCol + m_nDisplayCols))
				GetCellRect(i, j, &cellRect);
			CGridCellForPreview* pCell = GetPreviewCell(i, j);
			if (pCell != NULL)
				pCell->Draw(&dc, i, j, cellRect, 0);
			else
				dc.FillSolidRect(cellRect, dc.GetBkColor());
		}
	}
}
