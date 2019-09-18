#pragma once
#include "GsPreviewCtrl.h"

enum PREVIEW_LAYOUT_DIR
{
	PREVIEW_LAYOUT_VERTICAL,
	PREVIEW_LAYOUT_HORIZONTAL
};

class CPreviewWithDetail : public CStatic
{
public:
	CPreviewWithDetail();
	virtual ~CPreviewWithDetail();

	void SetText(CString sText) { m_sText = sText; }
	void SetPreviewRect(const CRect& recPreview) { m_recPreview = recPreview; }
	void SetTextRect(const CRect& recText) { m_recText = recText; }
	void SetLayoutMode(PREVIEW_LAYOUT_DIR dir, double rate = 0.5);
	void SetPreview(CString sPath);
	void SetSelected(bool bSelected);
	void DrawText(CDC* pDC);
	void DrawBackGround(CDC* pDC);

	afx_msg void OnPaint();

protected:
	DECLARE_MESSAGE_MAP()

	CGsPreviewCtrl* m_pGsPreviewCtrl;
	CString m_sText;
	CRect m_recPreview;
	CRect m_recText;
	bool m_bSelected;
};
