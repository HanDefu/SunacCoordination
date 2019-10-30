/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrAirCon.h"

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrAirCon, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEAIRCON, ZffCustomObjectDBAIRCON);

AttrAirCon::AttrAirCon()
{
	m_airW = 0;		//对应空调数据表格
	m_airH = 0;
	m_airD = 0;
}

AttrAirCon::~AttrAirCon()
{

}

AttrAirCon::AttrAirCon(const AttrAirCon &other) : AttrObject(other)
{
	m_prototypeCode = other.m_prototypeCode;	//空调原型编号
	m_power = other.m_power;				//空调匹数
	m_pipePos = other.m_pipePos;			//空调冷凝水管位置
	m_hasRainPipe = other.m_hasRainPipe;		//空调是否有雨水立管穿过
	m_rainPipePos = other.m_rainPipePos;	//空调雨水管位置
	//m_installNetSize = other.m_installNetSize;//空调安装净尺寸
	m_airW = other.m_airW;
	m_airH = other.m_airH;
	m_airD = other.m_airD;
}

AttrAirCon & AttrAirCon::operator=(const AttrAirCon &rhs)
{
	AttrObject::operator=(rhs);
	return *this;
}

Acad::ErrorStatus AttrAirCon::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}
	return Acad::eOk;
}

Acad::ErrorStatus AttrAirCon::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

bool AttrAirCon::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrAirCon * pRealObj = dynamic_cast<AttrAirCon *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}

bool AttrAirCon::IsPrototypeEqual(const AttrAirCon& p_att)
{
	if (p_att.m_power != m_power)
	{
		return false;
	}
	else if (p_att.m_pipePos != m_pipePos)
	{
		return false;
	}
	else if (p_att.m_hasRainPipe != m_hasRainPipe)
	{
		return false;
	}
	else if (p_att.m_rainPipePos != m_rainPipePos)
	{
		return false;
	}
	else if (p_att.m_airW != m_airW)
	{
		return false;
	}
	else if (p_att.m_airH != m_airH)
	{
		return false;
	}
	else if (p_att.m_airD != m_airD)
	{
		return false;
	}
	
	else if (p_att.m_prototypeCode != m_prototypeCode)
	{
		return false;
	}
	//else if (p_att.m_type != m_type)
	//{
	//	return false;
	//}
	else if (p_att.m_isDynamic != m_isDynamic)
	{
		return false;
	}
	else if (p_att.m_isJiTuan != m_isJiTuan)
	{
		return false;
	}
	else if (p_att.m_quyuId != m_quyuId)
	{
		return false;
	}
	else if (p_att.m_quyuName != m_quyuName)
	{
		return false;
	}
	else if (p_att.m_instanceCode != m_instanceCode)
	{
		return false;
	}
	else return true;
}


CString AttrAirCon::AutoInstanceCode() //空调自动编号只考虑匹数，仅用于统计，不用于查找块
{
	CString sInstance;
	sInstance.Format(_T("AirConditioner_%.1f"), m_power);
	SetInstanceCode(sInstance);
	return sInstance;
}