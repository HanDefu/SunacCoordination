#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include <algorithm>
#include "AttrWindow.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_Math.h"
#include "../../Common/TYFormula.h"

eWindowDimType ToEWindowType(CString type)
{
	if (type == "固定值" || type == "值")
	{
		return SINGLE;
	}
	else if (type == "值系列" || type == "系列")
	{
		return MULTI;
	}
	else if (type == "范围")
	{
		return SCOPE;
	}
	else if (type == "不限")
	{
		return UNLIMIT;
	}
	else if (type == "公式")
	{
		return CALC;
	}
	else if (type == "无")
	{
		return NOVALUE;
	}
	else
	{
		ASSERT(FALSE);
		return NOVALUE;
	}
}

CWindowsDimData::CWindowsDimData()
{
	type = NOVALUE;
	minValue = 0;
	maxValue = 10000;
	defaultValue = 0;
	value = 0;
}

bool CWindowsDimData::operator==(const CWindowsDimData &rhs) const
{
	if (sCodeName != rhs.sCodeName || type != rhs.type)
	{
		return false;
	}

	switch (type)
	{
	case SINGLE:
	case MULTI:
		return valueOptions == rhs.valueOptions;
		break;
	case SCOPE:
		return JHCOM_equ(minValue, rhs.minValue, 0.1) &&
			JHCOM_equ(maxValue, rhs.maxValue, 0.1);
		break;
	case CALC:
		return sFomula == rhs.sFomula;
		break;
	default:
		break;
	}

	return true;
}

bool CWindowsDimData::operator<(const CWindowsDimData &rhs) const
{
	return sCodeName < rhs.sCodeName;
}

bool CWindowsDimData::IsValueEqual(const CWindowsDimData &rhs)const
{
	if (type==NOVALUE)
	{
		return true;
	}
	else
	{
		return JHCOM_equ(value, rhs.value, 0.1);
	}
}

bool CWindowsDimData::IsParaEqual(const CWindowsDimData &rhs) const
{
	if (sCodeName != rhs.sCodeName)
		return false;
	if ((type == NOVALUE) && (rhs.type == NOVALUE))
		return true;
	if ((type == NOVALUE) ^ (rhs.type == NOVALUE))
		return false;
	if (type == CALC)
		return sFomula == rhs.sFomula;
	else
		return value == rhs.value;
}

bool CWindowsDimData::SetValue(double p_value)
{
	if (type == CALC || type==NOVALUE)
		return false;

	if (type == SCOPE)
	{
		if (p_value<minValue || p_value> maxValue)
			return false;
	}

	value = p_value;
	return true;
}

//////////////////////////////////////////////////////////////////////////

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrWindow, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEWINDOW, ZffCustomObjectDBWINDOW);
AttrWindow::AttrWindow()
{
	m_openType = L"";
	m_openQty = 1;
	m_isZhuanJiao = false;//是否转角窗
	m_isMirrorWindow = false;

	m_tongFengFormula = L"";//通风量计算公式
	m_tongFengQty = 0.0;

	//////////////////////////////////////////////////////////////////////////
	m_isMirror = false;
	m_viewDir = E_VIEW_FRONT;
	m_isBayWindow = false;
	m_wallDis = 0.0;
}

AttrWindow::~AttrWindow()
{

}

//AttrWindow::AttrWindow(const AttrWindow &other) : AttrObject(other)
//{
//	*this = other;
//}
//
//AttrWindow & AttrWindow::operator=(const AttrWindow &other)
//{
//	AttrObject::operator=(other);
//
//	m_openType = other.m_openType;
//	m_openQty = other.m_openQty;
//	m_isZhuanJiao = other.m_isZhuanJiao;
//	m_minWid = other.m_minWid;
//	m_maxWid = other.m_maxWid;
//	m_tongFengFormula = other.m_tongFengFormula;
//	m_tongFengQty = other.m_tongFengQty;
//
//	m_isMirror = other.m_isMirror;
//	m_viewDir = other.m_viewDir;
//	m_wallDis = other.m_wallDis;
//
//	////////////////////////////
//	m_id = other.m_id;					//序号
//	m_prototypeCode = other.m_prototypeCode;		//原型编号
//	m_name = other.m_name;		//原型文件
//	m_scopeOfApplication = other.m_scopeOfApplication;	//适用范围
//	m_Dynamic = other.m_Dynamic;			//是否动态
//	m_functionType = other.m_functionType;		//功能区类型
//	m_openType = other.m_openType;			//开启类型
//	m_openQty = other.m_openQty;			//开启数量
//	//widthMin = other.widthMin;			//宽度最小值
//	//widthMax = other.widthMax;			//宽度最大值
//	m_tongFengFormula = other.m_tongFengFormula;	//通风量公式	
//	m_width = other.m_width;		//静态宽度	
//	m_height = other.m_height;		//静态高度	
//	m_tongFengQty = other.m_tongFengQty;	//静态通风量	
//	m_topViewFile = other.m_topViewFile;	//原型平面文件
//	m_leftViewFile = other.m_leftViewFile;//原型俯视图文件
//
//	m_dimData = other.m_dimData;
//
//	return *this;
//}

const CWindowsDimData* AttrWindow::GetDimData(CString p_sCode)const
{
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		if (m_dimData[i].sCodeName.CompareNoCase(p_sCode) == 0)
		{
			return &(m_dimData[i]);
		}
	}

	//assert(false);
	return NULL;
}
CWindowsDimData* AttrWindow::GetDimDataByCode(CString p_sCode)
{
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		if (m_dimData[i].sCodeName.CompareNoCase(p_sCode) == 0)
		{
			return &(m_dimData[i]);
		}
	}
	return NULL;
}


void AttrWindow::CheckAndComplementDimeData() //检查并补全Dim数据，W/H/a确保都有
{
	if (GetDimData(_T("W"))==NULL)
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("W");
		dimData.type = UNLIMIT;
		dimData.defaultValue = 1200;
		dimData.value = 1200;
		SetDimData(dimData);
	}
	if (GetDimData(_T("H")) == NULL)
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("W");
		dimData.type = UNLIMIT;
		dimData.defaultValue = 1500;
		dimData.value = 1500;
		SetDimData(dimData);
	}
	if (GetDimData(_T("a")) == NULL)
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("A");
		dimData.type = UNLIMIT;
		dimData.defaultValue = 0;
		dimData.value = 0;
		SetDimData(dimData);
	}
}
void AttrWindow::SetDimData(const CWindowsDimData& p_dim)
{
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		if (m_dimData[i].sCodeName.CompareNoCase(p_dim.sCodeName)==0)
		{
			m_dimData[i] = p_dim;
			return ;
		}
	}

	m_dimData.push_back(p_dim);
}

double AttrWindow::GetTongFengQty(bool bDefaultValue/* = false*/) const
{
	if (m_isDynamic)
	{
		CFormulaParser parser(m_tongFengFormula);
		double ret;
		E_ParserStatus es;
		while ((es = parser.TryParse(ret)) == E_PS_UnknownVars)
		{
			vCString vars = parser.GetUnknownVars();
			for (UINT i = 0; i < vars.size(); i++)
			{
				parser.SetVar(vars[i], GetValue(vars[i], bDefaultValue));
			}
		}
		if (es == E_PS_InvalidFormula)
		{
			CString errMsg = L"通风量公式格式错误，无法计算\n";
			acutPrintf(errMsg);
			return 0;
		}
		else if (es == E_PS_PaserOverflow)
		{
			CString errMsg = L"通风量公式存在循环推导，无法计算\n";
			acutPrintf(errMsg);
			return 0;
		}
		else if (es == E_PS_Success)
		{
			return ret / 1E6;
		}
		else
		{
			assert(false);
			return 0;
		}
	}
	else
	{
		return m_tongFengQty;
	}
}

bool AttrWindow::HasValue(CString p_sCode)const
{
	const CWindowsDimData* pDimData = GetDimData(p_sCode);
	if (pDimData == NULL || pDimData->type == NOVALUE)
	{
		return false;
	}

	return true;
}

double AttrWindow::GetValue(CString p_sCode, bool bDefaultValue/* = false*/) const
{
	const CWindowsDimData* pDimData = GetDimData(p_sCode);
	if (pDimData == NULL)
	{
		assert(false);
		return 0;
	}
	
	assert(pDimData->type != NOVALUE);
	if (pDimData->type == CALC)
	{
		CFormulaParser parser(pDimData->sFomula);
		double ret;
		E_ParserStatus es;
		while ((es = parser.TryParse(ret)) == E_PS_UnknownVars)
		{
			vCString vars = parser.GetUnknownVars();
			for (UINT i = 0; i < vars.size(); i++)
			{
				parser.SetVar(vars[i], GetValue(vars[i], bDefaultValue));
			}
		}

		if (es == E_PS_InvalidFormula)
		{
			CString errMsg;
			errMsg.Format(L"参数%s的公式格式错误，无法计算\n", pDimData->sCodeName);
			acutPrintf(errMsg);
			return 0;
		}
		else if (es == E_PS_PaserOverflow)
		{
			CString errMsg;
			errMsg.Format(L"参数%s的公式存在循环推导，无法计算\n", pDimData->sCodeName);
			acutPrintf(errMsg);
			return 0;
		}
		else if (es == E_PS_Success)
		{
			return ret;
		}
		else
		{
			assert(false);
			return 0;
		}
	}
	else if (bDefaultValue)
	{
		return pDimData->defaultValue;
	}
	else
	{
		return pDimData->value;
	}
}

bool AttrWindow::SetValue(CString p_sCode, double p_dValue)
{
	CWindowsDimData* pDimData = GetDimDataByCode(p_sCode);
	if (pDimData==NULL)
	{
		return false;
	}

	return pDimData->SetValue(p_dValue);
}

Acad::ErrorStatus AttrWindow::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) 
		return es;

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];
	filer->readItem(&tempStr);
	m_openType = CString(tempStr);

	filer->readItem(&m_openQty);
	filer->readItem(&m_isZhuanJiao);

	filer->readItem(&tempStr);
	m_tongFengFormula = CString(tempStr);

	filer->readItem(&m_tongFengQty);


	//TODO 把所有的属性读取补充完整

	delete [] tempStr;


	return filer->filerStatus();
}

eRCType AttrWindow::GetType()const
{
	if (m_prototypeCode.Left(4) == L"Door")
		return DOOR;
	return WINDOW;
}

CString AttrWindow::GetMainPrototypeCode()const //返回原型主编码，如Window_NC2_0 返回的值为Window_NC2
{
	int nPos1 = m_prototypeCode.Find(_T('_'));
	if (nPos1>0)
	{
		int nPos2 = m_prototypeCode.Find(_T('_'), nPos1 + 1);
		if (nPos2>0)
		{
			return m_prototypeCode.Left(nPos2);
		}
	}

	return m_prototypeCode;
}

Acad::ErrorStatus AttrWindow::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->writeItem(m_openType);
	filer->writeItem(m_openQty);
	filer->writeItem(m_isZhuanJiao);
	filer->writeItem(m_tongFengFormula);
	filer->writeItem(m_tongFengQty);

	//TODO 把所有的属性读取补充完整

	return filer->filerStatus();
}

bool AttrWindow::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrWindow * pRealObj = dynamic_cast<AttrWindow *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	//TODO 变量比较完整

	return (m_openType == pRealObj->m_openType &&
		m_openQty == pRealObj->m_openQty &&
		m_isZhuanJiao == pRealObj->m_isZhuanJiao &&
		m_tongFengFormula == pRealObj->m_tongFengFormula &&
		m_tongFengQty == pRealObj->m_tongFengQty &&
		m_isMirror == pRealObj->m_isMirror &&
		m_viewDir == pRealObj->m_viewDir &&
		m_wallDis == pRealObj->m_wallDis
		);
}

bool AttrWindow::IsPrototypeEqual(const AttrWindow& p_att)
{
	AttrWindow LocalData = *this;
	if (!p_att.m_dimData.empty() && !m_dimData.empty())
	{
		vector<CWindowsDimData>::const_iterator it1 = p_att.m_dimData.begin();
		vector<CWindowsDimData>::iterator it2 = m_dimData.begin();
		while (it1 != p_att.m_dimData.end() && it2 != m_dimData.end())
		{
			for(;it1->sCodeName == it2->sCodeName; it1++)
			{
				if (it1->value != it2->value)
				{
					assert(FALSE);
					return false;
				}
				if (it1->type != it2->type)
				{
					assert(FALSE);
					return false;
				}
				if (it1->sFomula != it2->sFomula)
				{
					assert(FALSE);
					return false;
				}
				if (it1->prompt != it2->prompt)
				{
					assert(FALSE);
					return false;
				}
				if (it1->minValue != it2->minValue)
				{
					assert(FALSE);
					return false;
				}
				if (it1->maxValue != it2->maxValue)
				{
					assert(FALSE);
					return false;
				}
				/*if (it1->defaultValue != it2->defaultValue)
				{
					assert(FALSE);
					return false;
				}*/ //网页端无法输入默认值
			}

			it2++;
		}
	}

	else if (p_att.m_instanceCode != m_instanceCode)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_isBayWindow != m_isBayWindow)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_isDynamic != m_isDynamic)
	{
		assert(FALSE);
		return false;
	}
/*
	else if (p_att.m_isJiTuan != m_isJiTuan)
	{
		assert(FALSE);
		return false;
	}*/
	else if (p_att.m_isMirror != m_isMirror)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_isMirrorWindow != m_isMirrorWindow)
	{
		assert(FALSE);
		return false;
	}
/*
	else if (p_att.m_isZhuanJiao != m_isZhuanJiao)
	{
		assert(FALSE);
		return false;
	}*/
	else if (p_att.m_openQty != m_openQty)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_openType != m_openType)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_prototypeCode != m_prototypeCode)
	{
		assert(FALSE);
		return false;
	}
/*
	else if (p_att.m_quyuId != m_quyuId)
	{
		assert(FALSE);
		return false;
	}*/
/*
	else if (p_att.m_quyuName != m_quyuName)
	{
		return false;
	}*/
	else if (p_att.m_tongFengQty != m_tongFengQty)
	{
		assert(FALSE);
		return false;
	}
/*
	else if (p_att.m_tongFengFormula != m_tongFengFormula)
	{
		assert(FALSE);
		return false;
	}*/
/*
	else if (p_att.m_version != m_version)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_viewDir != m_viewDir)
	{
		assert(FALSE);
		return false;
	}
	else if (p_att.m_wallDis != m_wallDis)
	{
		assert(FALSE);
		return false;
	}
	else return true;*/
	return true;
}

bool AttrWindow::IsInstanceEqual(const AttrWindow& p_att) const
{
	if (GetMainPrototypeCode() != p_att.GetMainPrototypeCode())
		return false;
	
	vector<CWindowsDimData> dim1 = m_dimData;
	vector<CWindowsDimData> dim2 = p_att.m_dimData;
	sort(dim1.begin(), dim1.end());
	sort(dim2.begin(), dim2.end());

	if (dim1.size() != dim2.size())
		return false;
	for (UINT i = 0; i < dim1.size(); i++)
	{
		if (!dim1[i].IsParaEqual(dim2[i]))
			return false;
	}

	if (!m_isMirrorWindow && m_isMirror != p_att.m_isMirror)
		return false;

	return true;
}

E_WindowDoorType AttrWindow::GetWindowDoorType() const
{
	CString sType = (GetType() == WINDOW ? L"窗" : L"门");
	return ToWindowDoorType(m_openType + sType);
}
