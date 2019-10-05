#include "StdAfx.h"
#include "string"
#include "afx.h"
#include "WindowLocalData.h"
#include "..\Tool\Excel\Excel.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"

using namespace Excel;

CWindowLocalData::CWindowLocalData()
{
	CString localWindowPath = TY_GetLocalFilePath();
	LoadDataFromExcel((localWindowPath + ("外窗数据.xlsx")));
}

CWindowLocalData::~CWindowLocalData()
{
}


//门窗
RCDimData CWindowLocalData::ConvertStringToDimData ( CString code, CString  valueType, CString value, CString defaultValue,	CString state) const
{
	RCDimData  data;
	if (valueType == "无")
	{
		data.sCodeName = code;
		data.type = NOVALUE;
	}
	else if(valueType == L"公式")
	{
		data.sCodeName = code;
		data.type = CALC;
	}
	else if(valueType == L"固定值")
	{
		data.sCodeName = code;
		data.type = SINGLE;
		data.values.push_back(_wtof(value));
		data.defaultValue = _wtof(defaultValue);
		data.prompt = state;
	}
	else if(valueType == L"值系列")
	{
		data.sCodeName = code;
		data.type = MULTI;

		std::vector<CString> strs = YT_SplitCString(value, L',');
		for (UINT i = 0; i < strs.size(); i++)
		{
			data.values.push_back(_wtof(strs[i]));
		}
		data.defaultValue = _wtof(defaultValue);
		data.prompt = state;
	}
	return data;
}

void CWindowLocalData::LoadDataFromExcel(CString p_file) 
{
	Excel::CExcelUtil xls; 

	xls.OpenExcel(p_file); //打开表格
	xls.SetVisible(false); 


	//将数据从表格中读取到m_windows中，读取外窗数据表单
	xls.SetActiveSheet(1); //打开第一张表

	for (int i = 2; i <= 1000000; i++)  //循环excel表格  外窗数据表单的行
	{
		AttrWindow attrwindow;
		
		//通过行和列获取单元格的值，并将值赋给对象attrwindow
		attrwindow.m_id = xls.GetCellValue(i, 1); 
		attrwindow.m_prototypeCode = xls.GetCellValue(i, 2);
		if (attrwindow.m_prototypeCode.GetLength() == 0)  //对原型编号的长度进行判断，当原型编号为空的时候结束循环
			break;

		attrwindow.m_name = xls.GetCellValue(i, 3);
		attrwindow.m_scopeOfApplication = xls.GetCellValue(i, 4);
		attrwindow.m_Dynamic = xls.GetCellValue(i, 5);
		attrwindow.m_functionType = xls.GetCellValue(i, 6);
		attrwindow.m_openType = xls.GetCellValue(i, 7);
		attrwindow.m_openQty = _ttoi(xls.GetCellValue(i, 8));
		attrwindow.m_minWid =  _ttoi(xls.GetCellValue(i, 9));
		attrwindow.m_maxWid =  _ttoi(xls.GetCellValue(i, 10));
		attrwindow.m_tongFengFormula = xls.GetCellValue(i, 11);
		attrwindow.m_width =  _ttoi(xls.GetCellValue(i, 12));
		attrwindow.m_height =  _ttoi(xls.GetCellValue(i, 13));
		attrwindow.m_tongFengQty =  _ttoi(xls.GetCellValue(i, 14));
		
		//W 
		SRCDimData data;
		data.sCodeName = L"W";
		data.type = SCOPE;
		data.values.push_back(attrwindow.m_minWid);
		data.values.push_back(attrwindow.m_maxWid);
		data.prompt = L"";
		data.defaultValue = 0;
		attrwindow.m_dimData.push_back(data);

		//H
		data.sCodeName = L"H";
		data.type = UNLIMIT;
		data.prompt = L"";
		data.defaultValue = 0;
		attrwindow.m_dimData.push_back(data);

		//W1
		attrwindow.m_valueType = xls.GetCellValue(i, 15);
		attrwindow.m_value = xls.GetCellValue(i, 16);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 19);
		attrwindow.m_state = xls.GetCellValue(i, 20);

		data = ConvertStringToDimData(L"W1",attrwindow.m_valueType, attrwindow.m_value,attrwindow.m_defaultValue,attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		//W2
		attrwindow.m_valueType = xls.GetCellValue(i, 21);
		attrwindow.m_value = xls.GetCellValue(i, 22);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 25);
		attrwindow.m_state = xls.GetCellValue(i, 26);

		data = ConvertStringToDimData(L"W2",attrwindow.m_valueType, attrwindow.m_value,attrwindow.m_defaultValue,attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		//W3
		attrwindow.m_valueType = xls.GetCellValue(i, 27);
		attrwindow.m_value = xls.GetCellValue(i, 28);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 31);
		attrwindow.m_state = xls.GetCellValue(i, 32);

		data = ConvertStringToDimData(L"W3",attrwindow.m_valueType, attrwindow.m_value,attrwindow.m_defaultValue,attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		//H1
		attrwindow.m_valueType = xls.GetCellValue(i, 33);
		attrwindow.m_value = xls.GetCellValue(i, 34);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 37);
		attrwindow.m_state = xls.GetCellValue(i, 38);

		data = ConvertStringToDimData(L"H1",attrwindow.m_valueType, attrwindow.m_value,attrwindow.m_defaultValue,attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		//H2
		attrwindow.m_valueType = xls.GetCellValue(i, 39);
		attrwindow.m_value = xls.GetCellValue(i, 40);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 43);
		attrwindow.m_state = xls.GetCellValue(i, 44);

		data = ConvertStringToDimData(L"H2",attrwindow.m_valueType, attrwindow.m_value,attrwindow.m_defaultValue,attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		//H3
		attrwindow.m_valueType = xls.GetCellValue(i, 45);
		attrwindow.m_value = xls.GetCellValue(i, 46);
		attrwindow.m_defaultValue = xls.GetCellValue(i, 49);
		attrwindow.m_state = xls.GetCellValue(i, 50);

		data = ConvertStringToDimData(L"H3",attrwindow.m_valueType, attrwindow.m_value, attrwindow.m_defaultValue, attrwindow.m_state);
		attrwindow.m_dimData.push_back(data);

		attrwindow.m_prototypeFlatFile = xls.GetCellValue(i, 51);
		attrwindow.m_prototypeTopViewFile = xls.GetCellValue(i, 52);

		m_windows.push_back(attrwindow); 
	}

	xls.CloseExcel();//关闭表格
}


bool CWindowLocalData::GetWindowById(CString p_sId, AttrWindow& value) const //通过原型编号获取窗户
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].m_prototypeCode == p_sId)
		{
			value = m_windows[i];
			return true;
		}
	}

	return false;
}

bool  CWindowLocalData::GetWindowByFileName(CString p_sFileName, AttrWindow&value)const//通过文件名获取窗户
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].m_name == p_sFileName)
		{
			value = m_windows[i];
			return true;
		}
	}
	return false;
}

vector<AttrWindow> CWindowLocalData::GetAllWindows() //获取所有窗户
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] == "Window")
		{
			m_wins.push_back(m_windows[i]);
		}
	}
	return m_wins;
}

vector<AttrWindow> CWindowLocalData::GetAllDoors()  //获取所有门
{
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');
		if (strs[0] == "Door")
		{
			m_doors.push_back(m_windows[i]);
		}
	}
	return m_doors;
}

std::vector<AttrWindow >  CWindowLocalData::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i =0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Window")
		{
			continue;
		}

		if (width < m_windows[i].m_dimData[0].values[0] || width > m_windows[i].m_dimData[0].values[1])
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != m_windows[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_windows[i].m_functionType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != m_windows[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(m_windows[i]);
	}

	return data;
}

std::vector<AttrWindow >  CWindowLocalData::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
{
	std::vector<AttrWindow> data;

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].m_prototypeCode, L'_');  //用"_"拆分
		if (strs[0] != "Door")
		{
			continue;
		}

		if (width < m_windows[i].m_dimData[0].values[0] || width > m_windows[i].m_dimData[0].values[1])
		{
			continue;
		}

		if (openType != L"不限")
		{
			if (openType != m_windows[i].m_openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].m_openQty)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_windows[i].m_functionType)
			{
				continue;
			}
		}

		/*if (tongFengLiang != m_windows[i].ventilationFormula)
		{
			continue;
		}*/

		data.push_back(m_windows[i]);
	}

	return data;
}

