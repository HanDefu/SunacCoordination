#include "StdAfx.h"
#include "KitchenBathroomWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Common\ComFun_String.h"
#include "..\Common\ComFun_Sunac.h"
#include "WebCommon.h"
#include "WebIO.h"
#include <string>


std::vector<AttrKitchen > CKitchenBathroomWebData::ParseKitchensFromXML(CMarkup xml)const
{
	std::vector<AttrKitchen> vKitchenAttrs;
	AttrKitchen KitchenAttr;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vKitchenAttrs;
		}
	}
	if (xml.FindElem(_T("Kitchens")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Kitchen")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				KitchenAttr.m_prototypeCode = xml.GetData();
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
								KitchenAttr.m_file.id = _ttoi(sFileID);
								KitchenAttr.m_file.fileName = sFileName;
							}
							//检查文件是否存在，不存在则下载
							CString sDWGFilePath = TY_GetPrototypeFilePath() + sFileName;
							CString sImgFilePath = TY_GetPrototypeImagePath()+ sImgFileName;
							if (!JHCom_FileExist(sDWGFilePath))
							{
								WEBINST->DownloadFile(_ttoi(sFileID), "CAD", sDWGFilePath);
							}

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
				if (flag == "1" || flag == L"是")
				{
					KitchenAttr.m_isJiTuan = TRUE;
				}
				else KitchenAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("AreaId")))
			{
				KitchenAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				KitchenAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenTypeName")))
			{
				KitchenAttr.m_kitchenType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenPositionName")))
			{
				KitchenAttr.m_windowDoorPos = (xml.GetData() == L"门窗对开" ? DUIKAI : CHUIZHIKAI);
			}
			if (xml.FindElem(_T("KitchenIsAirduct")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"是")
				{
					KitchenAttr.m_hasPaiQiDao = TRUE;
				}
				else KitchenAttr.m_hasPaiQiDao = FALSE;
			}
			if (xml.FindElem(_T("KitchenOpenSizeMin")))
			{
				KitchenAttr.m_minWidth = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenOpenSizeMax")))
			{
				KitchenAttr.m_maxwidth = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenDepthsizeMin")))
			{
				KitchenAttr.m_minHeight = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenDepthsizeMax")))
			{
				KitchenAttr.m_maxHeight = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenBasinSize")))
			{
				KitchenAttr.m_shuiPenType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenFridgSize")))
			{
				KitchenAttr.m_bingXiangType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenHearthSize")))
			{
				KitchenAttr.m_zaoTaiType = xml.GetData();
			}
			xml.OutOfElem();
			vKitchenAttrs.push_back(KitchenAttr);	
		}


		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vKitchenAttrs;
}

std::vector<AttrBathroom > CKitchenBathroomWebData::ParseBathroomsFromXML(CMarkup xml)const
{
	std::vector<AttrBathroom> vBathroomAttrs;
	AttrBathroom BathroomAttr;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return vBathroomAttrs;
		}
	}
	if (xml.FindElem(_T("Bathrooms")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Bathroom")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				BathroomAttr.m_prototypeCode = xml.GetData();
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
								BathroomAttr.m_file.id = _ttoi(sFileID);
								BathroomAttr.m_file.fileName = sFileName;
							}
							//检查文件是否存在，不存在则下载
							CString sDWGFilePath = TY_GetPrototypeFilePath() + sFileName;
							CString sImgFilePath = TY_GetPrototypeImagePath() + sImgFileName;
							if (!JHCom_FileExist(sDWGFilePath))
							{
								WEBINST->DownloadFile(_ttoi(sFileID), "CAD", sDWGFilePath);
							}

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
				if (flag == "1" || flag == L"是")
				{
					BathroomAttr.m_isJiTuan = TRUE;
				}
				else BathroomAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				BathroomAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				BathroomAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("BathroomTypeName")))
			{
				BathroomAttr.m_sBathroomType = xml.GetData();
			}
			if (xml.FindElem(_T("BathroomShortsideSizeMin")))
			{
				//BathroomAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("BathroomShortsideSizeMax")))
			{
				//BathroomAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("BathroomLongsideSizeMin")))
			{
				//BathroomAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("BathroomLongsideSizeMax")))
			{
				//BathroomAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("BathroomDoorWindowPosition")))
			{
				BathroomAttr.m_windowDoorPos = ToEWindowDoorPos(xml.GetData());
			}
			if (xml.FindElem(_T("HasAirvent")))
			{
				CString flag = xml.GetData();
				if (flag == "1" || flag == L"是")
				{
					BathroomAttr.m_hasPaiQiDao = TRUE;
				}
				else BathroomAttr.m_hasPaiQiDao = FALSE;
			}
			xml.OutOfElem();
			vBathroomAttrs.push_back(BathroomAttr);	
		}


		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vBathroomAttrs;
}

std::vector<AttrKitchen> CKitchenBathroomWebData::GetAllKitchens()const
{
	_ns1__GetAllKitchen ns;
	_ns1__GetAllKitchenResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetAllKitchen(&ns, nsResponse);

	std::vector<AttrKitchen> vKitchenAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllKitchenResult == NULL)
	{
		return vKitchenAttrs;
	}

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllKitchenResult)).c_str());

	vKitchenAttrs = ParseKitchensFromXML(xml);

	return vKitchenAttrs;
}

std::vector<AttrKitchen> CKitchenBathroomWebData::GetKitchens( double kaiJian, double jinShen, CString weiZhiGuanXi, CString type, bool hasPaiQiDao)const 
{
	_ns1__GetAllKitchenParam ns;
	_ns1__GetAllKitchenParamResponse nsResponse;

	std::wstring sKitchenDoorWindowPosition = weiZhiGuanXi;
	std::wstring sKitchenType = type;
	std::wstring sAirVent;

	if (hasPaiQiDao)
	{
		sAirVent = _T("1");
	}
	else sAirVent = _T("0");

	ns.Width = kaiJian;
	ns.Height = jinShen;
	ns.KitchenType = &sKitchenType;
	ns.KitchenDoorWindowPosition = &sKitchenDoorWindowPosition;
	ns.AirVent = &sAirVent;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllKitchenParam(&ns, nsResponse);

	std::vector<AttrKitchen> vKitchenAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllKitchenParamResult == NULL)
	{
		return vKitchenAttrs;
	}

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllKitchenParamResult)).c_str());

	vKitchenAttrs = ParseKitchensFromXML(xml);

	return vKitchenAttrs;

}

std::vector<AttrBathroom> CKitchenBathroomWebData::GetAllBathrooms()const
{
	std::wstring sBathroomDoorWindowPosition, sToiletType;

	_ns1__GetAllBathroom ns;
	_ns1__GetAllBathroomResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetAllBathroom(&ns, nsResponse);

	std::vector<AttrBathroom> vBathroomAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllBathroomResult == NULL)
	{
		return vBathroomAttrs;
	}

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllBathroomResult)).c_str());

	vBathroomAttrs = ParseBathroomsFromXML(xml);

	return vBathroomAttrs;
}

std::vector<AttrBathroom> CKitchenBathroomWebData::GetBathrooms(double width, double height, CString weiZhiGuanXi, CString type)const
{
	std::wstring sWindowDoorPosition = weiZhiGuanXi;
	std::wstring sType = type;

	_ns1__GetAllBathroomByParam ns;
	_ns1__GetAllBathroomByParamResponse nsResponse;

	ns.Width = width;
	ns.Height = height;
	ns.BathroomDoorWindowPosition = &sWindowDoorPosition;
	ns.ToiletType = &sType;

	ArgumentSettingServiceSoapProxy cadWeb;
	InitSoapTime(cadWeb);
	int nRet = cadWeb.GetAllBathroomByParam(&ns, nsResponse);

	std::vector<AttrBathroom> vBathroomAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllBathroomByParamResult == NULL)
	{
		return vBathroomAttrs;
	}

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllBathroomByParamResult)).c_str());

	vBathroomAttrs = ParseBathroomsFromXML(xml);

	return vBathroomAttrs;
}