#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
	CAirConditionWebData(){}
public:
	static CAirConditionWebData* Instance();

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;

	//weiZhi冷凝水管位置
	std::vector<AttrAirCon> GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi);
	std::vector<AttrAirCon> GetAllAirCons();

protected:
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//从XML解析空调信息
};

