#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include <algorithm>
#include "AttrWindow.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_Math.h"
#include "../../Common/TYFormula.h"
#include "../../Src/DocumentData.h"

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
	m_isFireproofWindow = false;

	m_tongFengFormula = L"";//通风量计算公式
	m_tongFengQty = 0.0;

	//////////////////////////////////////////////////////////////////////////
	m_isMirror = false;
	m_viewDir = E_VIEW_FRONT;
	m_isBayWindow = false;
	m_wallDis = 0.0;
	m_heightUnderWindow = 0;

	//////////////////////////////////////////////////////////////////////////
	m_fromWinId = AcDbObjectId::kNull;

#ifdef INIT_HANLDLE_LATER_FOR_DWGIN 
	m_fromWinHandle.setNull();
#endif

	m_tangentOpeningId = AcDbObjectId::kNull;
}

AttrWindow::~AttrWindow()
{

}


AttrWindow* AttrWindow::GetWinAtt(AcDbObjectId p_id)
{
	AcDbObject* pObj = NULL;
	TY_GetAttributeData(p_id, pObj);

	AttrWindow *pWinAtt = AttrWindow::cast(pObj);

	return pWinAtt;
}
CString AttrWindow::GetWinInstanceCode(AcDbObjectId p_id)
{
	CString sNumber;
	AttrWindow* pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		sNumber = pWinAtt->GetInstanceCode();
	}
	return sNumber;
}

//AttrWindow::AttrWindow(const AttrWindow &other) : AttrObject(other)
//{
//	*this = other;
//}
//
//AttrWindow& AttrWindow::operator=(const AttrWindow &rhs)
//{
//	AttrObject::operator=(rhs);
//
//	m_dimData = rhs.m_dimData;
//
//	m_frontViewFile = rhs.m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
//	m_topViewFile = rhs.m_topViewFile;		//原型俯视图文件
//	m_leftViewFile = rhs.m_leftViewFile;	//原型侧视图文件
//
//	m_tongFengFormula = rhs.m_tongFengFormula;//通风量计算公式,主要用于动态原型
//	m_tongFengQty = rhs.m_tongFengQty;	//通风量
//
//	m_openType = rhs.m_openType;		//开启类型
//	m_openQty = rhs.m_openQty;	//开启扇数量
//	m_gongNengquType = rhs.m_gongNengquType;//功能区类型
//
//	m_isZhuanJiao = rhs.m_isZhuanJiao;		//是否转角窗
//	m_isMirrorWindow = rhs.m_isMirrorWindow;	//是否对称窗型 
//
//	//////////////////////////////////////////////////////////////////////////
//	//算量相关
//	m_material = rhs.m_material;
//
//	//////////////////////////////////////////////////////////////////////////
//	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
//	//视图属性
//	m_isMirror = rhs.m_isMirror;//是否镜像
//	m_viewDir = rhs.m_viewDir;//视图方向，平面图、立面图、侧视图
//
//	m_isBayWindow = rhs.m_isBayWindow;	 //是否凸窗
//	m_wallDis = rhs.m_wallDis;		 //外墙距离
//	m_heightUnderWindow = rhs.m_heightUnderWindow; //窗下墙高度
//
//	m_floorInfo = rhs.m_floorInfo; //楼层信息
//
//	//////////////////////////////////////////////////////////////////////////
//	m_fromWinId = rhs.m_fromWinId;
//	m_relatedWinIds = rhs.m_relatedWinIds;
//
//	return *this;
//}

void AttrWindow::Clone(const AttrWindow& rhs)
{	
	AttrObject::Clone(rhs);


	m_dimData = rhs.m_dimData;

	m_frontViewFile = rhs.m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	m_topViewFile = rhs.m_topViewFile;		//原型俯视图文件
	m_leftViewFile = rhs.m_leftViewFile;	//原型侧视图文件

	m_tongFengFormula = rhs.m_tongFengFormula;//通风量计算公式,主要用于动态原型
	m_tongFengQty = rhs.m_tongFengQty;	//通风量

	m_openType = rhs.m_openType;		//开启类型
	m_openQty = rhs.m_openQty;	//开启扇数量
	m_gongNengquType = rhs.m_gongNengquType;//功能区类型

	m_isZhuanJiao = rhs.m_isZhuanJiao;		//是否转角窗
	m_isMirrorWindow = rhs.m_isMirrorWindow;	//是否对称窗型 
	m_isFireproofWindow = rhs.m_isFireproofWindow; //是否防火窗型

	//////////////////////////////////////////////////////////////////////////
	//算量相关
	m_material = rhs.m_material;

	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
	//视图属性
	m_isMirror = rhs.m_isMirror;//是否镜像
	m_viewDir = rhs.m_viewDir;//视图方向，平面图、立面图、侧视图

	m_isBayWindow = rhs.m_isBayWindow;	 //是否凸窗
	m_wallDis = rhs.m_wallDis;		 //外墙距离
	m_heightUnderWindow = rhs.m_heightUnderWindow; //窗下墙高度


	//以下关联的实体关系保持原来的 
	//m_fromWinId 
	//m_relatedWinIds
}

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

bool AttrWindow::GetWRange(double& p_min, double& p_max)
{
	CWindowsDimData* pWdata = GetDimDataByCode(_T("W"));
	if (pWdata==NULL)
	{
		return false;
	}

	p_min = pWdata->minValue;
	p_max = pWdata->maxValue;
	return true;
}
bool AttrWindow::GetHRange(double& p_min, double& p_max)
{
	CWindowsDimData* pWdata = GetDimDataByCode(_T("H"));
	if (pWdata == NULL)
	{
		return false;
	}

	p_min = pWdata->minValue;
	p_max = pWdata->maxValue;
	return true;
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
	if (GetDimData(_T("D")) == NULL) //墙厚度
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

	if (m_version >= 5)
	{
		filer->readItem(&m_isFireproofWindow);
	}

	filer->readItem(&m_wallDis);

	if (m_version>=3)
	{
		filer->readItem(&m_heightUnderWindow);

		if (m_version<6) //20200324 版本6：楼层信息从AttrWindow移到基类，以便支持所有的类型
		{
			filer->readString(tempStr);
			CString sFloors = tempStr.kACharPtr();
			m_floorInfo.SetFloors(sFloors);

			double floorHeight;
			filer->readItem(&floorHeight);
			m_floorInfo.SetFloorHeight(floorHeight);
		}
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

	if (m_version >= 4)
	{
#ifndef INIT_HANLDLE_LATER_FOR_DWGIN
		AcDbHandle tempHandle;
		filer->readItem(&tempHandle);
		acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(m_fromWinId, false, tempHandle);

		filer->readItem(&size);
		for (UINT i = 0; i < size; i++)
		{
			filer->readItem(&tempHandle);
			AcDbObjectId tempId;
			es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(tempId, false, tempHandle);
			if (es != Acad::eOk)
			{
				continue;	//当关联的实体在保存前已经删除会出现此情况
			}
			m_relatedWinIds.append(tempId);
		}
#else
		filer->readItem(&m_fromWinHandle);

		filer->readItem(&size);
		for (UINT i = 0; i < size; i++)
		{
			AcDbHandle tempHandle;
			filer->readItem(&tempHandle);
			m_relatedWinHandles.push_back(tempHandle);
		}
#endif
	}

	if (m_version>=7)
	{
		AcDbObjectId curId = objectId();
		AcDbHandle tempHandle;
		filer->readItem(&tempHandle);
		if (filer->filerType()== AcDb::kFileFiler)
		{
			AcDbObjectId tWinOpenIdOut = AcDbObjectId::kNull;
			acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(tWinOpenIdOut, false, tempHandle);
			SetWinTangentOpenId(curId, tWinOpenIdOut);
		}
		else
		{
			m_tangentOpeningId = NULL;
		}
	}

	return filer->filerStatus();
}

eRCType AttrWindow::GetType()const
{
	if (m_prototypeCode.Left(4) == L"Door")
		return S_DOOR;
	return S_WINDOW;
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

	//FILE_VERSION 5 新增
	{
		filer->writeItem(m_isFireproofWindow);
	}

	filer->writeItem(m_wallDis);

	//FILE_VERSION 3 新增
	{
		filer->writeItem(m_heightUnderWindow);

		//20200324 版本6 注释：楼层信息从AttrWindow移到基类，以便支持所有的类型
		//filer->writeItem(m_floorInfo.GetFloors());
		//filer->writeItem(m_floorInfo.GetFloorHeight());
	}
	

	//FILE_VERSION 2 新增
	filer->writeItem(m_material.heatCoeff);
	filer->writeItem(m_material.sAluminumSerial);
	filer->writeItem(m_material.sGlassSerial);
	filer->writeItem(m_material.bHasAuxiliaryFrame);
	filer->writeItem(m_material.sAuxiliaryFrame);

	//FILE_VERSION 4 新增
	filer->writeItem(m_fromWinId.handle());
	filer->writeItem((Adesk::UInt32)m_relatedWinIds.length());
	for (int i = 0; i < m_relatedWinIds.length(); i++)
	{
		filer->writeItem(m_relatedWinIds[i].handle());
	}

	//FILE_VERSION 7 新增
	filer->writeItem(m_tangentOpeningId.handle());

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
		m_isMirror == pRealObj->m_isMirror &&  ////由于用户会通过CAD镜像，m_isMirror只用于创建时，不用于是否镜像的判断
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
	//else if (p_att.m_isMirror != m_isMirror)
	//{
	//	assert(FALSE);
	//	return false;
	//}
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

//主要用于自动编号区分
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

	//是否防火窗
	if (m_isFireproofWindow!=p_att.m_isFireproofWindow)
	{
		return false;
	}

	//是否有附框， 附框尺寸通常是相同的，不进行比较
	if (m_material.bHasAuxiliaryFrame!=p_att.m_material.bHasAuxiliaryFrame)
	{
		return false;
	}

	//型材
	if (0!=m_material.sAluminumSerial.CompareNoCase( p_att.m_material.sAluminumSerial))
	{
		return false;
	}

	return true;
}

E_WindowAluminumType AttrWindow::GetWindowDoorAluminumType() const
{
	CString sType = (GetType() == S_WINDOW ? L"窗" : L"门");

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

bool AttrWindow::IsMxMirror()const //实际的矩阵是否镜像
{
	bool bMirror = m_isMirror;
	if (GetViewDir() == E_VIEW_TOP)
	{
		bMirror = !bMirror; // yuan 1124 原来平面图原型的方向和立面图矛盾的问题 Mirror
	}

	return (bMirror && (m_isMirrorWindow == false));
}
void AttrWindow::SetMxMirror(bool p_bMirror)
{
	if (GetViewDir() == E_VIEW_TOP)
	{
		p_bMirror = !p_bMirror; // yuan 1124 原来平面图原型的方向和立面图矛盾的问题 Mirror
	}

	m_isMirror = p_bMirror;
}

bool AttrWindow::IsMirror()const//非对称窗型才有镜像
{
	return m_isMirror && (m_isMirrorWindow == false); 
} 
void AttrWindow::SetMirror(bool p_bMirror)
{
	if (m_isMirrorWindow)
	{
		m_isMirror = false;
		return;
	}

	m_isMirror = p_bMirror;
}

AcDbObjectId AttrWindow::GetFromWinId() 
{
#ifdef INIT_HANLDLE_LATER_FOR_DWGIN 
	if (m_fromWinId==AcDbObjectId::kNull && m_fromWinHandle.isNull()==false)
	{
		acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(m_fromWinId, false, m_fromWinHandle);
	}
#endif
	return m_fromWinId; 
}
AcDbObjectIdArray  AttrWindow::GetRelatedWinIds() 
{
#ifdef INIT_HANLDLE_LATER_FOR_DWGIN 
	if (m_relatedWinIds.length()==0 && m_relatedWinHandles.size()>0)
	{
		Acad::ErrorStatus es;
		for (UINT i = 0; i < m_relatedWinHandles.size(); i++)
		{
			AcDbObjectId tempId;
			es = acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(tempId, false, m_relatedWinHandles[i]);
			if (es != Acad::eOk)
			{
				assert(false);
				continue;
			}
			m_relatedWinIds.append(tempId);
		}
	}
#endif
	return m_relatedWinIds; 
}

void AttrWindow::SetFromWinId(AcDbObjectId p_id)
{
	m_fromWinId = p_id;
}
void AttrWindow::SetRelatedWinIds(const AcDbObjectIdArray& p_relatedWinIds)
{
	m_relatedWinIds = p_relatedWinIds;
}
void AttrWindow::ClearWinsRelation() //移除关联关系
{
	m_relatedWinIds.removeAll();
	m_fromWinId = AcDbObjectId::kNull;
}
AcDbObjectId AttrWindow::GetWinTangentOpenId()const 
{
	return m_tangentOpeningId; 
}
void AttrWindow::SetWinTangentOpenId(AcDbObjectId p_winId, AcDbObjectId p_tangentOpenid)
{
	m_tangentOpeningId = p_tangentOpenid;
	GetWinTangentOpenMap()->AddWindow(p_winId, p_tangentOpenid);
}