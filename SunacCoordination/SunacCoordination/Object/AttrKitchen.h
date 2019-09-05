/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN)
#define ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
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

	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
private:

public:
	bool m_isGuoBiao;//排气道
	int m_floorOption;//楼层选项
	double m_paiQiDaoLen;//排气道长度
	double m_paiQiDaoWid;//排气道宽度
	bool m_isMirror;//镜像
	bool m_hasPaiQiDao;//是否含有排气道
	CString m_windowDoorPos;//门窗位置关系

	//这些属性体现在图块中--可以从图块直接取出的 就不用从属性记录
	/*CString m_shuiPenType;//水盆类型
	CString m_bingXiangType;//冰箱类型
	CString m_zaoTaiWidth;//灶台宽度 
	double len;//长度
	double kuandu;//宽度*/
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_KITCHEN)