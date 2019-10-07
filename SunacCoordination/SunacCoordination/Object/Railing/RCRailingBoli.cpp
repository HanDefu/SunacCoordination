

#include "StdAfx.h"
#include "RCRailingBoli.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"


bool CRCRailingBoli::GenRailing()
{
	if (GetLength() < 300)
	{
		return false;    //对栏杆总长进行判断，如果栏杆总长小于1550，返回false
	}

	m_B = GenStandardRailingLen(GetLength());
	m_N = GenStandardSegCount(GetLength());
	//m_H = 150 * (m_N - 1);

	return true;
}

int CRCRailingBoli::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	return 0;
}
//////////////////////////////////////////////////////////////////////////
CRCRailingB1::CRCRailingB1()
{
	m_K = 137.5;
}

CRCRailingB1::~CRCRailingB1()
{
}



int CRCRailingB1::GenStandardSegCount(double p_lenth)const	//计算标准栏杆数量
{
	return int((p_lenth - 125) / 1650) + 1;
}

double CRCRailingB1::GenStandardRailingLen(double p_lenth)const				//计算标准栏杆尺寸
{
	int count = GenStandardSegCount(p_lenth);
	return (p_lenth - 275 - (150 * (count - 1))) / (double)count;
}


////////////////////////////////////////////////////////////////////////

CRCRailingB2::CRCRailingB2()
{
}
CRCRailingB2::~CRCRailingB2()
{
}

int CRCRailingB2::GenStandardSegCount(double p_lenth)const	//计算标准栏杆数量
{
	return int((p_lenth - 275) / 1500) + 1;
}

double CRCRailingB2::GenStandardRailingLen(double p_lenth)const				//计算标准栏杆尺寸
{
	int count = GenStandardSegCount(p_lenth);
	return (double)(p_lenth - 275) / count;
}

////////////////////////////////////////////////////////////////////////

CRCRailingB4::CRCRailingB4()
{
}
CRCRailingB4::~CRCRailingB4()
{
}

int CRCRailingB4::GenStandardSegCount(double p_lenth)const	//计算标准栏杆数量
{
	return int((p_lenth - 150) / 1500) + 1;
}

double CRCRailingB4::GenStandardRailingLen(double p_lenth)const				//计算标准栏杆尺寸
{
	int count = GenStandardSegCount(p_lenth);
	return (double)(p_lenth - 150) / count;
}