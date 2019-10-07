/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCLanGan.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCRailing.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"
#include "RCRailingTieyi.h"
#include "RCRailingBoli.h"
 
CRCRailing::CRCRailing(void)
{
	m_B = 1260;			//标准栏杆尺寸 1260或者1380
	m_N = 1;			//标准栏杆数量 
	m_n = 1;			//标准栏杆两侧单元花格数量
	m_K = 0;
}
 
CRCRailing::~CRCRailing(void)
{

}

//CRCRailing::CRCRailing(const CRCRailing &other):RCStandBlock(other)
//{
//	*this = other;
//}
//
//CRCRailing& CRCRailing::operator=(const CRCRailing &rhs)
//{
//	m_B = rhs.m_B;			//标准栏杆尺寸 1260或者1380
//	m_N = rhs.m_B;			//标准栏杆段数量 
//	m_n = rhs.m_B;			//标准栏杆两侧单元花格数量（两侧）
//	m_K = rhs.m_B;			//侧边立柱与结构墙间尺寸（单边）
//
//	m_railingAtt = rhs.m_railingAtt;
//
//	return *this;
//}

void CRCRailing::SetRailingAtt(const AttrRailing p_railingAtt)
{
	m_railingAtt = p_railingAtt;
}
bool CRCRailing::SetLength(double p_length) //设置栏杆总长度，参数(栏杆总长度)传进来，赋给m_length（总尺寸），返回GenRailing();
{
	m_railingAtt.m_length = p_length;
	return GenRailing(); //调用GenRailing()函数对栏杆总长进行判断，如果栏杆总长小于1550，返回false
}

//////////////////////////////////////////////////////////////////////////

CRCRailing* CreateRailing(const AttrRailing p_railingAtt)
{
	CRCRailing* pRailing = NULL;
	switch (p_railingAtt.m_railingType)
	{
	case E_RAILING_TIEYI:
	{
							if (p_railingAtt.m_prototypeCode.Find(_T("T1")) >= 0)
							{
								pRailing = new CRCRailingT1();
							}
	}
		break;
	case E_RAILING_BOLI://TODO
		break;
	default:
		break;
	}

	if (pRailing!=NULL)
	{
		pRailing->SetRailingAtt(p_railingAtt);
	}

	return pRailing; 
}

