/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_TOILET)
#define ARX__ZFFDWGSCALE_H__20140205_113541_TOILET

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
#include "../Sunac_def.h"
#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET "ZFFCUSTOMOBJECTDB_DBXSERVICE_TOILET"
#endif

class DLLIMPEXP  AttrToilet : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrToilet);

	// Constructor / Destructor
	AttrToilet();
	virtual ~AttrToilet();

	AttrToilet(const AttrToilet &other);
	AttrToilet & operator=(const AttrToilet &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致

public:

	bool m_isGuoBiao;//排气道
	int m_floorOption;//楼层选项
	double m_paiQiDaoLen;//排气道长度
	double m_paiQiDaoWid;//排气道宽度
	bool m_isMirror;//镜像

	bool m_hasPaiQiDao;//是否含有排气道
	eWindowDoorPos m_windowDoorPos;//门窗位置关系

	double m_guanXiWidth;//盥洗区宽度
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_TOILET)