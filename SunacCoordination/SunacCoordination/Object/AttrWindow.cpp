/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrWindow.h"
#include "../Common/ComFun_Sunac.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrWindow, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEWINDOW, ZffCustomObjectDBWINDOW);
AttrWindow::AttrWindow()
{
	m_openType = L"";
	m_openQty = 0;
	m_isZhuanJiao = false;//是否转角窗
	m_minWid = 0;//宽度尺寸最小值
	m_maxWid = 0;//宽度尺寸最大值
	m_tongFengFormula = L"";//通风量计算公式
	m_tongFengQty = 0.0;
	m_wallDis = 0.0;
	m_isMirror = false;
	m_viewDir = LIMINAN;
}

AttrWindow::~AttrWindow()
{

}

AttrWindow::AttrWindow(const AttrWindow &other) : AttrObject(other)
{
	m_openType = other.m_openType;
	m_openQty = other.m_openQty;
	m_isZhuanJiao = other.m_isZhuanJiao;
	m_minWid = other.m_minWid;
	m_maxWid = other.m_maxWid; 
	m_tongFengFormula = other.m_tongFengFormula;
	m_tongFengQty = other.m_tongFengQty;

	m_isMirror = other.m_isMirror;
	m_viewDir = other.m_viewDir;
	m_wallDis = other.m_wallDis;
}

AttrWindow & AttrWindow::operator=(const AttrWindow &rhs)
{
	AttrObject::operator=(rhs);
	m_openType = rhs.m_openType;
	m_openQty = rhs.m_openQty;
	m_isZhuanJiao = rhs.m_isZhuanJiao;
	m_minWid = rhs.m_minWid;
	m_maxWid = rhs.m_maxWid; 
	m_tongFengFormula = rhs.m_tongFengFormula;
	m_tongFengQty = rhs.m_tongFengQty;

	m_isMirror = rhs.m_isMirror;
	m_viewDir = rhs.m_viewDir;
	m_wallDis = rhs.m_wallDis;

	return *this;
}

Acad::ErrorStatus AttrWindow::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) {
		return es;
	}

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];
	filer->readItem(&tempStr);
	m_openType = CString(tempStr);

	filer->readItem(&m_openQty);
	filer->readItem(&m_isZhuanJiao);
	filer->readItem(&m_minWid);
	filer->readItem(&m_maxWid);

	filer->readItem(&tempStr);
	m_tongFengFormula = CString(tempStr);

	filer->readItem(&m_tongFengQty);

	delete [] tempStr;

	return filer->filerStatus();
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
	filer->writeItem(m_minWid);
	filer->writeItem(m_maxWid);
	filer->writeItem(m_tongFengFormula);
	filer->writeItem(m_tongFengQty);

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

	return (m_openType == pRealObj->m_openType &&
		m_openQty == pRealObj->m_openQty &&
		m_isZhuanJiao == pRealObj->m_isZhuanJiao &&
		m_minWid == pRealObj->m_minWid &&
		m_maxWid == pRealObj->m_maxWid &&
		m_tongFengFormula == pRealObj->m_tongFengFormula &&
		m_tongFengQty == pRealObj->m_tongFengQty &&
		m_isMirror == pRealObj->m_isMirror &&
		m_viewDir == pRealObj->m_viewDir &&
		m_wallDis == pRealObj->m_wallDis
		);
}

