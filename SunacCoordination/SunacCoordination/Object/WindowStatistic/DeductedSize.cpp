#include "StdAfx.h"
#include "DeductedSize.h"
#include "WindowFormula.h"
#include "AluminumSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"


CDeductedSize* CDeductedSize::Instance()
{
	static CDeductedSize instance;
	return &instance;
}
CDeductedSize::CDeductedSize()
{
}

CDeductedSize::~CDeductedSize()
{
}

sqlite3 * pDB2 = NULL;
double data = 0;

size_t ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB )
{
	size_t len=_tcslen(strIn);

#ifdef UNICODE
	int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, strIn, -1, 0, 0, 0, 0);

	strOutUTF8MB=new char[iRequiredSize];
	strOutUTF8MB[0]=0;

	WideCharToMultiByte(CP_UTF8, 0, strIn, -1, strOutUTF8MB, iRequiredSize, 0, 0);
#else
	WCHAR * wChar=new WCHAR[len+1];
	wChar[0]=0;
	MultiByteToWideChar(CP_ACP, 0, strIn, (int)len+1, wChar, (int)len+1);
	int iRequiredSize=WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, 0, 0, 0, 0);
	strOutUTF8MB=new char[iRequiredSize];
	strOutUTF8MB[0]=0;
	WideCharToMultiByte(CP_UTF8, 0, wChar, (int)len+1, strOutUTF8MB, iRequiredSize, 0, 0);
	delete [] wChar;
#endif

	return iRequiredSize;
}

static int OutputDeductedSize(void *NotUsed, int nCol, char **value, char **ColName)
{
	data = atof(value[0]);
	return 0;
}

bool CDeductedSize::GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut)
{
	int nRes = sqlite3_open("C:/Program Files/Program Files/Autodesk/AutoCAD 2014/Support/Sunac2019/RCData.db", &pDB2);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sqlString;
	CString sWinType = WindowTypeToCString(p_winType);
	
	USES_CONVERSION;
	sqlString.Format(L"select %s from `DeductedSize` where `Type` = '%s' and `ProfileSeries` =  '%s';", sDeductedName, sWinType, p_serials);
	char* sql = T2A(sqlString);
	ConvertStringToUTF8(sqlString, sql);
	//char* convert = T2A(GBKToUTF8(sql));
	int res = sqlite3_exec(pDB2, sql, OutputDeductedSize , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}
	p_valueOut = data;

	return true;
}


bool CDeductedSize::GetDeductedSizeBySeriesAndName(CString p_winType, CString p_serials, CString sDeductedName, double &p_valueOut)
{
	int nRes = sqlite3_open("C:/Program Files/Autodesk/AutoCAD 2014/Support/Sunac2019/RCData.db", &pDB2);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sqlString;

	USES_CONVERSION;
	sqlString.Format(L"select %s from `DeductedSize` where `Type` = '%s' and `ProfileSeries` = '%s';", sDeductedName, p_winType, p_serials);
	char* sql = T2A(sqlString);
	ConvertStringToUTF8(sqlString, sql);
	//char* convert = T2A(GBKToUTF8(sql));
	int res = sqlite3_exec(pDB2, sql, OutputDeductedSize , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}
	p_valueOut = data;

	return true;
}