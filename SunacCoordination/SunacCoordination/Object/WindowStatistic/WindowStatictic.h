#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "WindowMaterialUsage.h"

//单个窗型的属性、数量及用量
class CWindowUsage
{
public:
	CWindowUsage();
	virtual ~CWindowUsage();

	void InitWindMaterialUsage();

public:
	AttrWindow winAtt;
	int nCount;

	CWindowMaterialUsage* m_pMaterialUsage;
};


//门窗统计表
class CWindowStatictic
{
public:
	CWindowStatictic();
	virtual ~CWindowStatictic();
	
	void Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile); //统计并生成统计报表文件

protected:
	void WindowClassify(const vector<AttrWindow>& p_winAtts);

	void InitWindowMatrialUsage(vector<CWindowUsage>& p_winows);

	void Export(CString p_sReportFile);
	bool ExportWindowReport(Excel::CExcelUtil& xls);
	bool GenerateReport(Excel::CExcelUtil& xls);

#if 0
	void ExportSeparate(CString p_sReportFile); //算量表和统计表分开输出
	bool ExportWindowReport(CString p_sReportFile);
	bool GenerateReport(CString p_sReportFile);

#endif

protected:
	vector<CWindowUsage> m_windows;
};

