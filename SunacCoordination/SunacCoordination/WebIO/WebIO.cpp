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

}

WebIO::~WebIO()
{

}

//width,//宽度值，注意高度值不作为搜索条件 
//openType, //开启类型
//openNum,//开启扇数量
//gongNengQu,//功能区
//tongFengLiang//通风量
//读取门和窗
std::vector<AttrWindow >  WebIO::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//本地模式
	return m_windowLocalData.GetWindows(width, openType, openNum, gongNengQu);
#else
	return m_windowWebData.GetWindows(width, openType, openNum, gongNengQu);
#endif
}
std::vector<AttrWindow > WebIO::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//本地模式
	return m_windowLocalData.GetDoors(width, openType, openNum, gongNengQu);
#else
return m_windowWebData.GetDoors(width, openType, openNum, gongNengQu);
#endif
}

std::vector<AttrKitchen *> WebIO::GetKitchens
(
  double kaiJian,//开间
  double jinShen,//进深
  CString weiZhiGuanXi,//门窗位置关系
  CString type,//厨房类型
  bool hasPaiQiDao,//是否含有排气道
  bool bDynatic
)
{
	std::vector<AttrKitchen *> result;

#ifdef WORK_LOCAL//本地模式
	if (bDynatic==false)
	{
		return result;
	}

	CString localFile;

	if (type == L"U型")
	{
		if (kaiJian>jinShen)
		{
			if (weiZhiGuanXi == L"门窗对开")
			{
				if (jinShen < 2000)
					localFile = L"KUq_Dui.dwg";
				else
					localFile = L"KUq_Dui_ZhuanJIao.dwg";
			}
			else
			{
				if (jinShen < 2000)
					localFile = L"KUq_Chui.dwg";
				else
					localFile = L"KUq_Chui_ZhuanJIao.dwg";
			}
		}
		else
		{
			localFile = L"KUs.dwg";
		}
	}
	else if (type == L"L型")
		localFile = L"KL.dwg";
	else if (type == L"L型")
		localFile = L"KI.dwg";
	else
		return result;

	if (hasPaiQiDao)
		localFile.Replace(_T(".dwg"), _T("_P.dwg"));

	AttrKitchen *pAttribute = new AttrKitchen();
	if (type == L"U型")
		pAttribute->m_prototypeCode.Format(L"%s_%.0lf×%.0lf", localFile.Left(3), kaiJian, jinShen);
	else
		pAttribute->m_prototypeCode.Format(L"%s_%.0lf×%.0lf", localFile.Left(2), kaiJian, jinShen);
	if (weiZhiGuanXi == L"门窗垂直")
		pAttribute->m_prototypeCode += L"_c";

	pAttribute->m_isJiTuan = true;
	pAttribute->m_isDynamic = true;
	pAttribute->m_type = L"厨房";
	pAttribute->m_fileName = localFile;
	pAttribute->m_kitchenType = type;
	pAttribute->m_windowDoorPos = weiZhiGuanXi;

	result.push_back(pAttribute);
	pAttribute->close();
#else

#endif
	return result;
}

std::vector<AttrKitchen *> WebIO::GetAllKitchens()
{
	std::vector<AttrKitchen *> result;
#ifdef WORK_LOCAL//本地模式
	std::vector<AttrKitchen *> result1 = GetKitchens(0, 0, L"", L"", 0, false);
	result.insert(result.end(), result1.begin(), result1.end());

	std::vector<AttrKitchen *> result2 = GetKitchens(0, 0, L"", L"", 0, true);
	result.insert(result.end(), result2.begin(), result2.end());
#else

#endif
	return result;
}

std::vector<CPrototypeInfo> WebIO::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
	std::vector<CPrototypeInfo> ret;

	int xLen = int(p_xLen + 0.5);
	int yLen = int(p_yLen + 0.5);

	CString sType;

	switch (p_type)
	{
	case E_BATHROOM_I:
		sType = L"TI";
		break;
	case E_BATHROOM_L:
		sType = L"TL";
		break;
	case E_BATHROOM_U:
		sType = L"TU";
		break;
	}

	std::vector<CPrototypeInfo> allBathrooms = WebIO::GetAllBathrooms();

	for (UINT i = 0; i < allBathrooms.size(); i++)
		if (allBathrooms[i].m_sType.Left(2) == sType && allBathrooms[i].MatchPrototype(xLen, yLen, p_doorDir, p_windowDir))
			ret.push_back(allBathrooms[i]);

	return ret;
}

std::vector<CPrototypeInfo> WebIO::GetAllBathrooms()
{
	std::vector<CPrototypeInfo> result;

#ifdef WORK_LOCAL
	result.resize(12);

	result[0].m_sFileName = L"TI3.dwg";
	result[0].m_sType = L"TI3";
	result[0].m_doorPos = E_DIR_BOTTOM;
	result[0].m_windowPos = E_DIR_TOP;
	result[0].AddSizeRange(1600, 2450, 1600, 3050);
	result[0].AddSizeRange(1700, 2450, 1850, 3050);

	result[1].m_sFileName = L"TI3_g.dwg";
	result[1].m_sType = L"TI3G";
	result[1].m_doorPos = E_DIR_BOTTOM;
	result[1].m_windowPos = E_DIR_TOP;
	result[1].AddSizeRange(1600, 2750, 1600, 3200);
	result[1].AddSizeRange(1700, 2750, 1850, 3200);

	result[2].m_sFileName = L"TI4.dwg";
	result[2].m_sType = L"TI4";
	result[2].m_doorPos = E_DIR_BOTTOM;
	result[2].m_windowPos = E_DIR_TOP;
	result[2].AddSizeRange(1600, 3050, 1600, 3500);
	result[2].AddSizeRange(1700, 3050, 1850, 3500);

	result[3].m_sFileName = L"TI4_g.dwg";
	result[3].m_sType = L"TI4G";
	result[3].m_doorPos = E_DIR_BOTTOM;
	result[3].m_windowPos = E_DIR_TOP;
	result[3].AddSizeRange(1600, 3500, 1600, 3650);
	result[3].AddSizeRange(1700, 3500, 1850, 3650);

	result[4].m_sFileName = L"TL3.dwg";
	result[4].m_sType = L"TL3";
	result[4].m_doorPos = E_DIR_BOTTOM;
	result[4].m_windowPos = E_DIR_RIGHT;
	result[4].AddSizeRange(1700, 1850, 1850, 2300);
	//不支持以下尺寸
	result[4].DeleteSize(1700, 1850);
	result[4].DeleteSize(1700, 2000);

	result[5].m_sFileName = L"TL3_标准淋浴房.dwg";
	result[5].m_sType = L"TL3B";
	result[5].m_doorPos = E_DIR_BOTTOM;
	result[5].m_windowPos = E_DIR_RIGHT;
	result[5].AddSizeRange(2000, 1850, 2150, 2300);

	result[6].m_sFileName = L"TL4.dwg";
	result[6].m_sType = L"TL4";
	result[6].m_doorPos = E_DIR_BOTTOM;
	result[6].m_windowPos = E_DIR_RIGHT;
	result[6].AddSizeRange(1700, 2450, 1850, 2750);

	result[7].m_sFileName = L"TL4_标准淋浴房.dwg";
	result[7].m_sType = L"TL4B";
	result[7].m_doorPos = E_DIR_BOTTOM;
	result[7].m_windowPos = E_DIR_RIGHT;
	result[7].AddSizeRange(2000, 2450, 2150, 2450);

	result[8].m_sFileName = L"TU3-1600X2450.dwg";
	result[8].m_sType = L"TU3";
	result[8].m_doorPos = E_DIR_LEFT;
	result[8].m_windowPos = E_DIR_RIGHT;
	result[8].AddSize(1600, 2450);

	result[9].m_sFileName = L"TU3-1850X2000.dwg";
	result[9].m_sType = L"TU3";
	result[9].m_doorPos = E_DIR_LEFT;
	result[9].m_windowPos = E_DIR_TOP;
	result[9].AddSize(1850, 2000);

	result[10].m_sFileName = L"TU3-1850X2750.dwg";
	result[10].m_sType = L"TU3";
	result[10].m_doorPos = E_DIR_LEFT;
	result[10].m_windowPos = E_DIR_TOP;
	result[10].AddSize(1850, 2750);

	result[11].m_sFileName = L"TU4-2000X2750";
	result[11].m_sType = L"TU4";
	result[11].m_doorPos = E_DIR_LEFT;
	result[11].m_windowPos = E_DIR_TOP;
	result[11].AddSize(2000, 2750);
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
	for (UINT i = 0; i < localFiles.size(); i++)
	{
		AttrAirCon *pAttribute = new AttrAirCon();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_prototypeCode = strid;
		pAttribute->m_fileName = localFiles[i].first;
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
	for (UINT i = 0; i < localFiles.size(); i++)
	{
		AttrRailing *pAttribute = new AttrRailing();
		CString strid;
		strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
		pAttribute->m_prototypeCode = strid;
		pAttribute->m_fileName = localFiles[i].first;
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
	result = GetRailings(E_RAILING_TIEYI);
#else

#endif
	return result;
}


int WebIO::DownLoadFile(CString id, CString filePathName)
{
	return 0;
}
//
////读取尺寸值
//SRCDimData WebIO::Window_GetDim(CString yuanXingBianHao, CString dimStr, CString fileName)
//{
//	SRCDimData value;
//#ifdef WORK_LOCAL//本地模式
//	if (fileName == L"Window_N_7_0.dwg")
//	{
//		if (dimStr == L"W")
//		{
//			value.type = SCOPE;
//			value.values.push_back(1200);
//			value.values.push_back(1500);
//		}
//		else if (dimStr == L"W1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"W2")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H2")
//		{
//			value.type = MULTI;
//			value.values.push_back(400);
//			value.values.push_back(500);
//			value.values.push_back(950);
//			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
//		}
//	}
//
//	if (fileName == L"Window_N_7_1.dwg")
//	{
//		if (dimStr == L"W")
//		{
//			value.type = SCOPE;
//			value.values.push_back(1600);
//			value.values.push_back(2000);
//		}
//		else if (dimStr == L"W1")
//		{
//			value.type = SINGLE;
//			value.values.push_back(750);
//		}
//		else if (dimStr == L"W2")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H")
//		{
//			value.type = UNLIMIT;
//		}
//		else if (dimStr == L"H1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H2")
//		{
//			value.type = MULTI;
//			value.values.push_back(400);
//			value.values.push_back(500);
//			value.values.push_back(950);
//			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
//		}
//	}
//	if (fileName == L"Window_N_9_0.dwg")
//	{
//		if (dimStr == L"W")
//		{
//			value.type = SINGLE;
//			value.values.push_back(1800);
//		}
//		else if (dimStr == L"W1")
//		{
//			value.type = SINGLE;
//			value.values.push_back(550);
//		}
//		else if (dimStr == L"W2")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H")
//		{
//			value.type = UNLIMIT;
//		}
//		else if (dimStr == L"H1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H2")
//		{
//			value.type = MULTI;
//			value.values.push_back(500);
//			value.values.push_back(950);
//			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
//		}
//	}
//	if (fileName == L"Window_N_9_1.dwg")
//	{
//		if (dimStr == L"W")
//		{
//			value.type = SCOPE;
//			value.values.push_back(1900);
//			value.values.push_back(2300);
//		}
//		else if (dimStr == L"W1")
//		{
//			value.type = SINGLE;
//			value.values.push_back(600);
//		}
//		else if (dimStr == L"W2")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H")
//		{
//			value.type = UNLIMIT;
//		}
//		else if (dimStr == L"H1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H2")
//		{
//			value.type = MULTI;
//			value.values.push_back(500);
//			value.values.push_back(950);
//			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
//		}
//	}
//	if (fileName == L"Window_N_9_2.dwg")
//	{
//		if (dimStr == L"W")
//		{
//			value.type = SCOPE;
//			value.values.push_back(2400);
//			value.values.push_back(3000);
//		}
//		else if (dimStr == L"W1")
//		{
//			value.type = SINGLE;
//			value.values.push_back(750);
//		}
//		else if (dimStr == L"W2")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H")
//		{
//			value.type = UNLIMIT;
//		}
//		else if (dimStr == L"H1")
//		{
//			value.type = CALC;
//		}
//		else if (dimStr == L"H2")
//		{
//			value.type = MULTI;
//			value.values.push_back(500);
//			value.values.push_back(950);
//			value.prompt = L"卧室凸窗推荐值950;卧室非凸窗推荐值500;厨房卫生间推荐值400;";
//		}
//	}
//#else
//
//#endif
//
//	return value;
//}