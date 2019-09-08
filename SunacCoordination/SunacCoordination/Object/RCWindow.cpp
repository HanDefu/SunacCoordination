/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCWindow.h"
#include "float.h"
#include <algorithm>
#include "../Common/ComFun_Sunac.h"

//Constructor
RCWindow::RCWindow(void)
{
	m_pAttribute = 0;
}

//Destructor
RCWindow::~RCWindow(void)
{

}


//Constructor
RCWindow::RCWindow(const RCWindow &other):RCDynamicBlock(other)
{
	m_pAttribute = other.m_pAttribute;
}

//Operator = 
RCWindow & RCWindow::operator=(const RCWindow &other)
{
	m_pAttribute = other.m_pAttribute;
	return *this;
}


void RCWindow::Draw()
{
}

RCWindow * RCWindow::Clone()
{
	RCWindow * newObj = new RCWindow;

	return newObj;
}

double RCWindow::GetH()
{
	double value = 0;
	GetParameter(L"H",value);
	return value;
}
double RCWindow::GetH1()
{
	double value = 0;
	GetParameter(L"H1",value);
	return value;
}
double RCWindow::GetH2()
{
	double value = 0;
	GetParameter(L"H2",value);
	return value;
}
double RCWindow::GetW()
{
	double value = 0;
	GetParameter(L"W",value);
	return value;
}
double RCWindow::GetW1()
{
	double value = 0;
	GetParameter(L"W1",value);
	return value;
}
double RCWindow::GetW2()
{
	double value = 0;
	GetParameter(L"W2",value);
	return value;
}
double RCWindow::GetW3()
{
	double value = 0;
	GetParameter(L"W3",value);
	return value;
}
double RCWindow::GetA()
{
	double value = 0;
	GetParameter(L"A",value);
	return value;
}

int RCWindow::SetH(double newValue)
{
	return SetParameter(L"H", newValue);
}
int RCWindow::SetH1(double newValue)
{
	return SetParameter(L"H1", newValue);
}
int RCWindow::SetH2(double newValue)
{
	return SetParameter(L"H2", newValue);
}
int RCWindow::SetW(double newValue)
{
	return SetParameter(L"W", newValue);
}
int RCWindow::SetW1(double newValue)
{
	return SetParameter(L"W1", newValue);
}
int RCWindow::SetW2(double newValue)
{
	return SetParameter(L"W2", newValue);
}
int RCWindow::SetW3(double newValue)
{
	return SetParameter(L"W3", newValue);
}
int RCWindow::SetA(double newValue)
{
	return SetParameter(L"A", newValue);
}

AttrWindow * RCWindow::GetAttribute()
{
	if (m_pAttribute == 0)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(m_id, pDataEnt);
		m_pAttribute = dynamic_cast<AttrWindow *>(pDataEnt);
	}
	return m_pAttribute;
}

void RCWindow::AddAttribute(AttrWindow * attr)
{
	if (attr != 0)
	    TY_AddAttributeData(m_id, attr);
}

bool RCWindow::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCWindow * pObj = dynamic_cast<RCWindow *>(other);
	if (pObj == 0)
		return false;

	if (!RCDynamicBlock::isEqualTo(other))
		return false;

	AttrWindow * thisAttr = GetAttribute();
	AttrWindow * otherAttr = pObj->GetAttribute();
	if (!thisAttr->isEqualTo(otherAttr))
		return false;

	return true;
}

//门窗面积
double RCWindow::GetWindowArea()
{
    return GetH() * GetW();
}

//开启面积
double RCWindow::GetOpenArea()
{
	return GetW1() * GetH1();
}

void RCWindow::SetBianHao(CString str)
{
	GetAttribute()->SetBianHao(str);
}
CString RCWindow::GetBianHao()
{
	return GetAttribute()->GetBianHao();
}
