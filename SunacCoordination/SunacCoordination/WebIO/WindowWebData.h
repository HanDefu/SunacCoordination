#pragma once

#include <vector>
#include <atlconv.h>
#include "../Object/WindowDoor/AttrWindow.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "..\Tool\MarkupXml\Markup.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_Convert.h"
//#include "..\WebIO\WebIO.h"


class CWindowWebData
{
	CWindowWebData();
public:
	static CWindowWebData* Instance();
	~CWindowWebData();

	bool GetWindowById(CString p_sId, AttrWindow& value)const;  //通过原型编号从m_windows中获取窗户
	bool GetWindowByFileName(CString p_sFileName, AttrWindow&value)const;//通过文件名从m_windows中获取窗户

	//注意高度值不作为搜索条件 
	//width宽度值，openType开启类型, openNum开启扇数量  gongNengQu功能区, tongFengLiang通风量
	std::vector<AttrWindow >  GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetAllWindows()const;

	std::vector<AttrWindow >  GetAllDoors()const;
	std::vector<AttrWindow >  GetDoors(double p_width, double p_heigh, CString doorType, int openNum, CString gongNengQu)const;

protected:
	std::vector<AttrWindow > ParseWindowsFromXML(CMarkup xml)const;//从XML解析窗户信息
	std::vector<AttrWindow > ParseDoorsFromXML(CMarkup xml)const;//从XML解析门信息
};

