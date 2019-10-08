#include "StdAfx.h"
#include "WindowWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "Common\ComFun_String.h"
#include <string>

CWindowWebData::CWindowWebData()
{

}

CWindowWebData::~CWindowWebData()
{

}

std::vector<AttrWindow > CWindowWebData::ParseWindowsFromXML(CMarkup xml)const
{
	std::vector<AttrWindow> windowAtts;
	AttrWindow attrwindow;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Windows")))
	{
		xml.IntoElem();

		if (xml.FindElem(_T("Code")))
		{
			if(_ttoi(xml.GetData()) != 100)
			{
				return windowAtts;
			}
		}

		while (xml.FindElem(_T("Window")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				attrwindow.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathTop")))
			{
				attrwindow.m_topViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathFront")))
			{
				attrwindow.m_frontViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathLeft")))
			{
				attrwindow.m_leftViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathExpanded")))
			{
				attrwindow.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					attrwindow.m_isJiTuan = TRUE;
				}
				else attrwindow.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				attrwindow.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				attrwindow.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingType")))
			{
				attrwindow.m_type = xml.GetData();
			}
			if (xml.FindElem(_T("WindowDynamicType")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					attrwindow.m_isDynamic = TRUE;
				}
				else attrwindow.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("WindowOpenTypeid")))
			{
				attrwindow.m_openType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenQty")))
			{
				attrwindow.m_openQty = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowHasCorner")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					attrwindow.m_isZhuanJiao = TRUE;
				}
				else attrwindow.m_isZhuanJiao = FALSE;
			}
			if (xml.FindElem(_T("WindowSizeMin")))
			{
				attrwindow.m_minWid = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowSizeMax")))
			{
				attrwindow.m_maxWid = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowDesignFormula")))
			{
				attrwindow.m_tongFengFormula = xml.GetData();
			}
			if (xml.FindElem(_T("WindowVentilationQuantity")))
			{
				attrwindow.m_staticTongFengQty = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowPlugslotSize")))
			{
				attrwindow.m_plugslotSize = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowFunctionalArea")))
			{
				attrwindow.m_gongNengquType = xml.GetData();
			}

			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				while (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					SRCDimData tempData;
					if (xml.FindElem(_T("Code")))
					{
						tempData.sCodeName = xml.GetData();
					}
					if (xml.FindElem(_T("ValueType")))
					{
						tempData.type = ToEWindowType(xml.GetData());
					}
					if (xml.FindElem(_T("Value")))
					{
						CString value = xml.GetData();
						std::vector<CString> strs = YT_SplitCString(value, L',');
						for (UINT i = 0; i < strs.size(); i++)
						{
							tempData.values.push_back(_wtof(strs[i]));
						}
					}
					if (xml.FindElem(_T("MinValue")))
					{
						tempData.minValue = _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						tempData.maxValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						tempData.defaultValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("ValueDescription")))
					{
						tempData.prompt = xml.GetData();
					}
					attrwindow.m_dimData.push_back(tempData);
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
			windowAtts.push_back(attrwindow);
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();
	return windowAtts;
}

std::vector<AttrDoor> CWindowWebData::ParseDoorsFromXML(CMarkup xml)const
{
	std::vector<AttrDoor> DoorAttrs;
	AttrDoor Attrdoor;

	xml.ResetMainPos();
	xml.FindElem();	//根节点
	xml.IntoElem();
	if (xml.FindElem(_T("Code")))
	{
		if(_ttoi(xml.GetData()) != 100)
		{
			return DoorAttrs;
		}
	}
	if (xml.FindElem(_T("Doors")))
	{
		xml.IntoElem();

		while (xml.FindElem(_T("Door")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				Attrdoor.m_prototypeCode = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				//attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathTop")))
			{
				Attrdoor.m_topViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathFront")))
			{
				Attrdoor.m_frontViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathLeft")))
			{
				Attrdoor.m_leftViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathExpanded")))
			{
				Attrdoor.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					Attrdoor.m_isJiTuan = TRUE;
				}
				else Attrdoor.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				Attrdoor.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				Attrdoor.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingType")))
			{
				Attrdoor.m_type = xml.GetData();
			}
			if (xml.FindElem(_T("WindowDynamicType")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					Attrdoor.m_isDynamic = TRUE;
				}
				else Attrdoor.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("Doortype")))
			{
				Attrdoor.m_DoorType = xml.GetData();
			}
			if (xml.FindElem(_T("DoorSizeMin")))
			{
				Attrdoor.m_minWid = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("DoorSizeMax")))
			{
				Attrdoor.m_maxWid = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				while (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					SRCDimData tempData;
					if (xml.FindElem(_T("Code")))
					{
						tempData.sCodeName = xml.GetData();
					}
					if (xml.FindElem(_T("ValueType")))
					{
						tempData.type = ToEWindowType(xml.GetData());
					}
					if (xml.FindElem(_T("Value")))
					{
						CString value = xml.GetData();
						std::vector<CString> strs = YT_SplitCString(value, L',');
						for (UINT i = 0; i < strs.size(); i++)
						{
							tempData.values.push_back(_wtof(strs[i]));
						}
					}
					if (xml.FindElem(_T("MinValue")))
					{
						tempData.minValue = _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						tempData.maxValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						tempData.defaultValue =  _ttof(xml.GetData());
					}
					if (xml.FindElem(_T("ValueDescription")))
					{
						tempData.prompt = xml.GetData();
					}
					Attrdoor.m_dimData.push_back(tempData);
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
		DoorAttrs.push_back(Attrdoor);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return DoorAttrs;
}

std::vector<AttrBathroom > CWindowWebData::ParseBathroomsFromXML(CMarkup xml)const
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
				if (xml.FindElem(_T("DrawingPathTop")))
				{
					BathroomAttr.m_fileName = xml.GetData();
				}
				if (xml.FindElem(_T("Scope")))
				{
					CString flag = xml.GetData();
					if (flag == "是")
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
				if (xml.FindElem(_T("BathroomType")))
				{
					BathroomAttr.m_BathroomType = xml.GetData();
				}
				if (xml.FindElem(_T("HasAirvent")))
				{
					CString flag = xml.GetData();
					if (flag == "是")
					{
						BathroomAttr.m_hasPaiQiDao = TRUE;
					}
					else BathroomAttr.m_hasPaiQiDao = FALSE;
				}
				xml.OutOfElem();
			}
			vBathroomAttrs.push_back(BathroomAttr);	

			xml.OutOfElem();
		}
		xml.OutOfElem();
		return vBathroomAttrs;
}

std::vector<AttrKitchen > CWindowWebData::ParseKitchensFromXML(CMarkup xml)const
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
			if (xml.FindElem(_T("DrawingPathTop")))
			{
				KitchenAttr.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					KitchenAttr.m_isJiTuan = TRUE;
				}
				else KitchenAttr.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("Areaid")))
			{
				KitchenAttr.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				KitchenAttr.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenTypeId")))
			{
				KitchenAttr.m_kitchenType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenPositionId")))
			{
				KitchenAttr.m_windowDoorPos = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenIsAirduct")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
				{
					KitchenAttr.m_hasPaiQiDao = TRUE;
				}
				else KitchenAttr.m_hasPaiQiDao = FALSE;
			}
			if (xml.FindElem(_T("KitchenOpensizeMin")))
			{
				//KitchenAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenOpensizeMax")))
			{
				//KitchenAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenDepthsizeMin")))
			{
				//KitchenAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenDepthsizeMax")))
			{
				//KitchenAttr.m_ = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("KitchenBasinSize")))
			{
				//KitchenAttr.m_BathroomType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenFridgeSize")))
			{
				//KitchenAttr.m_BathroomType = xml.GetData();
			}
			if (xml.FindElem(_T("KitchenHearthSize")))
			{
				//KitchenAttr.m_BathroomType = xml.GetData();
			}
			xml.OutOfElem();
		}
		vKitchenAttrs.push_back(KitchenAttr);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vKitchenAttrs;
}

std::vector<AttrRailing > CWindowWebData::ParseRailingsFromXML(CMarkup xml)const
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
			if (xml.FindElem(_T("DrawingPath")))
			{
				RailingAttr.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
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
			if (xml.FindElem(_T("Handrailtype")))
			{
				RailingAttr.m_railingType = ToERailingType(xml.GetData());
			}
			xml.OutOfElem();
		}
		vRailingAttrs.push_back(RailingAttr);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vRailingAttrs;
}

std::vector<AttrAirCon > CWindowWebData::ParseAirConditionersFromXML(CMarkup xml)const
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
				AirConAttr.m_fileName = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "是")
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
				AirConAttr.m_airConHorseNumber = xml.GetData();
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
				AirConAttr.m_airConPipePos = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerIsRainpipe")))
			{
				AirConAttr.m_airConRainRiser = xml.GetData();
			}
			if (xml.FindElem(_T("AirconditionerRainpipePostion")))
			{
				AirConAttr.m_airConRainRiserPos = xml.GetData();
			}
			xml.OutOfElem();
		}
		vAirConAttrs.push_back(AirConAttr);	

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return vAirConAttrs;
}

//从Web端获取窗户
std::vector<AttrWindow> CWindowWebData::GetWindows(double p_width, double p_heigh, CString openType, int openNum, CString gongNengQu)const
{
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
	ns.gongNengQu = &sGongnengqu;

	_ns1__GetWindowsResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetWindows(&ns, nsResponse);

	std::vector<AttrWindow> windowAtts;

	//判断返回结果是否成功
	if (nsResponse.GetWindowsResult == NULL)
	{
		return windowAtts;
	}
	
	//AttrWindow attrwindow;

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetWindowsResult)).c_str());

	windowAtts = ParseWindowsFromXML(xml);

	return windowAtts;

}


//从Web端获取所有窗户
std::vector<AttrWindow>  CWindowWebData::GetAllWindows()const
{
	_ns1__GetAllWindows ns;
	_ns1__GetAllWindowsResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllWindows(&ns, nsResponse);

	std::vector<AttrWindow> allWindowAtts;

	//判断返回结果是否成功
	if (nsResponse.GetAllWindowsResult == NULL)
	{
		return allWindowAtts;
	}

	AttrWindow attrallwindow;

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllWindowsResult)).c_str());

	allWindowAtts = ParseWindowsFromXML(xml);

	return allWindowAtts;

}

std::vector<AttrDoor >  CWindowWebData::GetAllDoors()const
{


	//std::wstring sDoorType = doorType;

	_ns1__GetAllDoor ns;
/*
	ns.width = p_width;
	ns.height = p_heigh;
	ns.doorType = &sDoorType;*/

	_ns1__GetAllDoorResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
	int nRet = cadWeb.GetAllDoor(&ns, nsResponse);

	std::vector<AttrDoor> DoorAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllDoorResult == NULL)
	{
		return DoorAttrs;
	}
	
	AttrDoor Attrdoor;

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllDoorResult)).c_str());

	DoorAttrs = ParseDoorsFromXML(xml);

	return DoorAttrs;
}

std::vector<AttrBathroom> CWindowWebData::GetAllBathrooms()const
{
	std::wstring sBathroomDoorWindowPosition, sToiletType;

	_ns1__GetAllBathroom ns;
	_ns1__GetAllBathroomResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
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

std::vector<AttrKitchen> CWindowWebData::GetAllKitchens()const
{
	_ns1__GetAllKitchen ns;
	_ns1__GetAllKitchenResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
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

std::vector<AttrRailing> CWindowWebData::GetRailings(CString RailingType)const
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

	std::wstring sRailingType = RailingType;

	_ns1__GetAllHandrailByParam ns;

	ns.RailingType = 0;

	_ns1__GetAllHandrailByParamResponse nsResponse;

	ArgumentSettingServiceSoapProxy cadWeb;
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

std::vector<AttrAirCon> CWindowWebData::GetAirConditioners(CString AirconditionerPower, CString AirconditionerPipePosition, int AirconditionerIsRainpipe, CString RainpipePosition)const
{
	CString AirconditionerIsRainpipe_;
	AirconditionerIsRainpipe_.Format(L"%d", AirconditionerIsRainpipe);

	std::wstring sAirconditionerPower = AirconditionerPower;
	std::wstring sAirconditionerPipePosition = AirconditionerPipePosition;
	std::wstring sAirconditionerIsRainpipe = AirconditionerIsRainpipe_;
	std::wstring sRainpipePosition = RainpipePosition;

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

	//AttrWindow attrwindow;

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetAllAirconditionerByParamResult)).c_str());

	AirConAtts = ParseAirConditionersFromXML(xml);
	return AirConAtts;
}