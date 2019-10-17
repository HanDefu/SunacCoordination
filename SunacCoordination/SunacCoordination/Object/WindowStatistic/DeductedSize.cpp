#include "StdAfx.h"
#include "DeductedSize.h"

CDeductedSize* CDeductedSize::Instance()
{
	static CDeductedSize instance;
	return &instance;
}
CDeductedSize::CDeductedSize()
{
}

CDeductedSize::~CDeductedSize()
{
}