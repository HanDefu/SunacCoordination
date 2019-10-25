#pragma once

class CAluminumFormula
{
public:
	CAluminumFormula();
	~CAluminumFormula();

public:
	CString m_prototypeCode;   //原型编号
	CString m_aluminumClassify; //型材分类
	CString m_name; //型材名称
	CString m_pos;	//位置
	CString m_formula; //公式
	int m_nCount;
};

//玻璃计算公式
class CGlassFormula
{
public:
	CGlassFormula();
	~CGlassFormula();

public:
	CString m_prototypeCode;   //原型编号
	CString m_glassClassify; //玻璃分类
	CString m_name; //玻璃名称
	CString m_widthFormula; //宽公式
	CString m_heightFormula; //宽公式
	int m_nCount;
};

class CHardwareData //五金件数据
{
public:
	CHardwareData();
	~CHardwareData();

public:
	CString m_prototypeCode;   //原型编号
	CString m_name; 
	int m_nCount;
};


//////////////////////////////////////////////////////////////////////////

//窗型公式查询库  //YMY  
class CWindowFormula
{
	CWindowFormula();
public:
	static CWindowFormula* Instance();
	virtual ~CWindowFormula();

	vector<CAluminumFormula> GetAluminumFormulas(CString p_sPrototypeCode, CString p_sAluminumSeries = _T(""));//使用表AluminumFormulas
	vector<CGlassFormula> GetGlassFormulas(CString p_sPrototypeCode);//使用表GlassFormulas
	vector<CHardwareData> GetHardwareData(CString p_sPrototypeCode);//使用表HardwareData

protected:

};

CString GBKToUTF8(const std::string& strGBK);
CString UTF8ToGBK(const std::string& strUTF8);