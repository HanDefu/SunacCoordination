#include "stdafx.h"
#include "TreeCtrlEx.h"
 
void CTreeCtrlEx::UpdateMenuIDRange(CMenu* pMenu)
{
	for (int i = 0; i < pMenu->GetMenuItemCount(); i++)
	{
		CMenu* pSubMenu = pMenu->GetSubMenu(i);
		if (pSubMenu == NULL)
		{
			m_StartID = min(m_StartID, pMenu->GetMenuItemID(i));
			m_EndID = max(m_EndID, pMenu->GetMenuItemID(i));
		}
		else
			UpdateMenuIDRange(pSubMenu);
	}
}

// CMyTreeCtrl
 
IMPLEMENT_DYNAMIC(CTreeCtrlEx, CTreeCtrl)
 
CTreeCtrlEx::CTreeCtrlEx()
{
	m_MenuID = 0;
	m_MenuWidth = 150;
	m_StartID = ID_COMMAND_END;
	m_EndID = ID_COMMAND_START;
}

CTreeCtrlEx::~CTreeCtrlEx()
{

} 


BEGIN_MESSAGE_MAP(CTreeCtrlEx, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	ON_COMMAND_RANGE(ID_COMMAND_START, ID_COMMAND_END, OnClickedPopupMenu)
END_MESSAGE_MAP()


// CMyTreeCtrl 消息处理程序


void CTreeCtrlEx::OnPaint()
{
	CPaintDC dc(this);
	GetClientRect(&m_ClientRect); //创建兼容的内存DC和位图，并将位图选入到内存DC
	CBitmap bitmap;
	CDC MemeDc;
	MemeDc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc,m_ClientRect.Width(),m_ClientRect.Height());
	CBitmap* pOldBmp = MemeDc.SelectObject(&bitmap);
	DrawBackGround(&MemeDc); //向兼容的内存DC中绘制背景
	DrawItem(&MemeDc); //将每一项逐个绘制到树控件上
	dc.BitBlt( m_ClientRect.left, m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), &MemeDc, 0, 0,SRCCOPY);
	MemeDc.SelectObject(pOldBmp);
	MemeDc.DeleteDC();
}

void CTreeCtrlEx::DrawBackGround(CDC* pDC)
{
	pDC->FillSolidRect(m_ClientRect,RGB(255,255,255));
}
 
void CTreeCtrlEx::DrawItem(CDC* pDC)
{
	HTREEITEM hCurrentItem;//绘制的当前项句柄
	CRect CurItemRect;//当前项的区域
	int CurItemState;//当前项的状态
 
	hCurrentItem = GetFirstVisibleItem();//获取第一个可见的项,返回它的句柄值
	do 
	{
		if (GetItemRect(hCurrentItem,CurItemRect,TRUE))
		{
			CRect fillRect(0,CurItemRect.top,m_ClientRect.right,CurItemRect.bottom);
 
			CurItemState = GetItemState(hCurrentItem,TVIF_STATE);
 
			//当前正绘制的项已超出窗口的边界，所以不绘制，并退出绘制
			if (CurItemRect.bottom > m_ClientRect.bottom)  
			{
				break;
			}
			//绘制鼠标热点
			if (hCurrentItem == m_CurMouseItem)
			{
				pDC->FillSolidRect(&fillRect, RGB(220,220,255));
			}
			//绘制选中状态
			if(CurItemState & TVIS_SELECTED)
			{
				pDC->FillSolidRect(&fillRect, GetSysColor(COLOR_MENUHILIGHT));
			}
			//绘制分割线
			if (GetParentItem(hCurrentItem) == NULL)
			{
				CPoint ptTopLeft = fillRect.TopLeft();
				CPoint ptTopRight(fillRect.right, fillRect.top);
				CPoint ptBottomLeft(fillRect.left, fillRect.bottom);
				CPoint ptBottomRight = fillRect.BottomRight();

				pDC->MoveTo(ptTopLeft);
				pDC->LineTo(ptTopRight);
				pDC->MoveTo(ptBottomLeft);
				pDC->LineTo(ptBottomRight);
			}
			//绘制文字
			CurItemRect.right = m_ClientRect.right;
 
			DrawItemText(pDC, hCurrentItem, CurItemRect);
		}
	} while ((hCurrentItem = GetNextVisibleItem(hCurrentItem)) != NULL);
}

void CTreeCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM hItem = HitTest(point);//判断鼠标在控件的哪一项上面
 
	if (hItem != NULL && m_CurMouseItem != hItem)
	{
		m_CurMouseItem = hItem;
		Invalidate(FALSE);
	}
 
	CTreeCtrl::OnMouseMove(nFlags, point);
}
 
void CTreeCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	HTREEITEM hSel = HitTest(point);
	CString sText;

	if (GetParentItem(hSel) == NULL)
	{
		//一级节点设置选中状态
		SelectItem(hSel);
		//收起其它项
		for (HTREEITEM i = GetFirstVisibleItem(); i != NULL; i = GetNextVisibleItem(i))
		{
			if (hSel != i)
				Expand(i, TVE_COLLAPSE);
		}
		Expand(hSel, TVE_TOGGLE);
		Invalidate(FALSE);
	}
	else
	{
		//二级节点设置高亮
		m_CurMouseItem = hSel;
		Invalidate(FALSE);

		//弹出菜单的位置与当前节点对齐
		if (m_MenuID > 0)
		{
			CRect rect;
			GetItemRect(hSel, &rect, FALSE);
			CPoint menuPos(rect.right + 1, rect.top);
			ClientToScreen(&menuPos);

			CMenu menu;
			menu.LoadMenu(m_MenuID);
			//查找对应的菜单并显示
			for (int i = 0; i < menu.GetMenuItemCount(); i++)
			{
				menu.GetMenuStringW(i, sText, MF_BYPOSITION);
				if (sText == GetItemText(hSel))
				{
					CMenu *pMenu = menu.GetSubMenu(i);
					if (pMenu == NULL)
						break;
					UINT menuID = pMenu->GetMenuItemID(0);
					for (int j = 0; j < pMenu->GetMenuItemCount(); j++)
					{
						pMenu->ModifyMenu(menuID + j, MF_BYCOMMAND | MF_OWNERDRAW, menuID + j);
					}
					pMenu->TrackPopupMenu(TPM_LEFTALIGN, menuPos.x, menuPos.y, this);
					break;
				}
			}
		}
	}
}

void CTreeCtrlEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = GetItemHeight();
	lpMeasureItemStruct->itemWidth = m_MenuWidth;
}

void CTreeCtrlEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType == ODT_MENU)
	{
		CBrush *brush = new CBrush;
		CPen *pen = new CPen;
		CString strText;
		CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC); //获取菜单项的设备句柄
		//菜单项是否为选中状态
		if ((lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			//在菜单项上自绘矩形框的背景颜色
			brush->CreateSolidBrush(RGB(182, 189,210));
			//在菜单项自绘矩形的边框颜色
			pen->CreatePen(PS_SOLID, 1, RGB(10,36,106));
			//设置菜单项的文字背景颜色
			pDC->SetBkColor(RGB(182,189,210));
		}
		else
		{
			brush->CreateSolidBrush(GetSysColor(COLOR_MENU));
			pen->CreatePen(PS_SOLID, 0, GetSysColor(COLOR_MENU));
			pDC->SetBkColor(GetSysColor(COLOR_MENU));
		}
		pDC->SelectObject(pen);
		pDC->SelectObject(brush);
		//在当前菜单项上画一个矩形框
		pDC->Rectangle(lpDrawItemStruct->rcItem.left,
			lpDrawItemStruct->rcItem.top,
			lpDrawItemStruct->rcItem.right,
			lpDrawItemStruct->rcItem.bottom);
		/*--------------------------------------*/
		//获取当前消息所在菜单项的文本
		CMenu menu;
		menu.Attach((HMENU)lpDrawItemStruct->hwndItem);
		menu.GetMenuStringW(lpDrawItemStruct->itemID, strText,MF_BYCOMMAND);
		/*--------------------------------------*/
		//如果为菜单发出的DrawItem消息
		if (nIDCtl == 0)
		{
			//在菜单项上输出菜单文本
			CFont Font;
			Font.CreatePointFont(105,_T("Calibri"));//创建字体
			pDC->SelectObject(&Font);
			CRect rectText = lpDrawItemStruct->rcItem;
			rectText.left += 20;
			pDC->DrawText(strText, &rectText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
		menu.Detach();
		delete brush;
		delete pen;
	}
}

void CTreeCtrlEx::OnClickedPopupMenu(UINT nID)
{
	if (nID > m_EndID || nID < m_StartID)
		return;
	GetParent()->SendMessage(WM_MENU_CLICKED, nID, 0);
}

void CTreeCtrlEx::DrawItemText(CDC * pDC,HTREEITEM pItem,CRect Rect)
{
	CFont Font;
	Font.CreatePointFont(105,_T("Calibri"));//创建字体
	pDC->SelectObject(&Font);
	CString ItemText = GetItemText(pItem);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(ItemText, Rect, DT_SINGLELINE|DT_LEFT|DT_VCENTER);//显示项文本
}

void CTreeCtrlEx::SetMenuID(UINT menuID)
{
	CMenu menu;
	menu.LoadMenu(menuID);
	UpdateMenuIDRange(&menu);
	m_MenuID = menuID;
}
