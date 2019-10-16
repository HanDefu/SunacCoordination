#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "WindowMaterialUsage.h"

class CWindowAttCount
{
public:
	AttrWindow winAtt;
	int nCount;

	CWindowAttCount()
	{
		nCount = 1;
	}
};


//门窗统计表
class CWindowStatictic
{
public:
	CWindowStatictic();
	virtual ~CWindowStatictic();
	
	void Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile); //统计并生成统计报表文件

protected:
	vector<CWindowAttCount> WindowClassify(const vector<AttrWindow>& p_winAtts);

	void InitWindowMatrialUsage(const vector<CWindowAttCount>& p_winows);

	bool GenerateReport(CString p_sReportFile);

protected:
	vector<CWindowMaterialUsage*> m_allWindowMatUsage; //所有类型的窗型的算量对象
};

