#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

const int cPasswordMaxlength = 30;
const int cPasswordBufferSize = (cPasswordMaxlength*sizeof(wchar_t)/16+1)*16;

class GlobalSetting
{
public:
	~GlobalSetting();
	static GlobalSetting *GetInstance()
	{
		static GlobalSetting instance;
		return &instance;
	}

	CString GetFtpIp()const;
	const char*  GetCADServiceUrl()const;

private:
	GlobalSetting();

	CString GetXmlFilePath();
	bool LoadFromXml();
	bool SaveToXml();

public:
	bool m_bTestMode; //测试环境

	bool m_bRememberPwd; //记住密码
	CString m_userName;
	CString m_password;
}; 

#define GSINST (GlobalSetting::GetInstance())