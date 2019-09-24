#pragma once
#include "../Sunac_def.h"
#include "../Object/AttrAirCon.h"
#include "../Object/AttrDoor.h"
#include "../Object/AttrKitchen.h"
#include "../Object/AttrRailing.h"
#include "../Object/AttrToilet.h"
#include "../Object/AttrWindow.h"
#include "ConfigDictionary.h"


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

	static const CConfigDictionary* GetConfigDict() {	return &(GetInstance()->m_configDic);	}

public:
	//-----------------原型搜索函数--------------//
	//注意外部需要释放返回的内存
	static std::vector<AttrWindow *>  GetWindows
	(
		double width,//宽度值，注意高度值不作为搜索条件 
		double height,
		CString openType, //开启类型
		int openNum,//开启扇数量
		CString gongNengQu,//功能区
		double tongFengLiang//通风量
	);

	//注意外部需要释放返回的内存
	static std::vector<AttrWindow *>  GetAllWindows();

	//门窗读取尺寸值， yuanXingBianHao原型编号， dimStr字符串类(如"H2")，fileName文件名,本地状态根据文件名定义尺寸类型读取尺寸取值
	SRCDimData Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName);



	//////////////////////////////////////////////////////////////////////////
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
		CString weiZhiGuanXi,//门窗位置关系 门窗对开/门窗垂直开
		CString type,//厨房类型 I型
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
	//输入原型编号，下载到固定地址
	//ok返回0 否则返回-1
	static int DownLoadFile(CString id, CString filePathName);



protected:
	CConfigDictionary m_configDic;

}; 

#define WEBINST (WebIO::GetInstance())

