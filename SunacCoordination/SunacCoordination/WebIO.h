#pragma once
#include "Sunac_def.h"
#include "Object/AttrAirCon.h"
#include "Object/AttrDoor.h"
#include "Object/AttrKitchen.h"
#include "Object/AttrRailing.h"
#include "Object/AttrToilet.h"
#include "Object/AttrWindow.h"
class WebIO
{
public:
	static WebIO *GetInstance()
	{
		static WebIO instance;
		return &instance;
	}
	WebIO();
	~WebIO();

public:
	//-----------------原型搜索函数--------------//
	//注意外部需要释放返回的内存
	static std::vector<AttrWindow *>  GetWindows
	(
		double width,//宽度值，注意高度值不作为搜索条件 
		CString openType, //开启类型
		int openNum,//开启扇数量
		CString gongNengQu,//功能区
		double tongFengLiang//通风量
	);
	//注意外部需要释放返回的内存
	static std::vector<AttrWindow *>  GetAllWindows();

	//门窗读取尺寸值
	SRCDimData Window_GetDim
	(
		CString yuanXingBianHao, 
		CString dimStr,//L"H2"
		CString fileName//本地状态根据文件名定义尺寸类型
	);//读取尺寸取值
	//注意外部需要释放返回的内存
	static std::vector<AttrDoor *> GetDoors
	(
		double minWid, //宽度值，注意高度值不作为搜索条件 
		CString doorType //门类型
	);
	//注意外部需要释放返回的内存
	static std::vector<AttrDoor *> GetAllDoors();
	//注意外部需要释放返回的内存
	static std::vector<AttrKitchen *> GetKitchens
	(
		double kaiJian,//开间
		double jinShen,//进深
		CString weiZhiGuanXi,//门窗位置关系
		CString type,//厨房类型
		bool hasPaiQiDao//是否含有排气道
	);
	//注意外部需要释放返回的内存
	static std::vector<AttrKitchen *> GetAllKitchens();
	//注意外部需要释放返回的内存
	static std::vector<AttrToilet *> GetToilets
	(
		double kaiJian,//开间
		double jinShen,//进深
		CString weiZhiGuanXi,//门窗位置关系
		CString type,//卫生间类型
		bool hasPaiQiDao//是否含有排气道
	);
	//注意外部需要释放返回的内存
	static std::vector<AttrToilet *> GetAllToilets();
	//注意外部需要释放返回的内存
	static std::vector<AttrAirCon *> GetAirCons
	(
		double piShu,
		CString weiZhi,//冷凝水管位置
		bool hasYuShuiGuan
	);
	//注意外部需要释放返回的内存
	static std::vector<AttrAirCon *> GetAllAirCons();
	//注意外部需要释放返回的内存
	static std::vector<AttrRailing *> GetRailings(eRailingType type);
	//注意外部需要释放返回的内存
	static std::vector<AttrRailing *> GetAllRailings();

	//------其他---------//
	//输入原型编号，下载到固定地址
	//ok返回0 否则返回-1
	static int DownLoadFile(CString id, CString filePathName);
public:
	vCString m_quyus;//区域
	vCString  m_gongNengQus;//功能区
	vCString  m_windowDoorPoss;//门窗位置关系--厨房卫生间
	vCString m_windowOpenTypes;//外窗开启类型
	vCString m_doorTypes;//门类型
	vCString m_kitchenTypes;//厨房类型
	vCString m_kitchenShuiPenTypes;//厨房水盆类型
	vCString m_kitchenBingXiangTypes;//厨房冰箱类型
	vCString m_kitchenZaoTaiWidths;//厨房灶台宽度
	vCString m_toiletTypes;//卫生间类型
	vCString m_toiletTaiPenWidths;//卫生间类型
	vCString m_toiletMaTongTypes;//卫生间类型
	vCString m_toiletGuanXiWidths;//卫生间盥洗区宽度---是否支持手动输入？？
	vCString m_airPiShus;//空调所有的匹数
	vCString m_airLengNingShuiGuanPos;//空调冷凝水管位置

protected:
	//---------------通用查字典-----------------------//
	vCString GetQuyus();//区域
	vCString GetGongNengQus();//功能区
	vCString GetWindowDoorPoss();//门窗位置关系

	vCString Window_GetOpenTypes();//开启类型

	vCString Door_GetTypes();//门类型

	vCString Kitchen_GetTypes();//厨房类型
	vCString Kitchen_GetShuiPenTypes();//水盆类型
	vCString Kitchen_GetBingXiangTypes();//冰箱类型
	vCString Kitchen_GetZaoTaiWidths();//灶台宽度

	vCString Toilet_GetTypes();//卫生间类型
	vCString Toilet_GetTaiPenWidths();//卫生间类型
	vCString Toilet_GetMaTongTypes();//卫生间类型
	vCString Toilet_GetGuanXiWidths();//盥洗区宽度---是否支持手动输入？？

	vCString Air_GetPiShus();//所有的匹数
	vCString Air_GetLengNingShuiGuanPos();//冷凝水管位置
}; 

#define WEBINST (WebIO::GetInstance())

