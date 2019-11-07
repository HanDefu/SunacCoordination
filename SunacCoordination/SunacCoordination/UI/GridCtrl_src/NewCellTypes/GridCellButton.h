#ifndef __GRID_CELL_BUTTON__
#define __GRID_CELL_BUTTON__
#include "../../GridCtrl_src/GridCell.h"  
class CGridCellButton : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CGridCellButton)
public:
	CGridCellButton(void);
	~CGridCellButton(void);
public:
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);
protected:
	virtual void OnClickDown(CPoint PointCellRelative);
	virtual void OnClick(CPoint PointCellRelative);
	virtual void OnMouseOver();
	//virtual void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	CRect m_oriRect;
	CRect m_rect;
	BOOL m_bPushing;
};
#endif