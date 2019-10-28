#include "StdAfx.h"
#include "AuxiliaryFrameSeries.h"


CAuxiliaryFrameData::CAuxiliaryFrameData()
{
	m_type = EAuxiliaryFrame_¸Ö¸½¿ò;
}

CAuxiliaryFrameData::~CAuxiliaryFrameData()
{	
}

//////////////////////////////////////////////////////////////////////////
CAuxiliaryFrameSeries* CAuxiliaryFrameSeries::Instance()
{
	static CAuxiliaryFrameSeries instance;
	return &instance;
}

CAuxiliaryFrameSeries::CAuxiliaryFrameSeries()
{
}

CAuxiliaryFrameSeries::~CAuxiliaryFrameSeries()
{
}

vector<CString> CAuxiliaryFrameSeries::GetAllAuxiliaryFrameSerials()
{
	vector<CString> serails1 = GetAllAuxiliaryFrameSerials(EAuxiliaryFrame_¸Ö¸½¿ò);
	vector<CString> serails2 = GetAllAuxiliaryFrameSerials(EAuxiliaryFrame_¸Ö¸½¿ò);
	serails1.insert(serails1.end(), serails2.begin(), serails2.end());

	return serails1;
}
vector<CString> CAuxiliaryFrameSeries::GetAllAuxiliaryFrameSerials(EAuxiliaryFrameType p_type)
{
	vector<CString>  serialsOut;
	switch(p_type)
	{
	case EAuxiliaryFrame_¸Ö¸½¿ò:
		serialsOut.push_back(_T("20*40*1.5mm"));
		serialsOut.push_back(_T("20*40*2mm"));
		break;
	case EAuxiliaryFrame_½ÚÄÜ¸½¿ò:
		serialsOut.push_back(_T("55*24mm"));
		serialsOut.push_back(_T("60*24mm"));
		serialsOut.push_back(_T("65*24mm"));
		serialsOut.push_back(_T("70*24mm"));
		serialsOut.push_back(_T("75*24mm"));
		serialsOut.push_back(_T("80*24mm"));
		serialsOut.push_back(_T("90*24mm"));
		serialsOut.push_back(_T("100*24mm"));
		serialsOut.push_back(_T("110*24mm"));
		serialsOut.push_back(_T("120*24mm"));
		break;
	default:
			break;
	}

	return serialsOut;
}

vector<double> CAuxiliaryFrameSeries::GetPlugSlotSize(bool p_bHasAuxiliaryFrame) //Èû·ì³ß´ç
{
	vector<double>  sizesOut;
	if (p_bHasAuxiliaryFrame)
	{
		sizesOut.push_back(10);
		sizesOut.push_back(20);
		sizesOut.push_back(25);
		sizesOut.push_back(30);
	}
	else
	{
		sizesOut.push_back(40);
		sizesOut.push_back(50);
	}
	return sizesOut;
}