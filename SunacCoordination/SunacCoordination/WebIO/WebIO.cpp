#include "StdAfx.h"
#include "WebIO.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <utility>
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"

using namespace std;

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
	m_windowOpenAmount = Window_GetOpenAmount();//开启扇数量
	m_rate = Window_GetRate();//比值
	m_windowWallDis = Window_GetWallDis();//窗户距外墙距离
}

WebIO::~WebIO()
{

}

std::vector<AttrWindow *>  WebIO::GetWindows
(
   double width,//宽度值，注意高度值不作为搜索条件 
   CString openType, //开启类型
   int openNum,//开启扇数量
   CString gongNengQu,//功能区
   double tongFengLiang//通风量
)
{
	std::vector<AttrWindow *> result;

#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*_Window.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrWindow *pAttribute = new AttrWindow();
		CString strid;
		strid.Format(L"%s%d_", L"C",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_filePathName = localFiles[i].second;
		int len = pAttribute->m_name.GetLength();
		pAttribute->m_name.Delete(len - 4, 4);
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"外窗";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrWindow *>  WebIO::GetAllWindows()
{
	std::vector<AttrWindow *> result;

#ifdef WORK_LOCAL//本地模式
	result = GetWindows(0,L"",0,L"",0);
#else

#endif

	return result;
}

std::vector<AttrDoor *> WebIO::GetDoors
(
    double minWid, //宽度值，注意高度值不作为搜索条件 
    CString doorType //门类型
)
{
	std::vector<AttrDoor *> result;
#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrDoor *pAttribute = new AttrDoor();
		CString strid;
		strid.Format(L"%s_%d", L"L_DOOR_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"门";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrDoor *> WebIO::GetAllDoors()
{
	std::vector<AttrDoor *> result;
#ifdef WORK_LOCAL//本地模式
	result = GetDoors(0,L"");
#else

#endif
	return result;
}

std::vector<AttrKitchen *> WebIO::GetKitchens
(
  double kaiJian,//开间
  double jinShen,//进深
  CString weiZhiGuanXi,//门窗位置关系
  CString type,//厨房类型
  bool hasPaiQiDao//是否含有排气道
)
{
	std::vector<AttrKitchen *> result;
#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*_Kitchen.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrKitchen *pAttribute = new AttrKitchen();
		CString strid;
		strid.Format(L"%s_%d", L"L_KITCHEN_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"厨房";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrKitchen *> WebIO::GetAllKitchens()
{
	std::vector<AttrKitchen *> result;
#ifdef WORK_LOCAL//本地模式
	result = GetKitchens(0,0,L"",L"",0);
#else

#endif
	return result;
}

std::vector<AttrToilet *>  WebIO::GetToilets
(
    double kaiJian,//开间
    double jinShen,//进深
    CString weiZhiGuanXi,//门窗位置关系
    CString type,//卫生间类型
    bool hasPaiQiDao//是否含有排气道
)
{
	std::vector<AttrToilet *> result;
#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*_Toilet.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrToilet *pAttribute = new AttrToilet();
		CString strid;
		strid.Format(L"%s_%d", L"L_TOILETN_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"卫生间";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrToilet *> WebIO::GetAllToilets()
{
	std::vector<AttrToilet *> result;
#ifdef WORK_LOCAL//本地模式
	result = GetToilets(0,0,L"",L"",0);
#else

#endif
	return result;
}

std::vector<AttrAirCon *> WebIO::GetAirCons
(
    double piShu,
	CString weiZhi,//冷凝水管位置
	bool hasYuShuiGuan,
	CString yuShuiGuanWeizhi
)
{
	std::vector<AttrAirCon *> result;
#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"*.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrAirCon *pAttribute = new AttrAirCon();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"空调";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrAirCon *> WebIO::GetAllAirCons()
{
	std::vector<AttrAirCon *> result;
#ifdef WORK_LOCAL//本地模式
	result = GetAirCons(0,L"",0,L"");
#else

#endif
	return result;
}

std::vector<AttrRailing *> WebIO::GetRailings(eRailingType type)//一次搜索所有的
{
	std::vector<AttrRailing *> result;
#ifdef WORK_LOCAL//本地模式
	CString localWindowPath = TY_GetLocalFilePath();
	vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"_Railing1.dwg",localWindowPath);
	for (int i = 0; i < localFiles.size(); i++)
	{
		AttrRailing *pAttribute = new AttrRailing();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_yxid = strid;
		pAttribute->m_name = localFiles[i].first;
		pAttribute->m_name.TrimRight(L".dwg");
		pAttribute->m_isJiTuan = true;
		pAttribute->m_isDynamic = true;
		pAttribute->m_type = L"栏杆";

		result.push_back(pAttribute);
		pAttribute->close();
	}
#else

#endif
	return result;
}

std::vector<AttrRailing *> WebIO::GetAllRailings()
{
	std::vector<AttrRailing *> result;
#ifdef WORK_LOCAL//本地模式
	result = GetRailings(TIEYI);
#else

#endif
	return result;
}

vCString WebIO::GetQuyus()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"北京区域");
	strs.push_back(L"华北区域");
	strs.push_back(L"上海区域");
	strs.push_back(L"西南区域");
	strs.push_back(L"东南区域");
	strs.push_back(L"广深区域");
	strs.push_back(L"华中区域");
	strs.push_back(L"海南区域");
#else

#endif
	return strs;
}
vCString WebIO::GetGongNengQus()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"厨房");
	strs.push_back(L"卫生间");
	strs.push_back(L"阳台");
	strs.push_back(L"卧室");
	strs.push_back(L"客厅");
#else

#endif
	return strs;
}
vCString WebIO::GetWindowDoorPoss()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"门窗对开");
	strs.push_back(L"门窗垂直开");
#else

#endif
	return strs;
}
vCString WebIO::Window_GetOpenTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"内开");
	strs.push_back(L"外开");
	strs.push_back(L"推拉");
	strs.push_back(L"上悬");
#else

#endif
	return strs;
}

vCString WebIO::Window_GetOpenAmount()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"1");
	strs.push_back(L"2");
	strs.push_back(L"4");
#else

#endif
	return strs;
}

vCString WebIO::Window_GetRate()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"1/4");
	strs.push_back(L"1/5");
	strs.push_back(L"1/6");
	strs.push_back(L"1/8");
	strs.push_back(L"1/10");
#else

#endif
	return strs;
}

vCString WebIO::Window_GetWallDis()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"0");
	strs.push_back(L"100");
	strs.push_back(L"120");
#else

#endif
	return strs;
}

vCString WebIO::Door_GetTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"推拉");
#else

#endif
	return strs;
}
vCString WebIO::Kitchen_GetTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"深U型");
	strs.push_back(L"浅U型");
	strs.push_back(L"L型");
	strs.push_back(L"I型");
#else

#endif
	return strs;
}
vCString WebIO::Kitchen_GetShuiPenTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单盆600");
	strs.push_back(L"单盆700");
	strs.push_back(L"双盆900");
	strs.push_back(L"双盆1000");
	strs.push_back(L"双盆1200");
#else

#endif
	
	return strs;
}
vCString WebIO::Kitchen_GetBingXiangTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单开门700");
	strs.push_back(L"双开门800");
	strs.push_back(L"对开门1000");
#else

#endif
	
	return strs;
}
vCString WebIO::Kitchen_GetZaoTaiWidths()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"800");
	strs.push_back(L"900");
#else

#endif
	return strs;
}
vCString WebIO::Toilet_GetTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"U型");
	strs.push_back(L"L型");
	strs.push_back(L"I型");
#else

#endif
	return strs;
}
vCString WebIO::Toilet_GetTaiPenWidths()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单盆600");
	strs.push_back(L"单盆700");
	strs.push_back(L"双盆900");
	strs.push_back(L"双盆1000");
	strs.push_back(L"双盆1200");
#else

#endif
	return strs;
}
vCString WebIO::Toilet_GetMaTongTypes()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"800");
	strs.push_back(L"900");
#else

#endif
	return strs;
}
vCString WebIO::Toilet_GetGuanXiWidths()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"950");
	strs.push_back(L"1050");
#else

#endif
	return strs;
}
vCString WebIO::Air_GetPiShus()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"1.0");
	strs.push_back(L"1.5");
	strs.push_back(L"2.0");
	strs.push_back(L"3.0");
#else

#endif
	return strs;
}
vCString WebIO::Air_GetLengNingShuiGuanPos()
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"后边");
	strs.push_back(L"侧边");
#else

#endif
	return strs;
}

int WebIO::DownLoadFile(CString id, CString filePathName)
{
	return 0;
}

//读取尺寸值
SRCDimData WebIO::Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName)
{
	SRCDimData value;
#ifdef WORK_LOCAL//本地模式
	if (fileName == L"Window_N_7_0.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1200);
			value.values.push_back(1500);
		}
		else if (dimStr == L"W1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(400);
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
		}
	}

	if (fileName == L"Window_N_7_1.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1600);
			value.values.push_back(2000);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(750);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(400);
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
		}
	}
	if (fileName == L"Window_N_9_0.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SINGLE;
			value.values.push_back(1800);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(550);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
		}
	}
	if (fileName == L"Window_N_9_1.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(1900);
			value.values.push_back(2300);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(600);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
		}
	}
	if (fileName == L"Window_N_9_2.dwg")
	{
		if (dimStr == L"W")
		{
			value.type = SCOPE;
			value.values.push_back(2400);
			value.values.push_back(3000);
		}
		else if (dimStr == L"W1")
		{
			value.type = SINGLE;
			value.values.push_back(750);
		}
		else if (dimStr == L"W2")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H")
		{
			value.type = UNLIMIT;
		}
		else if (dimStr == L"H1")
		{
			value.type = CALC;
		}
		else if (dimStr == L"H2")
		{
			value.type = MULTI;
			value.values.push_back(500);
			value.values.push_back(950);
			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
		}
	}
#else

#endif

	return value;
}