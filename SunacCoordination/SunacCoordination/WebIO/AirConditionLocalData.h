#pragma  once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/AirCondition/AttrAirCon.h"

using namespace std;

class CAirConditionLocalData
{
public:
	CAirConditionLocalData();
	~CAirConditionLocalData();

	bool GetAirConById(CString p_sId,AttrAirCon& value)const;  //通过原型编号从m_aircon中获取空调
	bool GetAirConByFileName(CString p_sFileName, AttrAirCon&value)const;//通过文件名从m_aircon中获取空调

	std::vector<AttrAirCon >  GetAirCon(CString p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;

	vector<AttrAirCon> GetAllAirCon()const; //获取所有空调

protected:
	void LoadDataFromExcel(CString p_file); //从表格中把数据传到m_windows中

protected:
	vector<AttrAirCon> m_aircon; //所有空调
};

