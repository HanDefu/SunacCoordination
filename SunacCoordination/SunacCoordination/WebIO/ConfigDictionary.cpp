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
#include "WebCommon.h"
#include <xtree>

using namespace std;

CConfigDictionary::CConfigDictionary()
{
	//InitFromWeb();
}

CConfigDictionary::~CConfigDictionary()
{

}

//接口文档 https://docs.qq.com/doc/DUUpaanJLZmlSQ2d4

bool CConfigDictionary::GetConfig(wstring Term)
{
	vector<wstring> paraOut;
	bool bSuc = GetConfigFromWeb(Term, paraOut);
	if (bSuc && paraOut.size()>0)
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
	return bSuc;
}

bool CConfigDictionary::InitFromWeb()
{	
	bool bSuc = false;
	//区域
	bSuc = GetConfig(_T("area"));
	if (bSuc == false)
		return bSuc;

	//功能区
	bSuc = GetConfig(_T("actionType"));
	if (bSuc == false)
		return bSuc;

	//门窗位置关系
	bSuc = GetConfig(_T("doorWindowPosition"));
	if (bSuc == false)
		return bSuc;

	//开启类型
	bSuc = GetConfig(_T("openType"));
	if (bSuc == false)
		return bSuc;

	//开启扇数量
	bSuc = GetConfig(_T("openWindowNum"));
	if (bSuc == false)
		return bSuc;

	//通风量计算面积比值
	bSuc = GetConfig(_T("airVolumeAreaRation"));
	if (bSuc == false)
		return bSuc;

	//门窗距外墙距离
	bSuc = GetConfig(_T("doorWindowDistance"));
	if (bSuc == false)
		return bSuc;

	//门类型
	bSuc = GetConfig(_T("doorType"));
	if (bSuc == false)
		return bSuc;

	//厨房类型
	bSuc = GetConfig(_T("kitchenType"));
	if (bSuc == false)
		return bSuc;

	//厨房水盆类型
	bSuc = GetConfig(_T("kitchenBasinType"));
	if (bSuc == false)
		return bSuc;

	//冰箱类型
	bSuc = GetConfig(_T("refrigeratorType"));
	if (bSuc == false)
		return bSuc;

	//灶台宽度
	bSuc = GetConfig(_T("hearthWidth"));
	if (bSuc == false)
		return bSuc;

	//卫生间类型
	bSuc = GetConfig(_T("BathroomType"));
	if (bSuc == false)
		return bSuc;

	//卫生间台盆宽度
	bSuc = GetConfig(_T("BathroomBasinWidth"));
	if (bSuc == false)
		return bSuc;

	//马桶宽度
	bSuc = GetConfig(_T("closesToolWidth"));
	if (bSuc == false)
		return bSuc;

	//盥洗区宽度
	bSuc = GetConfig(_T("bathroomWidth"));
	if (bSuc == false)
		return bSuc;

	//空调匹数
	bSuc = GetConfig(_T("airConditionNumber"));
	if (bSuc == false)
		return bSuc;

	//冷凝水管位置
	bSuc = GetConfig(_T("condensatePipePosition"));
	if (bSuc == false)
		return bSuc;

	//雨水管位置
	bSuc = GetConfig(_T("rainPipePosition"));
	if (bSuc == false)
		return bSuc;

	//栏杆类型
	bSuc = GetConfig(_T("handRail"));
	if (bSuc == false)
		return bSuc;

	return true;
}

bool CConfigDictionary::GetConfigFromWeb(wstring p_paraTypeName, vector<wstring>& p_paraOut)
{
	p_paraOut.clear();

	_ns1__StandardDesignAttribute desingAtt;
	desingAtt.AtrributeName = &p_paraTypeName;
	_ns1__StandardDesignAttributeResponse attResult;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);

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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("area"));
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
		strs.push_back(L"华南区域");
		strs.push_back(L"华中区域");
		return strs;
	}
}
vCString CConfigDictionary::GetGongNengQus()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("actionType"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("doorWindowPosition"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("openType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"内开");
		strs.push_back(L"内开内倒");
		strs.push_back(L"外开");
		strs.push_back(L"上悬");
		strs.push_back(L"推拉");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetOpenAmount()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("openWindowNum"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("airVolumeAreaRation"));
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
		strs.push_back(L"1/16");
		strs.push_back(L"1/20");
		strs.push_back(L"1/30");
		return strs;
	}
}

vCString CConfigDictionary::Window_GetWallDis()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("doorWindowDistance"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("doorType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"外开");
		strs.push_back(L"推拉");
		strs.push_back(L"提升推拉");
		strs.push_back(L"门连窗");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetTypes()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("kitchenType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"U型");
		strs.push_back(L"L型");
		strs.push_back(L"I型");
		return strs;
	}
}
vCString CConfigDictionary::Kitchen_GetShuiPenTypes()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("kitchenBasinType"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("refrigeratorType"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("hearthWidth"));
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
vCString CConfigDictionary::Bathroom_GetTypes()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("BathroomType"));
	if (iter != m_configDict.end())
	{
		return iter->second;
	}
	else
	{
		vCString strs;
		strs.push_back(L"不限");
		strs.push_back(L"I型");
		strs.push_back(L"L型");
		strs.push_back(L"U型");
		return strs;
	}
}
vCString CConfigDictionary::Bathroom_GetTaiPenWidths()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("BathroomBasinWidth"));
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
vCString CConfigDictionary::Bathroom_GetMaTongTypes()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("closesToolWidth"));
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
vCString CConfigDictionary::Bathroom_GetGuanXiWidths()const
{
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("bathroomWidth"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("airConditionNumber"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("condensatePipePosition"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("rainPipePosition"));
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
	std::map<CString, vCString>::const_iterator iter = m_configDict.find(_T("handRail"));
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
