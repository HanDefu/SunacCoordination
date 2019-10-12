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

}

AttrAirCon::~AttrAirCon()
{

}

AttrAirCon::AttrAirCon(const AttrAirCon &other) : AttrObject(other)
{
	//push_back复制
	m_airConId = other.m_airConId;						//空调序号
	m_prototypeCode = other.m_prototypeCode;	//空调原型编号
//	m_name = other.m_name;//空调原型文件
	m_airConHorseNumber = other.m_airConHorseNumber;	//空调匹数
	m_airConPipePos = other.m_airConPipePos;			//空调冷凝水管位置
	m_airConRainRiser = other.m_airConRainRiser;		//空调是否有雨水立管穿过
	m_airConRainRiserPos = other.m_airConRainRiserPos;	//空调雨水管位置
	m_airConInstallNetSize = other.m_airConInstallNetSize;//空调安装净尺寸


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
	if (p_att.m_airConHorseNumber != m_airConHorseNumber)
	{
		return false;
	}
	else if (p_att.m_airConId != m_airConId)
	{
		return false;
	}
	else if (p_att.m_airConPipePos != m_airConPipePos)
	{
		return false;
	}
	else if (p_att.m_airConRainRiser != m_airConRainRiser)
	{
		return false;
	}
	else if (p_att.m_airConRainRiserPos != m_airConRainRiserPos)
	{
		return false;
	}
	else if (p_att.m_airConInstallNetSize != m_airConInstallNetSize)
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
	else if (p_att.m_fileName != m_fileName)
	{
		return false;
	}
	else if (p_att.m_instanceCode != m_instanceCode)
	{
		return false;
	}
	else return true;
}