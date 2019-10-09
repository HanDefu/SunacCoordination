#pragma once

#include <vector>
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"


class CAirConditionWebData
{
public:

	bool GetAirConditionById(CString p_sId, AttrAirCon& value)const;


	std::vector<AttrAirCon> GetAirCons(double piShu,
							CString weiZhi,//冷凝水管位置
							bool hasYuShuiGuan,
							CString yuShuiGuanWeizhi);

	std::vector<AttrAirCon> GetAllAirCons();

	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//从XML解析空调信息

protected:

};

