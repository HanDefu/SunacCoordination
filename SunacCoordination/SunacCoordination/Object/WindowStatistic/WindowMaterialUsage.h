#pragma once
#include "..\WindowDoor\AttrWindow.h"
#include "..\..\Tool\Excel\Excel.h"
#include "DeductedSize.h"
#include "AluminumSeries.h"
#include "WindowFormula.h"



//窗单价组合分析表  //ZJY
class CWindowMaterialUsage
{
public:
	CWindowMaterialUsage(const AttrWindow& p_winAtt, int p_nCount);  //p_nCount表示当前窗型的数量
	virtual ~CWindowMaterialUsage();

	//virtual void CalcMateriaUsage();
	bool ExportReportToExcel(CString p_sReportFile); //导出到excel报表
	bool ExportReportToExcel(Excel::CExcelUtil& p_excel); //导出到excel报表

	
	//////////////////////////////////////////////////////////////////////////
	//以下函数用于填写汇总表
	double GetHoleArea()const;
	double GetWindowFrameArea()const;
	CString GetAluminumeSeries()const ;		//获取该窗户的型材系列

	double GetAluminumeUsageAmount()const ; //获取该窗户的型材用量
	double GetGlassUsageAmount()const ;		//获取该窗户的玻璃用量

protected:
	double GetVauleByFomula(CString p_sFomula);

	virtual void ExportWindowInfo(Excel::CExcelUtil& p_excel);//输出基本信息
	virtual void ExprotAlInfo(Excel::CExcelUtil& p_excel);//输出型材数据
	virtual void ExportGlassInfo(Excel::CExcelUtil& p_excel); //输出玻璃数据
	virtual void ExportHardwareInfo(Excel::CExcelUtil& p_excel);//输出五金数据
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//输出辅料数据

	//部分情况下需要用到其他类型的型材，以下函数获取对应的型材
	E_WindowDoorType  GetSpecialWinType(E_WindowDoorType winType, CString p_sAlName)const ; //p_sAlName铝型材名称
	CString GetSpecialAlSerial(E_WindowDoorType winType, CString p_sAlName)const;

protected:
	const AttrWindow m_winAtt;
	int m_nCount;
	
	//////////////////////////////////////////////////////////////////////////
	double m_alTotalQTY;  //总计数量:
	double m_broAlQTY;    //断桥隔热铝型材
	double m_nonBroAlQTY; //非断桥隔热铝型材
	double m_anoAlQTY;	  //阳极氧化铝型材

	double m_glassTotalQTY;	//玻璃合计

	int m_hardwareNumber;	//五金套数

	double m_foamingQTY;	//发泡剂
	double m_glassSealantQTY; //中性硅酮密封胶(玻璃处耐候胶)
	double m_inSealantQTY;  //中性硅酮密封胶(内墙胶)
	double m_exSealantQTY;  //中性硅酮密封胶(外墙胶)
	double m_comGlueQTY;	//组角胶
	double m_sealStripQTY;	//密封胶条(m)
	double m_squSealStripQTY;//密封胶条(㎡)
	double m_sealBurrsQTY;	//密封毛条
	double m_windowCost;	//窗制作安装辅材费
	
	double m_yaxianTotalLength;//压线总长
	double m_chuangshanTotalLength;//窗扇总长
	double m_shanliaoTotalLength; //扇料总长
	double m_menshanTotalLength; //门扇总长
};

//////////////////////////////////////////////////////////////////////////
//内开窗
class CWindowMaterialUsageNC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageNC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	
};


//////////////////////////////////////////////////////////////////////////
//外开窗
class CWindowMaterialUsageWC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//输出辅料数据
};

//////////////////////////////////////////////////////////////////////////
//推拉窗
class CWindowMaterialUsageTC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTC(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//输出辅料数据
};

//////////////////////////////////////////////////////////////////////////
//推拉门
class CWindowMaterialUsageTLM :public CWindowMaterialUsageTC
{
public:
	CWindowMaterialUsageTLM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsageTC(p_winAtt, p_nCount) {}

protected:
	
};

//提升推拉门
class CWindowMaterialUsageTSTLM :public CWindowMaterialUsageTC
{
public:
	CWindowMaterialUsageTSTLM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsageTC(p_winAtt, p_nCount) {}

protected:

};

//////////////////////////////////////////////////////////////////////////
//外开门
class CWindowMaterialUsageWM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWM(const AttrWindow& p_winAtt, int p_nCount) : CWindowMaterialUsage(p_winAtt, p_nCount) {}

protected:
	virtual void ExportFuliaoInfo(Excel::CExcelUtil& p_excel);//输出辅料数据
};