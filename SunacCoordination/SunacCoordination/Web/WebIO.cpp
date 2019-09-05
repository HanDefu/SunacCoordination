#include "StdAfx.h"
#include "WebIO.h"
#include "float.h"
#include <acdocman.h>
#include <dbxutil.h>

WebIO::WebIO()
{
	m_quyus = GetQuyus();//区域
	m_gongNengQus = GetGongNengQus();//功能区
	m_windowDoorPoss = GetWindowDoorPoss();//门窗位置关系
	m_windowOpenTypes = Window_GetOpenTypes();//开启类型
	m_doorTypes = Door_GetTypes();//门类型
	m_kitchenTypes = Kitchen_GetTypes();//厨房类型
	m_kitchenShuiPenTypes = Kitchen_GetShuiPenTypes();//水盆类型
	m_kitchenBingXiangTypes = Kitchen_GetBingXiangTypes();//冰箱类型
	m_kitchenZaoTaiWidths = Kitchen_GetZaoTaiWidths();//灶台宽度
	m_toiletTypes = Toilet_GetTypes();//卫生间类型
	m_toiletTaiPenWidths = Toilet_GetTaiPenWidths();//卫生间类型
	m_toiletMaTongTypes = Toilet_GetMaTongTypes();//卫生间类型
	m_toiletGuanXiWidths =Toilet_GetGuanXiWidths();//盥洗区宽度---是否支持手动输入？？
	m_airPiShus = Air_GetPiShus();//所有的匹数
	m_airLengNingShuiGuanPos =  Air_GetLengNingShuiGuanPos();//冷凝水管位置
}

WebIO::~WebIO()
{

}

int WebIO::GetWindows
(
   double width,//宽度值，注意高度值不作为搜索条件 
   CString openType, //开启类型
   int openNum,//开启扇数量
   CString gongNengQu,//功能区
   double tongFengLiang//通风量
)
{
	return 0;
}

int WebIO::GetDoors
(
    double minWid, //宽度值，注意高度值不作为搜索条件 
    CString doorType //门类型
)
{
	return 0;
}

int WebIO::GetKitchens
(
  double kaiJian,//开间
  double jinShen,//进深
  CString weiZhiGuanXi,//门窗位置关系
  CString type,//厨房类型
  bool hasPaiQiDao//是否含有排气道
)
{
	return 0;
}

int WebIO::GetToilets
(
    double kaiJian,//开间
    double jinShen,//进深
    CString weiZhiGuanXi,//门窗位置关系
    CString type,//卫生间类型
    bool hasPaiQiDao//是否含有排气道
)
{
	return 0;
}

int WebIO::GetAirCon
(
    double piShu,
	CString weiZhi,//冷凝水管位置
	bool hasYuShuiGuan
)
{
	return 0;
}

int WebIO::GetRailings()//一次搜索所有的
{
	return 0;
}

vCString WebIO::GetQuyus()
{
	vCString strs;
	return strs;
}
vCString WebIO::GetGongNengQus()
{
	vCString strs;
	return strs;
}
vCString WebIO::GetWindowDoorPoss()
{
	vCString strs;
	return strs;
}
vCString WebIO::Window_GetOpenTypes()
{
	vCString strs;
	return strs;
}
//读取尺寸值
eWindowDimType WebIO::Window_GetDim(CString yuanXingBianHao, CString dimStr)
{
	eWindowDimType value;
	return value;
}

vCString WebIO::Door_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetShuiPenTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetBingXiangTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Kitchen_GetZaoTaiWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetTaiPenWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetMaTongTypes()
{
	vCString strs;
	return strs;
}
vCString WebIO::Toilet_GetGuanXiWidths()
{
	vCString strs;
	return strs;
}
vCString WebIO::Air_GetPiShus()
{
	vCString strs;
	return strs;
}
vCString WebIO::Air_GetLengNingShuiGuanPos()
{
	vCString strs;
	return strs;
}