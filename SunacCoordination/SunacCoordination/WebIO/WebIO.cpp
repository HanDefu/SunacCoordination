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
std::vector<AttrWindow>  WebIO::GetWindows(double width, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//本地模式
	return m_windowLocalData.GetWindows(width, openType, openNum, gongNengQu);
#else
	return m_windowWebData.GetWindows(width, openType, openNum, gongNengQu);
#endif
}
std::vector<AttrWindow> WebIO::GetDoors(double width, CString openType, int openNum, CString gongNengQu)const
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

std::vector<AttrBathroom> WebIO::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
	std::vector<AttrBathroom> ret;

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

	std::vector<CProBathroom> prototypes = CKitchenBathroomLocalData::GetInstance()->GetAllBathroomPrototypes();

	for (UINT i = 0; i < prototypes.size(); i++)
	{
		if ((p_type == E_BATHROOM_ALL || prototypes[i].m_sType.Left(2) == sType) && prototypes[i].MatchPrototype(xLen, yLen, p_doorDir, p_windowDir))
		{
			AttrBathroom attr(p_xLen, p_yLen, p_doorDir, p_windowDir, prototypes[i]);
			ret.push_back(attr);
		}
	}

	return ret;
}

std::vector<AttrBathroom> WebIO::GetAllBathrooms()
{
	std::vector<AttrBathroom> ret;
	std::vector<CProBathroom> prototypes = CKitchenBathroomLocalData::GetInstance()->GetAllBathroomPrototypes();
	for (UINT i = 0; i < prototypes.size(); i++)
	{
		AttrBathroom attr;
		//只绑定原型，其余不填
		attr.m_fileName = prototypes[i].m_sFileName;
		ret.push_back(attr);
	}

	return ret;
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

CProBase* WebIO::GetPrototypeByFileName(CString p_sFileName)
{
	CProBase* ret = NULL;
#ifdef WORK_LOCAL
	//暂时只支持返回卫生间
	ret = CKitchenBathroomLocalData::GetInstance()->GetBathroomPrototypeByFileName(p_sFileName);
	return ret;
#endif
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