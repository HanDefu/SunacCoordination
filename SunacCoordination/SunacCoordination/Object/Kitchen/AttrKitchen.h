
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "..\PrototypeCommonDef.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN "ZFFCUSTOMOBJECTDB_DBXSERVICE_KITCHEN"
#endif



class DLLIMPEXP  AttrKitchen : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrKitchen);

	// Constructor / Destructor
	AttrKitchen();
	virtual ~AttrKitchen();

	AttrKitchen(const AttrKitchen &other);
	AttrKitchen & operator=(const AttrKitchen &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return KITCHEN;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致

public:
	bool m_hasPaiQiDao;//是否含有排气道
	bool m_isGuoBiao;//排气道
	E_FLOOR_RANGE m_floorRange;//楼层选项
	double m_airVentW;//排气道长度
	double m_airVentH;//排气道宽度
	double m_airVentOffsetX;//排气道偏移x
	double m_airVentOffsetY;//排气道偏移y

	bool m_isMirror;//镜像

	CString m_kitchenType;//厨房类型
	CString m_windowDoorPos;//门窗位置关系

	CString m_shuiPenType;//水盆类型
	CString m_bingXiangType;//冰箱类型
	CString m_zaoTaiType;//灶台宽度 
	double m_width;//长度 面宽
	double m_height;//宽度 进深
};
