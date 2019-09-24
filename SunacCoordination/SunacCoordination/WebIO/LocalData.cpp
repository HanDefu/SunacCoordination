#include "StdAfx.h"
#include "LocalData.h"
#include "..\Tool\Excel\Excel.h"
#include "string"
#include "afx.h"
#include "../Common/ComFun_String.h"

using namespace Excel;


CLocalData::CLocalData()
{
}

CLocalData::~CLocalData()
{
}

CLocalData* CLocalData::GetInstance()
{
	static CLocalData instance;
	return &instance;
}


//门窗
RCDimData CLocalData::ConvertStringToDimData
(
    CString code,
    CString  valueType,
	CString value,
	CString defaultValue,
	CString state
)
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
		for (int i = 0; i < strs.size(); i++)
		{
			data.values.push_back(_wtof(strs[i]));
		}
		data.defaultValue = _wtof(defaultValue);
		data.prompt = state;
	}
	return data;
}

void CLocalData::LoadWindowFromExcel(CString p_file) //将数据从表格中读取到m_windows中
{
	Excel::CExcelUtil xls; 

	CString	widthMin;			//宽度最小值
	CString	widthMax;			//宽度最大值

	CString	valueType;		//W1值类型	
	CString	value;			//W1值	
	CString	min;				//W1最小值
	CString	max;				//W1最大值
	CString	defaultValue;		//W1默认值
	CString	state;			//W1说明

	xls.OpenExcel(p_file); //打开表格
	xls.SetVisible(false); 
	xls.SetActiveSheet(1); //打开第一张表

	

	for (int i = 2; i <= 1000000; i++)  //循环excel表格  外窗数据表单的行
	{
		AttrWindow attrwindow;
		
		//通过行和列获取单元格的值，并将值赋给对象attrwindow
		attrwindow.id = xls.GetCellValue(i, 1); 
		attrwindow.prototypeId = xls.GetCellValue(i, 2);
		if (attrwindow.prototypeId.GetLength() == 0)  //对原型编号的长度进行判断，当原型编号为空的时候结束循环
			break;

		attrwindow.prototypeFile = xls.GetCellValue(i, 3);
		attrwindow.scopeOfApplication = xls.GetCellValue(i, 4);
		attrwindow.isDynamic = xls.GetCellValue(i, 5);
		attrwindow.functionType = xls.GetCellValue(i, 6);
		attrwindow.openType = xls.GetCellValue(i, 7);
		attrwindow.openNum = xls.GetCellValue(i, 8);
		widthMin = xls.GetCellValue(i, 9);
		widthMax = xls.GetCellValue(i, 10);
		attrwindow.ventilationFormula = xls.GetCellValue(i, 11);
		attrwindow.staticWidth = xls.GetCellValue(i, 12);
		attrwindow.staticHeigth = xls.GetCellValue(i, 13);
		attrwindow.staticVentilation = xls.GetCellValue(i, 14);
		
		//W 
		SRCDimData data;
		data.sCodeName = L"W";
		data.type = SCOPE;
		data.values.push_back(_wtof(widthMin));
		data.values.push_back(_wtof(widthMax));
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
		valueType = xls.GetCellValue(i, 15);
		value = xls.GetCellValue(i, 16);
		defaultValue = xls.GetCellValue(i, 19);
		state = xls.GetCellValue(i, 20);

		data = ConvertStringToDimData(L"W1",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);


		//W2
		valueType = xls.GetCellValue(i, 21);
		value = xls.GetCellValue(i, 22);
		defaultValue = xls.GetCellValue(i, 25);
		state = xls.GetCellValue(i, 26);

		data = ConvertStringToDimData(L"W2",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);


		//W3
		valueType = xls.GetCellValue(i, 27);
		value = xls.GetCellValue(i, 28);
		defaultValue = xls.GetCellValue(i, 31);
		state = xls.GetCellValue(i, 32);

		data = ConvertStringToDimData(L"W3",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);

		//H1
		valueType = xls.GetCellValue(i, 33);
		value = xls.GetCellValue(i, 34);
		defaultValue = xls.GetCellValue(i, 37);
		state = xls.GetCellValue(i, 38);

		data = ConvertStringToDimData(L"H1",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);



		//H2
		valueType = xls.GetCellValue(i, 39);
		value = xls.GetCellValue(i, 40);
		defaultValue = xls.GetCellValue(i, 43);
		state = xls.GetCellValue(i, 44);

		data = ConvertStringToDimData(L"H2",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);

		//H3
		valueType = xls.GetCellValue(i, 45);
		value = xls.GetCellValue(i, 46);
		defaultValue = xls.GetCellValue(i, 49);
		state = xls.GetCellValue(i, 50);

		data = ConvertStringToDimData(L"H3",valueType, value,defaultValue,state);
		attrwindow.m_dimData.push_back(data);

		attrwindow.prototypeFlatFile = xls.GetCellValue(i, 51);
		attrwindow.prototypeTopViewFile = xls.GetCellValue(i, 52);

		m_windows.push_back(attrwindow); //将数据添加到vector数组m_windows
	}
	xls.CloseExcel();//关闭表格
}


bool CLocalData::GetWindowById(CString p_sId,AttrWindow& value)  //通过原型编号获取窗户
{
	for (int i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].prototypeId == p_sId)
		{
			value = m_windows[i];
			return true;
		}
	}

	return false;
}

bool  CLocalData::GetWindowByFileName(CString p_sFileName, AttrWindow&value)//通过文件名获取窗户
{
	for (int i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i].prototypeFile == p_sFileName)
		{
			value = m_windows[i];
			return true;
		}
	}
	return false;
}

vector<AttrWindow> CLocalData::GetAllWindows() //获取所有窗户
{
	for (int i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].prototypeId, L'_');  //用"_"拆分
		if (strs[0] == "Window")
		{
			m_wins.push_back(m_windows[i]);
		}
	}
	return m_wins;
}

vector<AttrWindow> CLocalData::GetAllDoors()  //获取所有门
{
	for (int i = 0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].prototypeId, L'_');
		if (strs[0] == "Door")
		{
			m_doors.push_back(m_windows[i]);
		}
	}
	return m_doors;
}

std::vector<AttrWindow >  CWindowLocalData::GetWindows(double width, CString openType, int openNum, CString gongNengQu)
{
	std::vector<AttrWindow> data;

	for (int i =0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].prototypeId, L'_');  //用"_"拆分
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
			if (openType != m_windows[i].openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].openNum)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_windows[i].functionType)
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

std::vector<AttrWindow >  CWindowLocalData::GetDoors(double width, CString openType, int openNum, CString gongNengQu)
{
	std::vector<AttrWindow> data;

	for (int i =0; i < m_windows.size(); i++)
	{
		std::vector<CString> strs = YT_SplitCString(m_windows[i].prototypeId, L'_');  //用"_"拆分
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
			if (openType != m_windows[i].openType)
			{
				continue;
			}
		}

		if (openNum != 0)
		{
			if (openNum != m_windows[i].openNum)
			{
				continue;
			}
		}

		if (gongNengQu != L"不限")
		{
			if (gongNengQu != m_windows[i].functionType)
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



//空调
void CLocalData::LoadAirConFromExcel(CString p_file)  //从表格中把数据传到m_aircon中
{
	Excel::CExcelUtil xls; 

	xls.OpenExcel(p_file); //打开表格
	xls.SetVisible(false); 
	xls.SetActiveSheet(2); //打开第一张表


	for (int i = 2; i <= 1000000; i++)  //循环excel表格  空调数据表单的行
	{
		AttrAirCon attrAirCon;

		attrAirCon.m_airConId = xls.GetCellValue(i, 1); //通过行和列获取单元格的值，并将值赋给对象attrwindow
		attrAirCon.m_airConPrototypeId = xls.GetCellValue(i, 2);
		if (attrAirCon.m_airConPrototypeId.GetLength() == 0)
			break;

		attrAirCon.m_airConPrototypeFile = xls.GetCellValue(i, 3);
		attrAirCon.m_airConHorseNumber = xls.GetCellValue(i, 4);
		attrAirCon.m_airConPipePos = xls.GetCellValue(i, 5);
		attrAirCon.m_airConRainRiser = xls.GetCellValue(i, 6);
		attrAirCon.m_airConRainRiserPos = xls.GetCellValue(i, 7);
		attrAirCon.m_airConInstallNetSize = xls.GetCellValue(i, 8);
		
		m_aircon.push_back(attrAirCon); //将数据添加到vector数组m_windows
	}
	xls.CloseExcel();//关闭表格
}

bool CLocalData::GetAirConById(CString p_sId,AttrAirCon& value)   //通过原型编号从m_aircon中获取空调
{
	for (int i = 0; i < m_aircon.size(); i++)
	{
		if (m_aircon[i].m_airConPrototypeId == p_sId)
		{
			value = m_aircon[i];
			return true;
		}
	}

	return false;
}

bool CLocalData::GetAirConByFileName(CString p_sFileName, AttrAirCon&value)   //通过文件名从m_aircon中获取空调
{
	for (int i = 0; i < m_aircon.size(); i++)
	{
		if (m_aircon[i].m_airConPrototypeFile == p_sFileName)
		{
			value = m_aircon[i];
			return true;
		}
	}
	return false;
}

vector<AttrAirCon> CLocalData::GetAllAirCon()   //获取所有空调
{
	return m_aircon;
}

