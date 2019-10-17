#pragma once
#include "AluminumSeries.h"


//扣减尺寸表 //YMY
class CDeductedSize
{
	CDeductedSize();
public:
	static CDeductedSize* Instance();
	virtual ~CDeductedSize();

	//通过门窗类型、型材系列，型材名称获取型材数据,没查找到返回false
	bool GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut);
	bool GetDeductedSizeBySeriesAndName(CString p_sWinType, CString p_serials, CString sDeductedName, double &p_valueOut);


protected:

};
