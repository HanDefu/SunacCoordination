#pragma once

//#include "GridCellCheckEx.h"
#include "GridCtrl_src\GridCtrl.h"
#include <vector>

#define WM_GRIDITEM_MOVE (WM_USER + 1003)

using namespace std;

typedef vector<CString> OneRowText;
typedef vector<OneRowText> MultiRowText;

vector<CString> Split(CString cstr, TCHAR key);

class CGridCtrlEx : public CGridCtrl
{
	DECLARE_DYNCREATE(CGridCtrlEx)

public:
	void LoadDefaltSettings();
	void SetDefaultStyle(UINT style);

	//设置表头文本，以;作为分隔符
	void SetHeaderText(CString sText);

	//设置列宽，以;作为分隔符，单位是百分比
	//m-n表示在m%到n%之间自动取值(m,n可省略)，+表示占满剩余客户区
	void SetHeaderWidth(CString sWidth);

	void SetColumnEditabe(CString sCol);
	void SetContentItemEditable(int nRow, int nCol, bool bEditable);
	void SetContentItemText(int nRow, int nCol, CString text);
	void SetContentItemState(int nRow, int nCol, UINT state);
	void SetContentItemFormat(int nRow, int nCol, UINT format);
	void SetContentItemData(int nRow, int nCol, LPARAM nValue);
	void SetSelectedRows(const vector<int> selectedRows);
	void SetContentItemBkColor(int nRow, int nCol, COLORREF color);

	CString GetContentItemText(int nRow, int nCol);
	vector<int> GetSelectedRows();
	vector<int> GetUnselectedRows();
	int GetContentRowCount();
	UINT GetContentItemState(int nRow, int nCol);
	LPARAM GetContentItemData(int nRow, int nCol);

	BOOL DeleteContentRow(int nRow);
	void Clear();

	void FillTable(MultiRowText data);
	//void SetCheckbox(int nRow, int nCol, bool checked);

	void OnBeginDrag();
	BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
protected:
	UINT m_dStyle;
};