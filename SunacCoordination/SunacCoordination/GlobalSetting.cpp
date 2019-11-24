#include "StdAfx.h"
#include "GlobalSetting.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include <algorithm>
#include "Tool/MarkupXml/Markup.h"
#include "Common/ComFun_Str.h"
#include "Common/ComFun_Sunac.h"

GlobalSetting::GlobalSetting()
{
	m_bTestMode = false;
	m_bRememberPwd = false;
	LoadFromXml();
}

GlobalSetting::~GlobalSetting()
{
	SaveToXml();
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
CString GlobalSetting::GetXmlFilePath()
{
	CString sFile = MD2010_GetAppPath() + +_T("Settings.xml");
	return sFile;
}

bool GlobalSetting::LoadFromXml()
{
	CString sFile = GetXmlFilePath();
	CMarkup xml;

	if (xml.Load(sFile) == false)
		return false;

	xml.ResetMainPos();
	if (xml.FindElem())
	{
		xml.IntoElem();

		if (xml.FindElem(_T("TestMode")))
			m_bTestMode = xml.GetData()==_T("TRUE") ? true :false;
		m_bTestMode = false;

		if (xml.FindElem(_T("RememberPwd")))
			m_bRememberPwd = xml.GetData() == _T("TRUE") ? true : false;

		if (xml.FindElem(_T("UserName")))
			m_userName = xml.GetData();
		if (m_bRememberPwd && xml.FindElem(_T("Password")))
			m_password = xml.GetData();

		xml.OutOfElem();
	}

	return true;
}
bool GlobalSetting::SaveToXml()
{
	CString sFile = GetXmlFilePath();

	CMarkup xml;
	xml.SetDoc(_T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"));
	xml.AddElem(_T("Settings"));
	xml.IntoElem();
	{
		xml.AddElem(_T("TestMode"), m_bTestMode ? _T("TRUE") : _T("FALSE"));
		xml.AddElem(_T("RememberPwd"), m_bRememberPwd ? _T("TRUE") : _T("FALSE"));

		xml.AddElem(_T("UserName"), m_userName);
		if (m_bRememberPwd)
		{
			xml.AddElem(_T("Password"), m_password);
		}
	}
	xml.OutOfElem();
	xml.Save(sFile);

	return true;
}

