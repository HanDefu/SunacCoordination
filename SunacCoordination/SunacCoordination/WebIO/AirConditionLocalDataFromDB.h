#pragma once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\SQLite\sqlite3.h"

using namespace std;

class CAirConditionLocalDataFromDB
{
	CAirConditionLocalDataFromDB();
public:
	~CAirConditionLocalDataFromDB();
	static CAirConditionLocalDataFromDB* Instance();
	static int OutputAirConData(void *NotUsed, int nCol, char **value, char **ColName);

	bool GetAirConById(CString p_sId,AttrAirCon& value)const;  //通过原型编号从m_allAirconditions中获取空调
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value)const;//通过文件名从m_allAirconditions中获取空调

	std::vector<AttrAirCon >  GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;
	vector<AttrAirCon> GetAllAirCons()const; //获取所有空调

protected:
	void LoadDataFromDataBase(); //从表格中把数据传到m_allAirconditions中

protected:
	vector<AttrAirCon> m_allAirconditions; //所有空调
};

