
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

	AttrWindow(const AttrWindow &other);
	AttrWindow & operator=(const AttrWindow &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return WINDOW;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致

public:
	CString m_openType;//开启类型
	Adesk::Int32 m_openQty;//开启扇数量

	bool m_isZhuanJiao;//是否转角窗
	bool m_isMirrorWindow; //是否对称窗型		//TODO 新增

	//动态窗型属性
	double m_minWid;//宽度尺寸最小值
	double m_maxWid;//宽度尺寸最大值
	CString m_tongFengFormula;//通风量计算公式

	//静态窗型属性
	double m_tongFengQty; //通风量
	double m_width;					//TODO 新增
	double m_height;				//TODO 新增


	//算量相关
	vSCalcData m_calFormulas;
	
	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置
	//视图属性
	bool m_isMirror;//是否镜像
	eViewDir m_viewDir;//视图方向

	bool m_isBayWindow; //是否凸窗
	double m_openWindowSize; //开启扇宽度
	double m_windowH2;  //下固定值（若存在下固定时）
	double m_wallDis;//外墙距离

	////CString	prototypeId;		//原型编号
	////CString	prototypeFile;		//原型文件
	////CString	openType;			//开启类型
	////CString	openNum;			//开启数量
	////CString	ventilationFormula;	//通风量公式	
	////CString	staticWidth;		//静态宽度	
	////CString	staticHeigth;		//静态高度	
	////CString	staticVentilation;	//静态通风量

	double m_plugslotSize;		//塞缝尺寸
	CString m_sizeId;			//尺寸代号
	CString m_id;					//序号
	CString	m_scopeOfApplication;	//适用范围
	CString	m_Dynamic;				//动态
	CString	m_functionType;		//功能区类型
	CString m_jiTuan;			//集团名称
	CString m_DynamicType;		//动态类型

	CString	m_valueType;		//值类型	
	CString	m_value;			//值	
	CString	m_min;				//最小值
	CString	m_max;				//最大值
	CString	m_defaultValue;		//默认值
	CString	m_state;			//说明

	CString	m_prototypeFlatFile;	//原型平面文件
	CString	m_prototypeTopViewFile;//原型俯视图文件
	CString m_prototypeFacadeFile; //原型立面文件
	CString m_prototypeSpreadFile; //原型展开图文件

	vSRCDimData m_dimData;
};

typedef std::vector<AttrWindow> vAttrWindow;
