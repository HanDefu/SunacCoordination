#pragma once

WCHAR* CharToWchar(const char * pchar);
CHAR* WCHARTOCHAR(const WCHAR * pchar);

void CF_STR_get_file_name(const WCHAR *fullname, WCHAR *filename);
void CF_STR_get_file_name_2(const WCHAR *filename, WCHAR *filename2);

//0623目前关于字体大小变化的问题还没搞清楚，0623暂时请注意, 两个scale请带小数点
CString CF_STR_get_single_font_str_from_str(const WCHAR* instr = NULL, const WCHAR* font = NULL, double scale1 = 1.0, double scale2 = 1.0);
void CF_STR_Process_MText_basic_on_chs(const WCHAR *instr,CString &outstr, 
                                       const WCHAR* font = L"ARIAL", double scale1 = 1, double scale2 = 1);