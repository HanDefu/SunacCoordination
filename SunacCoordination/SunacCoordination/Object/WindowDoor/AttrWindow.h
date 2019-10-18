
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



//门窗取值参数类型
typedef enum eWindowDimType
{
	SINGLE,//固定值
	MULTI,//值系列
	SCOPE,//范围
	UNLIMIT,//不限
	CALC,//公式
	NOVALUE //无
}eWindowDimType;

static eWindowDimType ToEWindowType(CString type)
{
	if (type == "固定值"|| type == "值")
	{
		return SINGLE;
	} 
	else if (type == "值系列"|| type == "系列")
	{
		return MULTI;
	}
	else if (type == "范围")
	{
		return SCOPE;
	}
	else if (type == "不限")
	{
		return UNLIMIT;
	}
	else if (type == "公式")
	{
		return CALC;
	}
	else if(type == "无")
	{
		return NOVALUE;
	}
	else
	{
		ASSERT(FALSE);
		return NOVALUE;
	}

}

class CWindowsDimData
{
public:
	CString sCodeName;		//代号 W W1 W2 W3 H H1 H2 H3
	eWindowDimType type;	//值类型
	vdouble valueOptions;	//值选项存储单个值、系列
	double minValue;
	double maxValue;
	CString sFomula;	//公式
	double defaultValue;
	CString prompt;  //说明

	double value; //具体实例中的取值

public:
	CWindowsDimData();
	bool operator==(const CWindowsDimData &rhs) const;
	bool IsValueEqual(const CWindowsDimData &rhs)const;
	bool IsParaEqual(const CWindowsDimData &rhs)const;

	//排序用
	bool operator<(const CWindowsDimData &rhs) const;
};

//////////////////////////////////////////////////////////////////////////

struct CWindowMaterial	//门窗材料系列
{
	CString sAluminumSerial; //型材系列
	CString sGlassSerial;
	bool bHasAuxiliaryFrame; //是否有附框
	CString sAuxiliaryFrame; //附框系列

	CWindowMaterial()
	{
		bHasAuxiliaryFrame = false;
	}
};

//////////////////////////////////////////////////////////////////////////


class DLLIMPEXP  AttrWindow : public AttrObject  //门窗共用AttrWindow
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
	virtual bool isEqualTo(AttrObject*other);//窗型是否一致
	virtual bool IsPrototypeEqual(const AttrWindow& p_att);
	virtual bool IsInstanceEqual(const AttrWindow& p_att) const;
	
	//////////////////////////////////////////////////////////////////////////
	const CWindowsDimData* GetDimData(CString p_sCode)const;
	void SetDimData(const CWindowsDimData& p_dataOut);
	void CheckAndComplementDimeData(); //检查并补全Dim数据，W/H/a确保都有

	double GetTongFengQty(bool bDefaultValue = false);
	
	double GetValue(CString p_sCode, bool bDefaultValue = false) const;
	double GetH(bool bDefaultValue = false) const { return GetValue(L"H", bDefaultValue); }
	double GetH1(bool bDefaultValue = false) const { return GetValue(L"H1", bDefaultValue); }
	double GetH2(bool bDefaultValue = false) const { return GetValue(L"H2", bDefaultValue); }
	double GetW(bool bDefaultValue = false) const { return GetValue(L"W", bDefaultValue); }
	double GetW1(bool bDefaultValue = false) const { return GetValue(L"W1", bDefaultValue); }
	double GetW2(bool bDefaultValue = false) const { return GetValue(L"W2", bDefaultValue); }
	double GetW3(bool bDefaultValue = false) const { return GetValue(L"W3", bDefaultValue); }
	double GetA(bool bDefaultValue = false) const { return GetValue(L"a", bDefaultValue); } //塞缝尺寸
	double GetR(bool bDefaultValue = false) const { return GetValue(L"R", bDefaultValue); }

	bool SetValue(CString p_sCode, double p_dValue);
	bool SetH(double newValue) { return SetValue(L"H", newValue); }
	bool SetH1(double newValue) { return SetValue(L"H1", newValue); }
	bool SetH2(double newValue) { return SetValue(L"H2", newValue); }
	bool SetW(double newValue) { return SetValue(L"W", newValue); }
	bool SetW1(double newValue) { return SetValue(L"W1", newValue); }
	bool SetW2(double newValue) { return SetValue(L"W2", newValue); }
	bool SetW3(double newValue) { return SetValue(L"W3", newValue); }
	bool SetA(double newValue) { return SetValue(L"a", newValue); }//塞缝尺寸
	bool SetR(double newValue) { return SetValue(L"R", newValue); }

protected:
	vector<CWindowsDimData> m_dimData; //存储W/W1/W2/W3   H/H1/H2/H3 R的尺寸数据

public:
	CString m_openType;		//开启类型

	CDwgFileInfo m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	CDwgFileInfo m_topViewFile;		//原型俯视图文件
	CDwgFileInfo m_leftViewFile;	//原型侧视图文件
	
	CString m_tongFengFormula;//通风量计算公式,主要用于动态原型
	double m_tongFengQty;	//通风量

	Adesk::Int32 m_openQty;	//开启扇数量
	CString m_gongNengquType;//功能区类型

	bool m_isZhuanJiao;		//是否转角窗
	bool m_isMirrorWindow;	//是否对称窗型 

	//////////////////////////////////////////////////////////////////////////
	//算量相关
	CWindowMaterial m_material;

	//////////////////////////////////////////////////////////////////////////
	//以下属性为具体外窗插入时才设置,单个窗户实例的属性，非原型属性
	//视图属性
	bool m_isMirror;//是否镜像
	eViewDir m_viewDir;//视图方向

	bool   m_isBayWindow;	 //是否凸窗
	double m_wallDis;		 //外墙距离

};

typedef std::vector<AttrWindow> vAttrWindow;
