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

	bool ExportWindowReport(CString p_sReportFile);
	bool GenerateReport(CString p_sReportFile);

protected:
	vector<CWindowUsage> m_windows;
};

