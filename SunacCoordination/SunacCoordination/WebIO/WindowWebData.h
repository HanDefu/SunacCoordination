#pragma once

#include <vector>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/WindowDoor/AttrDoor.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"

class CWindowWebData
{
public:
	CWindowWebData();
	~CWindowWebData();

	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//通过文件名从m_windows中获取窗户

	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	//读取门和窗
	std::vector<AttrWindow >  GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetAllWindows()const;

	std::vector<AttrDoor >  GetAllDoors()const;
	std::vector<AttrDoor >  GetDoors(double p_width, double p_heigh, CString doorType)const;

	std::vector<AttrBathroom> GetBathrooms(double width, double height, CString BathroomDoorWindowPosition, CString ToiletType, int Airvent)const;
	std::vector<AttrBathroom> GetAllBathrooms()const;

	std::vector<AttrKitchen> GetAllKitchens()const;
	std::vector<AttrRailing> GetRailings(CString RailingType)const;
	std::vector<AttrAirCon> GetAirConditioners(CString AirconditionerPower, CString AirconditionerPipePosition, int AirconditionerIsRainpipe, CString RainpipePosition)const;

	std::vector<AttrWindow > ParseWindowsFromXML(CMarkup xml)const;//从XML解析窗户信息
	std::vector<AttrDoor > ParseDoorsFromXML(CMarkup xml)const;//从XML解析门信息
	std::vector<AttrBathroom > ParseBathroomsFromXML(CMarkup xml)const;//从XML解析卫生间信息
	std::vector<AttrKitchen > ParseKitchensFromXML(CMarkup xml)const;//从XML解析厨房信息
	std::vector<AttrRailing > ParseRailingsFromXML(CMarkup xml)const;//从XML解析栏杆信息
	std::vector<AttrAirCon > ParseAirConditionersFromXML(CMarkup xml)const;//从XML解析空调信息

protected:

};

