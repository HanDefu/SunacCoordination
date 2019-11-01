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
#include "WindowLocalDataFromDB.h"
#include "AirConditionLocalData.h"
#include "AirConditionLocalDataFromDB.h"
#include "AirconditionWebData.h"
#include "KitchenBathroomLocalData.h"
#include "KitchenBathroomWebData.h"
#include "RailingWebData.h"


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

	const CConfigDictionary* GetConfigDict() { return &m_configDic; }

	bool Login(CString p_sUserName, CString p_key);
	bool IsLogin()const { return m_bLogin; }

public:
	//width宽度值，注意高度值不作为搜索条件 
	//openType开启类型，openNum开启扇数量， gongNengQu功能区， tongFengLiang通风量
	std::vector<AttrWindow >  GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const;
	std::vector<AttrWindow >  GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const;

	std::vector<AttrWindow> GetAllWindows(); //获取所有窗户
	std::vector<AttrWindow> GetAllDoors();  //获取所有门

	//////////////////////////////////////////////////////////////////////////

	std::vector<AttrKitchen> GetKitchens (EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao);
	std::vector<AttrKitchen> GetAllKitchens();

	std::vector<AttrBathroom> GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir);
	std::vector<AttrBathroom> GetAllBathrooms();

	//weiZhi冷凝水管位置
	std::vector<AttrAirCon> GetAirCons(double piShu, CString weiZhi, CString hasYuShuiGuan, CString yuShuiGuanWeizhi);
	std::vector<AttrAirCon > GetAllAirCons();

	std::vector<AttrRailing> GetRailings(eRailingType type);
	std::vector<AttrRailing> GetAllRailings();


	//从web下载原型文件， ok返回0 否则返回-1
	bool DownloadFile(const int fileId, CString type, CString filePathName);

	//获得从Web端下载文件的文件名（包含扩展名）
	CString GetFileName(const WCHAR *fullname);

	//获取登陆的用户ID
	int GetUserID();


protected:
	CConfigDictionary m_configDic;

	//CWindowLocalData m_windowLocalData;
	//CWindowLocalDataFromDB m_windowLocalData;
	//CWindowWebData m_windowWebData;

	//CAirConditionLocalData m_airConLocalData;
	//CAirConditionLocalDataFromDB m_airConLocalData;
	//CAirConditionWebData m_airConWebData;

	CKitchenBathroomLocalData m_kitchenBathroomLocalData;
	CKitchenBathroomWebData m_kitchenBathroomWebData;

	CRailingWebData m_railingWebData;

	bool m_bLogin;	
}; 

#define WEBINST (WebIO::GetInstance())

