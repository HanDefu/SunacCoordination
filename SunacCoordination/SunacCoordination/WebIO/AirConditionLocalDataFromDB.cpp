#include "stdafx.h"
#include "AirConditionLocalDataFromDB.h"
#include "string"
#include "afx.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Sunac.h"
#include "..\Common\ComFun_Convert.h"

CAirConditionLocalDataFromDB* CAirConditionLocalDataFromDB::Instance()
{
	static CAirConditionLocalDataFromDB instance;
	return &instance;
}

CAirConditionLocalDataFromDB::CAirConditionLocalDataFromDB(void)
{
	LoadDataFromDataBase();
}


CAirConditionLocalDataFromDB::~CAirConditionLocalDataFromDB(void)
{
}

vector<AttrAirCon> vAttrAirConData;

int CAirConditionLocalDataFromDB::OutputAirConData(void *NotUsed, int nCol, char **value, char **ColName)
{
	AttrAirCon TempData;
	for (int i = 0; i < nCol; i++)
	{
		if (strcmp(ColName[i], "PrototypeCode") == 0)
		{
			TempData.m_prototypeCode = value[i];
		}
		else if (strcmp(ColName[i],"PrototypeFile") == 0)
		{
			TempData.m_file.fileName = value[i];
		}
		else if (strcmp(ColName[i],"Power") == 0)
		{
			TempData.m_power = atof(value[i]);
		}
		else if (strcmp(ColName[i],"CondensatePipePos") == 0)
		{
			TempData.m_pipePos = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"IsRainPipe") == 0)
		{
			if (UTF8ToGBK(value[i]) == L"是")
			{
				TempData.m_hasRainPipe = true;
			}
			else TempData.m_hasRainPipe = false;
		}
		else if (strcmp(ColName[i],"RainPipePos") == 0)
		{
			TempData.m_rainPipePos = UTF8ToGBK(value[i]);
		}
		else if (strcmp(ColName[i],"W") == 0)
		{
			TempData.m_airW = atoi(value[i]);
		}
		else if (strcmp(ColName[i],"H") == 0)
		{
			TempData.m_airH = atoi(value[i]);
		}
		else if (strcmp(ColName[i],"D") == 0)
		{
			TempData.m_airD = atoi(value[i]);
		}
	}
	vAttrAirConData.push_back(TempData);
	return 0;
}

void CAirConditionLocalDataFromDB::LoadDataFromDataBase()
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
	sqlString.Format(L"select * from `AirConLocalData`;");
	char* sql;
	ConvertStringToUTF8(sqlString, sql);
	int res = sqlite3_exec(m_pDB, sql, OutputAirConData, 0 , &cErrMsg);  

	if (res != SQLITE_OK)
	{
		AfxMessageBox(L"select fail");
	}
	sqlite3_close(m_pDB);
}


bool CAirConditionLocalDataFromDB::GetAirConById(CString p_sId, AttrAirCon& value)const   //通过原型编号从vAttrAirConData中获取空调
{
	for (UINT i = 0; i < vAttrAirConData.size(); i++)
	{
		if (vAttrAirConData[i].m_prototypeCode == p_sId)
		{
			value = vAttrAirConData[i];
			return true;
		}
	}

	return false;
}

bool CAirConditionLocalDataFromDB::GetAirConByFileName(CString p_sFileName, AttrAirCon&value)  const //通过文件名从vAttrAirConData中获取空调
{
	for (UINT i = 0; i < vAttrAirConData.size(); i++)
	{
		if (vAttrAirConData[i].GetFileName() == p_sFileName)
		{
			value = vAttrAirConData[i];
			return true;
		}
	}
	return false;
}

vector<AttrAirCon> CAirConditionLocalDataFromDB::GetAllAirCons() const  //获取所有空调
{
	return vAttrAirConData;
}

//获取满足筛选条件的空调
std::vector<AttrAirCon >  CAirConditionLocalDataFromDB::GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const
{
	std::vector<AttrAirCon> data;

	for (UINT i =0; i < vAttrAirConData.size(); i++)
	{
		if (p_airConHorseNumber != vAttrAirConData[i].m_power)
		{
			continue;
		}

		if (p_airConPipePos != vAttrAirConData[i].m_pipePos)
		{
			continue;
		}

		if (p_airConRainRiser != L"否")
		{
			if (p_airConRainRiserPos !=  vAttrAirConData[i].m_rainPipePos)
			{
				continue;
			}
		}

		data.push_back(vAttrAirConData[i]);
	}

	return data;

}