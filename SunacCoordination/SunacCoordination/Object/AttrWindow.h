/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW)
#define ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW

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

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW "ZFFCUSTOMOBJECTDB_DBXSERVICE_WINDOW"
#endif

class DLLIMPEXP  AttrWindow : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrWindow);

	// Constructor / Destructor
	AttrWindow();
	virtual ~AttrWindow();
	AttrWindow(const AttrWindow &other);
	AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
private:

public:
	CString m_openType;//开启类型
	Adesk::Int32 m_openQty;//开启扇数量
	bool m_isZhuanJiao;//是否转角窗
	double m_minWid;//宽度尺寸最小值
	double m_maxWid;//宽度尺寸最大值
	CString m_tongFengFormula;//通风量计算公式
	double m_tongFengQty;
	vSCalcData m_calFormulas;

	bool m_isMirror;//是否镜像
	eViewDir m_viewDir;//视图方向
	double m_wallDis;//外墙距离
};

typedef std::vector<AttrWindow> vAttrWindow;


#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_WINDOW)