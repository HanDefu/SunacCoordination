#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
	CAirConditionWebData();
public:
	static CAirConditionWebData* Instance();

	~CAirConditionWebData();

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;

	//weiZhi冷凝水管位置
	/*std::vector<AttrAirCon> GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi);*/
	std::vector<AttrAirCon> GetAllAirCons();
	std::vector<AttrAirCon >  GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const;

protected:
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//从XML解析空调信息
	vector<AttrAirCon> m_allAirconditions; //所有空调
};

