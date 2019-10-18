#include "StdAfx.h"
#include "AluminumSeries.h"


E_WindowDoorType GetWindowDoorType(CString sOpenType)
{

	//TODO Ò¶Ã÷Ô¶

	return E_WindowDoor_NC;
}

CAluminumSeries* CAluminumSeries::Instance()
{
	static CAluminumSeries instance;
	return &instance;
}

CAluminumSeries::CAluminumSeries()
{
}

CAluminumSeries::~CAluminumSeries()
{
}