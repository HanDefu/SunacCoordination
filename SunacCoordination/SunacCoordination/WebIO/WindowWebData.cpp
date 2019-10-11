#include "StdAfx.h"
#include "WindowWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Common\ComFun_String.h"
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
			AttrWindow attrwindow;
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
				if (flag == "1")
				{
					attrwindow.m_isJiTuan = TRUE;
				}
				else attrwindow.m_isJiTuan = FALSE;
			}
			if (xml.FindElem(_T("AreaId")))
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
				if (flag == "1")
				{
					attrwindow.m_isDynamic = TRUE;
				}
				else attrwindow.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("WindowOpenTypeName")))
			{
				attrwindow.m_openType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenQtyName")))
			{
				attrwindow.m_openQty = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowHasCorner")))
			{
				CString flag = xml.GetData();
				if (flag == "1")
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
					if (xml.FindElem(_T("SizeNo")))
					{
						tempData.sCodeName = xml.GetData();
					}
					if (xml.FindElem(_T("ValueTypeName")))
					{
						tempData.type = ToEWindowType(xml.GetData());
					}
					if (xml.FindElem(_T("Val")))
					{
						CString value = xml.GetData();
						if (tempData.type == MULTI)
						{
							std::vector<CString> strs = YT_SplitCString(value, L',');
							for (UINT i = 0; i < strs.size(); i++)
							{
								tempData.values.push_back(_wtof(strs[i]));
							}
						}
						else if (tempData.type == CALC)
						{
							tempData.sFomula = value;
						}
						else tempData.values.push_back(_ttof(value));

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
					if (xml.FindElem(_T("Desc")))
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

std::vector<AttrWindow> CWindowWebData::ParseDoorsFromXML(CMarkup xml)const
{
	std::vector<AttrWindow> DoorAttrs;

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
			AttrWindow Attrdoor;
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				Attrdoor.m_prototypeCode = xml.GetData();
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
							if (xml.FindElem(_T("CADType")))
							{
								if (xml.GetData() == "FrontViewFile")
								{
									xml.FindElem(_T("FileClass"));
									Attrdoor.m_frontViewFile = xml.GetData();
								}
								else if (xml.GetData() == "LeftViewFile")
								{
									xml.FindElem(_T("FileClass"));
									Attrdoor.m_leftViewFile = xml.GetData();
								}
								else if (xml.GetData() == "TopViewFile")
								{
									xml.FindElem(_T("FileClass"));
									Attrdoor.m_topViewFile = xml.GetData();
								}
								else if (xml.GetData() == "ExpandViewFile")
								{
									xml.FindElem(_T("FileClass"));
									Attrdoor.m_fileName = xml.GetData();
								}
							}
						}
						xml.OutOfElem();
					}
				}
				xml.OutOfElem();
			}
			/*if (xml.FindElem(_T("DrawingPathTop")))
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
			}*/
			if (xml.FindElem(_T("Scope")))
			{
				CString flag = xml.GetData();
				if (flag == "1")
				{
					Attrdoor.m_isJiTuan = TRUE;
				}
				else Attrdoor.m_isJiTuan = FALSE;
			}
/*TODO
			if (xml.FindElem(_T("Areaid")))
			{
				Attrdoor.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				Attrdoor.m_quyuName = xml.GetData();
			}*/
			if (xml.FindElem(_T("DrawingType")))
			{
				Attrdoor.m_type = xml.GetData();
			}
			if (xml.FindElem(_T("DynamicType")))
			{
				CString flag = xml.GetData();
				if (flag == "1")
				{
					Attrdoor.m_isDynamic = TRUE;
				}
				else Attrdoor.m_isDynamic = FALSE;
			}
			if (xml.FindElem(_T("DoorTypeName")))
			{
				Attrdoor.m_openType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowSizeMin")))
			{
				Attrdoor.m_minWid = _ttof(xml.GetData());
			}
			if (xml.FindElem(_T("WindowSizeMax")))
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
			DoorAttrs.push_back(Attrdoor);
		}
		

		xml.OutOfElem();
	}
	xml.OutOfElem();
	return DoorAttrs;
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

std::vector<AttrWindow >  CWindowWebData::GetAllDoors()const
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

	std::vector<AttrWindow> DoorAttrs;

	//判断返回结果是否成功
	if (nsResponse.GetAllDoorResult == NULL)
	{
		return DoorAttrs;
	}
	
	//解析字符串出结果
	CMarkup xml;	
	xml.SetDoc((*(nsResponse.GetAllDoorResult)).c_str());

	DoorAttrs = ParseDoorsFromXML(xml);

	return DoorAttrs;
}

