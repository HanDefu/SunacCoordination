#include "StdAfx.h"
#include "WebIO.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <vector>
#include <algorithm>
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
#include "math.h"

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
	_ns1__CheckUserInfo ns;
	_ns1__CheckUserInfoResponse nsResponse;

	std::wstring sUserName, sKey;
	sUserName = p_sUserName;
	sKey = p_key;

	ns.userName = &sUserName;
	ns.password = &sKey;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.CheckUserInfo(&ns, nsResponse);


	//判断返回结果是否成功
	if (nsResponse.CheckUserInfoResult == NULL)
	{
		m_bLogin = false;
		return false;
	}


	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.CheckUserInfoResult)).c_str());

	if (ParseLoginInfo(xml))
	{
		m_userName = p_sUserName;
		m_password = p_key;
		m_bLogin = true;
		return true;
	}
	else
	{
		m_bLogin = false;
		return false;
	}
	
	
}

int WebIO::GetUserID()
{
	return m_userID;
}

CString WebIO::GetUserName()
{
	return m_userName;
}

bool WebIO::ParseLoginInfo(CMarkup xml)
{
	xml.ResetMainPos();
	xml.FindElem();	//根节点

	xml.IntoElem();
	{
		if (xml.FindElem(_T("Code")))
		{
			if(_ttoi(xml.GetData()) != 100)
			{
				return false;
			}
		}
		if (xml.FindElem(_T("User")))
		{
			xml.IntoElem();
			{
				if (xml.FindElem(_T("Id")))
				{
					m_userID = _ttoi(xml.GetData());
				}
			}
			xml.OutOfElem();
		}
	}
	xml.OutOfElem();
	return true;
}

//注意高度值不作为搜索条件 
//width宽度值，openType开启类型, openNum开启扇数量  gongNengQu功能区, tongFengLiang通风量
std::vector<AttrWindow>  WebIO::GetWindows(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL		//本地模式
	vAttrWindow Local = CWindowLocalDataFromDB::Instance()->GetWindows(width, height, openType, openNum, gongNengQu);
	return Local;
#else
	vAttrWindow Web = CWindowWebData::Instance()->GetWindows(width, height, openType, openNum, gongNengQu);

#ifdef _WEB_TEST
	vAttrWindow Local = CWindowLocalDataFromDB::Instance()->GetWindows(width, openType, openNum, gongNengQu);
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
				if(Local[j].IsPrototypeEqual_test(Web[i]))
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
#endif

	return Web;
#endif
}
std::vector<AttrWindow> WebIO::GetDoors(double width, double height, CString openType, int openNum, CString gongNengQu)const
{
#ifdef WORK_LOCAL//本地模式
	vAttrWindow Local = CWindowLocalDataFromDB::Instance()->GetDoors(width, height, openType, openNum, gongNengQu);
	return Local;
#else
	vAttrWindow Web = CWindowWebData::Instance()->GetDoors(width, openType, openNum, gongNengQu);
	return Web;
#endif
}

std::vector<AttrKitchen> WebIO::GetKitchens(EKitchType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_hasPaiQiDao)
{
#ifdef WORK_LOCAL//本地模式
	vector<AttrKitchen> Local = m_kitchenBathroomLocalData.GetKitchens(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir, p_hasPaiQiDao);
	sort(Local.begin(), Local.end(), SortKitchenFun);
	return Local;
#else	
	/*CString WindowDoorPos;
	if (abs(p_doorDir - p_windowDir) == 2)
	{
		WindowDoorPos = L"门窗对开";
	}
	else 
	{
		WindowDoorPos = L"门窗垂直开";
	}
	
	return m_kitchenBathroomWebData.GetKitchens(p_xLen, p_yLen, WindowDoorPos, KitchenTypeToCString(p_type), (p_hasPaiQiDao == true)? 1:0);*/
	vector<AttrKitchen> Web = m_kitchenBathroomWebData.GetKitchens(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir, p_hasPaiQiDao);
	sort(Web.begin(), Web.end(), SortKitchenFun);
	return Web;
#endif
}

std::vector<AttrKitchen> WebIO::GetAllKitchens()
{
#ifdef WORK_LOCAL//本地模式
	vector<AttrKitchen> Local = m_kitchenBathroomLocalData.GetAllKitchens();
	sort(Local.begin(), Local.end(), SortKitchenFun);
	return Local;
#else
	vector<AttrKitchen> Web = m_kitchenBathroomWebData.GetAllKitchens();
	sort(Web.begin(), Web.end(), SortKitchenFun);
	return Web;
#endif
}

std::vector<AttrBathroom> WebIO::GetBathrooms(EBathroomType p_type, double p_xLen, double p_yLen, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir)
{
#ifdef WORK_LOCAL//本地模式
	vector<AttrBathroom> Local = m_kitchenBathroomLocalData.GetBathrooms(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir);
	sort(Local.begin(), Local.end(), SortBathroomFun);
	return Local;
#else
	/*CString WindowDoorPos;
	if (abs(p_doorDir - p_windowDir) == 2)
	{
		WindowDoorPos = L"门窗对开";
	}
	else WindowDoorPos = L"门窗垂直开";

	return m_kitchenBathroomWebData.GetBathrooms(p_xLen, p_yLen, WindowDoorPos, BathroomTypeToCString(p_type));*/
	vector<AttrBathroom> Web = m_kitchenBathroomWebData.GetBathrooms(p_type, p_xLen, p_yLen, p_doorDir, p_windowDir);
	sort(Web.begin(), Web.end(), SortBathroomFun);
	return Web;
#endif
}

std::vector<AttrBathroom> WebIO::GetAllBathrooms()
{
#ifdef WORK_LOCAL//本地模式
	vector<AttrBathroom> Local = m_kitchenBathroomLocalData.GetAllBathrooms();
	sort(Local.begin(), Local.end(), SortBathroomFun);
	return Local;
#else
	vector<AttrBathroom> Web = m_kitchenBathroomWebData.GetAllBathrooms();
	sort(Web.begin(), Web.end(), SortBathroomFun);
	return Web;
#endif
}

//weiZhi冷凝水管位置
std::vector<AttrAirCon> WebIO::GetAirCons(double piShu, CString weiZhi, CString hasYuShuiGuan, CString yuShuiGuanWeizhi)
{
#ifdef WORK_LOCAL//本地模式
	return CAirConditionLocalDataFromDB::Instance()->GetAirCons(piShu, weiZhi, hasYuShuiGuan, yuShuiGuanWeizhi);
#else
	return CAirConditionWebData::Instance()->GetAirCons(piShu, weiZhi, hasYuShuiGuan, yuShuiGuanWeizhi);
#endif
}

std::vector<AttrAirCon> WebIO::GetAllAirCons()
{
#ifdef WORK_LOCAL//本地模式
	return CAirConditionLocalDataFromDB::Instance()->GetAllAirCons();
#else
	return CAirConditionWebData::Instance()->GetAllAirCons();
#endif
}

std::vector<AttrRailing> WebIO::GetRailings(eRailingType type)//一次搜索所有的
{
#ifdef WORK_LOCAL//本地模式
	std::vector<AttrRailing> result;
	return result;
#else
	return m_railingWebData.GetRailings(type);
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
	return m_railingWebData.GetAllRailings();
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
	ofstream ofs(filenameOut.c_str(), ofstream::out | ofstream::binary);

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

