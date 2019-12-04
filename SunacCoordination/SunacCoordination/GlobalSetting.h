#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

const int cPasswordMaxlength = 30;
const int cPasswordBufferSize = (cPasswordMaxlength*sizeof(wchar_t)/16+1)*16;


//门窗设置
struct CWinSetting
{
	CString m_sWinLayer; //门窗结构图层
	CString m_sWinWallLayer; //门窗外的墙的图层
	CString m_sWinHardwareLayer; //门窗五金件图层
	CString m_sWinOpenLayer; //门窗开启部分图层
	CString m_sWinNumberLayerLimian; //门窗立面图编号图层
	CString m_sWinNumberLayerPingmian; //门窗立面图编号图层

	int m_winNumberTextSize; //门窗编号字体
};

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

	CWinSetting m_winSetting;
}; 

#define GSINST (GlobalSetting::GetInstance())