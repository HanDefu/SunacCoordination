#pragma once
#include "WindowMaterialUsage.h"


struct CAluminumData
{
	CString sCode; //型材代号
	CString sName; 
	CString sSerial;
	double weightPerMeter; //米重

	CAluminumData()
	{
		weightPerMeter = 0;
	}
}; 

class CAluminumSeries
{
public:
	CAluminumSeries();
	virtual ~CAluminumSeries();

	//通过门窗类型、型材系列，型材名称获取型材数据
	bool GetAluminumCodeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sName, CAluminumData& p_dataOut);


protected:

};
