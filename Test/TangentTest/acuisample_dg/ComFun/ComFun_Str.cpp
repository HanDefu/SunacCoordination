#include "stdafx.h"
#include "ComFun_Str.h"
//#include "ComFun_Math.h"


bool JHCOM_big(double a, double b, double tol)
{
	if(a - b > tol) return true;
	return false;
}

bool JHCOM_sma(double a, double b, double tol)
{
	if(b - a > tol) return true;
	return false;
}
bool JHCOM_equ(double a, double b, double tol)
{
	if(!JHCOM_big(a,b,tol) && !JHCOM_sma(a,b,tol)) return true;
	return false;
}

void CF_STR_get_file_name(const WCHAR *fullname, WCHAR *filename)
{
	WCHAR name[256];
	wcscpy_s(name,fullname);
	WCHAR *p = wcstok(name,L"\\");
	WCHAR *pre = NULL;
	while(p != NULL)
	{
		pre = p;
        p = wcstok(NULL, L"\\");
	}
	wcscpy(filename,pre);
	return;
}

void CF_STR_get_file_name_2(const WCHAR *filename, WCHAR *filename2)
{
	WCHAR name[256];
	wcscpy_s(name,filename);
	WCHAR *p = wcstok(name,L".");
	/*WCHAR *pre = NULL;
	while(p != NULL)
	{
		pre = p;
		p = wcstok(NULL, L"\\");
	}*/
	wcscpy(filename2,p);
	return;
}



CHAR* WCHARTOCHAR(const WCHAR * pchar)
{
	CHAR *m_pchar = NULL;

	int iSize = WideCharToMultiByte(CP_ACP,0,pchar,-1,NULL,0,NULL,NULL);
	//m_pchar = new CHAR[outlen];
	m_pchar = (char*)malloc((iSize+1)/**sizeof(char)*/);
	WideCharToMultiByte(CP_ACP, 0, pchar, -1, m_pchar, iSize, NULL, NULL);

	return m_pchar;
}