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

bool CWindowsDimData::SetDefaultValue(double p_value)
{
	if (type == CALC || type==NOVALUE)
		return false;

	if (type == SCOPE)
	{
		if (p_value<minValue || p_value> maxValue)
			return false;
	}

	defaultValue = p_value;
	value = defaultValue;
	return true;
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
	m_heightUnderWindow = 900;

	//////////////////////////////////////////////////////////////////////////
	m_fromHandle = AcDbObjectId::kNull;
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
		dimData.SetDefaultValue(1200);
		SetDimData(dimData);
	}
	if (GetDimData(_T("H")) == NULL)
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("W");
		dimData.type = UNLIMIT;
		dimData.SetDefaultValue(1500);
		SetDimData(dimData);
	}
	if (GetDimData(_T("a")) == NULL)
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("A");
		dimData.type = UNLIMIT;
		dimData.SetDefaultValue(25);
		SetDimData(dimData);
	}
	if (GetDimData(_T("d")) == NULL) //墙厚度
	{
		CWindowsDimData dimData;
		dimData.sCodeName = _T("D");
		dimData.type = UNLIMIT;
		dimData.SetDefaultValue(200);
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
double AttrWindow::GetWindowArea()const
{
	return GetH() * GetW() / 1000000.0;
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
bool AttrWindow::IsValueCanbeSet(CString p_sCode)const //当前代号对应的值能否被设置
{
	const CWindowsDimData* pDimData = GetDimData(p_sCode);
	if (pDimData == NULL || 
		pDimData->type == NOVALUE ||
		pDimData->type==CALC) //若为计算的也不能设置
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

	if (pDimData->type == NOVALUE)
	{
		return 0;
	}
	
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

bool AttrWindow::SetHeightUnderWindow(double newValue)
{
	m_heightUnderWindow = newValue;

	SetH3(newValue);//原型的H3也是窗下高度
	return true;
}

Acad::ErrorStatus AttrWindow::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) 
		return es;

	AcString tempStr;
	
	Adesk::UInt32 size;
	filer->readItem(&size);
	m_dimData.resize(size);
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		filer->readString(tempStr);
		m_dimData[i].sCodeName = tempStr.kACharPtr();
		filer->readString(tempStr);
		m_dimData[i].sFomula = tempStr.kACharPtr();
		filer->readString(tempStr);
		m_dimData[i].prompt = tempStr.kACharPtr();

		filer->readItem(&m_dimData[i].minValue);
		filer->readItem(&m_dimData[i].maxValue);
		filer->readItem(&m_dimData[i].defaultValue);
		filer->readItem(&m_dimData[i].value);

		filer->readItem((Adesk::UInt32*)&m_dimData[i].type);

		filer->readItem(&size);
		m_dimData[i].valueOptions.resize(size);
		for (UINT j = 0; j < m_dimData[i].valueOptions.size(); j++)
			filer->readItem(&m_dimData[i].valueOptions[j]);
	}

	dwgInFileInfo(filer, m_frontViewFile);
	dwgInFileInfo(filer, m_leftViewFile);
	dwgInFileInfo(filer, m_topViewFile);

	filer->readString(tempStr);
	m_tongFengFormula = tempStr.kACharPtr();

	filer->readItem(&m_tongFengQty);

	filer->readString(tempStr);
	m_openType = tempStr.kACharPtr();

	filer->readItem(&m_openQty);

	filer->readString(tempStr);
	m_gongNengquType = tempStr.kACharPtr();

	filer->readItem(&m_isZhuanJiao);

	filer->readItem(&m_isMirrorWindow);

	filer->readItem(&m_isMirror);

	filer->readItem((Adesk::UInt32*)&m_viewDir);

	filer->readItem(&m_isBayWindow);

	filer->readItem(&m_wallDis);

	if (m_version>=3)
	{
		filer->readItem(&m_heightUnderWindow);

		filer->readString(tempStr);
		CString sFloors = tempStr.kACharPtr();
		m_floorInfo.SetFloors(sFloors);

		double floorHeight;
		filer->readItem(&floorHeight);
		m_floorInfo.SetFloorHeight(floorHeight);
	}

	if (m_version >= 2)
	{
		filer->readItem(&m_material.heatCoeff);
		filer->readString(tempStr);
		m_material.sAluminumSerial = tempStr.kACharPtr();
		filer->readString(tempStr);
		m_material.sGlassSerial = tempStr.kACharPtr();
		filer->readItem(&m_material.bHasAuxiliaryFrame);
		filer->readString(tempStr);
		m_material.sAuxiliaryFrame = tempStr.kACharPtr();
	}

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

	filer->writeItem((Adesk::UInt32)m_dimData.size());
	for (UINT i = 0; i < m_dimData.size(); i++)
	{
		filer->writeItem(m_dimData[i].sCodeName);
		filer->writeItem(m_dimData[i].sFomula);
		filer->writeItem(m_dimData[i].prompt);

		filer->writeItem(m_dimData[i].minValue);
		filer->writeItem(m_dimData[i].maxValue);
		filer->writeItem(m_dimData[i].defaultValue);
		filer->writeItem(m_dimData[i].value);

		filer->writeItem((Adesk::UInt32)m_dimData[i].type);

		filer->writeItem((Adesk::UInt32)m_dimData[i].valueOptions.size());
		for (UINT j = 0; j < m_dimData[i].valueOptions.size(); j++)
			filer->writeItem(m_dimData[i].valueOptions[j]);
	}

	dwgOutFileInfo(filer, m_frontViewFile);
	dwgOutFileInfo(filer, m_leftViewFile);
	dwgOutFileInfo(filer, m_topViewFile);

	filer->writeItem(m_tongFengFormula);

	filer->writeItem(m_tongFengQty);

	filer->writeItem(m_openType);

	filer->writeItem(m_openQty);

	filer->writeItem(m_gongNengquType);

	filer->writeItem(m_isZhuanJiao);

	filer->writeItem(m_isMirrorWindow);

	filer->writeItem(m_isMirror);

	filer->writeItem((Adesk::UInt32)m_viewDir);

	filer->writeItem(m_isBayWindow);

	filer->writeItem(m_wallDis);

	//FILE_VERSION 3 新增
	{
		filer->writeItem(m_heightUnderWindow);

		filer->writeItem(m_floorInfo.GetFloors());
		filer->writeItem(m_floorInfo.GetFloorHeight());
	}
	

	//FILE_VERSION 2 新增
	filer->writeItem(m_material.heatCoeff);
	filer->writeItem(m_material.sAluminumSerial);
	filer->writeItem(m_material.sGlassSerial);
	filer->writeItem(m_material.bHasAuxiliaryFrame);
	filer->writeItem(m_material.sAuxiliaryFrame);

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

bool AttrWindow::IsPrototypeEqual_test(const AttrWindow& p_att)
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

E_WindowAluminumType AttrWindow::GetWindowDoorAluminumType() const
{
	CString sType = (GetType() == WINDOW ? L"窗" : L"门");

	CString sFullName = m_openType;
	if (m_openType.Find(sType)<0)
	{
		sFullName = m_openType + sType;
	}
	return ToWindowDoorAluminumType(sFullName);
}

CString AttrWindow::GetPrototypeDwgFilePath(eViewDir p_view)const
{
	CString sFileName;
	switch (p_view)
	{
	case E_VIEW_FRONT:
		sFileName = m_frontViewFile.fileName;
		break;
	case E_VIEW_TOP:
		sFileName = m_topViewFile.fileName;
		break;
	case E_VIEW_LEFT:
		sFileName = m_leftViewFile.fileName;
		break;
	case E_VIEW_EXTEND:
		sFileName = m_file.fileName;
		break;
	default:
		break;
	}

	if (sFileName.IsEmpty())
	{
		return sFileName;
	}


	return TY_GetPrototypeFilePath() + sFileName;
}

//////////////////////////////////////////////////////////////////////////
CWindowAndCount::CWindowAndCount()
{
	nCount = 0;
}

void CWindowCountArray::InitByWindowIds(const vAcDbObjectId& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = p_winIds[i];
		oneWindow.InitParameters();

		AttrWindow* pAtt = oneWindow.GetAttribute();
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i]);
		}
	}

	InitByWindowAtts(winAtts, winIds);
}

//vRCWindow allWindowsTypes;
//for (UINT i = 0; i < winIds.size(); i++)
//{
//	RCWindow oneWindow;
//	oneWindow.m_id = winIds[i];
//	oneWindow.InitParameters();
//	oneWindow.GetAttribute();
//	int index = vFind(oneWindow, allWindowsTypes);
//	if (index == -1)
//	{
//		oneWindow.m_sameTypeIds.push_back(oneWindow.m_id);
//		allWindowsTypes.push_back(oneWindow);
//	}
//	else
//	{
//		allWindowsTypes[index].m_sameTypeIds.push_back(oneWindow.m_id);
//	}
//}

void CWindowCountArray::InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids)
{
	assert(p_ids.size() == p_winAtts.size());
	bool bUseIds = p_ids.size() == p_winAtts.size();
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //原型编号
		bool bFind = false;
		for (UINT n = 0; n < m_winCountArray.size(); n++)
		{
			if (m_winCountArray[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode) == 0)
			{
				bFind = true;
				if (p_winAtts[i].m_viewDir == E_VIEW_TOP) //平面图
				{
					m_winCountArray[n].nCount += p_winAtts[i].m_floorInfo.GetFloorCount();
				}
				else
				{
					m_winCountArray[n].nCount++;
				}

				m_winCountArray[n].objIds.append(p_ids[i]);
				break;
			}
		}

		if (bFind == false)
		{
			CWindowAndCount winNew;
			winNew.winAtt = p_winAtts[i];
			if (p_winAtts[i].m_viewDir == E_VIEW_TOP) //平面图
			{
				winNew.nCount = p_winAtts[i].m_floorInfo.GetFloorCount();
			}
			else
			{
				winNew.nCount = 1;
			}
			winNew.objIds.append(p_ids[i]);
			m_winCountArray.push_back(winNew);
		}
	}
}


AcDbObjectId  GenerateWindow(const AttrWindow& curWinAtt, const AcGePoint3d pos, 
	eViewDir p_view, E_DIRECTION p_winDir, bool p_bDetailWnd, CString p_sLayerName)
{
	AcGePoint3d insertPos = pos;
	double rotateAngle = 0;
	AcGePoint3d mirrorBasePt(pos.x + curWinAtt.GetW() / 2, 0, 0);
	AcGeVector3d mirrorAxis = AcGeVector3d(0, 1, 0);

	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			rotateAngle = PI;
			offsetXY.x += curWinAtt.GetW();
			break;
		case E_DIR_RIGHT:
			rotateAngle = -PI / 2;
			offsetXY.y += curWinAtt.GetW();
			mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			rotateAngle = PI / 2;
			mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_UNKNOWN:
		default:
			return AcDbObjectId::kNull;
			break;
		}

		insertPos += offsetXY;
	}

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, insertPos, rotateAngle, p_sLayerName, 256);

	//////////////////////////////////////////////////////////////////////////
	//更新参数
	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", curWinAtt.GetH());
	oneWindow.SetParameter(L"W", curWinAtt.GetW());
	oneWindow.SetParameter(L"A", curWinAtt.GetA());
	oneWindow.SetParameter(L"D", curWinAtt.GetD());

	oneWindow.SetParameter(L"W1", curWinAtt.GetW1());
	if (curWinAtt.HasValue(_T("W2")))
		oneWindow.SetParameter(L"W2", curWinAtt.GetW2());
	if (curWinAtt.HasValue(_T("W3")))
		oneWindow.SetParameter(L"W3", curWinAtt.GetW3());

	if (curWinAtt.HasValue(_T("H1")))
		oneWindow.SetParameter(L"H1", curWinAtt.GetH1());
	if (curWinAtt.HasValue(_T("H2")))
		oneWindow.SetParameter(L"H2", curWinAtt.GetH2());
	if (curWinAtt.HasValue(_T("H3")))
		oneWindow.SetParameter(L"H3", curWinAtt.GetH3());

	oneWindow.RunParameters();
	//////////////////////////////////////////////////////////////////////////
	//处理可见性
	if (p_view==E_VIEW_FRONT)
	{
		DQ_SetDynamicAttribute(id, _T("可见性1"), p_bDetailWnd ? _T("详图") : _T("立面"));
	}


	//////////////////////////////////////////////////////////////////////////
	//处理镜像
	bool bMirror = curWinAtt.m_isMirror;
	if (p_view == E_VIEW_TOP)
	{
		bMirror = !bMirror; // yuan 1124 原来平面图原型的方向和立面图矛盾的问题
	}
	if (bMirror && (curWinAtt.m_isMirrorWindow == false))
	{
		TYCOM_Mirror(oneWindow.m_id, mirrorBasePt, mirrorAxis);
	}


	//////////////////////////////////////////////////////////////////////////
	//添加属性
	if (p_bDetailWnd==false) //门窗详图不需要添加属性
	{
		//把数据记录在图框的扩展字典中
		AttrWindow * pWindow = new AttrWindow(curWinAtt);
		pWindow->m_viewDir = p_view;
		oneWindow.AddAttribute(pWindow);
		pWindow->close();
	}

	//针对USC坐标处理
	AcGeMatrix3d mat; 
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity)==false)
	{
		TYCOM_Transform(id, mat);
	}
	
	return id;
}
