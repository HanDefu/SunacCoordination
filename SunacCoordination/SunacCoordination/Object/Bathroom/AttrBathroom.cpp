/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrBathroom.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrBathroom, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEBathroom, ZffCustomObjectDBBathroom);

AttrBathroom::AttrBathroom()
{

}

AttrBathroom::AttrBathroom(double p_xLen, double p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, const CPrototypeInfo& p_protptype)
{
	m_type = L"ÎÀÉú¼ä";
	m_sBathroomType = p_protptype.m_sType;
	m_width = p_xLen;
	m_height = p_yLen;
	if (p_doorPos == E_DIR_LEFT || p_doorPos == E_DIR_RIGHT)
		swap(m_width, m_height);

	m_prototypeCode.Format(L"%s-%.0lf¡Á%.0lf", m_sBathroomType.Left(3), min(p_xLen, p_yLen), max(p_xLen, p_yLen));
	if (m_sBathroomType.Find(L"_g") != -1)
		m_prototypeCode += L"/g";

	m_fileName = p_protptype.m_sFileName;
	m_isDynamic = p_protptype.m_bIsDynamic;
	m_isJiTuan = true;
}

AttrBathroom::~AttrBathroom()
{

}

Acad::ErrorStatus AttrBathroom::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

Acad::ErrorStatus AttrBathroom::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

bool AttrBathroom::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrBathroom * pRealObj = dynamic_cast<AttrBathroom *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}

bool AttrBathroom::IsPrototypeEqual(const AttrBathroom& p_att)
{
	if (p_att.m_hasPaiQiDao != m_hasPaiQiDao)
	{
		return false;
	}
	else if (p_att.m_isGuoBiao != m_isGuoBiao)
	{
		return false;
	}
	else if (p_att.m_airVentW != m_airVentW)
	{
		return false;
	}
	else if (p_att.m_airVentH != m_airVentH)
	{
		return false;
	}
	else if (p_att.m_floorRange != m_floorRange)
	{
		return false;
	}
	else if (p_att.m_airVentOffsetX != m_airVentOffsetX)
	{
		return false;
	}
	else if (p_att.m_airVentOffsetY != m_airVentOffsetY)
	{
		return false;
	}
	else if (p_att.m_isMirror != m_isMirror)
	{
		return false;
	}
	else if (p_att.m_sBathroomType != m_sBathroomType)
	{
		return false;
	}
	else if (p_att.m_windowDoorPos != m_windowDoorPos)
	{
		return false;
	}
	else if (p_att.m_taipenWidth != m_taipenWidth)
	{
		return false;
	}
	else if (p_att.m_matongWidth != m_matongWidth)
	{
		return false;
	}
	else if (p_att.m_guanXiWidth != m_guanXiWidth)
	{
		return false;
	}
	else if (p_att.m_width != m_width)
	{
		return false;
	}
	else if (p_att.m_height != m_height)
	{
		return false;
	}

	else if (p_att.m_prototypeCode != m_prototypeCode)
	{
		return false;
	}
	else if (p_att.m_type != m_type)
	{
		return false;
	}
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
