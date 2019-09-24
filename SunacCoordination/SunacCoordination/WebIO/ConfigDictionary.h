#pragma once
#include <map>
using namespace std;



class CConfigDictionary
{
public:
//	static CConfigDictionary *GetInstance();

	CConfigDictionary();
	~CConfigDictionary();

protected:
	bool InitFromWeb();
	bool GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut);

protected:
	map<CString, vCString> m_configDict;

	//vCString m_quyus;//区域
	//vCString  m_gongNengQus;//功能区
	//vCString  m_windowDoorPoss;//门窗位置关系--厨房卫生间
	//vCString m_windowOpenTypes;//外窗开启类型
	//vCString m_windowOpenAmount;//开启扇数量
	//vCString m_windowWallDis;//窗户距外墙距离
	//vCString m_rate;//比值，用于计算通风量
	//vCString m_doorTypes;//门类型
	//vCString m_kitchenTypes;//厨房类型
	//vCString m_kitchenShuiPenTypes;//厨房水盆类型
	//vCString m_kitchenBingXiangTypes;//厨房冰箱类型
	//vCString m_kitchenZaoTaiWidths;//厨房灶台宽度
	//vCString m_toiletTypes;//卫生间类型
	//vCString m_toiletTaiPenWidths;//卫生间类型
	//vCString m_toiletMaTongTypes;//卫生间类型
	//vCString m_toiletGuanXiWidths;//卫生间盥洗区宽度---是否支持手动输入？？
	//vCString m_airPiShus;//空调所有的匹数
	//vCString m_airLengNingShuiGuanPos;//空调冷凝水管位置

public:
	//---------------通用查字典-----------------------//
	vCString GetQuyus() const ;//区域
	vCString GetGongNengQus()const;//功能区
	vCString GetWindowDoorPoss()const;//门窗位置关系

	vCString Window_GetOpenTypes()const;//开启类型
	vCString Window_GetOpenAmount()const;//开启扇数量
	vCString Window_GetRate()const;//比值，用于计算通风量
	vCString Window_GetWallDis()const;//窗户距外墙距离

	vCString Door_GetTypes()const;//门类型

	vCString Kitchen_GetTypes()const;//厨房类型
	vCString Kitchen_GetShuiPenTypes()const;//水盆类型
	vCString Kitchen_GetBingXiangTypes()const;//冰箱类型
	vCString Kitchen_GetZaoTaiWidths()const;//灶台宽度

	vCString Toilet_GetTypes()const;//卫生间类型
	vCString Toilet_GetTaiPenWidths()const;//卫生间类型
	vCString Toilet_GetMaTongTypes()const;//卫生间类型
	vCString Toilet_GetGuanXiWidths()const;//盥洗区宽度---是否支持手动输入？？

	vCString Air_GetPiShus()const;//所有的匹数
	vCString Air_GetLengNingShuiGuanPos()const;//冷凝水管位置
	vCString Air_GetYuShuiGuanPos();
};