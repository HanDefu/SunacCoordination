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
	m_floorRange = E_FLOOR_1_7;//楼层选项
	 m_airVentW = 0;//排气道长度
	 m_airVentH = 0;//排气道宽度
	 m_airVentOffsetX = 0;//排气道长度
	 m_airVentOffsetY = 0;//排气道宽度
	 m_isMirror = false;//镜像
	 m_hasPaiQiDao = true;//是否含有排气道
	 m_isGuoBiao = true;//排气道
	//m_kitchenType;//厨房类型
	m_windowDoorPos = DUIKAI;//门窗位置关系

	//这些属性体现在图块中--可以从图块直接取出的 就不用从属性记录
	//m_shuiPenType;//水盆类型
	//m_bingXiangType;//冰箱类型
	//m_zaoTaiType;//灶台宽度 
	m_width = 0;//长度 面宽
	m_height = 0;//宽度 进深
	m_width = 0;//长度 面宽
	m_height = 0;//宽度 进深
	m_minWidth = 0;//最小开间
	m_maxwidth = 0;//最大开间
	m_maxHeight = 0;//最大进深
	m_minHeight = 0;//最小进深
	m_BasinSize = 0;//厨房水盆尺寸
	m_FridgeSize = 0;//厨房冰箱尺寸
	m_HearthSize = 0;//厨房灶台尺寸
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
/*

	filer->readItem(&m_hasPaiQiDao);
	filer->readItem(&m_isGuoBiao);
	filer->readItem((int*)&m_floorRange);

	filer->readItem(&m_airVentW);
	filer->readItem(&m_airVentH);
	filer->readItem(&m_airVentOffsetX);
	filer->readItem(&m_airVentOffsetY);

	filer->readItem(&m_isMirror);

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];

	filer->readItem(&tempStr);
	m_kitchenType = CString(tempStr);
	filer->readItem(&tempStr);
	m_windowDoorPos = CString(tempStr);
	filer->readItem(&tempStr);
	m_shuiPenType = CString(tempStr);
	filer->readItem(&tempStr);
	m_bingXiangType = CString(tempStr);
	filer->readItem(&tempStr);
	m_zaoTaiType = CString(tempStr);

	filer->readItem(&m_width);
	filer->readItem(&m_height);

	delete [] tempStr;
*/

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
/*

	filer->writeItem(m_hasPaiQiDao);
	filer->writeItem(m_isGuoBiao);
	filer->writeItem(m_floorRange);

	filer->writeItem(m_airVentW);
	filer->writeItem(m_airVentH);
	filer->writeItem(m_airVentOffsetX);
	filer->writeItem(m_airVentOffsetY);

	filer->writeItem(m_isMirror);

	filer->writeItem(m_kitchenType);
	filer->writeItem(m_windowDoorPos);
	filer->writeItem(m_shuiPenType);
	filer->writeItem(m_bingXiangType);
	filer->writeItem(m_zaoTaiType);

	filer->writeItem(m_width);
	filer->writeItem(m_height);
*/

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
