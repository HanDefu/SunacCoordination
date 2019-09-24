#include "stdafx.h"
#include "ComFun_MFC.h"

void TYUI_SetText(CWnd& wnd, CString sText)
{
	wnd.SetWindowText(sText);
}

void TYUI_SetInt(CWnd& wnd, int nVal)
{
	CString sText;
	sText.Format(_T("%d"), nVal);
	TYUI_SetText(wnd, sText);
}

void TYUI_SetDouble(CWnd& wnd, double dVal)
{
	CString sText;
	sText.Format(_T("%.2lf"), dVal);
	TYUI_SetText(wnd, sText);
}

CString TYUI_GetText(const CWnd& wnd)
{
	CString str;
	wnd.GetWindowText(str);
	return str;
}

int TYUI_GetInt(const CWnd& wnd)
{
	CString str = TYUI_GetText(wnd);
	return _ttoi(str);
}

double TYUI_GetDouble(const CWnd& wnd)
{
	CString str = TYUI_GetText(wnd);
	return _ttof(str);
}

void TYUI_Show(CWnd& wnd)
{
	wnd.ShowWindow(SW_SHOW);
}

void TYUI_Hide(CWnd& wnd)
{
	wnd.ShowWindow(SW_HIDE);
}

void TYUI_Enable(CWnd& wnd)
{
	wnd.EnableWindow(TRUE);
}

void TYUI_Disable(CWnd& wnd)
{
	wnd.EnableWindow(FALSE);
}

void TYUI_InitComboBox(CComboBox& comboBox, vCString options, CString defaultValue)
{
	comboBox.ResetContent();
	for (UINT i = 0; i < options.size(); i++)
	{
		comboBox.InsertString(i, options[i]);
		if (defaultValue == options[i])
			comboBox.SetCurSel(i);
	}
}

void TYUI_InitComboBox(CComboBox& comboBox, vint options, int defaultValue)
{
	vCString vstr(options.size());
	CString str;

	for (UINT i = 0; i < options.size(); i++)
	{
		vstr[i].Format(_T("%d"), options[i]);
		if (defaultValue == options[i])
			str = vstr[i];
	}
	TYUI_InitComboBox(comboBox, vstr, str);
}

void TYUI_InitComboBox(CComboBox& comboBox, vdouble options, double defaultValue)
{
	vCString vstr(options.size());
	CString str;

	for (UINT i = 0; i < options.size(); i++)
	{
		vstr[i].Format(_T("%.2lf"), options[i]);
		if (defaultValue == options[i])
			str = vstr[i];
	}
	TYUI_InitComboBox(comboBox, vstr, str);
}

CString TYUI_GetComboBoxText(CComboBox& comboBox)
{
	CString str;
	int sel = comboBox.GetCurSel();
	comboBox.GetLBText(sel, str);
	return str;
}

