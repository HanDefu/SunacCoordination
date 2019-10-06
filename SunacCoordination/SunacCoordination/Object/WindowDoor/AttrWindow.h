
#pragma once

#include "dbmain.h"
#include "..\AttrObject.h"
#include "../../Sunac_def.h"
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

	AttrWindow();
	virtual ~AttrWindow();

	//AttrWindow(const AttrWindow &other);
	//AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致

public:
	CString m_openType;		//开启类型
	Adesk::Int32 m_openQty;	//开启扇数量
	CString m_gongNengquType;//功能区类型

	bool m_isZhuanJiao;		//是否转角窗
	bool m_isMirrorWindow;	//是否对称窗型 

	CString m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	CString	m_topViewFile;		//原型俯视图文件
	CString m_leftViewFile;		//原型侧视图文件
	

	//动态窗型属性
	double m_minWid;//宽度尺寸最小值
	double m_maxWid;//宽度尺寸最大值
	CString m_tongFengFormula;//通风量计算公式

	//静态窗型属性
	double m_staticTongFengQty;	//通风量
	double m_staticWidth;		
	double m_staticHeight;		

	vSRCDimData m_dimData;

	//////////////////////////////////////////////////////////////////////////
	//算量相关
	vSCalcData m_calFormulas;
	




	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
	//视图属性
	bool m_isMirror;//是否镜像
	eViewDir m_viewDir;//视图方向

	bool   m_isBayWindow;	 //是否凸窗
	double m_openWindowSize; //开启扇宽度
	double m_windowH2;		 //下固定值（若存在下固定时）
	double m_wallDis;		 //外墙距离

	//////////////////////////////////////////////////////////////////////////
	//二次深化属性
	double m_plugslotSize;		//塞缝尺寸


};

typedef std::vector<AttrWindow> vAttrWindow;
