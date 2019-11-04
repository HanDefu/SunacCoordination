#include "StdAfx.h"
#include "..\..\Tool\Excel\Excel.h"
#include "WindowStatictic.h"
#include "WindowMaterialUsage.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Str.h"
#include "..\..\Common\TYFormula.h"


CWindowUsage::CWindowUsage()
{
	nCount = 1;
	m_pMaterialUsage = NULL;
}

CWindowUsage::~CWindowUsage()
{
	if (m_pMaterialUsage!=NULL)
	{
		delete m_pMaterialUsage;
		m_pMaterialUsage = NULL;
	}
}

void CWindowUsage::InitWindMaterialUsage()
{
	if (m_pMaterialUsage != NULL)
	{
		delete m_pMaterialUsage;
		m_pMaterialUsage = NULL;
	}

	E_WindowDoorType winDoorType = winAtt.GetWindowDoorType();
	switch (winDoorType)
	{
	case E_WindowDoor_NC:
		m_pMaterialUsage = new CWindowMaterialUsageNC(winAtt, nCount);
		break;
	case E_WindowDoor_WC:
		m_pMaterialUsage = new CWindowMaterialUsageWC(winAtt, nCount);
		break;
	case E_WindowDoor_TC:
		m_pMaterialUsage = new CWindowMaterialUsageTC(winAtt, nCount);
		break;
	case E_WindowDoor_WM:
		m_pMaterialUsage = new CWindowMaterialUsageWM(winAtt, nCount);
		break;
	case E_WindowDoor_TLM:
		m_pMaterialUsage = new CWindowMaterialUsageTLM(winAtt, nCount);
		break;
	case E_WindowDoor_TSTLM:
		m_pMaterialUsage = new CWindowMaterialUsageTSTLM(winAtt, nCount);
		break;
	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
CWindowStatictic::CWindowStatictic()
{
}

CWindowStatictic::~CWindowStatictic()
{
}


void CWindowStatictic::Statictic(const vector<AttrWindow>& p_winAtts, CString p_sReportFile) //统计并生成统计报表文件
{
	//1. 首先对门窗进行分类和数量
	WindowClassify(p_winAtts);	

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		m_windows[i].InitWindMaterialUsage();
	}

	//3. 输出报表
	Export(p_sReportFile);
}

void CWindowStatictic::WindowClassify(const vector<AttrWindow>& p_winAtts)
{
	m_windows.clear();
	
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //原型编号
		bool bFind = false;
		for (UINT n = 0; n < m_windows.size(); n++)
		{
			if (m_windows[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode)==0)
			{
				bFind = true;
				m_windows[n].nCount++;
				break;
			}
		}

		if (bFind==false)
		{
			CWindowUsage winNew;
			winNew.winAtt = p_winAtts[i];
			winNew.nCount = 1;
			m_windows.push_back(winNew);
		}
	}
}

void CWindowStatictic::Export(CString p_sReportFile)//算量表和统计表分开输出
{
	CString reportTemplateXlsFile = TY_GetDataFilePath() + _T("门窗算量表格模板.xlsx");

	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(reportTemplateXlsFile); //打开表格
	if (bSuc == false)
	{
		AfxMessageBox(_T("无法打开 门窗算量表格模板.xlsx"));
		return ;
	}

	xls.SetVisible(false);

	//2. 对各个门窗生成报表
	ExportWindowReport(xls);

	//3. 汇总各个门窗得到汇总表
	GenerateReport(xls);

	//保存
	xls.SaveAs(p_sReportFile);
	xls.CloseExcel();
}

bool CWindowStatictic::ExportWindowReport(Excel::CExcelUtil& xls)
{
	bool bAllSuc = true;

	//根据门窗数量新增门窗算量表
	for (UINT i = 1; i < m_windows.size(); i++)
	{
		xls.CopySheet(_T("算量表"), m_windows[i].winAtt.GetInstanceCode());
	}

	if (m_windows.size()>0)
	{
		xls.SetSheetName(2, m_windows[0].winAtt.GetInstanceCode());
	}

	for (UINT i = 0; i < m_windows.size(); i++)
	{
		xls.SetActiveSheet(i+2); //sheet序号从1开始，且第一个表是汇总表
		bool bSuc = m_windows[i].m_pMaterialUsage->ExportReportToExcel(xls);
		if (bSuc == false)
			bAllSuc = false;
	}

	return bAllSuc;
}

#if 0
void CWindowStatictic::ExportSeparate(CString p_sReportFile)//算量表和统计表分开输出
{
	//2. 对各个门窗生成报表
	ExportWindowReport(p_sReportFile);


	//3. 汇总各个门窗得到汇总表
	GenerateReport(p_sReportFile);
}
//每个窗编号导出一个报表
bool CWindowStatictic::ExportWindowReport(CString p_sReportFile)
{
	int nPos = p_sReportFile.ReverseFind(_T('.'));
	if (nPos<0)
		return false;

	CString sPath = p_sReportFile.Left(nPos);
	
	bool bAllSuc = true;
	for (UINT i = 0; i < m_windows.size(); i++)
	{
		CString sOutFilePath = sPath + _T("_") + m_windows[i].winAtt.GetInstanceCode() + _T(".xlsx");
		bool bSuc = m_windows[i].m_pMaterialUsage->ExportReportToExcel(sOutFilePath);
		if (bSuc==false)
			bAllSuc = false;
	}

	return bAllSuc;
}

//2.生成汇总表
bool CWindowStatictic::GenerateReport(CString p_sReportFile)
{
	CString reportTemplateXlsFile = TY_GetDataFilePath() + _T("门窗算量表格模板.xlsx");

	Excel::CExcelUtil xls;
	bool bSuc = xls.OpenExcel(reportTemplateXlsFile); //打开表格
	if (bSuc==false)
	{
		AfxMessageBox(_T("无法打开 门窗算量表格模板.xlsx"));
		return false;
	}

	xls.SetVisible(false); 
	xls.SetActiveSheet(1); //打开汇总表

	GenerateReport(xls);
	
	xls.SaveAs(p_sReportFile);
	xls.CloseExcel();

	return true;
}
#endif

bool CWindowStatictic::GenerateReport(Excel::CExcelUtil& xls)
{
	CString str;
	xls.SetActiveSheet(1); //打开汇总表

	int nRow = 6; //汇总数据开始行号为6
	for (UINT i=0; i<m_windows.size(); i++, nRow++)
	{
		//类别(一级分项)
		if (m_windows[i].winAtt.m_prototypeCode.Find(_T("Window")) >= 0)
		{
			xls.SetCellValue(nRow, 4, _T("铝合金窗"));
		}
		if (m_windows[i].winAtt.m_prototypeCode.Find(_T("Door")) >= 0)
		{
			xls.SetCellValue(nRow, 4, _T("铝合金门"));
		}

		//窗型(二级分项)
		xls.SetCellValue(nRow, 5, WindowDoorTypeToCSting(m_windows[i].winAtt.GetWindowDoorType()));

		//门窗编号
		xls.SetCellValue(nRow, 6, m_windows[i].winAtt.GetInstanceCode());

		//型材类型
		str = m_windows[i].winAtt.m_material.sAluminumSerial;
		xls.SetCellValue(nRow, 9, str);
		xls.SetCellValue(nRow, 10, _T("断桥铝"));
		if (str.Find(_T("50")) >= 0 || str.Find(_T("90")) >= 0 || str.Find(_T("120")) >= 0)
		{
			xls.SetCellValue(nRow, 10, _T("普铝"));
		}

		//洞口尺寸
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetW()));
		xls.SetCellValue(nRow, 12, str);
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetH()));
		xls.SetCellValue(nRow, 13, str);

		//扣减尺寸
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetA()));
		xls.SetCellValue(nRow, 14, str);
		xls.SetCellValue(nRow, 15, str);

		//外框尺寸
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetW() - m_windows[i].winAtt.GetA() * 2));
		xls.SetCellValue(nRow, 16, str);
		str.Format(_T("%d"), (int)(m_windows[i].winAtt.GetH() - m_windows[i].winAtt.GetA() * 2));
		xls.SetCellValue(nRow, 17, str);
	
		//工程量汇总
		str.Format(_T("%d"), m_windows[i].nCount);
		xls.SetCellValue(nRow, 18, str);

		//单樘洞口面积
		double holeArea = m_windows[i].m_pMaterialUsage->GetHoleArea();
		str.Format(_T("%.2f"), holeArea);
		xls.SetCellValue(nRow, 22, str);

		//洞口总面积
		str.Format(_T("%.2f"), double(m_windows[i].nCount* holeArea));
		xls.SetCellValue(nRow, 23, str);

		//单樘外框面积
		double frameArea = m_windows[i].m_pMaterialUsage->GetWindowFrameArea();
		str.Format(_T("%.2f"), frameArea);
		xls.SetCellValue(nRow, 24, str);

		//外框总面积
		str.Format(_T("%.2f"), double(m_windows[i].nCount* frameArea));
		xls.SetCellValue(nRow, 25, str);

		//型材(kg/㎡)
		double alUsageAmount = m_windows[i].m_pMaterialUsage->GetAluminumeUsageAmount();
		str.Format(_T("%.2f"), alUsageAmount/holeArea);
		xls.SetCellValue(nRow, 26, str);

		//玻璃(㎡/㎡)
		double glassUsageAmount = m_windows[i].m_pMaterialUsage->GetGlassUsageAmount();
		str.Format(_T("%.2f"), glassUsageAmount/holeArea);
		xls.SetCellValue(nRow, 27, str);

		//五金件(套/樘)
		double hardwareUsageAmount = m_windows[i].m_pMaterialUsage->GetHardwareUsageAmount();
		str.Format(_T("%.2f"), hardwareUsageAmount/holeArea);
		xls.SetCellValue(nRow, 29, str);
	}


	return true;
}