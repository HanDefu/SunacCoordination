/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrDoor.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrDoor, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEDOOR, ZffCustomObjectDBDOOR);
AttrDoor::AttrDoor()
{
}

AttrDoor::~AttrDoor()
{
	// TODO: clean up.

}

AttrDoor::AttrDoor(const AttrDoor &other) : AttrObject(other)
{

}

AttrDoor & AttrDoor::operator=(const AttrDoor &rhs)
{
	AttrObject::operator=(rhs);
	return *this;
}

Acad::ErrorStatus AttrDoor::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	Adesk::Int32 size1 = 0, m_coolerType = 0, m_lengthType = 0, m_handleType = 0, lengbianType = 0;

	filer->readItem(&m_version);
	return filer->filerStatus();
}

Acad::ErrorStatus AttrDoor::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	Adesk::Int32 size1 = 0;

	filer->writeItem(m_version);

	
	return filer->filerStatus();
}

bool AttrDoor::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrDoor * pRealObj = dynamic_cast<AttrDoor *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}

