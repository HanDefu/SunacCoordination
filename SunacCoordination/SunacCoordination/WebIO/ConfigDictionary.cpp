#include "StdAfx.h"
#include "../Sunac_def.h"
#include "ConfigDictionary.h"
#include <xstring>
#include <atlconv.h>  
#include "SunacCadWeb\ArgumentSettingServiceSoap.nsmap" //只用包含一次，且必须包含一次
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "../WebIO/WindowWebData.h"

#include "..\Tool\MarkupXml\Markup.h"

using namespace std;

CConfigDictionary::CConfigDictionary()
{
	InitFromWeb();
}

CConfigDictionary::~CConfigDictionary()
{

}

//接口文档 https://docs.qq.com/doc/DUUpaanJLZmlSQ2d4

void CConfigDictionary::GetConfig(wstring Term)
{
	vector<wstring> paraOut;
	bool bSuc1 = GetConfigFromWeb(Term, paraOut);
	if (bSuc1 && paraOut.size()>0)
	{
		vCString allPara;
		for (vector<wstring>::iterator it = paraOut.begin(); it != paraOut.end(); it++)
		{
			CString str = it->c_str();
			allPara.push_back(str);
		}

		CString cstr = Term.c_str();
		m_configDict[cstr] = allPara;
	}
}

bool CConfigDictionary::InitFromWeb()
{	
	//区域
	GetConfig(_T("area"));

	//功能区
	GetConfig(_T("actionType"));

	//门窗位置关系
	GetConfig(_T("doorWindowPosition"));

	//开启类型
	GetConfig( _T("openType"));

	//开启扇数量
	GetConfig(_T("openWindowNum"));

	//通风量计算面积比值
	GetConfig(_T("airVolumeAreaRation"));

	//门窗距外墙距离
	GetConfig(_T("doorWindowDistance"));

	//门类型
	GetConfig(_T("doorType"));

	//厨房类型
	GetConfig(_T("kitchenType"));

	//厨房水盆类型
	GetConfig(_T("kitchenBasinType"));

	//冰箱类型
	GetConfig(_T("refrigeratorType"));

	//灶台宽度
	GetConfig(_T("hearthWidth"));

	//卫生间类型
	GetConfig(_T("toiletType"));

	//卫生间台盆宽度
	GetConfig(_T("toiletBasinWidth"));

	//马桶宽度
	GetConfig(_T("closesToolWidth"));

	//盥洗区宽度
	GetConfig(_T("bathroomWidth"));

	//空调匹数
	GetConfig(_T("airConditionNumber"));

	//冷凝水管位置
	GetConfig(_T("condensatePipePosition"));

	//雨水管位置
	GetConfig(_T("rainPipePosition"));

	//栏杆类型
	GetConfig(_T("handRail"));

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
	//判断当指针为空，未读到xml时，返回false
	if (attResult.StandardDesignAttributeResult == NULL)
	{
		return false;
	}

	//UINT  len = (attResult.StandardDesignAttributeResult)->length();
	//MessageBox(NULL, attResult.StandardDesignAttributeResult->c_str(), _T("返回结果"), 0);

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(attResult.StandardDesignAttributeResult)).c_str());
	
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
	//if (m_configDict.count("area") > 0) // 找下有没有这个key，有的
	//{
	//	return m_configDict[_T("area")];
	//}
	auto iter = m_configDict.find(_T("area"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"北京区域");
		strs.push_back(L"华北区域");
		strs.push_back(L"上海区域");
		strs.push_back(L"西南区域");
		strs.push_back(L"东南区域");
		strs.push_back(L"广深区域");
		strs.push_back(L"华中区域");
		strs.push_back(L"海南区域");
		return strs;
	}
}
vCString CConfigDictionary::GetGongNengQus()const
{
	auto iter = m_configDict.find(_T("actionType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"厨房");
		strs.push_back(L"卫生间");
		strs.push_back(L"阳台");
		strs.push_back(L"卧室");
		strs.push_back(L"客厅");
		return strs;
	}
}
vCString CConfigDictionary::GetWindowDoorPoss()const
{
	auto iter = m_configDict.find(_T("doorWindowPosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"门窗对开");
		strs.push_back(L"门窗垂直开");
		return strs;
	}
}
vCString CConfigDictionary::Window_GetOpenTypes()const
{
	auto iter = m_configDict.find(_T("openType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"内开");
		strs.push_back(L"外开");
		strs.push_back(L"推拉");
		strs.push_back(L"上悬");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetOpenAmount()const
{
	auto iter = m_configDict.find(_T("openWindowNum"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"1");
		strs.push_back(L"2");
		strs.push_back(L"3");
		strs.push_back(L"4");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetRate()const
{
	auto iter = m_configDict.find(_T("airVolumeAreaRation"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"1/4");
		strs.push_back(L"1/5");
		strs.push_back(L"1/6");
		strs.push_back(L"1/8");
		strs.push_back(L"1/10");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetWallDis()const
{
	auto iter = m_configDict.find(_T("doorWindowDistance"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"0");
		strs.push_back(L"50");
		strs.push_back(L"100");
		strs.push_back(L"150");
		strs.push_back(L"200");
		return strs;
	}
}

vCString CConfigDictionary::Door_GetTypes()const
{
	auto iter = m_configDict.find(_T("doorType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"推拉");
		strs.push_back(L"外开");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	auto iter = m_configDict.find(_T("kitchenType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"U型");
		strs.push_back(L"L型");
		strs.push_back(L"I型");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetShuiPenTypes()const
{
	auto iter = m_configDict.find(_T("kitchenBasinType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"单盆600");
		strs.push_back(L"单盆800");
		strs.push_back(L"双盆900");
		strs.push_back(L"双盆1000");
		strs.push_back(L"双盆1200");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetBingXiangTypes()const
{
	auto iter = m_configDict.find(_T("refrigeratorType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"单开门700");
		strs.push_back(L"对开门800");
		strs.push_back(L"对开门1000");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetZaoTaiWidths()const
{
	auto iter = m_configDict.find(_T("hearthWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"800");
		strs.push_back(L"900");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetTypes()const
{
	auto iter = m_configDict.find(_T("toiletType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"U型");
		strs.push_back(L"L型");
		strs.push_back(L"I型");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetTaiPenWidths()const
{
	auto iter = m_configDict.find(_T("toiletBasinWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"650");
		strs.push_back(L"750");
		strs.push_back(L"800");
		strs.push_back(L"900");
		strs.push_back(L"1000");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetMaTongTypes()const
{
	auto iter = m_configDict.find(_T("closesToolWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"750");
		strs.push_back(L"800");
		return strs;
	}
}
vCString CConfigDictionary::Toilet_GetGuanXiWidths()const
{
	auto iter = m_configDict.find(_T("bathroomWidth"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"950");
		strs.push_back(L"1050");
		return strs;
	}
}
vCString CConfigDictionary::Air_GetPiShus()const
{
	auto iter = m_configDict.find(_T("airConditionNumber"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"1.0P");
		strs.push_back(L"1.5P");
		strs.push_back(L"2.0P");
		strs.push_back(L"3.0P");
		return strs;
	}
}
vCString CConfigDictionary::Air_GetLengNingShuiGuanPos()const//获得冷凝水管位置
{
	auto iter = m_configDict.find(_T("condensatePipePosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"侧面");
		strs.push_back(L"后面");
		return strs;
	}
}

vCString CConfigDictionary::Air_GetYuShuiGuanPos()const//获得雨水管位置
{
	auto iter = m_configDict.find(_T("rainPipePosition"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"侧面");
		strs.push_back(L"后面");
		return strs;
	}
}

vCString CConfigDictionary::Railing_GetTypes()
{
	auto iter = m_configDict.find(_T("handRail"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"玻璃栏杆");
		strs.push_back(L"铁艺栏杆");
		return strs;
	}
}
