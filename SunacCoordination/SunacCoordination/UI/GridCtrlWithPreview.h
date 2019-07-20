#pragma once
#include "GridCtrlEx.h"
#include "GsPreviewCtrl.h"

class CGridCtrlWithPreview : public CGridCtrlEx
{
public:
	CGridCtrlWithPreview();
	~CGridCtrlWithPreview();

	void LoadDefaltSettings();
	int SetRowCount(int nRows);
	int SetColumnCount(int nCols);
	void SetDisplayRows(int nRows); //设置显示区域内的行数
	void SetDisplayColumns(int nCols); //设置显示区域内的列数
	void SetMargin(int nMargin); //设置边距

	bool AddPreview(int nRow, int nCol, CString sPath); //绑定dwg预览图到指定单元格
	bool ClearAllPreviews();
	void ShowPreviews(); //显示预览图
	void UpdatePreviews(); //滚动条变化时刷新预览图

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnPreviewSelected(WPARAM wParam,LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()

	vector<vector<CGsPreviewCtrl*>> m_pPreviews;
	int m_nDisplayRows;
	int m_nDisplayCols;
	int m_nHPos; //记录水平滚动条位置
	int m_nVPos; //记录垂直滚动条位置
	int m_nMargin;
};

