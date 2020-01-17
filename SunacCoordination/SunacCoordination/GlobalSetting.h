#pragma once
#include "Sunac_def.h"
#include <utility>

using namespace std;

const int cPasswordMaxlength = 30;
const int cPasswordBufferSize = (cPasswordMaxlength*sizeof(wchar_t)/16+1)*16;


//门窗设置
struct CWinSetting
{
public:
	CWinSetting();
	CString GetWinLayerDefault() { return L"Sunac_Window"; }

	CString GetWinFrameLayerDefault() { return L"Sunac_Gridline_Glass"; }
	CString GetWinWallLayerDefault() { return L"Sunac_Wall"; }
	CString GetWinHardwareLayerDefault() { return L"Sunac_HardWare"; }
	CString GetWinNumberLayerPingmianDefault() { return L"Sunac_WinNumber_Pingmian"; }

	CString GetWinOpenLayerDefault() { return L"Sunac_Gridline_Glass_Open"; }
	CString GetWinNumberLayerLimianDefault() { return L"Sunac_WinNumber_Limian";}



	CString m_sWinLayer;		//门窗块图层

	CString m_sWinFrameLayer;	//门窗框图层
	CString m_sWinWallLayer;	//窗洞土建图层
	CString m_sWinHardwareLayer; //门窗五金件图层
	CString m_sWinOpenLayer; //门窗开启部分图层

	CString m_sWinNumberLayerLimian;	//门窗立面图编号图层
	CString m_sWinNumberLayerPingmian;	//门窗平面图编号图层

	bool m_bUseAinLimian;  //立面图是否使用塞缝尺寸

	int m_numberTextSize; //门窗编号字体

	bool m_bShowLimianNumber; //是否显示立面图的门窗编号

	int m_bWinDetailDimRate; //门窗详图标注比例
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

	static CWinSetting* GetWinSetting(){return &(GetInstance()->m_winSetting);}

	CString GetFtpIp()const;
	const char*  GetCADServiceUrl()const;

	static CString GetWindowBlockLayer() { return GetInstance()->m_winSetting.m_sWinLayer; }

	bool UpdateToXml();

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