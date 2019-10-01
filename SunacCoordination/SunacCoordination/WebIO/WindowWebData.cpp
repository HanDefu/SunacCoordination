#include "StdAfx.h"
#include "WindowWebData.h"
#include "SunacCadWeb\soapArgumentSettingServiceSoapProxy.h"
#include "../Common\ComFun_Str.h"
#include "..\Tool\MarkupXml\Markup.h"
#include <string>

CWindowWebData::CWindowWebData()
{

}

CWindowWebData::~CWindowWebData()
{

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
	
	AttrWindow attrwindow;

	//解析字符串出结果
	CMarkup xml;	

	xml.SetDoc((*(nsResponse.GetWindowsResult)).c_str());

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
				attrwindow.m_yxid = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				attrwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathTop")))
			{
				attrwindow.m_prototypeFlatFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathFront")))
			{
				attrwindow.m_prototypeFacadeFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathLeft")))
			{
				attrwindow.m_prototypeTopViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathExpanded")))
			{
				attrwindow.m_prototypeSpreadFile = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				attrwindow.m_jiTuan = xml.GetData();
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
				attrwindow.m_DynamicType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenTypeid")))
			{
				attrwindow.m_openType = xml.GetData();

			/*	if (ns.openType != NULL && ns.openType->c_str() != attrwindow.m_openType)
				{
					continue;
				}*/

			}
			if (xml.FindElem(_T("WindowOpenQty")))
			{
				attrwindow.m_openQty =  _ttoi(xml.GetData());

				/*if (ns.openNum != NULL && _ttoi(ns.openNum->c_str()) != attrwindow.m_openQty)
				{
					continue;
				}*/
			}
			if (xml.FindElem(_T("WindowHasCorner")))
			{
				attrwindow.m_isZhuanJiao = xml.GetData();
			}
			if (xml.FindElem(_T("WindowSizeMin")))
			{
				attrwindow.m_minWid = _ttoi(xml.GetData());

				/*if (ns.width < attrwindow.m_minWid)
				{
					continue;
				}*/
			}
			if (xml.FindElem(_T("WindowSizeMax")))
			{
				attrwindow.m_maxWid = _ttoi(xml.GetData());

				/*if (ns.width > attrwindow.m_maxWid)
				{
					continue;
				}*/
			}
			if (xml.FindElem(_T("WindowDesignFormula")))
			{
				attrwindow.m_tongFengFormula = xml.GetData();
			}
			if (xml.FindElem(_T("WindowVentilationQuantity")))
			{
				attrwindow.m_tongFengQty = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowPlugslotSize")))
			{
				attrwindow.m_plugslotSize = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowFunctionalArea")))
			{
				attrwindow.m_functionType = xml.GetData();

				/*if (ns.gongNengQu != NULL && ns.gongNengQu->c_str() != attrwindow.m_functionType)
				{
					continue;
				}*/
			}

			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				if (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					if (xml.FindElem(_T("Code")))
					{
						attrwindow.m_sizeId = xml.GetData();
					}
					if (xml.FindElem(_T("ValueType")))
					{
						attrwindow.m_valueType = xml.GetData();
					}
					if (xml.FindElem(_T("Value")))
					{
						attrwindow.m_value = xml.GetData();
					}
					if (xml.FindElem(_T("MinValue")))
					{
						attrwindow.m_min = xml.GetData();
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						attrwindow.m_max = xml.GetData();
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						attrwindow.m_defaultValue = xml.GetData();
					}
					if (xml.FindElem(_T("ValueDescription")))
					{
						attrwindow.m_state = xml.GetData();
					}
					xml.OutOfElem();
				}
				xml.OutOfElem();
			}
			windowAtts.push_back(attrwindow);	

			xml.OutOfElem();
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();

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
				return allWindowAtts;
			}
		}

		while (xml.FindElem(_T("Window")))
		{
			xml.IntoElem();
			if (xml.FindElem(_T("DrawingCode")))
			{
				attrallwindow.m_yxid = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingName")))
			{
				attrallwindow.m_name = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathTop")))
			{
				attrallwindow.m_prototypeFlatFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathFront")))
			{
				attrallwindow.m_prototypeFacadeFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathLeft")))
			{
				attrallwindow.m_prototypeTopViewFile = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingPathExpanded")))
			{
				attrallwindow.m_prototypeSpreadFile = xml.GetData();
			}
			if (xml.FindElem(_T("Scope")))
			{
				attrallwindow.m_jiTuan = xml.GetData();
			}
			if (xml.FindElem(_T("Areaid")))
			{
				attrallwindow.m_quyuId = xml.GetData();
			}
			if (xml.FindElem(_T("AreaName")))
			{
				attrallwindow.m_quyuName = xml.GetData();
			}
			if (xml.FindElem(_T("DrawingType")))
			{
				attrallwindow.m_type = xml.GetData();
			}
			if (xml.FindElem(_T("WindowDynamicType")))
			{
				attrallwindow.m_DynamicType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenTypeid")))
			{
				attrallwindow.m_openType = xml.GetData();
			}
			if (xml.FindElem(_T("WindowOpenQty")))
			{
				attrallwindow.m_openQty =  _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowHasCorner")))
			{
				attrallwindow.m_isZhuanJiao = xml.GetData();
			}
			if (xml.FindElem(_T("WindowSizeMin")))
			{
				attrallwindow.m_minWid = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowSizeMax")))
			{
				attrallwindow.m_maxWid = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowDesignFormula")))
			{
				attrallwindow.m_tongFengFormula = xml.GetData();
			}
			if (xml.FindElem(_T("WindowVentilationQuantity")))
			{
				attrallwindow.m_tongFengQty = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowPlugslotSize")))
			{
				attrallwindow.m_plugslotSize = _ttoi(xml.GetData());
			}
			if (xml.FindElem(_T("WindowFunctionalArea")))
			{
				attrallwindow.m_functionType = xml.GetData();
			}

			if (xml.FindElem(_T("SizePara")))
			{
				xml.IntoElem();
				while (xml.FindElem(_T("Item")))
				{
					xml.IntoElem();
					if (xml.FindElem(_T("Code")))
					{
						attrallwindow.m_sizeId = xml.GetData();
					}
					if (xml.FindElem(_T("ValueType")))
					{
						attrallwindow.m_valueType = xml.GetData();
					}
					if (xml.FindElem(_T("Value")))
					{
						attrallwindow.m_value = xml.GetData();
					}
					if (xml.FindElem(_T("MinValue")))
					{
						attrallwindow.m_min = xml.GetData();
					}
					if (xml.FindElem(_T("MaxValue")))
					{
						attrallwindow.m_max = xml.GetData();
					}
					if (xml.FindElem(_T("DefaultValue")))
					{
						attrallwindow.m_defaultValue = xml.GetData();
					}
					if (xml.FindElem(_T("ValueDescription")))
					{
						attrallwindow.m_state = xml.GetData();
					}
					xml.OutOfElem();

					allWindowAtts.push_back(attrallwindow);
				}
				xml.OutOfElem();
			}
			xml.OutOfElem();
		}
		xml.OutOfElem();
	}
	xml.OutOfElem();

	return allWindowAtts;
}