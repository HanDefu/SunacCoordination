
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrRailing.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrRailing, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALERAILING, ZffCustomObjectDBRAILING);
AttrRailing::AttrRailing()
{
	m_height = 1100;		//栏杆高度
	m_fanKanHeight = 200;	//反坎高度
	m_length = 3000;	//栏杆长
	m_railingType = E_RAILING_TIEYI; //栏杆类型
}

AttrRailing::~AttrRailing()
{

}

//AttrRailing::AttrRailing(const AttrRailing &other) : AttrObject(other)
//{
//
//}
//
//AttrRailing & AttrRailing::operator=(const AttrRailing &rhs)
//{
//	AttrObject::operator=(rhs);
//
//	return *this;
//}

Acad::ErrorStatus AttrRailing::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	return filer->filerStatus();
}

Acad::ErrorStatus AttrRailing::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}
	
	return filer->filerStatus();
}

bool AttrRailing::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrRailing * pRealObj = dynamic_cast<AttrRailing *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return true;
	/*return (m_openType == pRealObj->m_openType
		);*/
}
