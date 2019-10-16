#include "StdAfx.h"
#include "WindowStatictic.h"
#include "..\..\Tool\Excel\Excel.h"


CWindowStatictic::CWindowStatictic()
{
}

CWindowStatictic::~CWindowStatictic()
{
	for (UINT i = 0; i < m_allWindowMatUsage.size(); i++)
	{
		delete m_allWindowMatUsage[i];
	}

	m_allWindowMatUsage.clear();
}

void CWindowStatictic::Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile) //统计并生成统计报表文件
{
	////1. 首先对门窗进行分类和数量
	//vector<CWindowAttCount> windwosAttCounts = WindowClassify(const vector<AttrWindow>& p_winAtts);


	////2. 对各个门窗生成报表
	//InitWindowMatrialUsage(windwosAttCounts);



	////3. 汇总各个门窗得到汇总表
	//GenerateReport(p_sReportFile);



}