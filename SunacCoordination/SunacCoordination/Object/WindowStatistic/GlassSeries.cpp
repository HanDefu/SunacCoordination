#include "StdAfx.h"
#include "AluminumSeries.h"
#include "GlassSeries.h"
#include "..\..\Tool\SQLite\sqlite3.h"
#include "..\..\Common\ComFun_Sunac.h"


CGlassData::CGlassData()
{
	m_heatInsulationCoefficient = 0;
	m_shadingCoefficient = 0; 
	m_glassCount = 2;
}

CGlassData::~CGlassData()
{
	
}


CGlassSeries* CGlassSeries::Instance()
{
	static CGlassSeries instance;
	return &instance;
}

CGlassSeries::CGlassSeries()
{
}

CGlassSeries::~CGlassSeries()
{
}

vector<CString> CGlassSeries::GetAllGlassSerials()
{
	vector<CString> glassSerials;
	glassSerials.push_back(_T("5+9A+5"));
	glassSerials.push_back(_T("5+12A+5"));
	glassSerials.push_back(_T("5+12A+5+12A+5"));
	glassSerials.push_back(_T("5+12A+5+12A+5Low-E"));
	glassSerials.push_back(_T("5+12A+5Low-E"));
	glassSerials.push_back(_T("5+12A+5双银Low-E"));
	glassSerials.push_back(_T("5+12Ar+5(2#面Low-E)"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5(单银Low-E)"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5Low-E双银"));
	glassSerials.push_back(_T("5+12Ar+5+12Ar+5单银Low-E（双暖边）"));
	glassSerials.push_back(_T("5+12Ar+5Low-E双银"));
	glassSerials.push_back(_T("5+6A+5"));
	glassSerials.push_back(_T("5+9A+5"));
	glassSerials.push_back(_T("5Low-E+12A+5"));
	glassSerials.push_back(_T("5Low-E+12Ar+5"));
	glassSerials.push_back(_T("5Low-E+15Ar+5（高性能暖边）"));
	glassSerials.push_back(_T("5Low-E+6A+5"));
	glassSerials.push_back(_T("5Low-E+9A+5"));
	glassSerials.push_back(_T("5双银Low-E+12A+5"));
	glassSerials.push_back(_T("6+12A+6"));
	glassSerials.push_back(_T("6Low-E+12A+6"));
	glassSerials.push_back(_T("6Low-E+9A+6"));
	glassSerials.push_back(_T("6单银Low-E+12A+6"));
	glassSerials.push_back(_T("6单银Low-E+12Ar+6"));
	glassSerials.push_back(_T("6高透+12Ar+6（高性能暖边）"));
	glassSerials.push_back(_T("6高透Low-E+12A+6"));
	glassSerials.push_back(_T("6高透光Low-E+12A+6"));
	glassSerials.push_back(_T("6双银Low-E+12A+6"));
	glassSerials.push_back(_T("6中透光Low-E+12A+6"));
	glassSerials.push_back(_T("6中透光Low-E+9A+6"));	

	//后续改为读数据库

	return glassSerials;
}