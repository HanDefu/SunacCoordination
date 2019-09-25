#include "StdAfx.h"
#include "GridCellForPreview.h"

IMPLEMENT_DYNCREATE(CGridCellForPreview, CGridCell)

CGridCellForPreview::CGridCellForPreview() : CGridCell()
{
	m_pPreview = NULL;
}

CGridCellForPreview::~CGridCellForPreview()
{
	DeletePreview();
}

BOOL CGridCellForPreview::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /*= TRUE*/)
{
	if (m_pPreview == NULL)
		return FALSE;

	if (rect.IsRectEmpty())
		m_pPreview->ShowWindow(SW_HIDE);
	else
	{
		m_pPreview->MoveWindow(rect, FALSE);
		m_pPreview->SetSelected(IsSelected() != FALSE);
		m_pPreview->ShowWindow(SW_SHOW);
	}

	return TRUE;
}

void CGridCellForPreview::SetPreview(CPreviewWithDetail* preview)
{
	if (m_pPreview != NULL)
		return;
	m_pPreview = preview;
}

void CGridCellForPreview::DeletePreview()
{
	delete m_pPreview;
	m_pPreview = NULL;
}
