#include "StdAfx.h"
#include "GridCtrl_src/GridCell.h"
#include "GridCtrl_src/InPlaceEdit.h"
#include "GridCtrl_src/GridCtrl.h"
#include "GridCellWithPicture.h"

IMPLEMENT_DYNCREATE(CGridCellWithPicture, CGridCell)

CGridCellWithPicture::CGridCellWithPicture()
{
}


CGridCellWithPicture::~CGridCellWithPicture()
{
}

BOOL CGridCellWithPicture::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /* = TRUE */)
{
	CRect imageRect(rect), textRect(rect);
	imageRect.DeflateRect(GetMargin(), GetMargin());
	GetTextRect(textRect);
	imageRect.bottom = textRect.top;

	// Note - all through this function we totally brutalise 'rect'. Do not
    // depend on it's value being that which was passed in.

    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0 || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;                             //  though cell is hidden

    //TRACE3("Drawing %scell %d, %d\n", IsFixed()? _T("Fixed ") : _T(""), nRow, nCol);

    int nSavedDC = pDC->SaveDC();
    pDC->SetBkMode(TRANSPARENT);

    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
    CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
    if (!pDefaultCell)
        return FALSE;

    // Set up text and background colours
    COLORREF TextClr, TextBkClr;

    TextClr = (GetTextClr() == CLR_DEFAULT)? pDefaultCell->GetTextClr() : GetTextClr();
    if (GetBackClr() == CLR_DEFAULT)
        TextBkClr = pDefaultCell->GetBackClr();
    else
    {
        bEraseBkgnd = TRUE;
        TextBkClr = GetBackClr();
    }

    // Draw cell background and highlighting (if necessary)
    if ( IsFocused() || IsDropHighlighted() )
    {
        // Always draw even in list mode so that we can tell where the
        // cursor is at.  Use the highlight colors though.
        if(GetState() & GVIS_SELECTED)
        {
            TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
            TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
            bEraseBkgnd = TRUE;
        }

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBkgnd)
        {
            TRY 
            {
                CBrush brush(TextBkClr);
                pDC->FillRect(rect, &brush);
            } 
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }

        // Don't adjust frame rect if no grid lines so that the
        // whole cell is enclosed.
        if(pGrid->GetGridLines() != GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        if (pGrid->GetFrameFocusCell())
        {
                // Use same color as text to outline the cell so that it shows
                // up if the background is black.
            TRY 
            {
                CBrush brush(TextClr);
                pDC->FrameRect(rect, &brush);
            }
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }
        pDC->SetTextColor(TextClr);

        // Adjust rect after frame draw if no grid lines
        if(pGrid->GetGridLines() == GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

		//rect.DeflateRect(0,1,1,1);  - Removed by Yogurt
    }
    else if ((GetState() & GVIS_SELECTED))
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        rect.right--; rect.bottom--;
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    else
    {
        if (bEraseBkgnd)
        {
            rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
        pDC->SetTextColor(TextClr);
    }

    // Draw lines only when wanted
    if (IsFixed() && pGrid->GetGridLines() != GVL_NONE)
    {
        CCellID FocusCell = pGrid->GetFocusCell();

        // As above, always show current location even in list mode so
        // that we know where the cursor is at.
        BOOL bHiliteFixed = pGrid->GetTrackFocusCell() && pGrid->IsValid(FocusCell) &&
                            (FocusCell.row == nRow || FocusCell.col == nCol);

        // If this fixed cell is on the same row/col as the focus cell,
        // highlight it.
        if (bHiliteFixed)
        {
            rect.right++; rect.bottom++;
            pDC->DrawEdge(rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
            rect.DeflateRect(1,1);
        }
        else
        {
            CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
                darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
                *pOldPen = pDC->GetCurrentPen();

            pDC->SelectObject(&lightpen);
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.left, rect.top);
            pDC->LineTo(rect.left, rect.bottom);

            pDC->SelectObject(&darkpen);
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.right, rect.bottom);
            pDC->LineTo(rect.left, rect.bottom);

            pDC->SelectObject(pOldPen);
            rect.DeflateRect(1,1);
        }
    }

    // Draw Text and image
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    if (!pDC->m_bPrinting)
#endif
    {
        CFont *pFont = GetFontObject();
		ASSERT(pFont);
        if (pFont)
            pDC->SelectObject(pFont);
    }

    //rect.DeflateRect(GetMargin(), 0); - changed by Yogurt
    rect.DeflateRect(GetMargin(), GetMargin());    
    rect.right++;    
    rect.bottom++;

	if (!m_img.IsNull())
	{
		pDC->SetStretchBltMode(HALFTONE);
		m_img.Draw(pDC->m_hDC, imageRect);
	}

    // We want to see '&' characters so use DT_NOPREFIX

    DrawText(pDC->m_hDC, GetText(), -1, textRect, GetFormat() | DT_NOPREFIX);

    pDC->RestoreDC(nSavedDC);

    return TRUE;
}

BOOL CGridCellWithPicture::GetTextRect(LPRECT pRect)
{
	CGridCtrl* pGrid = GetGrid();
	if (pGrid == NULL)
		return FALSE;

	CString sText = L"T";
	pRect->left += GetMargin();
	pRect->right -= GetMargin();
	pRect->bottom -= GetMargin();
	CDC* pDC = pGrid->GetDC();
	pRect->top = pRect->bottom - pDC->GetTextExtent(sText).cy - 4;
	pGrid->ReleaseDC(pDC);

	return TRUE;
}

BOOL CGridCellWithPicture::Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
{
	if ( m_bEditing )
	{ 
		if (m_pEditWnd)
		{
			if(nChar < 0x80)
			{
				m_pEditWnd->SendMessage ( WM_CHAR, nChar ); 
			}
			else
			{
				m_pEditWnd->SendMessage (WM_IME_CHAR, nChar);
			}
		}
	}
	else  
	{   
		DWORD dwStyle = ES_LEFT;
		if (GetFormat() & DT_RIGHT) 
			dwStyle = ES_RIGHT;
		else if (GetFormat() & DT_CENTER) 
			dwStyle = ES_CENTER;

		m_bEditing = TRUE;

		// InPlaceEdit auto-deletes itself
		CGridCtrl* pGrid = GetGrid();
		GetTextRect(rect);
		m_pEditWnd = new CInPlaceEdit(pGrid, rect, dwStyle, nID, nRow, nCol, GetText(), nChar);
	}
	return TRUE;
}

bool CGridCellWithPicture::LoadImageFromFile(CString p_sFileName)
{
	if (!m_img.IsNull())
		m_img.Destroy();
	if (m_img.Load(p_sFileName) == S_OK)
	{
		m_sFileName = p_sFileName;
		return true;
	}
	else
		return false;
}
