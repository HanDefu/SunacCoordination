#include "StdAfx.h"
#include "AirconditionWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include <string>

std::vector<AttrAirCon> CAirConditionWebData::GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi)
{
	CString AirconditionerIsRainpipe_;
	AirconditionerIsRainpipe_.Format(L"%d", hasYuShuiGuan);

/*
	std::wstring sAirconditionerPower = piShu;
	std::wstring sAirconditionerPipePosition = AirconditionerPipePosition;
	std::wstring sAirconditionerIsRainpipe = AirconditionerIsRainpipe_;
	std::wstring sRainpipePosition = RainpipePosition;*/

	_ns1__GetAllAirconditionerByParam ns;
	/*ns.AirconditionerPower = &sAirconditionerPower;
	ns.AirconditionerPipePosition = &sAirconditionerPipePosition;
	ns.AirconditionerIsRainpipe = &sAirconditionerIsRainpipe;
	ns.RainpipePosition = &sRainpipePosition;*/
	ns.AirconditionerPower = 0;
	ns.AirconditionerPipePosition = 0;
	ns.AirconditionerIsRainpipe = 0;
	ns.RainpipePosition = 0;

	_ns1__GetAllAirconditionerByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllAirconditionerByParam(&ns, nsResponse);

	std::vector<AttrAirCon> AirConAtts;

	//判断返回结果是否成功
	if (nsResponse.GetAllAirconditionerByParamResult == NULL)
	{
		return AirConAtts;
	}
	
	//解析字符串出结果
	CMarkup xml;
	xml.SetDoc((*(nsResponse.GetAllAirconditionerByParamResult)).c_str());

	AirConAtts = ParseAirConditionersFromXML(xml);
	return AirConAtts;
}

std::vector<AttrAirCon > CAirConditionWebData::ParseAirConditionersFromXML(CMarkup xml)const
{
	std::vector<AttrAirCon> vAirConAttrs;
	AttrAirCon AirConAttr;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vAirConAttrs;
		}
	}
	if (xml.FindElem(_T("Airconditioners")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Airconditioner")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				AirConAttr.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPath")))
			{
				AirConAttr.m_file.fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "1")
				{
					AirConAttr.m_isJiTuan = TRUE;
				}
				else AirConAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				AirConAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				AirConAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerPower")))
			{
				AirConAttr.m_power = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("AirconditionerMinWidth")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerMinLength")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerPipePosition")))
			{
				AirConAttr.m_pipePos = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerIsRainpipe")))
			{
				CString str = xml.GetData();
				AirConAttr.m_hasRainPipe = (str==_T("是"))? true : false;
			}
			if (xml.FindElem(_T("AirconditionerRainpipePostion")))
			{
				AirConAttr.m_rainPipePos = xml.GetData();
			}
			xml.OutOfElem();
			vAirConAttrs.push_back(AirConAttr);	
		}
		

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vAirConAttrs;
}