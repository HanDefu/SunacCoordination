#include "stdafx.h"
#include "ComFun_Str.h"
#include "ComFun_Math.h"

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

CString CF_STR_get_single_font_str_from_str(const WCHAR* instr, const WCHAR* font, double scale1, double scale2)
{
    if (font == NULL)//Ä¬ÈÏ·µ»Ø¿Õ
        return L"";
    if (instr == NULL)
        return L"";
        
    CString str;
    if (wcscmp(font, L"ARIAL") == 0)
    {
        if (JHCOM_equ(scale1, 1.0) == TRUE)
        {
            str.Format(L"%s%s%s", L"{\\fArial|b0|i0|c0|p34;", instr, L"}");
            return str;
        }
        else
        {
            str.Format(L"%s%f%s%s%s%f%s", L"{\\fArial|b0|i0|c0|p34;\\H", scale1, L"x;", instr, L"\\H", scale2, L"x;}");
            return str;
        }
    }
    
    if (wcscmp(font, L"YOUYUAN") == 0)
    {
        if (JHCOM_equ(scale1, 1.0) == TRUE)
        {
            str.Format(L"%s%s%s", L"{\\fYouYuan|b0|i0|c134|p49;", instr, L"}");
                return str;
        }
        else
        {
            str.Format(L"%s%f%s%s%s%f%s", L"{\\fYouYuan|b0|i0|c134|p49;\\H", scale1, L"x;", instr, L"\\H", scale2, L"x;}");
            return str;
        }
    }
    
    if (wcscmp(font, L"SONG") == 0)
    {
        if (JHCOM_equ(scale1, 1.0) == TRUE)
        {
            str.Format(L"%s%s%s", L"{\\fSimSun|b0|i0|c134|p2;", instr, L"}");
                return str;
        }
        else
        {
            str.Format(L"%s%f%s%s%s%f%s", L"{\\fSimSun|b0|i0|c134|p2;\\H", scale1, L"x;", instr, L"\\H", scale2, L"x;}");
            return str;
        }
    }
    
    return L"";
}


void CF_STR_Process_MText_basic_on_chs(const WCHAR *instr,CString &outstr, 
                                       const WCHAR* font, double scale1, double scale2)
{
                                       
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