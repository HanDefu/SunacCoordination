/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
#define ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../AttrObject.h"
#include "../../Sunac_def.h"
#include "../PrototypeCommonDef.h"
#include "../PrototypeInfo.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom "ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom"
#endif

class DLLIMPEXP  AttrBathroom : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrBathroom);

	// Constructor / Destructor
	AttrBathroom();
	AttrBathroom(double p_xLen, double p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, const CProBathroom& p_prototype);
	virtual ~AttrBathroom();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return Bathroom;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致

	CProBathroom* GetProBathroom();

public:
	bool m_hasPaiQiDao; //是否含有排气道
	bool m_isGuoBiao; //排气道
	E_FLOOR_RANGE m_floorRange; //楼层选项
	double m_airVentW; //排气道长度
	double m_airVentH; //排气道宽度
	double m_airVentOffsetX; //排气道偏移x
	double m_airVentOffsetY; //排气道偏移y

	bool m_isMirror; //镜像

	CString m_sBathroomType; //卫生间类型
	eWindowDoorPos m_windowDoorPos; //门窗位置关系

	CString m_taipenWidth;//台盆宽度
	CString m_matongWidth;//马桶宽度
	double m_guanXiWidth; //盥洗区宽度
	double m_width;//开间
	double m_height;//进深
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
