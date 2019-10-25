#pragma once
#include "AluminumSeries.h"

//扣减尺寸表 //YMY
class CDeductedSize
{
	CDeductedSize();
public:
	static CDeductedSize* Instance();
	virtual ~CDeductedSize();

	//通过门窗类型p_sWinType、型材系列p_serials，型材名称sDeductedName获取型材数据,没查找到返回false
	bool GetDeductedSizeBySeriesAndName(E_WindowDoorType p_winType, CString p_serials, CString sDeductedName, double &p_valueOut);

	//使用表DeductedSize
protected:
	bool GetDeductedSizeBySeriesAndName(CString p_sWinType, CString p_serials, CString sDeductedName, double &p_valueOut);

};

size_t ConvertStringToUTF8( LPCTSTR strIn, char *& strOutUTF8MB );