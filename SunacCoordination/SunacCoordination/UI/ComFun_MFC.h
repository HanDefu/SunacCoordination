#pragma once

void TYUI_SetText(CWnd& wnd, CString sText);
void TYUI_SetInt(CWnd& wnd, int nVal);
void TYUI_SetDouble(CWnd& wnd, double dVal);

CString TYUI_GetText(const CWnd& wnd);
int TYUI_GetInt(const CWnd& wnd);
double TYUI_GetDouble(const CWnd& wnd);

void TYUI_Show(CWnd& wnd);
void TYUI_Hide(CWnd& wnd);
void TYUI_Enable(CWnd& wnd);
void TYUI_Disable(CWnd& wnd);

void TYUI_InitComboBox(CComboBox& comboBox, vCString options, CString defaultValue);
void TYUI_InitComboBox(CComboBox& comboBox, vint options, int defaultValue);
void TYUI_InitComboBox(CComboBox& comboBox, vdouble options, double defaultValue);