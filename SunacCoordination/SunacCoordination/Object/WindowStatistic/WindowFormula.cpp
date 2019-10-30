#include "StdAfx.h"
#include "WindowFormula.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include <afxwin.h>
#include <string>
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Convert.h"
#include "AluminumSeries.h"
#include "DeductedSize.h"

CAluminumFormula::CAluminumFormula()
{
	m_nCount = 1;
}

CAluminumFormula::~CAluminumFormula()
{
}

CGlassFormula::CGlassFormula()
{
	m_nCount = 1;
}

CGlassFormula::~CGlassFormula()
{
}
CHardwareData::CHardwareData()
{
	m_nCount = 1;
}

CHardwareData::~CHardwareData()
{
}


//////////////////////////////////////////////////////////////////////////

CWindowFormula* CWindowFormula::Instance()
{
	static CWindowFormula instance;
	return &instance;
}

CWindowFormula::CWindowFormula()
{
}

CWindowFormula::~CWindowFormula()
{
}

vector<CAluminumFormula> vAluminumFormulas;
vector<CGlassFormula> vGlassFormulas;
vector<CHardwareData> vHardwareFormulas;

static int OutputAlFormula(void *NotUsed, int nCol, char **value, char **ColName)
{
	CAluminumFormula AluminumFormula;
	for(int i = 0 ; i < nCol ; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			AluminumFormula.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i],"AluminumClassify") == 0)
		{
			AluminumFormula.m_aluminumClassify = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] ,"Name") == 0)
		{
			AluminumFormula.m_name = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] ,"Position") == 0)
		{
			AluminumFormula.m_pos = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] , "Formula") == 0)
		{
			AluminumFormula.m_formula = value[i];
		}
		else if (strcmp(ColName[i], "Count") == 0)
		{
			AluminumFormula.m_nCount = atoi(value[i]);
		}
	}
	vAluminumFormulas.push_back(AluminumFormula);
	return 0;
}

vector<CAluminumFormula> CWindowFormula::GetAluminumFormulas(CString p_sPrototypeCode, CString p_sAluminumSeries)
{
	sqlite3 * pDB = NULL;
	vAluminumFormulas.clear();
	char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB);
	
	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return vAluminumFormulas;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select * from `AluminumFormulas` where `PrototypeCode` = '%s' and `AluminumSeries` = '%s';", p_sPrototypeCode, p_sAluminumSeries);
	USES_CONVERSION;
	char* sql = W2A(sqlString);
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB, sql, OutputAlFormula , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return vAluminumFormulas;
	}

	sqlite3_close(pDB);
	return vAluminumFormulas;
}

static int OutputGlassFormula(void *NotUsed, int nCol, char **value, char **ColName)
{
	CGlassFormula GlassFormula;
	for(int i = 0 ; i < nCol ; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			GlassFormula.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i],"GlassClassify") == 0)
		{
			GlassFormula.m_glassClassify = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] ,"GlassName") == 0)
		{
			GlassFormula.m_name = value[i];
		}
		else if (strcmp(ColName[i] ,"WidthFormula") == 0)
		{
			GlassFormula.m_widthFormula = value[i];
		}
		else if (strcmp(ColName[i] , "HeightFormula") == 0)
		{
			GlassFormula.m_heightFormula = value[i];
		}
		else if (strcmp(ColName[i], "Count") == 0)
		{
			GlassFormula.m_nCount = atoi(value[i]);
		}
	}
	vGlassFormulas.push_back(GlassFormula);
	return 0;
}

vector<CGlassFormula> CWindowFormula::GetGlassFormulas(CString p_sPrototypeCode)
{
	sqlite3 * pDB = NULL;
	vGlassFormulas.clear();
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return vGlassFormulas;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select * from `GlassFormulas` where `PrototypeCode` = '%s';", p_sPrototypeCode);
	USES_CONVERSION;
	char* sql = T2A(sqlString);
	int res = sqlite3_exec(pDB, sql, OutputGlassFormula , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return vGlassFormulas;
	}

	sqlite3_close(pDB);
	return vGlassFormulas;
}

static int OutputHardwareFormula(void *NotUsed, int nCol, char **value, char **ColName)
{
	CHardwareData HardwareData;
	for(int i = 0 ; i < nCol ; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			HardwareData.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i] ,"Name") == 0)
		{
			HardwareData.m_name = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i], "Count") == 0)
		{
			HardwareData.m_nCount = atoi(value[i]);
		}
	}
	vHardwareFormulas.push_back(HardwareData);
	return 0;
}

vector<CHardwareData> CWindowFormula::GetHardwareData(CString p_sPrototypeCode)
{
	sqlite3 * pDB = NULL;
	vHardwareFormulas.clear();
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return vHardwareFormulas;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select * from `HardwareData` where `PrototypeCode` = '%s';", p_sPrototypeCode);
	USES_CONVERSION;
	char* sql = T2A(sqlString);
	int res = sqlite3_exec(pDB, sql, OutputHardwareFormula , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return vHardwareFormulas;
	}

	sqlite3_close(pDB);
	return vHardwareFormulas;
}