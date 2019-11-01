#pragma once
#include "stdAfx.h"
#include "malloc.h" 

//HKEY_USERS
#define  MAINKEY HKEY_LOCAL_MACHINE 

#define  SUBKEY L"Software\\SunacCAD"
//#define  SUBKEY L"S-1-5-18\\AutoPipe"

class Regedit
{
public:
	Regedit(void);
	~Regedit(void);
	static UINT SetValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double mmnumber);
	static UINT QueryValue(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,double& mmnumber);
	static UINT QueryString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,CString& kword);
	static UINT SetString(HKEY rootkey,LPCTSTR SubKey,wchar_t *Name1,wchar_t* kword);

	static bool GetDoubleManuly(CString name, CString prompt, double& value);
	static void SetDouble(CString name, double value);
	static double GetDouble(CString name);
    static CString GetString(CString name);
	static int SetString(CString name,CString msb);
};
