#include "StdAfx.h"
#include "string"
#include "afx.h"
#include "WindowLocalDataFromDB.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"
#include "..\Common\ComFun_Convert.h"

CWindowLocalDataFromDB* CWindowLocalDataFromDB::Instance()
{
	static CWindowLocalDataFromDB instance;
	return &instance;
}

CWindowLocalDataFromDB::CWindowLocalDataFromDB()
{
	LoadDataFromDataBase();
}

CWindowLocalDataFromDB::~CWindowLocalDataFromDB()
{
}

vAttrWindow vAttrWindowData;

CWindowsDimData CWindowLocalDataFromDB::GetWindowDimData(CString code, char **value, int nStart)
{
	CString  valueType = UTF8ToGBK(value[nStart]);
	CString sValue = value[nStart+1];
	double minValue =  atof(value[nStart+2]);
	double maxValue =  atof(value[nStart+3]);
	double defaultValue = atof(value[nStart+4]);
	CString sComment = UTF8ToGBK(value[nStart+5]);


	CWindowsDimData  data;
	data.sCodeName = code;
	data.defaultValue = defaultValue;
	data.prompt = sComment;

	if (valueType == "无")
	{
		data.type = NOVALUE;
	}
	else if (valueType == L"范围")
	{
		data.type = SCOPE;
		data.minValue = minValue;
		data.maxValue = maxValue;
	}
	else if (valueType == L"公式")
	{
		data.type = CALC;
		data.sFomula = sValue;
	}
	else if (valueType == L"固定值")
	{
		data.type = SINGLE;
		data.valueOptions.push_back(_wtof(sValue));
	}
	else if (valueType == L"值系列")
	{
		data.type = MULTI;

		std::vector<CString> strs = YT_SplitCString(sValue, L',');
		for (UINT i = 0; i < strs.size(); i++)
		{
			data.valueOptions.push_back(_wtof(strs[i]));
		}
	}

	return data;
}

int CWindowLocalDataFromDB::OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName)
{
	AttrWindow TempData;

	const CWindowsDimData TempDimDataW = GetWindowDimData(_T("W"), value, 12);
	const CWindowsDimData TempDimDataW1 = GetWindowDimData(_T("W1"), value, 18);
	const CWindowsDimData TempDimDataW2 = GetWindowDimData(_T("W2"), value, 24);
	const CWindowsDimData TempDimDataW3 = GetWindowDimData(_T("W3"), value, 30);
	const CWindowsDimData TempDimDataH = GetWindowDimData(_T("H"), value, 36);
	const CWindowsDimData TempDimDataH1= GetWindowDimData(_T("H1"), value, 42);
	const CWindowsDimData TempDimDataH2= GetWindowDimData(_T("H2"), value, 48);
	const CWindowsDimData TempDimDataH3= GetWindowDimData(_T("H3"), value, 54);
	

	//TempDimDataW.sCodeName = L"W";
	//TempDimDataW1.sCodeName = L"W1";
	//TempDimDataW2.sCodeName = L"W2";
	//TempDimDataW3.sCodeName = L"W3";
	//TempDimDataH.sCodeName = L"H";
	//TempDimDataH1.sCodeName = L"H1";
	//TempDimDataH2.sCodeName = L"H2";
	//TempDimDataH3.sCodeName = L"H3";

	for (int i = 0; i < 12; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			TempData.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i],"PrototypeFile") == 0)
		{
			TempData.m_file.fileName = value[i];
		}
		else if (strcmp(ColName[i],"FrontViewFile") == 0)
		{
			TempData.m_frontViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"TopViewFile") == 0)
		{
			TempData.m_topViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"LeftViewFile") == 0)
		{
			TempData.m_leftViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"Scope") == 0)
		{
			if (UTF8ToGBK(value[i]) == L"全部")
			{

				TempData.m_isJiTuan = true;
			}
			else TempData.m_isJiTuan = false;
		}
		else if (strcmp(ColName[i],"IsDynamic") == 0)
		{
			if (UTF8ToGBK(value[i]) == L"是")
			{
				TempData.m_isDynamic = true;
			}
			else TempData.m_isDynamic = false;
		}
		else if (strcmp(ColName[i],"FunctionalType") == 0)
		{
			TempData.m_gongNengquType = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"OpenType") == 0)
		{
			TempData.m_openType = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"OpenQty") == 0)
		{
			TempData.m_openQty = atoi(value[i]);
		}
		else if (strcmp(ColName[i],"VentilationFormula") == 0)
		{
			TempData.m_tongFengFormula = value[i];
		}
		else if (strcmp(ColName[i],"Ventilation") == 0)
		{
			TempData.m_tongFengQty = atoi(value[i]);
		}/*
		else if (strcmp(ColName[i],"WValueType") == 0)
		{
			TempDimDataW.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"WValue") == 0)
		{
			TempDimDataW.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"WMinValue") == 0)
		{
			TempDimDataW.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"WMaxValue") == 0)
		{
			TempDimDataW.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"WDefaultValue") == 0)
		{
			TempDimDataW.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"WValueCommitment") == 0)
		{
			TempDimDataW.prompt = value[i];
		}

		else if (strcmp(ColName[i],"HValueType") == 0)
		{
			TempDimDataH.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"HValue") == 0)
		{
			TempDimDataH.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"HMinValue") == 0)
		{
			TempDimDataH.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"HMaxValue") == 0)
		{
			TempDimDataH.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"HDefaultValue") == 0)
		{
			TempDimDataH.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"HValueCommitment") == 0)
		{
			TempDimDataH.prompt = value[i];
		}

		else if (strcmp(ColName[i],"W1ValueType") == 0)
		{
			TempDimDataW1.type = ToEWindowType(value[i]);
		}
		else if (strcmp(ColName[i],"W1Value") == 0)
		{
			TempDimDataW1.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W1MinValue") == 0)
		{
			TempDimDataW1.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W1MaxValue") == 0)
		{
			TempDimDataW1.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W1DefaultValue") == 0)
		{
			TempDimDataW1.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W1ValueCommitment") == 0)
		{
			TempDimDataW1.prompt = value[i];
		}

		else if (strcmp(ColName[i],"W2ValueType") == 0)
		{
			TempDimDataW2.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"W2Value") == 0)
		{
			TempDimDataW2.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W2MinValue") == 0)
		{
			TempDimDataW2.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W2MaxValue") == 0)
		{
			TempDimDataW2.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W2DefaultValue") == 0)
		{
			TempDimDataW2.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W2ValueCommitment") == 0)
		{
			TempDimDataW2.prompt = value[i];
		}

		else if (strcmp(ColName[i],"W3ValueType") == 0)
		{
			TempDimDataW3.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"W3Value") == 0)
		{
			TempDimDataW3.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W3MinValue") == 0)
		{
			TempDimDataW3.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W3MaxValue") == 0)
		{
			TempDimDataW3.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W3DefaultValue") == 0)
		{
			TempDimDataW3.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"W3ValueCommitment") == 0)
		{
			TempDimDataW3.prompt = value[i];
		}

		else if (strcmp(ColName[i],"H1ValueType") == 0)
		{
			TempDimDataH1.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"H1Value") == 0)
		{
			TempDimDataH1.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H1MinValue") == 0)
		{
			TempDimDataH1.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H1MaxValue") == 0)
		{
			TempDimDataH1.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H1DefaultValue") == 0)
		{
			TempDimDataH1.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H1ValueCommitment") == 0)
		{
			TempDimDataH1.prompt = value[i];
		}

		else if (strcmp(ColName[i],"H2ValueType") == 0)
		{
			TempDimDataH2.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"H2Value") == 0)
		{
			TempDimDataH2.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H2MinValue") == 0)
		{
			TempDimDataH2.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H2MaxValue") == 0)
		{
			TempDimDataH2.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H2DefaultValue") == 0)
		{
			TempDimDataH2.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H2ValueCommitment") == 0)
		{
			TempDimDataH2.prompt = value[i];
		}

		else if (strcmp(ColName[i],"H3ValueType") == 0)
		{
			TempDimDataH3.type = ToEWindowType(UTF8ToGBK(value[i]));
		}
		else if (strcmp(ColName[i],"H3Value") == 0)
		{
			TempDimDataH3.value = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H3MinValue") == 0)
		{
			TempDimDataH3.minValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H3MaxValue") == 0)
		{
			TempDimDataH3.maxValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H3DefaultValue") == 0)
		{
			TempDimDataH3.defaultValue = atof(value[i]);
		}
		else if (strcmp(ColName[i],"H3ValueCommitment") == 0)
		{
			TempDimDataH3.prompt = value[i];
		}*/
	}

	TempData.SetDimData(TempDimDataW);
	TempData.SetDimData(TempDimDataW1);
	TempData.SetDimData(TempDimDataW2);
	TempData.SetDimData(TempDimDataW3);
	TempData.SetDimData(TempDimDataH);
	TempData.SetDimData(TempDimDataH1);
	TempData.SetDimData(TempDimDataH2);
	TempData.SetDimData(TempDimDataH3);

	vAttrWindowData.push_back(TempData);
	return 0;
}

void CWindowLocalDataFromDB::LoadDataFromDataBase()
{
	m_pDB =NULL;
	const char * path = TY_GetLocalDataDatabasePath();
	int nRes = sqlite3_open(path, &m_pDB);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select * from `WindowDoorLocalData`;");
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(m_pDB, sql, OutputWindowData, 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
	}
	sqlite3_close(m_pDB);
}

bool CWindowLocalDataFromDB::GetWindowById(CString p_sId, AttrWindow& value) const //通过原型编号获取窗户
{
	for (UINT i = 0; i < vAttrWindowData.size(); i++)
	{
		if (vAttrWindowData[i].m_prototypeCode == p_sId)
		{
			value = vAttrWindowData[i];
			return true;
		}
	}

	return false;
}

bool  CWindowLocalDataFromDB::GetWindowByFileName(CString p_sFileName, AttrWindow&value)const//通过文件名获取窗户
{
	for (UINT i = 0; i < vAttrWindowData.size(); i++)
	{
		if (vAttrWindowData[i].GetFileName() == p_sFileName)
		{
			value = vAttrWindowData[i];
			return true;
		}
	}
	return false;
}

vector<AttrWindow> CWindowLocalDataFromDB::GetAllWindows()const //获取所有窗户
{
	vector<AttrWindow>  allwin;
	for (UINT i = 0; i < vAttrWindowData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(vAttrWindowData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] == "Window")
		{
			allwin.push_back(vAttrWindowData[i]);
		}
	}
	return allwin;
}

vector<AttrWindow> CWindowLocalDataFromDB::GetAllDoors()const  //获取所有门
{
	vector<AttrWindow> alldoor;
	for (UINT i = 0; i < vAttrWindowData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(vAttrWindowData[i].m_prototypeCode, L'_');
		if (strs[0] == "Door")
		{
			alldoor.push_back(vAttrWindowData[i]);
		}
	}
	return alldoor;
}

std::vector<AttrWindow >  CWindowLocalDataFromDB::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i =0; i < vAttrWindowData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(vAttrWindowData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Window")
		{
			continue;
		}

		const CWindowsDimData* pDiwW = vAttrWindowData[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != vAttrWindowData[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != vAttrWindowData[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != vAttrWindowData[i].m_gongNengquType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != vAttrWindowData[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(vAttrWindowData[i]);
	}

	return data;
}

std::vector<AttrWindow >  CWindowLocalDataFromDB::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i = 0; i < vAttrWindowData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(vAttrWindowData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Door")
		{
			continue;
		}

		const CWindowsDimData* pDiwW = vAttrWindowData[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != vAttrWindowData[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != vAttrWindowData[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != vAttrWindowData[i].m_gongNengquType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != vAttrWindowData[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(vAttrWindowData[i]);
	}

	return data;
}