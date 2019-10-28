#include "StdAfx.h"
#include "string"
#include "afx.h"
#include "AirConditionLocalData.h"
#include "..\Tool\Excel\Excel.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"

using namespace Excel;

CAirConditionLocalData::CAirConditionLocalData()
{
	CString localWindowPath = TY_GetDataFilePath();
	LoadDataFromExcel((localWindowPath + ("原型数据.xlsx")));
}

CAirConditionLocalData::~CAirConditionLocalData()
{
}


void CAirConditionLocalData::LoadDataFromExcel(CString p_file) 
{
	Excel::CExcelUtil xls; 
	bool bSuc = xls.OpenExcel(p_file); //打开表格
	if (bSuc == false)
	{
		AfxMessageBox(_T("无法打开文件：") + p_file);
		return ;
	}

	xls.SetVisible(false); 

	//读取空调数据表单的空调数据
	xls.SetActiveSheet(2); //打开第二张表单

	for (int i = 2; i <= 1000000; i++)  //循环excel表格  空调数据表单的行
	{
		AttrAirCon attrAirCon;

		attrAirCon.m_prototypeCode = xls.GetCellValue(i, 2);
		if (attrAirCon.m_prototypeCode.GetLength() == 0)
			break;

		attrAirCon.m_file.fileName = xls.GetCellValue(i, 3);
		attrAirCon.m_power = _ttof(xls.GetCellValue(i, 4));
		attrAirCon.m_pipePos = xls.GetCellValue(i, 5);
		CString str = xls.GetCellValue(i, 6);
		attrAirCon.m_hasRainPipe = (str==_T("是"))? true : false;
		attrAirCon.m_rainPipePos = xls.GetCellValue(i, 7);
		attrAirCon.m_airW = _ttoi(xls.GetCellValue(i, 8));
		attrAirCon.m_airH = _ttoi(xls.GetCellValue(i, 9));
		attrAirCon.m_airD = _ttoi(xls.GetCellValue(i, 10));
		
		m_allAirconditions.push_back(attrAirCon); //将数据添加到vector数组m_allAirconditions
	}

	xls.CloseExcel();//关闭表格
}


bool CAirConditionLocalData::GetAirConById(CString p_sId, AttrAirCon& value)const   //通过原型编号从m_allAirconditions中获取空调
{
	for (UINT i = 0; i < m_allAirconditions.size(); i++)
	{
		if (m_allAirconditions[i].m_prototypeCode == p_sId)
		{
			value = m_allAirconditions[i];
			return true;
		}
	}

	return false;
}

bool CAirConditionLocalData::GetAirConByFileName(CString p_sFileName, AttrAirCon&value)  const //通过文件名从m_allAirconditions中获取空调
{
	for (UINT i = 0; i < m_allAirconditions.size(); i++)
	{
		if (m_allAirconditions[i].GetFileName() == p_sFileName)
		{
			value = m_allAirconditions[i];
			return true;
		}
	}
	return false;
}

vector<AttrAirCon> CAirConditionLocalData::GetAllAirCons() const  //获取所有空调
{
	return m_allAirconditions;
}

//获取满足筛选条件的空调
std::vector<AttrAirCon >  CAirConditionLocalData::GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const
{
	std::vector<AttrAirCon> data;

	for (UINT i =0; i < m_allAirconditions.size(); i++)
	{
		if (p_airConHorseNumber != m_allAirconditions[i].m_power)
		{
			continue;
		}

		if (p_airConPipePos != m_allAirconditions[i].m_pipePos)
		{
			continue;
		}

		if (p_airConRainRiser != L"否")
		{
			if (p_airConRainRiserPos !=  m_allAirconditions[i].m_rainPipePos)
			{
				continue;
			}
		}
		
		data.push_back(m_allAirconditions[i]);
	}

	return data;

}