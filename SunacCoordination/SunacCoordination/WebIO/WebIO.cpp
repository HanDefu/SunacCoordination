#include "StdAfx.h"
#include "WebIO.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <utility>
#include <iostream>
#include <fstream>
#include <xstring>
#include <atlconv.h> 
#include <stdlib.h>
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_Sunac.h"
#include "cpp_base64\src\Base64Decoder.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "WindowWebData.h"
#include "AirconditionWebData.h"
#include "KitchenBathroomWebData.h"
#include "RailingWebData.h"

using namespace std;

WebIO::WebIO()
{
	m_bLogin = false;
}

WebIO::~WebIO()
{

}

bool WebIO::Login(CString p_sUserName, CString p_key)
{
	//TODO
	m_bLogin = true;
	return true;
}
//width,//宽度值，注意高度值不作为搜索条件 
//openType, //开启类型
//openNum,//开启扇数量
//gongNengQu,//功能区
//tongFengLiang//通风量
//读取门和窗
std::vector<AttrWindow>  WebIO::GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL		//本地模式
	vAttrWindow Local = m_windowLocalData.GetWindows(width, openType, openNum, gongNengQu);
	return Local;
#else
	vAttrWindow Local = m_windowLocalData.GetWindows(width, openType, openNum, gongNengQu);
	vAttrWindow Web = m_windowWebData.GetWindows(width, height, openType, openNum, gongNengQu);

	for(int i = 0; i < Web.size(); i++ )
	{
		AttrWindow &curWebWin = Web[i];

		//从window1找到相同编号的
		bool bFind = false;
		for (UINT j = 0; j < Local.size(); j++)
		{
			if (curWebWin.m_prototypeCode == Local[j].m_prototypeCode)
			{
				bFind = true;
				if(Local[j].IsPrototypeEqual(Web[i]))
				{
					AfxMessageBox(L"确实相等！");
				}
				else
				{
					AfxMessageBox(L"不完全相等！");
				}
				break;
			}
		}

	}
	return Local;
#endif
}
std::vector<AttrWindow> WebIO::GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//本地模式
	return m_windowLocalData.GetDoors(width, openType, openNum, gongNengQu);
#else
	return m_windowWebData.GetDoors(width, height, openType, openNum, gongNengQu);
#endif
}

std::vector<AttrKitchen> WebIO::GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao)
{
#ifdef WORK_LOCAL//本地模式
	return m_kitchenBathroomLocalData.GetKitchens(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir, p_hasPaiQiDao);
#else
	return m_kitchenBathroomWebData.GetKitchens(p_xLen, p_yLen, L"", KitchenTypeToCString(p_type), (p_hasPaiQiDao == true)? 1:0);
#endif
}

std::vector<AttrKitchen> WebIO::GetAllKitchens()
{
#ifdef WORK_LOCAL//本地模式
	return m_kitchenBathroomLocalData.GetAllKitchens();
#else
	std::vector<AttrKitchen> ret;
	return ret;
#endif
}

std::vector<AttrBathroom> WebIO::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
#ifdef WORK_LOCAL//本地模式
	return m_kitchenBathroomLocalData.GetBathrooms(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir);
#else
	std::vector<AttrBathroom> ret;
	return ret;
#endif
}

std::vector<AttrBathroom> WebIO::GetAllBathrooms()
{
#ifdef WORK_LOCAL//本地模式
	return m_kitchenBathroomLocalData.GetAllBathrooms();
#else
	std::vector<AttrBathroom> ret;
	return ret;
#endif
}

//weiZhi冷凝水管位置
std::vector<AttrAirCon> WebIO::GetAirCons(double piShu, CString weiZhi, CString hasYuShuiGuan, CString yuShuiGuanWeizhi)
{
#ifdef WORK_LOCAL//本地模式
	return m_airConLocalData.GetAirCons(piShu, weiZhi, hasYuShuiGuan, yuShuiGuanWeizhi);
#else
	return m_airConWebData.GetAirCons(piShu, weiZhi, (hasYuShuiGuan == L"有"), yuShuiGuanWeizhi);
#endif
}

std::vector<AttrAirCon> WebIO::GetAllAirCons()
{
#ifdef WORK_LOCAL//本地模式
	return m_airConLocalData.GetAllAirCons();
#else
	return m_airConWebData.GetAllAirCons();
#endif
}

std::vector<AttrRailing> WebIO::GetRailings(eRailingType type)//一次搜索所有的
{
#ifdef WORK_LOCAL//本地模式
	std::vector<AttrRailing> result;
	//CString localWindowPath = TY_GetPrototypeFilePath();
	//vector<pair<CString,CString>> localFiles = TY_FindFilesInDirecotry(L"_Railing1.dwg",localWindowPath);
	//for (UINT i = 0; i < localFiles.size(); i++)
	//{
	//	AttrRailing pAttribute;
	//	CString strid;
	//	strid.Format(L"%s_%d", L"L_AIRCON_",i+1);
	//	pAttribute.m_prototypeCode = strid;
	//	pAttribute.SetFileName(localFiles[i].first);
	//	pAttribute.m_isJiTuan = true;
	//	pAttribute.m_isDynamic = true;
	//	pAttribute.close();

	//	//TODO 补全栏杆其他的属性初始化

	//	result.push_back(pAttribute);
	//}
	return result;
#else
	std::vector<AttrRailing> result;
	return result;
#endif
}

std::vector<AttrRailing> WebIO::GetAllRailings()
{
#ifdef WORK_LOCAL//本地模式
	std::vector<AttrRailing> result1 = GetRailings(E_RAILING_TIEYI);
	std::vector<AttrRailing> result2 = GetRailings(E_RAILING_BOLI);
	result1.insert(result1.end(), result2.begin(), result2.end());
	return result1;
#else
	std::vector<AttrRailing> result1 = GetRailings(E_RAILING_TIEYI);
	std::vector<AttrRailing> result2 = GetRailings(E_RAILING_BOLI);
	result1.insert(result1.end(), result2.begin(), result2.end());
	return result1;
#endif
}

//wstring=>string
std::string WString2String(const std::wstring& ws)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const wchar_t* wchSrc = ws.c_str();
	size_t nDestSize = wcstombs((char*)NULL, wchSrc, 0) + 1;
	char *chDest = new char[nDestSize];
	memset(chDest, 0, nDestSize);
	wcstombs(chDest, wchSrc, nDestSize);
	std::string strResult = chDest;
	delete[]chDest;
	setlocale(LC_ALL, strLocale.c_str());
	return strResult;
}
// string => wstring
std::wstring String2WString(const std::string& s)
{
	std::string strLocale = setlocale(LC_ALL, "");
	const char* chSrc = s.c_str();
	size_t nDestSize = mbstowcs((wchar_t*)NULL, chSrc, 0) + 1;
	wchar_t* wchDest = new wchar_t[nDestSize];
	wmemset(wchDest, 0, nDestSize);
	mbstowcs(wchDest, chSrc, nDestSize);
	std::wstring wstrResult = wchDest;
	delete[]wchDest;
	setlocale(LC_ALL, strLocale.c_str());
	return wstrResult;
}

bool WebIO::DownloadFile(const int fileId, CString type, CString filePathName)
{
	std::wstring sType = type;

	_ns1__CadFileDownload nsCadFile;
	nsCadFile.Id = fileId;
	nsCadFile.Type = &sType;
	_ns1__CadFileDownloadResponse cadFileResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.CadFileDownload(&nsCadFile, cadFileResponse);

	wstring* swReturn = cadFileResponse.CadFileDownloadResult;
	if (swReturn == NULL)
	{
		return false;
	}

	std::string sReturn = WString2String(*swReturn);

	Base64Decoder decoder;

	wstring filenameOut = filePathName;
	ofstream ofs(filenameOut, ofstream::out | ofstream::binary);
	if (ofs)
	{
		int numberOfBytes = (int)sReturn.length() + 1;
		char* decodedBuffer = new char[numberOfBytes * 2];
		memset(decodedBuffer, 0, numberOfBytes * 2);

		int numberOfBytesDecoded = decoder.decode(sReturn.c_str(), numberOfBytes, decodedBuffer);
		ofs.write(decodedBuffer, numberOfBytesDecoded);
	}
	else
	{
		wcout << L"Cannot open file: " << filenameOut << endl;
	}

	ofs.close();

	return true;
}

CString WebIO::GetFileName(const WCHAR *fullname)
{
	WCHAR filename[256]; 
	WCHAR name[256];
	wcscpy_s(name,fullname);
	WCHAR *p = wcstok(name,L"/");
	WCHAR *pre = NULL;
	while(p != NULL)
	{
		pre = p;
		p = wcstok(NULL, L"/");
	}
	wcscpy_s(filename,pre);
	return filename;
}

CString KitchenTypeToCString(EKitchType type)
{
	switch(type)
	{
	case E_KITCH_U:
		return L"U型";
		break;
	case E_KITCH_L:
		return L"L型";
		break;
	case E_KITCH_I:
		return L"I型";
		break;
	case E_KITCH_OTHER:
		return L"其他";
		break;
	case E_KITCH_ALL:
		return L"所有类型";
		break;
	default:
		return L"";
		break;
	}
}