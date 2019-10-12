#pragma once
#include "../Sunac_def.h"
#include "../Object/AirCondition/AttrAirCon.h"
#include "../Object/Kitchen/AttrKitchen.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Object/Bathroom/AttrBathroom.h"
#include "../Object/WindowDoor/AttrWindow.h"
#include "ConfigDictionary.h"
#include "WindowLocalData.h"
#include "WindowWebData.h"
#include "AirConditionLocalData.h"
#include "KitchenBathroomLocalData.h"


class WebIO
{
	WebIO();
public:
	~WebIO();

	static WebIO *GetInstance()
	{
		static WebIO instance;
		return &instance;
	}

	static const CConfigDictionary* GetConfigDict() { return &(GetInstance()->m_configDic); }
	//static const CWindowLocalData* GetWindowLocalData() { return &(GetInstance()->m_windowLocalData); }
	static const CAirConditionLocalData* GetAirConditionLocalData() { return &(GetInstance()->m_airConLocalData); }

public:
	////-----------------原型搜索函数--------------//
	//width,//宽度值，注意高度值不作为搜索条件 
	//openType, //开启类型
	//openNum,//开启扇数量
	//gongNengQu,//功能区
	//tongFengLiang//通风量
	//读取门和窗
	std::vector<AttrWindow >  GetWindows(double width, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, CString openType, int openNum, CString gongNengQu)const;

	vector<AttrWindow> GetAllWindows(); //获取所有窗户
	vector<AttrWindow> GetAllDoors();  //获取所有门

	//////////////////////////////////////////////////////////////////////////

	//注意外部需要释放返回的内存
	static std::vector<AttrKitchen> GetKitchens (EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao);
	
	static std::vector<AttrKitchen> GetAllKitchens();
	//获取卫生间
	static std::vector<AttrBathroom> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	static std::vector<AttrBathroom> GetAllBathrooms();
	//注意外部需要释放返回的内存
	static std::vector<AttrAirCon *> GetAirCons(double piShu,
												CString weiZhi,//冷凝水管位置
												bool hasYuShuiGuan,
												CString yuShuiGuanWeizhi);
	//注意外部需要释放返回的内存
	static std::vector<AttrAirCon *> GetAllAirCons();
	//注意外部需要释放返回的内存
	static std::vector<AttrRailing *> GetRailings(eRailingType type);
	//注意外部需要释放返回的内存
	static std::vector<AttrRailing *> GetAllRailings();

	//------其他---------//
	//从web下载原型文件， ok返回0 否则返回-1
	static bool DownLoadFile(const int fileId, const CString filePathName);

protected:
	CConfigDictionary m_configDic;

	CWindowLocalData m_windowLocalData;
	CAirConditionLocalData m_airConLocalData;
	CKitchenBathroomLocalData m_kitchenBathroomLocalData;

	CWindowWebData m_windowWebData;
}; 

#define WEBINST (WebIO::GetInstance())

