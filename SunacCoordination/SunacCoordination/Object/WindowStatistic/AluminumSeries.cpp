#include "StdAfx.h"
#include "AluminumSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include "DeductedSize.h"
#include "WindowFormula.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Convert.h"



E_WindowDoorType ToWindowDoorType(CString type)
{
	if (type == L"内开窗" || type == L"内开内倒窗")
	{
		return E_WindowDoor_NC;
	}
	else if (type == L"外开窗" || type == L"上悬窗")
	{
		return E_WindowDoor_WC;
	}
	else if (type == L"推拉窗")
	{
		return E_WindowDoor_TC;
	}
	else if (type == L"外开门" || type == L"门连窗")
	{
		return E_WindowDoor_WM;
	}
	else if (type == L"推拉门")
	{
		return E_WindowDoor_TLM;
	}
	else if (type == L"提升推拉门")
	{
		return E_WindowDoor_TSTLM;
	}
	else
	{
		assert(false);
		return E_WindowDoor_NC;
	}
}

CString WindowDoorTypeToCSting(E_WindowDoorType type)
{
	switch (type)
	{
	case E_WindowDoor_NC:
							return L"内开窗";
							break;
	case E_WindowDoor_WC:
							return L"外开窗";
							break;
	case E_WindowDoor_TC:
							return L"推拉窗";
							break;
	case E_WindowDoor_WM:
							return L"外开门";
							break;
	case E_WindowDoor_TLM:
							 return L"推拉门";
							 break;

	case E_WindowDoor_TSTLM:
							   return L"提升推拉门";
							   break;
	default:
			   return L"内开窗";
			   break;
	}
}


CAluminumSeries* CAluminumSeries::Instance()
{
	static CAluminumSeries instance;
	return &instance;
}

CAluminumSeries::CAluminumSeries()
{
}

CAluminumSeries::~CAluminumSeries()
{
}

E_AluminumType ToE_AluminumType(CString type)
{
	if (type == "断桥隔热铝型材")
	{
		return E_断桥隔热铝型材;
	}
	else if (type == "非断桥隔热铝型材")
	{
		return E_非断桥隔热铝型材;
	}
	else if (type == "阳极氧化铝型材")
	{
		return E_阳极氧化铝型材;
	}
	else return E_断桥隔热铝型材;
}

CString AluminumTypeToCSting(E_AluminumType type)
{
	switch(type)
	{
	case E_断桥隔热铝型材:
		{
			return L"断桥隔热铝型材";
			break;
		}
	case E_非断桥隔热铝型材:
		{
			return L"非断桥隔热铝型材";
			break;
		}
	case E_阳极氧化铝型材:
		{
			return L"阳极氧化铝型材";
			break;
		}
	default:
		{
			return L"断桥隔热铝型材";
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////


CAluminumData AluminumData;
CString AlString;
vector<CString> AlSeries;

void CAluminumSeries::InitAluminum()
{
	AluminumData.sCode = "";
	AluminumData.sName = "";
	AluminumData.sSerial = "";
	AluminumData.weightPerMeter = 0;
	AluminumData.windowDoorType = E_WindowDoor_NC;
	AluminumData.wastageRate = 0.125;
	AluminumData.aluminumType = E_断桥隔热铝型材;
}

static int OutputAlData(void *NotUsed, int nCol, char **value, char **ColName)
{
	for (int i = 0; i < nCol; i++)
	{
		if (strcmp(ColName[i], "WindowDoorType") == 0)
		{
			AluminumData.windowDoorType = ToWindowDoorType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"Code") == 0)
		{
			AluminumData.sCode = value[i];
		}
		else if (strcmp(ColName[i] ,"Name") == 0)
		{
			AluminumData.sName = UTF8ToGBK(value[i]);		
		}
		else if (strcmp(ColName[i] ,"Serial") == 0)
		{
			AluminumData.sSerial = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i] , "WeightPerMeter") == 0)
		{
			AluminumData.weightPerMeter = atof(value[i]);
		}
		else if (strcmp(ColName[i] , "WastageRate") == 0)
		{
			AluminumData.wastageRate = atof(value[i]);
		}
		else if (strcmp(ColName[i], "AluminumType") == 0)
		{
			AluminumData.aluminumType = ToE_AluminumType(UTF8ToGBK(value[i]));
		}
	}
	return 0;
}

static int OutputAlSerial(void *NotUsed, int nCol, char **value, char **ColName)
{
	AlString = UTF8ToGBK(value[0]);
	return 0;
}

static int OutputAlSeriesFromWindowType(void *NotUsed, int nCol, char **value, char **ColName)
{
	AlSeries.push_back(UTF8ToGBK(value[0]));
	return 0;
}

static int OutputAlSeriesFromProtoType(void *NotUsed, int nCol, char **value, char **ColName)
{
	AlSeries.push_back(UTF8ToGBK(value[0]));
	return 0;
}


bool CAluminumSeries::GetAluminumDataBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut)
{
	InitAluminum();
	sqlite3 * pDB3 = NULL;
	const char* path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sWinType = WindowDoorTypeToCSting(p_winType);
	CString sqlString;
	sqlString.Format(L"select * from `AluminumSeries` where `WindowDoorType` = '%s' and `Serial` = '%s' and `Name` = '%s' limit 1;", sWinType, p_serials, sName);
	
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlData , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}

	p_dataOut = AluminumData;
	if (p_dataOut.sCode == "" && p_dataOut.sName == "" && p_dataOut.sSerial == "")
	{
		return false;
	}

	sqlite3_close(pDB3);
	return true;
}

bool CAluminumSeries::GetAluminumSerialByCode(CString p_code, CString& p_serialOut)
{
	sqlite3 * pDB3 = NULL;
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return false;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select Serial from `AluminumSeries` where `Code` = '%s';", p_code);
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlSerial , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return false;
	}

	p_serialOut = AlString;
	sqlite3_close(pDB3);
	return true;
}

vector<CString> CAluminumSeries::GetAluminumSerialsByWindowType(E_WindowDoorType p_winType)
{
	AlSeries.clear();
	sqlite3 * pDB3 = NULL;
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return AlSeries;
	}
	char* cErrMsg;
	CString sWinType = WindowDoorTypeToCSting(p_winType);
	CString sqlString;
	sqlString.Format(L"select distinct Serial  from `AluminumSeries` where `WindowDoorType` = '%s' and `Serial`  !='';", sWinType);
	
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlSeriesFromWindowType , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return AlSeries;
	}

	sqlite3_close(pDB3);
	return AlSeries;
}

vector<CString> CAluminumSeries::GetAluminumSerialsByPrototype(CString p_sProtoTypeCode)
{
	AlSeries.clear();
	sqlite3 * pDB3 = NULL;
	const char * path = TY_GetAluminumDatabasePath();
	int nRes = sqlite3_open(path, &pDB3);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
		return AlSeries;
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select distinct AluminumSerial  from `WindowDoorAluminum` where `PrototypeCode` = '%s';", p_sProtoTypeCode);

	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB3, sql, OutputAlSeriesFromProtoType , 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
		return AlSeries;
	}

	sqlite3_close(pDB3);
	return AlSeries;
}