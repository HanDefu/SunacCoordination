#include "StdAfx.h"
#include "WindowMaterialUsage.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_Str.h"
#include "..\..\Common\TYFormula.h"

CWindowMaterialUsage::CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount)
:m_winAtt(p_winAtt)
, m_nCount(p_nCount)
{
	m_alTotalQTY = 0;  
	m_broAlQTY = 0;   
	m_nonBroAlQTY = 0; 
	m_anoAlQTY = 0;	 

	m_glassTotalQTY = 0;	
	m_hardwareNumber = 0;	

	m_foamingQTY = 0;	
	m_glassSealantQTY = 0; 
	m_inSealantQTY = 0;  
	m_exSealantQTY = 0;  
	m_comGlueQTY = 0;	
	m_sealStripQTY = 0;	
	m_squSealStripQTY = 0;
	m_sealBurrsQTY = 0;	
	m_windowCost = 0;	

	m_yaxianTotalLength = 0;
	m_chuangshanTotalLength = 0;
	m_shanliaoTotalLength = 0;
	m_menshanTotalLength = 0;
}

CWindowMaterialUsage::~CWindowMaterialUsage()
{
}

//void CWindowMaterialUsage::CalcMateriaUsage()
//{
//
//}


double CWindowMaterialUsage::GetVauleByFomula(CString p_sFomula)
{
	//p_sFomula.MakeUpper();
	CFormulaParser parser(p_sFomula);
	double ret = 0;
	if (parser.TryParse(ret) == E_PS_UnknownVars)
	{
		vCString vars = parser.GetUnknownVars();
		for (UINT i = 0; i < vars.size(); i++)
		{
			CString sParaName = vars[i];
			double valueOut = 0;
			//W.. H..a  R  的参数从m_winAtt 取
			if (sParaName.Find(_T("W")) >= 0 || sParaName.Find(_T("H")) >= 0 || sParaName.CompareNoCase(_T("a")) == 0 || sParaName.Compare(_T("R")) == 0)
			{
				valueOut = m_winAtt.GetValue(sParaName);
			}
			else
			{
				//其他的从扣减尺寸边里取
				bool bSuc = CDeductedSize::Instance()->GetDeductedSizeBySeriesAndName(m_winAtt.m_openType, m_winAtt.m_material.sAluminumSerial, sParaName, valueOut);
				assert(bSuc);
			}
			parser.SetVar(sParaName, valueOut);
		}
	}
	E_ParserStatus es = parser.TryParse(ret);
	
	if (es == E_PS_InvalidFormula)
	{
		CString errMsg;
		errMsg.Format(L"公式%s格式错误，无法计算\n", p_sFomula);
		acutPrintf(errMsg);
		return 0;
	}
	else if (es == E_PS_PaserOverflow)
	{
		CString errMsg;
		errMsg.Format(L"公式%s存在循环推导，无法计算\n", p_sFomula);
		acutPrintf(errMsg);
		return 0;
	}
	else if (es == E_PS_Success)
	{
		return ret;
	}
	else
	{
		assert(false);
		return 0;
	}
}

bool CWindowMaterialUsage::ExportReportToExcel(CString p_sReportFile)
{
	CString reportTemplateXlsFile = TY_GetLocalFilePath() + _T("门窗算量表格.xlsx");

	Excel::CExcelUtil xls;
	xls.OpenExcel(reportTemplateXlsFile); //打开表格
	xls.SetVisible(false); 
	xls.SetActiveSheet(2); //打开第二张表

	ExportReportToExcel(xls);

	xls.SaveAs(p_sReportFile);

	return true;
}

bool CWindowMaterialUsage::ExportReportToExcel(Excel::CExcelUtil& p_excel)
{
	//p_excel.SetActiveSheet(2); //打开第二张表

	ExportWindowInfo(p_excel);
	ExprotAlInfo(p_excel);
	ExportGlassInfo(p_excel);
	ExportHardwareInfo(p_excel);
	ExportFuliaoInfo(p_excel);

	return true;
}

void CWindowMaterialUsage::ExportWindowInfo(Excel::CExcelUtil& p_excel)//输出基本信息
{
	CString str;

	p_excel.SetCellValue(2, 2, m_winAtt.m_prototypeCode);

	//型材系列
	p_excel.SetCellValue(2, 5, m_winAtt.m_material.sAluminumSerial);
	
	//高度
	str.Format(_T("%d"),(int)(m_winAtt.GetH()));
	p_excel.SetCellValue(4, 8, str);
	p_excel.SetCellValue(2, 9, str);

	//宽
	str.Format(_T("%d"),(int)(m_winAtt.GetW()));
	p_excel.SetCellValue(5, 8, str);
	p_excel.SetCellValue(2, 7, str);


	//窗框尺寸
	str.Format(_T("%d"), (int)((m_winAtt.GetH() - m_winAtt.GetA() * 2)));
	p_excel.SetCellValue(4, 9, str);
	str.Format(_T("%d"), (int)((m_winAtt.GetW() - m_winAtt.GetA() * 2)));
	p_excel.SetCellValue(5, 9, str);

	//H1
	str.Format(_T("%d"),(int)(m_winAtt.GetH1()));
	p_excel.SetCellValue(6, 9, str);
	
	if (m_winAtt.HasValue(_T("H2")))
	{
		double value = m_winAtt.GetValue(_T("H2"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(7, 9, str);		
	}
	if (m_winAtt.HasValue(_T("H3")))
	{
		double value = m_winAtt.GetValue(_T("H3"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(8, 9, str);
	}

	//W1
	str.Format(_T("%d"),(int)(m_winAtt.GetW1()));
	p_excel.SetCellValue(9, 9, str);


	if (m_winAtt.HasValue(_T("W2")))
	{
		double value = m_winAtt.GetValue(_T("W2"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(10, 9, str);
	}
	if (m_winAtt.HasValue(_T("W3")))
	{
		double value = m_winAtt.GetValue(_T("W3"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(11, 9, str);
	}
	if (m_winAtt.HasValue(_T("W4")))
	{
		double value = m_winAtt.GetValue(_T("W4"));
		str.Format(_T("%d"), (int)value);
		p_excel.SetCellValue(12, 9, str);
	}

	//a
	str.Format(_T("%d"),(int)(m_winAtt.GetA()));
	p_excel.SetCellValue(13, 9, str);

	//门窗面积
	double area = m_winAtt.GetH()* m_winAtt.GetW()/1e6; //单位㎡	
	if (m_winAtt.GetR() > 0)
	{
		area = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA())* (m_winAtt.GetR()+ m_winAtt.GetA())/ 2)/1e6;
	}
	str.Format(_T("%.2f"), area);
	p_excel.SetCellValue(2, 12, str);
	p_excel.SetCellValue(14, 9, str);
	p_excel.SetCellValue(72, 9, str);
	p_excel.SetCellValue(73, 9, str);

	//门窗周长
	double circumference = (m_winAtt.GetH() + m_winAtt.GetW()) * 2 / 1000;	
	if (m_winAtt.GetR() > 0)
	{
		circumference = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* 2+ m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA()))/1000;
	}
	str.Format(_T("%.2f"), circumference);
	p_excel.SetCellValue(15, 9, str);
}

E_WindowDoorType  CWindowMaterialUsage::GetSpecialWinType(E_WindowDoorType winType, CString p_sAlName)const  //p_sAlName铝型材名称
{
	if (winType==E_WindowDoor_WM && p_sAlName.CompareNoCase(_T("内开扇"))==0)
	{
		return E_WindowDoor_NC;
	}

	return winType;
}

CString CWindowMaterialUsage::GetSpecialAlSerial(E_WindowDoorType winType, CString p_sAlName)const
{
	if (winType==E_WindowDoor_WM && p_sAlName.CompareNoCase(_T("内开扇"))==0)
	{
		CString sAlSerial = m_winAtt.m_material.sAluminumSerial;
		if (sAlSerial.CompareNoCase(_T("SW55M系列"))==0)
		{
			return _T("SN55M系列");
		}		
		else if (sAlSerial.CompareNoCase(_T("SW55BM系列"))==0)
		{
			return _T("SN55BM系列");
		}		
		else if (sAlSerial.CompareNoCase(_T("SW60M系列"))==0)
		{
			return _T("SN60系列");
		}	
		else if (sAlSerial.CompareNoCase(_T("SW60AM系列"))==0)
		{
			return _T("SN60A系列");
		}
		else if (sAlSerial.CompareNoCase(_T("SW65AM系列"))==0)
		{
			return _T("SN65A系列");
		}
	}

	return m_winAtt.m_material.sAluminumSerial;
}

void CWindowMaterialUsage::ExprotAlInfo(Excel::CExcelUtil& p_excel)//输出型材数据
{
	CString str;
	m_alTotalQTY = 0;
	m_broAlQTY = 0;
	m_nonBroAlQTY = 0;
	m_anoAlQTY = 0;
	m_shanliaoTotalLength = 0;
	m_menshanTotalLength = 0;

	vector<CAluminumFormula> alFormulas = CWindowFormula::Instance()->GetAluminumFormulas(m_winAtt.m_prototypeCode, m_winAtt.m_material.sAluminumSerial);

	if (alFormulas.empty())
	{
		alFormulas = CWindowFormula::Instance()->GetAluminumFormulas(m_winAtt.m_prototypeCode);
	}

	E_WindowDoorType winType = m_winAtt.GetWindowDoorType();

	int nRow = 17; //型材数据开始行号为17
	for (UINT i=0; i<alFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 2, alFormulas[i].m_aluminumClassify);
		p_excel.SetCellValue(nRow, 3, alFormulas[i].m_name);

		if (_T("框角码") == alFormulas[i].m_name || _T("扇角码1") == alFormulas[i].m_name || _T("扇角码2") == alFormulas[i].m_name)
		{
			p_excel.SetCellValue(nRow, 3, _T("角码"));
		}
		if (_T("中梃角码1") == alFormulas[i].m_name || _T("中梃角码2") == alFormulas[i].m_name )
		{
			p_excel.SetCellValue(nRow, 3, _T("中梃角码"));
		}
		if (_T("收口1") == alFormulas[i].m_name)
		{
			p_excel.SetCellValue(nRow, 3, _T("收口"));
		}

		p_excel.SetCellValue(nRow, 4, alFormulas[i].m_pos);

		CAluminumData dataOut;
		bool bSuc = CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(winType, m_winAtt.m_material.sAluminumSerial, alFormulas[i].m_name, dataOut);
		if (bSuc == false) 
		{
			//没有则检查是否有包含了其他的类型门窗的型材
			E_WindowDoorType wintype2 = GetSpecialWinType(winType, alFormulas[i].m_name);
			CString sAlSerial2 = GetSpecialAlSerial(winType, alFormulas[i].m_name);
			bSuc = CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(wintype2, sAlSerial2, alFormulas[i].m_name, dataOut);

			if (bSuc == false) //没有则查询系列为空的数据
			{
				bSuc = CAluminumSeries::Instance()->GetAluminumDataBySeriesAndName(winType, _T(""), alFormulas[i].m_name, dataOut);
			}
		}

		assert(bSuc);
		if (bSuc)
		{
			//型材编号
			p_excel.SetCellValue(nRow, 5, dataOut.sCode);
		}

		//长度
		double length = GetVauleByFomula(alFormulas[i].m_formula);
		str.Format(_T("%.2f"),length);
		p_excel.SetCellValue(nRow, 6, str);

		//数量
		str.Format(_T("%d"),alFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 7, str);

		//米重
		str.Format(_T("%.3f"),dataOut.weightPerMeter);
		p_excel.SetCellValue(nRow, 8, str);

		//总重
		double totalWeight = length * alFormulas[i].m_nCount * dataOut.weightPerMeter / 1000;
		str.Format(_T("%.2f"),totalWeight);
		p_excel.SetCellValue(nRow, 9, str);

		//耗损
		str.Format(_T("%.2f%%"),dataOut.wastageRate);
		p_excel.SetCellValue(nRow, 12, str);

		//型材种类
		p_excel.SetCellValue(nRow, 13, AluminumTypeToCSting(dataOut.aluminumType));

		//计算断桥隔热铝型材等的总重							
		if (E_断桥隔热铝型材 == dataOut.aluminumType)
		{
			m_broAlQTY += totalWeight;
		}
		else if (E_非断桥隔热铝型材 == dataOut.aluminumType)
		{
			m_nonBroAlQTY += totalWeight;
		}
		else 
		{
			m_anoAlQTY += totalWeight;
		}

		//用于输出辅材及其他
		if (_T("压线") == alFormulas[i].m_name)
		{
			m_yaxianTotalLength += length * alFormulas[i].m_nCount;
		}

		if (alFormulas[i].m_name.Find(_T("角码")) >= 0)
		{
			m_comGlueQTY += alFormulas[i].m_nCount;
		}

		if (_T("窗扇") == alFormulas[i].m_name)
		{
			m_chuangshanTotalLength += length*alFormulas[i].m_nCount;
		}

		if (_T("外开扇") == alFormulas[i].m_name)
		{
			m_chuangshanTotalLength += length*alFormulas[i].m_nCount;
		}

		if (_T("扇料") == alFormulas[i].m_aluminumClassify && m_winAtt.m_prototypeCode.Find(_T("TC"))>=0)
		{
			m_shanliaoTotalLength += length*alFormulas[i].m_nCount;
		}
		
		if (_T("外开扇") == alFormulas[i].m_name || _T("内开扇") == alFormulas[i].m_name || _T("门扇") == alFormulas[i].m_name )
		{
			m_menshanTotalLength += length*alFormulas[i].m_nCount;
		}

		if (_T("扇料") == alFormulas[i].m_aluminumClassify && m_winAtt.m_prototypeCode.Find(_T("TLM"))>=0)
		{
			m_shanliaoTotalLength += length*alFormulas[i].m_nCount;
		}
	}

	//用于计算推拉门TLM3、TML4的玻璃处耐候胶
	if (_T("Door_TLM3") == m_winAtt.m_prototypeCode || _T("Door_TLM4") == m_winAtt.m_prototypeCode)
	{
		double len = _ttof(p_excel.GetCellValue(25, 6));
		double count = _ttof(p_excel.GetCellValue(25, 7));
		m_shanliaoTotalLength = m_shanliaoTotalLength - count * len;
	}

	//断桥隔热铝型材总重
	str.Format(_T("%.2f"),m_broAlQTY);
	p_excel.SetCellValue(44, 9, str);

	//非断桥隔热铝型材总重
	str.Format(_T("%.2f"),m_nonBroAlQTY);
	p_excel.SetCellValue(45, 9, str);

	//阳极氧化铝型材总重
	str.Format(_T("%.2f"),m_anoAlQTY);
	p_excel.SetCellValue(46, 9, str);

	//铝型材总计
	m_alTotalQTY = m_broAlQTY + m_nonBroAlQTY + m_anoAlQTY;
	str.Format(_T("%.2f"),m_alTotalQTY);
	p_excel.SetCellValue(47, 9, str);
}

void CWindowMaterialUsage::ExportGlassInfo(Excel::CExcelUtil& p_excel) //输出玻璃数据
{
	m_glassTotalQTY = 0;

	CString str;
	const vector<CGlassFormula> glassFormulas = CWindowFormula::Instance()->GetGlassFormulas(m_winAtt.m_prototypeCode);

	int nRow = 49;
	for (UINT i=0; i<glassFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 2, glassFormulas[i].m_glassClassify);
		p_excel.SetCellValue(nRow, 3, glassFormulas[i].m_name);

		//玻璃宽度
		double width = GetVauleByFomula(glassFormulas[i].m_widthFormula);
		str.Format(_T("%.2f"),width);
		p_excel.SetCellValue(nRow, 5, str);

		//玻璃高度
		double height = GetVauleByFomula(glassFormulas[i].m_heightFormula);
		str.Format(_T("%.2f"),height);
		p_excel.SetCellValue(nRow, 6, str);

		//数量
		str.Format(_T("%d"),glassFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 7, str);

		//面积
		double area = height* width* glassFormulas[i].m_nCount/1e6; //单位㎡
		if (_T("Window_WC11") == m_winAtt.m_prototypeCode && _T("G3") == glassFormulas[i].m_name)
		{
			area = height/1e6;
		}
		str.Format(_T("%.2f"),area);
		p_excel.SetCellValue(nRow, 9, str);
		m_glassTotalQTY += area;
	}
	//玻璃合计
	str.Format(_T("%.2f"),m_glassTotalQTY);
	p_excel.SetCellValue(55, 9, str);
}

void CWindowMaterialUsage::ExportHardwareInfo(Excel::CExcelUtil& p_excel) //输出五金数据
{
	m_hardwareNumber = 0;

	CString str;
	const vector<CHardwareData> hardwareFormulas = CWindowFormula::Instance()->GetHardwareData(m_winAtt.m_prototypeCode);

	int nRow = 57;
	for (UINT i=0; i<hardwareFormulas.size(); i++, nRow++)
	{
		p_excel.SetCellValue(nRow, 3, hardwareFormulas[i].m_name);
		p_excel.SetCellValue(nRow, 7, _T("套"));

		//五金数量
		str.Format(_T("%.2f"),(double)hardwareFormulas[i].m_nCount);
		p_excel.SetCellValue(nRow, 9, str);
		m_hardwareNumber += hardwareFormulas[i].m_nCount;
	}
	//五金件合计
	str.Format(_T("%.2f"),(double)m_hardwareNumber);
	p_excel.SetCellValue(58, 9, str);
}

void CWindowMaterialUsage::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) //输出辅料数据
{
	CString str;

	//发泡剂
	m_foamingQTY = (m_winAtt.GetH() + m_winAtt.GetW())*2/1000;
	if (m_winAtt.GetR() > 0)
	{
		m_foamingQTY = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* 2+ m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA()))/1000;
	}
	str.Format(_T("%.2f"),m_foamingQTY);
	p_excel.SetCellValue(60, 9, str);

	//中性硅酮密封胶(玻璃处耐候胶)
	m_glassSealantQTY = m_yaxianTotalLength*2/1000;
	str.Format(_T("%.2f"),m_glassSealantQTY);
	p_excel.SetCellValue(61, 9, str);

	//中性硅酮密封胶(内墙胶)
	m_inSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_inSealantQTY);
	p_excel.SetCellValue(62, 9, str);

	//中性硅酮密封胶(外墙胶)
	m_exSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_exSealantQTY);
	p_excel.SetCellValue(63, 9, str);

	//组角胶
	str.Format(_T("%.2f"),m_comGlueQTY);
	p_excel.SetCellValue(64, 9, str);
	
	//密封胶条(m)
	m_sealStripQTY = m_chuangshanTotalLength*2/1000;
	str.Format(_T("%.2f"),m_sealStripQTY);
	p_excel.SetCellValue(65, 9, str);

	//密封胶条(㎡)
	m_squSealStripQTY = m_chuangshanTotalLength/1000;
	str.Format(_T("%.2f"),m_squSealStripQTY);
	p_excel.SetCellValue(66, 9, str);

	//窗制作安装辅材费
	m_windowCost = m_winAtt.GetH()* m_winAtt.GetW()/1e6;	
	if (m_winAtt.GetR() > 0)
	{
		m_windowCost = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA())* (m_winAtt.GetR()+ m_winAtt.GetA())/ 2)/1e6;
	}
	str.Format(_T("%.2f"),m_windowCost);
	p_excel.SetCellValue(67, 9, str);
}

//外开窗
void CWindowMaterialUsageWC::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) //输出辅料数据
{
	CString str;

	//发泡剂
	m_foamingQTY = (m_winAtt.GetH() + m_winAtt.GetW())*2/1000;
	if (m_winAtt.GetR() > 0)
	{
		m_foamingQTY = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* 2+ m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA()))/1000;
	}
	str.Format(_T("%.2f"),m_foamingQTY);
	p_excel.SetCellValue(60, 9, str);

	//中性硅酮密封胶(玻璃处耐候胶)
	m_glassSealantQTY = m_yaxianTotalLength*2/1000;
	str.Format(_T("%.2f"),m_glassSealantQTY);
	p_excel.SetCellValue(61, 9, str);

	//中性硅酮密封胶(内墙胶)
	m_inSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_inSealantQTY);
	p_excel.SetCellValue(62, 9, str);

	//中性硅酮密封胶(外墙胶)
	m_exSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_exSealantQTY);
	p_excel.SetCellValue(63, 9, str);

	//组角胶
	str.Format(_T("%.2f"),m_comGlueQTY);
	p_excel.SetCellValue(64, 9, str);

	//密封胶条(m)
	m_sealStripQTY = m_chuangshanTotalLength*2/1000;
	str.Format(_T("%.2f"),m_sealStripQTY);
	p_excel.SetCellValue(65, 9, str);

	//窗制作安装辅材费
	m_windowCost = m_winAtt.GetH()* m_winAtt.GetW()/1e6;	
	if (m_winAtt.GetR() > 0)
	{
		m_windowCost = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA())* (m_winAtt.GetR()+ m_winAtt.GetA())/ 2)/1e6;
	}
	str.Format(_T("%.2f"),m_windowCost);
	p_excel.SetCellValue(67, 9, str);
}

//推拉窗
void CWindowMaterialUsageTC::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) //输出辅料数据
{
	CString str;

	//发泡剂
	m_foamingQTY = (m_winAtt.GetH() + m_winAtt.GetW())*2/1000;
	str.Format(_T("%.2f"),m_foamingQTY);
	p_excel.SetCellValue(60, 9, str);

	//中性硅酮密封胶(玻璃处耐候胶)
	m_glassSealantQTY = m_shanliaoTotalLength*2/1000;
	str.Format(_T("%.2f"),m_glassSealantQTY);
	p_excel.SetCellValue(61, 9, str);

	//中性硅酮密封胶(内墙胶)
	m_inSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_inSealantQTY);
	p_excel.SetCellValue(62, 9, str);

	//中性硅酮密封胶(外墙胶)
	m_exSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_exSealantQTY);
	p_excel.SetCellValue(63, 9, str);

	//组角胶
	str.Format(_T("%.2f"),m_comGlueQTY);
	p_excel.SetCellValue(64, 9, str);

	//密封毛条
	p_excel.SetCellValue(65, 3, _T("密封毛条"));
	m_sealBurrsQTY = m_glassSealantQTY;
	str.Format(_T("%.2f"),m_sealBurrsQTY);
	p_excel.SetCellValue(65, 9, str);

	//窗制作安装辅材费
	m_windowCost = m_winAtt.GetH()* m_winAtt.GetW()/1e6;
	str.Format(_T("%.2f"),m_windowCost);
	p_excel.SetCellValue(67, 9, str);
}

//外开门
void CWindowMaterialUsageWM::ExportFuliaoInfo(Excel::CExcelUtil& p_excel) //输出辅料数据
{
	CString str;

	//发泡剂
	m_foamingQTY = (m_winAtt.GetH() + m_winAtt.GetW())*2/1000;
	str.Format(_T("%.2f"),m_foamingQTY);
	p_excel.SetCellValue(60, 9, str);

	//中性硅酮密封胶(玻璃处耐候胶)
	m_glassSealantQTY = m_yaxianTotalLength*2/1000;
	str.Format(_T("%.2f"),m_glassSealantQTY);
	p_excel.SetCellValue(61, 9, str);

	//中性硅酮密封胶(内墙胶)
	m_inSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_inSealantQTY);
	p_excel.SetCellValue(62, 9, str);

	//中性硅酮密封胶(外墙胶)
	m_exSealantQTY = m_foamingQTY;
	str.Format(_T("%.2f"),m_exSealantQTY);
	p_excel.SetCellValue(63, 9, str);

	//组角胶
	str.Format(_T("%.2f"),m_comGlueQTY);
	p_excel.SetCellValue(64, 9, str);

	//密封胶条(m)
	m_sealStripQTY = m_menshanTotalLength*2/1000;
	str.Format(_T("%.2f"),m_sealStripQTY);
	p_excel.SetCellValue(65, 9, str);

	//窗制作安装辅材费
	m_windowCost = m_winAtt.GetH()* m_winAtt.GetW()/1e6;
	str.Format(_T("%.2f"),m_windowCost);
	p_excel.SetCellValue(66, 9, str);
}


//////////////////////////////////////////////////////////////////////////
//以下函数用于填写汇总表
double CWindowMaterialUsage::GetHoleArea()const
{
	double area = m_winAtt.GetH()* m_winAtt.GetW()/1e6; //单位㎡	
	if (m_winAtt.GetR() > 0)
	{
		area = ((m_winAtt.GetH1()+ m_winAtt.GetH2()+ m_winAtt.GetA())* m_winAtt.GetW()+ 3.14* (m_winAtt.GetR()+ m_winAtt.GetA())* (m_winAtt.GetR()+ m_winAtt.GetA())/ 2)/1e6;
	}

	return area;
}

double CWindowMaterialUsage::GetWindowFrameArea()const
{
	double area = m_winAtt.GetH()* m_winAtt.GetW()/1e6; //单位㎡	
	if (m_winAtt.GetR() > 0)
	{
		area = ((m_winAtt.GetH1()+ m_winAtt.GetH2())* (m_winAtt.GetW() - m_winAtt.GetA()* 2)+ 3.14* m_winAtt.GetR()* m_winAtt.GetR()/ 2)/1e6;
	}

	return area;
}

CString CWindowMaterialUsage::GetAluminumeSeries()const 	//获取该窗户的型材系列
{
	return m_winAtt.m_material.sAluminumSerial;
}

double CWindowMaterialUsage::GetAluminumeUsageAmount()const  //获取该窗户的型材用量总计
{
	return m_alTotalQTY;
}

double CWindowMaterialUsage::GetGlassUsageAmount()const 	//获取该窗户的玻璃用量总计
{
	return m_glassTotalQTY;
}

double CWindowMaterialUsage::GetHardwareUsageAmount()const 	//获取该窗户的五金用量总计
{
	return m_hardwareNumber;
}