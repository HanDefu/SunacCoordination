#include "StdAfx.h"
#include "../Sunac_def.h"
#include "ConfigDictionary.h"
#include <xstring>
#include <atlconv.h>  
#include "SunacCadWeb\ArgumentSettingServiceSoap.nsmap"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"


#include "..\Tool\MarkupXml\Markup.h"

using namespace std;

CConfigDictionary::CConfigDictionary()
{
	//InitFromWeb();
	//m_quyus = GetQuyus();//区域
	//m_gongNengQus = GetGongNengQus();//功能区
	//m_windowDoorPoss = GetWindowDoorPoss();//门窗位置关系
	//m_windowOpenTypes = Window_GetOpenTypes();//开启类型
	//m_doorTypes = Door_GetTypes();//门类型
	//m_kitchenTypes = Kitchen_GetTypes();//厨房类型
	//m_kitchenShuiPenTypes = Kitchen_GetShuiPenTypes();//水盆类型
	//m_kitchenBingXiangTypes = Kitchen_GetBingXiangTypes();//冰箱类型
	//m_kitchenZaoTaiWidths = Kitchen_GetZaoTaiWidths();//灶台宽度
	//m_toiletTypes = Toilet_GetTypes();//卫生间类型
	//m_toiletTaiPenWidths = Toilet_GetTaiPenWidths();//卫生间类型
	//m_toiletMaTongTypes = Toilet_GetMaTongTypes();//卫生间类型
	//m_toiletGuanXiWidths = Toilet_GetGuanXiWidths();//盥洗区宽度---是否支持手动输入？？
	//m_airPiShus = Air_GetPiShus();//所有的匹数
	//m_airLengNingShuiGuanPos = Air_GetLengNingShuiGuanPos();//冷凝水管位置
	//m_windowOpenAmount = Window_GetOpenAmount();//开启扇数量
	//m_rate = Window_GetRate();//比值
	//m_windowWallDis = Window_GetWallDis();//窗户距外墙距离
}

CConfigDictionary::~CConfigDictionary()
{

}

//接口文档 https://docs.qq.com/doc/DUUpaanJLZmlSQ2d4
bool CConfigDictionary::InitFromWeb()
{
	vector<wstring> p_paraOut;
	bool bSuc1 = GetConfigFromWeb(_T("area"), p_paraOut);
	if (bSuc1)
	{
		//m_configDict[_T("area")] = p_paraOut;
	}



	return true;
}

bool CConfigDictionary::GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut)
{
	p_paraOut.clear();

	_ns1__StandardDesignAttribute desingAtt;
	desingAtt.AtrributeName = &p_paraTypeName;
	_ns1__StandardDesignAttributeResponse attResult;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.StandardDesignAttribute(&desingAtt, attResult);

	//UINT  len = (attResult.StandardDesignAttributeResult)->length();
	//MessageBox(NULL, attResult.StandardDesignAttributeResult->c_str(), _T("返回结果"), 0);


	//解析字符串出结果
	CMarkup xml;
	xml.SetDoc(*(attResult.StandardDesignAttributeResult));
	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	{
		wstring sCode;
		if (xml.FindElem(_T("Code")))
		{
			sCode = xml.GetData();
		}
		wstring sMsg;
		if (xml.FindElem(_T("Message")))
		{
			sMsg = xml.GetData();
		}

		wstring str;
		if (false == xml.FindElem(_T("ArgumentSettings")))
		{
			return false;
		}

		xml.IntoElem();
		while (xml.FindElem(_T("ArgumentSetting")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("ArgumentText")))
			{
				str = xml.GetData();
				p_paraOut.push_back(str);
			}
			xml.OutOfElem();
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();
	
	return true;
}

vCString CConfigDictionary::GetQuyus()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"北京区域");
	strs.push_back(L"华北区域");
	strs.push_back(L"上海区域");
	strs.push_back(L"西南区域");
	strs.push_back(L"东南区域");
	strs.push_back(L"广深区域");
	strs.push_back(L"华中区域");
	strs.push_back(L"海南区域");
#else

#endif
	return strs;
}
vCString CConfigDictionary::GetGongNengQus()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"厨房");
	strs.push_back(L"卫生间");
	strs.push_back(L"阳台");
	strs.push_back(L"卧室");
	strs.push_back(L"客厅");
#else

#endif
	return strs;
}
vCString CConfigDictionary::GetWindowDoorPoss()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"门窗对开");
	strs.push_back(L"门窗垂直开");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Window_GetOpenTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"内开");
	strs.push_back(L"外开");
	strs.push_back(L"推拉");
	strs.push_back(L"上悬");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetOpenAmount()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"1");
	strs.push_back(L"2");
	strs.push_back(L"4");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetRate()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"1/4");
	strs.push_back(L"1/5");
	strs.push_back(L"1/6");
	strs.push_back(L"1/8");
	strs.push_back(L"1/10");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Window_GetWallDis()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"0");
	strs.push_back(L"100");
	strs.push_back(L"120");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Door_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"不限");
	strs.push_back(L"推拉");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"深U型");
	strs.push_back(L"浅U型");
	strs.push_back(L"L型");
	strs.push_back(L"I型");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Kitchen_GetShuiPenTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单盆600");
	strs.push_back(L"单盆700");
	strs.push_back(L"双盆900");
	strs.push_back(L"双盆1000");
	strs.push_back(L"双盆1200");
#else

#endif

	return strs;
}
vCString CConfigDictionary::Kitchen_GetBingXiangTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单开门700");
	strs.push_back(L"对开门800");
	strs.push_back(L"对开门1000");
#else

#endif

	return strs;
}
vCString CConfigDictionary::Kitchen_GetZaoTaiWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"800");
	strs.push_back(L"900");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"U型");
	strs.push_back(L"L型");
	strs.push_back(L"I型");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetTaiPenWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"单盆600");
	strs.push_back(L"单盆700");
	strs.push_back(L"双盆900");
	strs.push_back(L"双盆1000");
	strs.push_back(L"双盆1200");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetMaTongTypes()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"800");
	strs.push_back(L"900");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Toilet_GetGuanXiWidths()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"950");
	strs.push_back(L"1050");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Air_GetPiShus()const
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"1.0");
	strs.push_back(L"1.5");
	strs.push_back(L"2.0");
	strs.push_back(L"3.0");
#else

#endif
	return strs;
}
vCString CConfigDictionary::Air_GetLengNingShuiGuanPos()const//获得冷凝水管位置
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"后边");
	strs.push_back(L"侧边");
#else

#endif
	return strs;
}

vCString CConfigDictionary::Air_GetYuShuiGuanPos() //获得雨水管位置
{
	vCString strs;
#ifdef WORK_LOCAL//本地模式
	strs.push_back(L"侧边");
	strs.push_back(L"后边");
#else

#endif
	return strs;
}
