/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrObject.h"
#include "../Common/ComFun_Sunac.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrObject, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEOBJECT, ZffCustomObjectDBOBJECT);

AttrObject::AttrObject()
{
	m_version = 0;
	m_id = L"testid";
    m_name = L"";
	m_isJiTuan = false;
	m_quyuName = L"";
	m_type = L"testtype"; 
	m_isDynamic = true;
}

AttrObject::~AttrObject()
{
	// TODO: clean up.

}

AttrObject::AttrObject(const AttrObject &other)
{
	m_version = other.m_version;
	m_id = other.m_id;
	m_name = other.m_name;
	m_isJiTuan = other.m_isJiTuan;
	m_quyuName = other.m_quyuName;
	m_type = other.m_type; 
	m_isDynamic = other.m_isDynamic;
}

AttrObject & AttrObject::operator=(const AttrObject &rhs)
{
	m_version = rhs.m_version;
	m_id = rhs.m_id;
	m_name = rhs.m_name;
	m_isJiTuan = rhs.m_isJiTuan;
	m_quyuName = rhs.m_quyuName;
	m_type = rhs.m_type; 
	m_isDynamic = rhs.m_isDynamic;
	return *this;
}

Acad::ErrorStatus AttrObject::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AcDbObject::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	}

	filer->readItem(&m_version);

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];
    filer->readItem(&tempStr);
	m_id = CString(tempStr);

	filer->readItem(&tempStr);
	m_name = CString(tempStr);

	filer->readItem(&m_isJiTuan);

	filer->readItem(&tempStr);
	m_quyuName = CString(tempStr);

	filer->readItem(&tempStr);
	m_type = CString(tempStr);

	filer->readItem(&m_isDynamic);

	delete [] tempStr;
	return filer->filerStatus();
}

Acad::ErrorStatus AttrObject::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	Adesk::Int32 version = FILE_VERSION;
	filer->writeItem(version);
	filer->writeItem(m_id);
	filer->writeItem(m_name);
	filer->writeItem(m_isJiTuan);
	filer->writeItem(m_quyuName);
	filer->writeItem(m_type);
	filer->writeItem(m_isDynamic);

	return filer->filerStatus();
}

bool AttrObject::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	//不用比较version
	return (m_id == other->m_id && 
		m_name == other->m_name && 
		m_isJiTuan == other->m_isJiTuan && 
		m_quyuName == other->m_quyuName && 
		m_type == other->m_type && 
		m_isDynamic == other->m_isDynamic
		);
}

