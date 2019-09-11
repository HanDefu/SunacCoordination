#pragma once

// CMyTreeCtrl

#define WM_MENU_CLICKED (WM_USER + 1002)

#define ID_COMMAND_START 40000
#define ID_COMMAND_END 49999

class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)
public:
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point); //鼠标移至树控件时高亮
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); //单击左键展开
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct); //设置菜单项的高度和宽度
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct); //绘制菜单项
	afx_msg void OnClickedPopupMenu(UINT nID);

	void DrawBackGround(CDC* pDC);                   //绘制背景
	void DrawItem(CDC* pDC);                         //绘制树控件中的每一项
	void DrawItemText(CDC * pDC,HTREEITEM pItem,CRect Rect);//绘制树控件每一项的文本

	void SetMenuID(UINT menuID);
	void SetMenuWidth(UINT menuWidth) { m_MenuWidth = menuWidth; }
	void UpdateMenuIDRange(CMenu* pMenu);

public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

protected:
	DECLARE_MESSAGE_MAP()

	HTREEITEM m_CurMouseItem;                       //当前鼠标移动到的节点句柄
	CRect m_ClientRect;                             //树控件客户端大小
	UINT m_MenuID;									//绑定的弹出菜单资源ID
	UINT m_MenuWidth;								//弹出菜单的宽度，默认为150
	UINT m_StartID;									//绑定菜单的起始ID
	UINT m_EndID;									//绑定菜单的终止ID
};
