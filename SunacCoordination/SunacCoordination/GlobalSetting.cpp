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

string GlobalSetting::GetCADServiceUrl()const
{
	if (m_bTestMode)
	{
		return "http://des.sunac.com.cn/SunacCADService.asmx?WSDL";
	}
	else
	{
		return "http://ades.sunac.com.cn/SunacCADService.asmx?WSDL";
	}
}

