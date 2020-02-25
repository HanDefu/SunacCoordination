#include "StdAfx.h"
#include "string"
#include "afx.h"
#include <vector>
#include <algorithm>
#include "WindowLocalDataFromDB.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"
#include "..\Common\ComFun_Convert.h"

CWindowLocalDataFromDB* CWindowLocalDataFromDB::Instance()
{
	static CWindowLocalDataFromDB instance;
	return &instance;
}

bool SortWinFun(AttrWindow attr1, AttrWindow attr2)
{
	if (attr1.m_isDynamic)
	{
		return false;
	}
	else return true;
}

CWindowLocalDataFromDB::CWindowLocalDataFromDB()
{
	LoadDataFromDataBase();
}

CWindowLocalDataFromDB::~CWindowLocalDataFromDB()
{
}


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
	data.prompt = sComment;

	valueType.Trim();
	if (valueType == L"范围")
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
	else if (valueType == L"固定值" || valueType == L"值")
	{
		data.type = SINGLE;
		data.valueOptions.push_back(_wtof(sValue));
	}
	else if (valueType.Find(L"系列") >=0 )
	{
		data.type = MULTI;

		std::vector<CString> strs = YT_SplitCString(sValue, L',');
		for (UINT i = 0; i < strs.size(); i++)
		{
			data.valueOptions.push_back(_wtof(strs[i]));
		}
	}
	else if (valueType == L"不限")
	{
		data.type = UNLIMIT;
	}
	else
	{
		data.type = NOVALUE;
	}

	data.SetDefaultValue(defaultValue);
	return data;
}

int CWindowLocalDataFromDB::OutputWindowData(void *NotUsed, int nCol, char **value, char **ColName)
{
	AttrWindow oneWinAtt;
		
	for (int i = 0; i < 14; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			oneWinAtt.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i],"PrototypeFile") == 0)
		{
			oneWinAtt.m_file.fileName = value[i];
		}
		else if (strcmp(ColName[i],"FrontViewFile") == 0)
		{
			oneWinAtt.m_frontViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"TopViewFile") == 0)
		{
			oneWinAtt.m_topViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"LeftViewFile") == 0)
		{
			oneWinAtt.m_leftViewFile.fileName = value[i];
		}
		else if (strcmp(ColName[i],"Scope") == 0)
		{
			if (UTF8ToGBK(value[i]) == L"全部")
			{
				oneWinAtt.m_isJiTuan = true;
			}
			else oneWinAtt.m_isJiTuan = false;
		}
		else if (strcmp(ColName[i],"IsDynamic") == 0)
		{
			if (UTF8ToGBK(value[i]) == L"是")
			{
				oneWinAtt.m_isDynamic = true;
			}
			else oneWinAtt.m_isDynamic = false;
		}
		else if (strcmp(ColName[i],"FunctionalType") == 0)
		{
			oneWinAtt.m_gongNengquType = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"OpenType") == 0)
		{
			oneWinAtt.m_openType = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"OpenQty") == 0)
		{
			oneWinAtt.m_openQty = atoi(value[i]);
		}
		else if (strcmp(ColName[i],"VentilationFormula") == 0)
		{
			oneWinAtt.m_tongFengFormula = value[i];
		}
		else if (strcmp(ColName[i],"Ventilation") == 0)
		{
			oneWinAtt.m_tongFengQty = atoi(value[i]);
		}
		else if (strcmp(ColName[i], "IsSymmetrical") == 0)
		{
			oneWinAtt.m_isMirrorWindow = atoi(value[i]) ? true : false;
		}
	}

	const CWindowsDimData TempDimDataW = GetWindowDimData(_T("W"), value, 13);
	const CWindowsDimData TempDimDataH = GetWindowDimData(_T("H"), value, 19);
	const CWindowsDimData TempDimDataW1 = GetWindowDimData(_T("W1"), value, 25);
	const CWindowsDimData TempDimDataW2 = GetWindowDimData(_T("W2"), value, 31);
	const CWindowsDimData TempDimDataW3 = GetWindowDimData(_T("W3"), value, 37);
	const CWindowsDimData TempDimDataH1 = GetWindowDimData(_T("H1"), value, 43);
	const CWindowsDimData TempDimDataH2 = GetWindowDimData(_T("H2"), value, 49);
	const CWindowsDimData TempDimDataH3 = GetWindowDimData(_T("H3"), value, 55);
	oneWinAtt.SetDimData(TempDimDataW);
	oneWinAtt.SetDimData(TempDimDataW1);
	oneWinAtt.SetDimData(TempDimDataW2);
	oneWinAtt.SetDimData(TempDimDataW3);
	oneWinAtt.SetDimData(TempDimDataH);
	oneWinAtt.SetDimData(TempDimDataH1);
	oneWinAtt.SetDimData(TempDimDataH2);
	oneWinAtt.SetDimData(TempDimDataH3);

	oneWinAtt.CheckAndComplementDimeData();

	Instance()->m_allWindowsData.push_back(oneWinAtt);
	return 0;
}

void CWindowLocalDataFromDB::LoadDataFromDataBase()
{
	m_allWindowsData.clear();

	sqlite3* pDB = NULL;
	const char * path = TY_GetLocalDataDatabasePath();
	int nRes = sqlite3_open(path, &pDB);

	if (nRes != SQLITE_OK)
	{
		AfxMessageBox(L"Open database fail");
	}
	char* cErrMsg;
	CString sqlString;
	sqlString.Format(L"select * from `WindowDoorLocalData`;");
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(pDB, sql, OutputWindowData, 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
	}
	sqlite3_close(pDB);
}

bool CWindowLocalDataFromDB::GetWindowById(CString p_sId, AttrWindow& value) const //通过原型编号获取窗户
{
	for (UINT i = 0; i < m_allWindowsData.size(); i++)
	{
		if (m_allWindowsData[i].m_prototypeCode == p_sId)
		{
			value = m_allWindowsData[i];
			return true;
		}
	}

	return false;
}

bool  CWindowLocalDataFromDB::GetWindowByFileName(CString p_sFileName, AttrWindow&value)const//通过文件名获取窗户
{
	for (UINT i = 0; i < m_allWindowsData.size(); i++)
	{
		if (m_allWindowsData[i].GetFileName() == p_sFileName)
		{
			value = m_allWindowsData[i];
			return true;
		}
	}
	return false;
}

vector<AttrWindow> CWindowLocalDataFromDB::GetAllWindows()const //获取所有窗户
{
	vector<AttrWindow>  allwin;
	for (UINT i = 0; i < m_allWindowsData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_allWindowsData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] == "Window")
		{
			allwin.push_back(m_allWindowsData[i]);
		}
	}

	sort(allwin.begin(), allwin.end(), SortWinFun);
	return allwin;
}

vector<AttrWindow> CWindowLocalDataFromDB::GetAllDoors()const  //获取所有门
{
	vector<AttrWindow> alldoor;
	for (UINT i = 0; i < m_allWindowsData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_allWindowsData[i].m_prototypeCode, L'_');
		if (strs[0] == "Door")
		{
			alldoor.push_back(m_allWindowsData[i]);
		}
	}

	sort(alldoor.begin(), alldoor.end(), SortWinFun);
	return alldoor;
}

std::vector<AttrWindow >  CWindowLocalDataFromDB::GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i =0; i < m_allWindowsData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_allWindowsData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Window")
		{
			continue;
		}

		if (!m_allWindowsData[i].m_isDynamic)
		{
			double dimW = m_allWindowsData[i].GetW();
			double dimH = m_allWindowsData[i].GetH();
			if ((width != dimW) || (height != dimH))
				continue;
		}

		const CWindowsDimData* pDiwW = m_allWindowsData[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != m_allWindowsData[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_allWindowsData[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_allWindowsData[i].m_gongNengquType)
			{
				continue;
			}
		}

		data.push_back(m_allWindowsData[i]);
	}

	sort(data.begin(), data.end(), SortWinFun);
	return data;
}

std::vector<AttrWindow >  CWindowLocalDataFromDB::GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i = 0; i < m_allWindowsData.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_allWindowsData[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Door")
		{
			continue;
		}

		const CWindowsDimData* pDiwW = m_allWindowsData[i].GetDimData(_T("W"));
		if (width < pDiwW->minValue || width > pDiwW->maxValue)
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != m_allWindowsData[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_allWindowsData[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_allWindowsData[i].m_gongNengquType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != m_allWindowsData[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(m_allWindowsData[i]);
	}

	sort(data.begin(), data.end(), SortWinFun);
	return data;
}