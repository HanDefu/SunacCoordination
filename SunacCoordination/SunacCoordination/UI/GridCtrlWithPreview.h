#pragma once
#include "GridCtrlEx.h"
#include "GridCellForPreview.h"

class CGridCtrlWithPreview : public CGridCtrlEx
{
public:
	CGridCtrlWithPreview();
	virtual ~CGridCtrlWithPreview();
	void LoadDefaltSettings();
	void SetDisplayRows(int nRows); //设置显示区域内的行数
	void SetDisplayColumns(int nCols); //设置显示区域内的列数

	bool AddPreview(int nRow, int nCol, CString sPath, CString sText, PREVIEW_LAYOUT_DIR dir = PREVIEW_LAYOUT_HORIZONTAL); //绑定dwg预览图到指定单元格

	CGridCellForPreview* AddPreviewPng(int nRow, int nCol, CString sPngPath, CString sText, PREVIEW_LAYOUT_DIR dir = PREVIEW_LAYOUT_HORIZONTAL); //绑定PNG预览图到指定单元格


	void ClearAllPreviews();
	void SelectPreview(int nRow, int nCol);
	CGridCellForPreview* GetPreviewCell(int nRow, int nCol);

	afx_msg void OnPaint();
protected:
	DECLARE_MESSAGE_MAP()

	int m_nDisplayRows;
	int m_nDisplayCols;
};

