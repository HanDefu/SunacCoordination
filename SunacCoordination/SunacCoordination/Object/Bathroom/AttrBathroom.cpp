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
	//搜索到原型后填写
	m_width = 0;				//开间
	m_height = 0;			//进深

	//插入时填写
	m_taipenWidth = "";		//台盆宽度
	m_matongWidth = "";		//马桶宽度
	m_guanXiWidth = 0;		//盥洗区宽度

	//排气道相关设置，插入时填写
	m_isGuoBiao = 0;			//是否国标
	m_airVentW = 0;			//排气道长度
    m_airVentH = 0;			//排气道宽度
	m_airVentOffsetX = 0;	//排气道偏移x
	m_airVentOffsetY = 0;	//排气道偏移y

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

bool AttrBathroom::IsInstanceEqual(const AttrBathroom& p_att) const
{
	//比较原型编号
	if (m_prototypeCode != p_att.m_prototypeCode)
		return false;

	//比较尺寸和门窗位置
	if (m_width != p_att.m_width)
		return false;
	if (m_height != p_att.m_height)
		return false;
	if (m_windowDoorPos != p_att.m_windowDoorPos)
		return false;
	if (m_hasPaiQiDao != p_att.m_hasPaiQiDao)
		return false;

	//比较厨房配置
	if (m_taipenWidth != p_att.m_taipenWidth)
		return false;
	if (m_matongWidth != p_att.m_matongWidth)
		return false;
	if (m_guanXiWidth != p_att.m_guanXiWidth)
		return false;

	//比较排气道
	if (m_isGuoBiao != p_att.m_isGuoBiao)
		return false;
	if (m_isGuoBiao)
	{
		if (m_floorRange != p_att.m_floorRange)
			return false;
		if (m_airVentOffsetX != p_att.m_airVentOffsetX)
			return false;
		if (m_airVentOffsetY != p_att.m_airVentOffsetY)
			return false;
	}
	else
	{
		if (m_airVentW != p_att.m_airVentW)
			return false;
		if (m_airVentH != p_att.m_airVentH)
			return false;
	}

	return true;
}
