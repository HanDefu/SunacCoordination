#include "StdAfx.h"
#include "RailingWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_Sunac.h"
#include "WebCommon.h"
#include "WebIO.h"
#include <string>

std::vector<AttrRailing> CRailingWebData::GetRailings(eRailingType RailingType)const
{
/*
	CString opNum;
	opNum.Format(_T("%d"),openNum);

	std::wstring sOpenType = openType;
	std::wstring sOpenNum = opNum.GetString();
	std::wstring sGongnengqu = gongNengQu;

	_ns1__GetWindows ns;
	ns.width = p_width;
	ns.height = p_heigh;
	ns.openType = &sOpenType;
	ns.openNum = &sOpenNum;
	ns.gongNengQu = &sGongnengqu;*/

	//std::wstring sRailingType = RailingType;

	_ns1__GetAllHandrailByParam ns;

	ns.RailingType = 0;

	_ns1__GetAllHandrailByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetAllHandrailByParam(&ns, nsResponse);

	std::vector<AttrRailing> vRailingAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllHandrailByParamResult == NULL)
	{
		return vRailingAttrs;
	}

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllHandrailByParamResult)).c_str());

	vRailingAttrs = ParseRailingsFromXML(xml);

	return vRailingAttrs;
}
std::vector<AttrRailing> CRailingWebData::GetAllRailings()const
{
	std::vector<AttrRailing> railingAtts;


	return railingAtts;
}

std::vector<AttrRailing > CRailingWebData::ParseRailingsFromXML(CMarkup xml)const
{
	std::vector<AttrRailing> vRailingAttrs;
	AttrRailing RailingAttr;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vRailingAttrs;
		}
	}
	if (xml.FindElem(_T("Handrails")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Handrail")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				RailingAttr.m_prototypeCode = xml.GetData();
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
							CString sFileType, tempString, sFileName, sFileID, sImgFileName, sJPGUrl, sDWGUrl;
							if (xml.FindElem(_T("Id")))
							{
								sFileID = xml.GetData();
							}
							if (xml.FindElem(_T("ImgPath")))
							{
								sJPGUrl = xml.GetData();
								if (sJPGUrl != "")
								{
									sImgFileName = WEBINST->GetFileName(sJPGUrl);//获得带扩展名的文件名
								}
							}
							if (xml.FindElem(_T("CADPath")))
							{
								sDWGUrl = xml.GetData();
								if (sDWGUrl != "")
								{
									sFileName = WEBINST->GetFileName(sDWGUrl);//获得带扩展名的文件名
								}
							}
							if (xml.FindElem(_T("CADType")))
							{
								sFileType = xml.GetData();
							}
							if (sFileType == "ExpandViewFile")
							{
								RailingAttr.m_file.id = _ttoi(sFileID);
								RailingAttr.m_file.fileName = sFileName;
							}
							//检查文件是否存在，不存在则下载
							CString sDWGFilePath = TY_GetPrototypeFilePath_Web() + sFileName;
							CString sImgFilePath = TY_GetPrototypeImagePath_Web() + sImgFileName;
							if (!JHCom_FileExist(sDWGFilePath))
							{
								//WEBINST->DownloadFile(_ttoi(sFileID), "CAD", sDWGFilePath);
								WEBINST->DownloadFile(sDWGUrl, sDWGFilePath);
							}

							if (!JHCom_FileExist(sImgFilePath))
							{
								//WEBINST->DownloadFile(_ttoi(sFileID), "Img", sImgFilePath);
								WEBINST->DownloadFile(sJPGUrl, sImgFilePath);
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
				if (flag == "1" || flag == L"是")
				{
					RailingAttr.m_isJiTuan = TRUE;
				}
				else RailingAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				RailingAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				RailingAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("HandrailTypeName")))
			{
				RailingAttr.m_railingType = ToERailingType(xml.GetData());
			}
			xml.OutOfElem();
			vRailingAttrs.push_back(RailingAttr);	
		}


		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vRailingAttrs;
}