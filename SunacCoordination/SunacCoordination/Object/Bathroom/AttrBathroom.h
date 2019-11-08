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
#include "../KitchenBathroom/KitchenBathroomProp.h"

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
	virtual ~AttrBathroom();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return Bathroom;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
	bool IsInstanceEqual(const AttrBathroom& p_att) const;

	CString GetPrototypeCode() const;

	void GetAirVentSize(double& ventX, double& ventY);

public:
	//原型相关属性，初始化时填写
	CString m_sBathroomType;	//卫生间类型
	eWindowDoorPos m_windowDoorPos;	//门窗位置关系
	bool m_hasPaiQiDao;			//是否含有排气道

	//原型数据，记录原型的支持尺寸和门窗初始位置，初始化时填写
	CKitchenBathroomProp m_prop;

	//搜索到原型后填写
	double m_width;				//开间
	double m_height;			//进深

	//插入时填写
	CString m_taipenWidth;		//台盆宽度
	CString m_matongWidth;		//马桶宽度
	double m_guanXiWidth;		//盥洗区宽度

	//排气道相关设置，插入时填写
	bool m_isGuoBiao;			//是否国标
	E_BATHROOM_FLOOR_RANGE m_floorRange;	//楼层选项
	double m_airVentW;			//排气道长度
	double m_airVentH;			//排气道宽度
	double m_airVentOffsetX;	//排气道偏移x
	double m_airVentOffsetY;	//排气道偏移y

	//插入时填写
	int m_angle;				//逆时针旋转角度
	bool m_isMirror;			//镜像
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
