#pragma once
#include "malloc.h" 

class Regedit
{
public:
	Regedit(void);
	~Regedit(void);
	static void SetDouble(CString name, double value);
	static double GetDouble(CString name);
	static CString GetString(CString name);
	static int SetString(CString name,CString msb);

	static void SetInt(CString name, int value);
	static int GetInt(CString name);

private:
	static UINT SetValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double mmnumber);
	static UINT QueryValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double& mmnumber);

	static UINT SetValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,int mmnumber);
	static UINT QueryValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,int& mmnumber);

	static UINT QueryString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,CString& kword);
	static UINT SetString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,wchar_t* kword);	
};
