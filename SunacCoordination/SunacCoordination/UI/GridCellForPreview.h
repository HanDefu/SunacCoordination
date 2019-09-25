#pragma once
#include "GridCtrl_src/GridCell.h"
#include "PreviewWithDetail.h"

class CGridCellForPreview : public CGridCell
{
	DECLARE_DYNCREATE(CGridCellForPreview)

public:
	CGridCellForPreview();
	virtual ~CGridCellForPreview();
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
	void SetPreview(CPreviewWithDetail* preview);
	void DeletePreview();

protected:
	CPreviewWithDetail* m_pPreview;
};