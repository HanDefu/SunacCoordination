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
	bool GetConfig(wstring Term);
	bool GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut);

protected:
	map<CString, vCString> m_configDict;


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

	vCString Bathroom_GetTypes()const;//卫生间类型
	vCString Bathroom_GetTaiPenWidths()const;//卫生间类型
	vCString Bathroom_GetMaTongTypes()const;//卫生间类型
	vCString Bathroom_GetGuanXiWidths()const;//盥洗区宽度---是否支持手动输入？？

	vCString Air_GetPiShus()const;//所有的匹数
	vCString Air_GetLengNingShuiGuanPos()const;//冷凝水管位置
	vCString Air_GetYuShuiGuanPos() const;
	vCString Railing_GetTypes();

};