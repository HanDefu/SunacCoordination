#include "StdAfx.h"
#include "WindowWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Tool\MarkupXml\Markup.h"


CWindowWebData::CWindowWebData()
{

}

CWindowWebData::~CWindowWebData()
{

}

std::vector<AttrWindow >  CWindowWebData::GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const
{
	std::wstring sOpenType = openType;
	std::wstring sOpenNum;
	std::wstring sGongnengqu = gongNengQu;

	_ns1__GetWindows ns;
	ns.width = p_width;
	ns.height = p_heigh;
	ns.openType = &sOpenType;
	ns.openNum = &sOpenNum;
	ns.gongNengQu = &sGongnengqu;

	_ns1__GetWindowsResponse nsResponse;


	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetWindows(&ns, nsResponse);

	//判断返回结果是否成功


	std::vector<AttrWindow > windowAtts;



	return windowAtts;
}