/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrKitchen.h"
#include "../../Common/ComFun_Sunac.h"

//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrKitchen, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEKITCHEN, ZffCustomObjectDBKITCHEN);

AttrKitchen::AttrKitchen()
{
	m_width = 0;//长度 面宽
	m_height = 0;//宽度 进深
	m_windowDoorPos = DUIKAI;//门窗位置关系

	m_angle = 0;//逆时针旋转角度
	m_isMirror = false;//镜像

	//排气道相关设置
	m_hasPaiQiDao = true;//是否含有排气道
	m_isGuoBiao = true;//是否国标
	m_floorRange = E_KITCHEN_FLOOR_1_7;//楼层选项
	m_airVentOffsetX = 0;//排气道长度
	m_airVentOffsetY = 0;//排气道宽度
	m_airVentW = 0;//排气道长度
	m_airVentH = 0;//排气道宽度

	//Web端使用
	m_minWidth = 0;//最小开间
	m_maxWidth = 0;//最大开间
	m_maxHeight = 0;//最大进深
	m_minHeight = 0;//最小进深
}

AttrKitchen::~AttrKitchen()
{

}

Acad::ErrorStatus AttrKitchen::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AttrObject::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	}

	AcString tempStr;
	filer->readString(tempStr);
	m_kitchenType = tempStr.kACharPtr();
	filer->readItem((Adesk::UInt32*)&m_windowDoorPos);
	filer->readItem(&m_hasPaiQiDao);

	filer->readItem((Adesk::UInt32*)&m_prop.m_doorPos);
	filer->readItem((Adesk::UInt32*)&m_prop.m_windowPos);
	Adesk::UInt32 size;
	filer->readItem(&size);
	m_prop.m_sizeList.resize(size);
	for (UINT i = 0; i < m_prop.m_sizeList.size(); i++)
	{
		filer->readItem(&m_prop.m_sizeList[i].xLen);
		filer->readItem(&m_prop.m_sizeList[i].yLen);
	}

	filer->readItem(&m_width);
	filer->readItem(&m_height);

	filer->readString(tempStr);
	m_shuiPenType = tempStr.kACharPtr();
	filer->readString(tempStr);
	m_bingXiangType = tempStr.kACharPtr();
	filer->readString(tempStr);
	m_zaoTaiType = tempStr.kACharPtr();

	filer->readItem(&m_isGuoBiao);
	filer->readItem((Adesk::UInt32*)&m_floorRange);
	filer->readItem(&m_airVentW);
	filer->readItem(&m_airVentH);
	filer->readItem(&m_airVentOffsetX);
	filer->readItem(&m_airVentOffsetY);

	filer->readItem(&m_angle);
	filer->readItem(&m_isMirror);

	filer->readItem(&m_minWidth);
	filer->readItem(&m_maxWidth);
	filer->readItem(&m_minHeight);
	filer->readItem(&m_maxHeight);

	return filer->filerStatus();
}

Acad::ErrorStatus AttrKitchen::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AttrObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	filer->writeItem(m_kitchenType);
	filer->writeItem((Adesk::UInt32)m_windowDoorPos);
	filer->writeItem(m_hasPaiQiDao);

	filer->writeItem((Adesk::UInt32)m_prop.m_doorPos);
	filer->writeItem((Adesk::UInt32)m_prop.m_windowPos);
	filer->writeItem((Adesk::UInt32)m_prop.m_sizeList.size());
	for (UINT i = 0; i < m_prop.m_sizeList.size(); i++)
	{
		filer->writeItem(m_prop.m_sizeList[i].xLen);
		filer->writeItem(m_prop.m_sizeList[i].yLen);
	}

	filer->writeItem(m_width);
	filer->writeItem(m_height);

	filer->writeItem(m_shuiPenType);
	filer->writeItem(m_bingXiangType);
	filer->writeItem(m_zaoTaiType);

	filer->writeItem(m_isGuoBiao);
	filer->writeItem((Adesk::UInt32)m_floorRange);
	filer->writeItem(m_airVentW);
	filer->writeItem(m_airVentH);
	filer->writeItem(m_airVentOffsetX);
	filer->writeItem(m_airVentOffsetY);

	filer->writeItem(m_angle);
	filer->writeItem(m_isMirror);

	filer->writeItem(m_minWidth);
	filer->writeItem(m_maxWidth);
	filer->writeItem(m_minHeight);
	filer->writeItem(m_maxHeight);

	return filer->filerStatus();
}

bool AttrKitchen::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	AttrKitchen * pRealObj = dynamic_cast<AttrKitchen *>(other);
	if (pRealObj == 0)
		return false;

	if (!AttrObject::isEqualTo(other))
		return false;

	return (m_isGuoBiao == pRealObj->m_isGuoBiao &&
		m_floorRange == pRealObj->m_floorRange &&
		m_airVentW == pRealObj->m_airVentW &&
		m_airVentH == pRealObj->m_airVentH &&
		m_airVentOffsetX == pRealObj->m_airVentOffsetX &&
		m_airVentOffsetY == pRealObj->m_airVentOffsetY &&
		m_isMirror == pRealObj->m_isMirror 
		);
}

bool AttrKitchen::IsInstanceEqual(const AttrKitchen& p_att) const
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
	if (m_shuiPenType != p_att.m_shuiPenType)
		return false;
	if (m_bingXiangType != p_att.m_bingXiangType)
		return false;
	if (m_zaoTaiType != p_att.m_zaoTaiType)
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

	if (m_isMirror != p_att.m_isMirror)
		return false;

	return true;
}
