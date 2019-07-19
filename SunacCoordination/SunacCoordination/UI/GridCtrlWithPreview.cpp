#include "stdafx.h"
#include "GridCtrlWithPreview.h"


CGridCtrlWithPreview::CGridCtrlWithPreview() : m_nVPos(0), m_nHPos(0), m_nMargin(0)
{
}


CGridCtrlWithPreview::~CGridCtrlWithPreview()
{
	for (UINT i = 0; i < m_pPreviews.size(); i++)
		for (UINT j = 0; j < m_pPreviews[i].size(); j++)
			if (m_pPreviews[i][j] != NULL)
				delete m_pPreviews[i][j];
}

void CGridCtrlWithPreview::LoadDefaltSettings()
{
	EnableDragAndDrop(FALSE);
	SetEditable(FALSE);
	EnableTitleTips(FALSE);
	m_dStyle = DT_LEFT;
	SetGridLines(0);
	SetMargin(10);
}

BEGIN_MESSAGE_MAP(CGridCtrlWithPreview, CGridCtrlEx)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_PREVIEW_SELECTED, &CGridCtrlWithPreview::OnPreviewSelected)
END_MESSAGE_MAP()

int CGridCtrlWithPreview::SetRowCount(int nRows)
{
	m_pPreviews.resize(nRows);
	return CGridCtrlEx::SetRowCount(nRows);
}

int CGridCtrlWithPreview::SetColumnCount(int nCols)
{
	for (UINT i = 0; i < m_pPreviews.size(); i++)
		m_pPreviews[i].resize(nCols);
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

void CGridCtrlWithPreview::SetMargin(int nMargin)
{
	SetDefCellMargin(nMargin);
	m_nMargin = nMargin;
}

bool CGridCtrlWithPreview::AddPreview(int nRow, int nCol, CString sPath)
{
	if (nRow >= m_pPreviews.size() || nCol >= m_pPreviews[nRow].size())
		return false;
	if (m_pPreviews[nRow][nCol] != NULL)
		return false;

	CRect gridRect;
	GetCellRect(0, 0, gridRect);
	gridRect.DeflateRect(m_nMargin, m_nMargin);
	CGsPreviewCtrl* pPreview = new CGsPreviewCtrl;
	pPreview->Create(_T(""), WS_CHILD, gridRect, this);
	pPreview->Init(theArxDLL.ModuleResourceInstance(), true);
	pPreview->SetDwgFile(sPath);
	m_pPreviews[nRow][nCol] = pPreview;

	return true;
}

void CGridCtrlWithPreview::ShowPreviews()
{
	if (m_nVPos >= m_pPreviews.size() || m_nHPos >= m_pPreviews[m_nVPos].size())
		return;

	for (UINT i = 0; i < m_pPreviews.size(); i++)
		for (UINT j = 0; j < m_pPreviews[i].size(); j++)
			if (m_pPreviews[i][j] != NULL)
			m_pPreviews[i][j]->ShowWindow(SW_HIDE);

	CRect gridRect;
	for (int i = m_nVPos; i < min(m_nDisplayRows + m_nVPos, GetRowCount()); i++)
		for (int j = m_nHPos; j < min(m_nDisplayCols + m_nHPos, GetColumnCount()); j++)
		{
			if (m_pPreviews[i][j] == NULL)
				continue;
			GetCellRect(i, j, gridRect);
			gridRect.DeflateRect(m_nMargin, m_nMargin);
			m_pPreviews[i][j]->MoveWindow(gridRect, FALSE);
			m_pPreviews[i][j]->ShowWindow(SW_SHOW);
		}

	Invalidate();
}

void CGridCtrlWithPreview::UpdatePreviews()
{
	CRect gridRect;
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			GetCellRect(i, j, gridRect);
			if (!gridRect.IsRectEmpty())
			{
				if (i != m_nVPos || j != m_nHPos)
				{
					m_nVPos = i;
					m_nHPos = j;
					ShowPreviews();
				}
				return;
			}
		}
}

void CGridCtrlWithPreview::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CGridCtrlEx::OnVScroll(nSBCode, nPos, pScrollBar);
	UpdatePreviews();
}

void CGridCtrlWithPreview::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CGridCtrlEx::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdatePreviews();
}

LRESULT CGridCtrlWithPreview::OnPreviewSelected(WPARAM wParam,LPARAM lParam)
{
	CPoint point((int)wParam, (int)lParam);
	ScreenToClient(&point);
	CCellID cellID = GetCellFromPt(point);
	
	UINT state;
	for (int i = 0; i < GetRowCount(); i++)
		for (int j = 0; j < GetColumnCount(); j++)
		{
			state = GetContentItemState(i, j);
			SetContentItemState(i, j, state & ~GVIS_SELECTED & ~GVIS_FOCUSED);
		}

	state = GetContentItemState(cellID.row, cellID.col);
	SetContentItemState(cellID.row, cellID.col, state | GVIS_SELECTED | GVIS_FOCUSED);

	return 0;
}
