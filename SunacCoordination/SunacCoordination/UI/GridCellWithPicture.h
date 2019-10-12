#pragma once
#include "gridctrl_src\gridcell.h"

class CGridCellWithPicture : public CGridCell
{
public:
	DECLARE_DYNCREATE(CGridCellWithPicture)

	CGridCellWithPicture();
	virtual ~CGridCellWithPicture();

	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /* = TRUE */);
	virtual BOOL GetTextRect(LPRECT pRect);
	virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);

	bool LoadImageFromFile(CString p_sFileName);
	
protected:
	CImage m_img;
	CString m_sFileName;
};

