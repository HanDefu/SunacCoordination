#include "StdAfx.h"
#include "AirconditionWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_Sunac.h"
#include "WebCommon.h"
#include "WebIO.h"
#include <string>


CAirConditionWebData* CAirConditionWebData::Instance()
{
	static CAirConditionWebData instance;
	return &instance;
}

CAirConditionWebData::CAirConditionWebData()
{
	m_allAirconditions = GetAllAirCons();
}

CAirConditionWebData::~CAirConditionWebData()
{
}
/*
std::vector<AttrAirCon> CAirConditionWebData::GetAirCons(double piShu, CString weiZhi, bool hasYuShuiGuan, CString yuShuiGuanWeizhi)
{
	CString AirconditionerIsRainpipe_;
	if (hasYuShuiGuan)
	{
		AirconditionerIsRainpipe_ = L"1";
	}
	else AirconditionerIsRainpipe_ = L"-1";
	CString pishu_;
	pishu_.Format(L"%.1f", piShu);

	std::wstring sAirconditionerPower = pishu_;
	std::wstring sAirconditionerPipePosition = weiZhi;
	std::wstring sAirconditionerIsRainpipe = AirconditionerIsRainpipe_;
	std::wstring sRainpipePosition = yuShuiGuanWeizhi;

	_ns1__GetAllAirconditionerByParam ns;
	ns.AirconditionerPower = &sAirconditionerPower;
	ns.AirconditionerPipePosition = &sAirconditionerPipePosition;
	ns.AirconditionerIsRainpipe = &sAirconditionerIsRainpipe;
	ns.RainpipePosition = &sRainpipePosition;
	/ *ns.AirconditionerPower = 0;
	ns.AirconditionerPipePosition = 0;
	ns.AirconditionerIsRainpipe = 0;
	ns.RainpipePosition = 0;* /

	_ns1__GetAllAirconditionerByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);

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
}*/

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
			if (xml.FindElem(_T("Drawings")))
			{
				xml.IntoElem();
				{
					while (xml.FindElem(_T("Drawing")))
					{
						xml.IntoElem();
						{
							CString sFileType, tempString, sFileName, sFileID, sImgFileName;
							if (xml.FindElem(_T("Id")))
							{
								sFileID = xml.GetData();
							}
							if (xml.FindElem(_T("ImgPath")))
							{
								tempString = xml.GetData();
								if (tempString != "")
								{
									sImgFileName = WEBINST->GetFileName(tempString);//获得带扩展名的文件名
								}
							}

							if (xml.FindElem(_T("CADPath")))
							{
								tempString = xml.GetData();
								if (tempString != "")
								{
									sFileName = WEBINST->GetFileName(tempString);//获得带扩展名的文件名
								}
							}

							if (xml.FindElem(_T("CADType")))
							{
								sFileType = xml.GetData();
							}
							if (sFileType == "ExpandViewFile")
							{
								AirConAttr.m_file.id = _ttoi(sFileID);
								AirConAttr.m_file.fileName = sFileName;
							}

							//检查文件是否存在，不存在则下载
							CString sDWGFilePath = TY_GetPrototypeFilePath_Web() + sFileName;
							if (!JHCom_FileExist(sDWGFilePath))
							{
								WEBINST->DownloadFile(_ttoi(sFileID), "CAD", sDWGFilePath);
							}

							CString sImgFilePath = TY_GetPrototypeImagePath() + sImgFileName;
							if (!JHCom_FileExist(sImgFilePath))
							{
								WEBINST->DownloadFile(_ttoi(sFileID), "Img", sImgFilePath);
							}
						}
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
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
			if (xml.FindElem(_T("AirconditionerPowerName")))
			{
				CString Power = xml.GetData();
				Power.Delete(Power.GetLength()-1, 1);
				AirConAttr.m_power = _ttof(Power);
			}
			if (xml.FindElem(_T("AirconditionerMinWidth")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerMinLength")))
			{
				//AirConAttr.m_ = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerPipePositionName")))
			{
				AirConAttr.m_pipePos = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerIsRainPipe")))
			{
				CString str = xml.GetData();
				AirConAttr.m_hasRainPipe = (str ==_T("1"))? true : false;
			}
			if (xml.FindElem(_T("AirconditionerRainPipePositionName")))
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

std::vector<AttrAirCon> CAirConditionWebData::GetAllAirCons()
{
	_ns1__GetAllAirconditionerByParam ns;

	_ns1__GetAllAirconditionerByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetAllAirconditionerByParam(&ns, nsResponse);

	//判断返回结果是否成功
	if (nsResponse.GetAllAirconditionerByParamResult == NULL)
	{
		return m_allAirconditions;
	}

	//解析字符串出结果
	CMarkup xml;
	xml.SetDoc((*(nsResponse.GetAllAirconditionerByParamResult)).c_str());

	m_allAirconditions = ParseAirConditionersFromXML(xml);
	return m_allAirconditions;
}

std::vector<AttrAirCon >  CAirConditionWebData::GetAirCons(double p_airConHorseNumber, CString p_airConPipePos, CString p_airConRainRiser, CString p_airConRainRiserPos)const
{
	std::vector<AttrAirCon> data;

	for (UINT i =0; i < m_allAirconditions.size(); i++)
	{
		if (p_airConHorseNumber != m_allAirconditions[i].m_power)
		{
			continue;
		}

		if (p_airConPipePos != m_allAirconditions[i].m_pipePos)
		{
			continue;
		}

		if ((p_airConRainRiser != L"否") != m_allAirconditions[i].m_hasRainPipe)
		{
			continue;
		}

		if (p_airConRainRiser != L"否")
		{
			if (p_airConRainRiserPos !=  m_allAirconditions[i].m_rainPipePos)
			{
				continue;
			}
		}

		data.push_back(m_allAirconditions[i]);
	}

	return data;

}