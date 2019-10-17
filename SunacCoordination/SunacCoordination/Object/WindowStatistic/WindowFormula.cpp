#include "StdAfx.h"
#include "WindowFormula.h"

CAluminumFormula::CAluminumFormula()
{
	m_nCount = 1;
}

CAluminumFormula::~CAluminumFormula()
{
}

CGlassFormula::CGlassFormula()
{
	m_nCount = 1;
}

CGlassFormula::~CGlassFormula()
{
}
CHardwareData::CHardwareData()
{
	m_nCount = 1;
}

CHardwareData::~CHardwareData()
{
}


//////////////////////////////////////////////////////////////////////////

CWindowFormula* CWindowFormula::Instance()
{
	static CWindowFormula instance;
	return &instance;
}

CWindowFormula::CWindowFormula()
{
}

CWindowFormula::~CWindowFormula()
{
}