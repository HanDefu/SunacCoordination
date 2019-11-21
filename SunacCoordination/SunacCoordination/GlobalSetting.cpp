#include "StdAfx.h"
#include "GlobalSetting.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include "Common/ComFun_Sunac.h"
#include <algorithm>

GlobalSetting::GlobalSetting()
{
	m_bTestMode = true;
}

GlobalSetting::~GlobalSetting()
{
	
}
CString GlobalSetting::GetFtpIp()const
{
	if (m_bTestMode)
	{
		return L"10.4.64.91";
	}
	else
	{
		return L"1.203.161.246";
	}
}

const char* GlobalSetting::GetCADServiceUrl()const
{
	static const char sUrlTestMode[256] = "http://des.sunac.com.cn/SunacCADService.asmx";
	static const char sUrl[256] = "http://ades.sunac.com.cn/SunacCADService.asmx";
	if (m_bTestMode)
	{
		return sUrlTestMode;
	}
	else
	{
		return sUrl;
	}
}

