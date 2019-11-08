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
	m_width = 0;			//开间
	m_height = 0;			//进深
	m_windowDoorPos = DUIKAI;

	m_angle = 0;
	m_isMirror = false;

	m_guanXiWidth = 0;		//盥洗区宽度

	//排气道相关设置
	m_hasPaiQiDao = true;	//是否含排气道
	m_isGuoBiao = true;		//是否国标
	m_floorRange = E_BATHROOM_FLOOR_1_14;	//楼层选项
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

	AcString tempStr;
	filer->readString(tempStr);
	m_sBathroomType = tempStr.kACharPtr();
	filer->readItem((Adesk::UInt32*)&m_windowDoorPos);
	filer->readItem(&m_hasPaiQiDao);

	m_prop.ReadFromDwg(filer);

	filer->readItem(&m_width);
	filer->readItem(&m_height);

	filer->readString(tempStr);
	m_taipenWidth = tempStr.kACharPtr();
	filer->readString(tempStr);
	m_matongWidth = tempStr.kACharPtr();
	filer->readItem(&m_guanXiWidth);

	filer->readItem(&m_isGuoBiao);
	filer->readItem((Adesk::UInt32*)&m_floorRange);
	filer->readItem(&m_airVentW);
	filer->readItem(&m_airVentH);
	filer->readItem(&m_airVentOffsetX);
	filer->readItem(&m_airVentOffsetY);

	filer->readItem(&m_angle);
	filer->readItem(&m_isMirror);

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

	filer->writeItem(m_sBathroomType);
	filer->writeItem((Adesk::UInt32)m_windowDoorPos);
	filer->writeItem(m_hasPaiQiDao);

	m_prop.WriteToDwg(filer);

	filer->writeItem(m_width);
	filer->writeItem(m_height);

	filer->writeItem(m_taipenWidth);
	filer->writeItem(m_matongWidth);
	filer->writeItem(m_guanXiWidth);

	filer->writeItem(m_isGuoBiao);
	filer->writeItem((Adesk::UInt32)m_floorRange);
	filer->writeItem(m_airVentW);
	filer->writeItem(m_airVentH);
	filer->writeItem(m_airVentOffsetX);
	filer->writeItem(m_airVentOffsetY);

	filer->writeItem(m_angle);
	filer->writeItem(m_isMirror);

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

	//比较卫生间配置
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

	if (m_isMirror != p_att.m_isMirror)
		return false;

	return true;
}

CString AttrBathroom::GetPrototypeCode() const
{
	CString prototypeCode = m_prototypeCode;
	int pos = prototypeCode.Find(L'_');
	if (pos != -1)
		prototypeCode = prototypeCode.Left(pos);

	CString ret;
	if ((m_prop.m_doorPos == E_DIR_TOP) || (m_prop.m_doorPos == E_DIR_BOTTOM))
		ret.Format(L"%s-%0.lf×%0.lf", prototypeCode, m_width, m_height);
	else
		ret.Format(L"%s-%0.lf×%0.lf", prototypeCode, m_height, m_width);
	if (m_prototypeCode.Find(L"_g") != -1)
		ret += L"/g";

	return ret;
}

void AttrBathroom::GetAirVentSize(double& ventX, double& ventY)
{
	if (m_hasPaiQiDao)
	{
		if (m_isGuoBiao) //国标
		{
			ventX = m_airVentOffsetX + c_bathroomAirVentSize[m_floorRange];
			ventY = m_airVentOffsetY + c_bathroomAirVentSize[m_floorRange];
		}
		else
		{
			ventX = m_airVentW;
			ventY = m_airVentH;
		}
	}
	else
	{
		ventX = 0;
		ventY = 0;
	}
}
