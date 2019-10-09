/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrBathroom.h"
#include "..\..\WebIO\WebIO.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrBathroom, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEBathroom, ZffCustomObjectDBBathroom);

AttrBathroom::AttrBathroom()
{

}

AttrBathroom::AttrBathroom(double p_xLen, double p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, const CProBathroom& p_protptype)
{
	m_type = L"卫生间";
	m_sBathroomType = p_protptype.m_sType;
	m_width = p_xLen;
	m_height = p_yLen;
	if (p_doorPos == E_DIR_LEFT || p_doorPos == E_DIR_RIGHT)
		swap(m_width, m_height);

	m_prototypeCode.Format(L"%s-%.0lf×%.0lf", m_sBathroomType.Left(3), min(p_xLen, p_yLen), max(p_xLen, p_yLen));
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

CProBathroom* AttrBathroom::GetProBathroom()
{
	CProBathroom *pProBathroom = dynamic_cast<CProBathroom *>(GetPrototype());
	if (NULL == pProBathroom)
	{
		//原型与原型实例的类型不匹配
		assert(false);
	}
	return pProBathroom;
}
