#pragma once
#include "..\WindowDoor\AttrWindow.h"

enum E_WindowDoorType
{
	E_WindowDoor_NC,
	E_WindowDoor_WC,
};


class CWindowMaterialUsage
{
public:
	CWindowMaterialUsage(const AttrWindow& p_winAtt);
	virtual ~CWindowMaterialUsage();
	
	//virtual void SetWindonAtt();

	double GetHoleArea()const;
	double GetWindowFrameArea()const;
	CString GetAluminumeSeries()const ;//获取该窗户的型材系列

	double GetAluminumeUsageAmount()const ; //获取该窗户的型材用量
	double GetGlassUsageAmount()const ; //获取该窗户的玻璃用量


protected:
	//根据原型名称p_propotypeName，材料名称p_sMatName查找计算公式
	CString LookUpFomula(CString p_propotypeName, CString p_sMatName);

	//根据原型名称p_propotypeName，材料名称p_sMatName查找数量
	CString LookUpCount(CString p_propotypeName, CString p_sMatName);


	//解析公式
	void ParseFomula(CString p_sFomula);


protected:
		//总计数量:
		//断桥隔热铝型材
		//非断桥隔热铝型材
		//阳极氧化铝型材
		//玻璃合计
		//五金套数
		//发泡剂
		//中性硅酮密封胶
		//中性硅酮密封胶
		//中性硅酮密封胶
		//组角胶
		//密封胶条
		//密封胶条
		//窗制作安装辅材费



};

//////////////////////////////////////////////////////////////////////////
//内开窗
class CWindowMaterialUsageNC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageNC();
	~CWindowMaterialUsageNC();

protected:

};


//////////////////////////////////////////////////////////////////////////
//外开窗
class CWindowMaterialUsageWC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWC();
	~CWindowMaterialUsageWC();

protected:

};

//////////////////////////////////////////////////////////////////////////
//推拉窗
class CWindowMaterialUsageTC :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTC();
	~CWindowMaterialUsageTC();

protected:

};

//////////////////////////////////////////////////////////////////////////
//推拉门,含提升推拉门
class CWindowMaterialUsageTLM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageTLM();
	~CWindowMaterialUsageTLM();

protected:

};

//////////////////////////////////////////////////////////////////////////
//外开门
class CWindowMaterialUsageWM :public CWindowMaterialUsage
{
public:
	CWindowMaterialUsageWM();
	~CWindowMaterialUsageWM();

protected:

};