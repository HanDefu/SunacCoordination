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
	m_K = 0;
}
 
CRCRailing::~CRCRailing(void)
{

}

void CRCRailing::SetRailingAtt(const AttrRailing p_railingAtt)
{
	m_railingAtt = p_railingAtt;
}
bool CRCRailing::SetLength(double p_length) //设置栏杆总长度，参数(栏杆总长度)传进来，赋给m_length（总尺寸），返回GenRailing();
{
	m_railingAtt.m_length = p_length;
	return GenRailing(); //调用GenRailing()函数对栏杆总长进行判断，如果栏杆总长小于1550，返回false
}
CString CRCRailing::GetPrototypeFilePath()const
{
	return TY_GetPrototypeFilePath_Local() + m_railingAtt.m_prototypeCode + L".dwg";
}
bool CRCRailing::CheckLengthHeight()
{
	return (m_railingAtt.m_length > GetMinWidth()) && (m_railingAtt.m_height > GetMinHeight());
}
//start 为栏杆的左下角
int CRCRailing::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	if (CheckLengthHeight()==false)
	{
		return -1;
	}

	//1. 计算各分段的值
	bool bSuc = GenRailing();
	if (bSuc == false)
		return -1;

	CString sRailingDefName;
	sRailingDefName.Format(_T("%s_%d_%d"), m_railingAtt.m_prototypeCode, (int)(m_railingAtt.m_length), (int)(m_railingAtt.m_height));
	//若之前有，则直接插入
	AcDbObjectId railingBlockDef = MD2010_GetBlockDefID(sRailingDefName);
	if (railingBlockDef == AcDbObjectId::kNull)
	{
		AcDbObjectId blkDefId = CreateRailingBlockDefine(sRailingDefName);
	}

	MD2010_InsertBlockReference_ModelSpace(sRailingDefName, p_railingIdOut, start);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CRCRailing* CreateRailing(const AttrRailing p_railingAtt)
{
	CRCRailing* pRailing = NULL;
	switch (p_railingAtt.m_railingType)
	{
	case E_RAILING_TIEYI:
	{
		if (p_railingAtt.m_prototypeCode.Find(_T("T1")) >= 0 ||
			p_railingAtt.m_prototypeCode.Find(_T("T2")) >= 0)
		{
			pRailing = new CRCRailingT1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T3")) >= 0)
		{
			pRailing = new CRCRailingT3();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T4")) >= 0)
		{
			pRailing = new CRCRailingT4();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T5")) >= 0)
		{
			pRailing = new CRCRailingT5();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T6")) >= 0)
		{
			pRailing = new CRCRailingT6();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("T7")) >= 0)
		{
			pRailing = new CRCRailingT7();
		}
	}
		break;
	case E_RAILING_BOLI:

		if (p_railingAtt.m_prototypeCode.Find(_T("B1")) >= 0)
		{
			pRailing = new CRCRailingB1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B2")) >= 0 )
		{
			pRailing = new CRCRailingB2();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B3_1")) >= 0)
		{
			pRailing = new CRCRailingB3_1();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B3_2")) >= 0)
		{
			pRailing = new CRCRailingB3_2();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B4")) >= 0)
		{
			pRailing = new CRCRailingB4();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B5")) >= 0 )
		{
			pRailing = new CRCRailingB5();
		}
		else if (p_railingAtt.m_prototypeCode.Find(_T("B6")) >= 0)
		{
			pRailing = new CRCRailingB6();
		}

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

