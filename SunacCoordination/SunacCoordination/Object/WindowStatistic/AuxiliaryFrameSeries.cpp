#include "StdAfx.h"
#include "AuxiliaryFrameSeries.h"


CAuxiliaryFrameData::CAuxiliaryFrameData()
{
	m_type = EAuxiliaryFrame_钢附框;
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
	vector<CString> serails1 = GetAllAuxiliaryFrameSerials(EAuxiliaryFrame_钢附框);
	vector<CString> serails2 = GetAllAuxiliaryFrameSerials(EAuxiliaryFrame_节能附框);
	serails1.insert(serails1.end(), serails2.begin(), serails2.end());

	return serails1;
}
vector<CString> CAuxiliaryFrameSeries::GetAllAuxiliaryFrameSerials(EAuxiliaryFrameType p_type)
{
	vector<CString>  serialsOut;
	switch(p_type)
	{
	case EAuxiliaryFrame_钢附框:
		serialsOut.push_back(_T("钢20*40*1.5mm"));
		serialsOut.push_back(_T("钢20*40*2mm"));
		break;
	case EAuxiliaryFrame_节能附框:
		serialsOut.push_back(_T("节能附框55*24mm"));
		serialsOut.push_back(_T("节能附框60*24mm"));
		serialsOut.push_back(_T("节能附框65*24mm"));
		serialsOut.push_back(_T("节能附框70*24mm"));
		serialsOut.push_back(_T("节能附框75*24mm"));
		serialsOut.push_back(_T("节能附框80*24mm"));
		serialsOut.push_back(_T("节能附框90*24mm"));
		serialsOut.push_back(_T("节能附框100*24mm"));
		serialsOut.push_back(_T("节能附框110*24mm"));
		serialsOut.push_back(_T("节能附框120*24mm"));
		break;
	default:
			break;
	}

	return serialsOut;
}

vector<double> CAuxiliaryFrameSeries::GetPlugSlotSize(bool p_bHasAuxiliaryFrame) //塞缝尺寸
{
	vector<double>  sizesOut;
	if (p_bHasAuxiliaryFrame)
	{
		sizesOut.push_back(40);
		sizesOut.push_back(50);
	}
	else
	{
		sizesOut.push_back(10);
		sizesOut.push_back(20);
		sizesOut.push_back(25);
		sizesOut.push_back(30);
	}
	return sizesOut;
}