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
CRCRailingTieyi::CRCRailingTieyi()
{
}

CRCRailingTieyi::~CRCRailingTieyi()
{
}


//////////////////////////////////////////////////////////////////////////

CRCRailingT1::CRCRailingT1()
{
}

//start 为栏杆的左下角
int CRCRailingT1::GenerateRailing(AcGePoint3d start, AcGePoint3d end)
{
	//1. 设置长度，并计算各分段的值
	double length = (end - start).length();
	SetLength(length);

	//2 插入到图形
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + 1100, 0);

	//2.1 非标段
	AcDbObjectId id1;
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), leftTopPt.y - GetHandRailHeight(), 0); //左上角点x方向上减去与结构墙间隙，y方向上减去扶手的厚度
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_NonStandard"), id1, pos1, 0, AcGeScale3d(1));

	//设置非标段长度
	DQ_SetDynamicAttribute(id1, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id1, _T("Ln"), GetNonstandardLen()-GetPillarWidth()*2); //内部的小花格区间为非标段减去两侧立柱

	//2.2 标准段
	AcDbObjectId id2;
	AcGePoint3d pos2 = pos1;
	pos2.x = pos1.x + GetNonstandardLen() - GetPillarWidth();
	CString sStandardBlockName = GetB() < 1380 ? _T("Railing_T1_1260") : _T("Railing_T1_1380");
	for (int i = 0; i < GetN(); i++)
	{
		MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, sStandardBlockName, id2, pos2, 0, AcGeScale3d(1));
		pos2.x += GetB() - GetPillarWidth(); //减去立柱宽
	}


	//2.3 非标段
	AcDbObjectId id3;
	AcGePoint3d pos3 = pos2;
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_NonStandard"), id3, pos3, 0, AcGeScale3d(1));
	//设置非标段长度
	DQ_SetDynamicAttribute(id3, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id3, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2); //内部的小花格区间为非标段减去两侧立柱

	//2.4 扶手
	AcDbObjectId id4;
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, m_railingAtt.m_filePathName, _T("Railing_T1_Handrail"), id1, leftTopPt, 0, AcGeScale3d(1));
	//设置扶手长度
	DQ_SetDynamicAttribute(id4, _T("L"), length);


	acDocManager->unlockDocument(curDoc());

	return 0;
}


bool CRCRailingT1::GenRailing()  //对栏杆总长进行判断，如果栏杆总长小于1550，返回false
{
	if (GetLenght() < 1550)
	{
		return false;    //点击计算按钮后，如果judge()返回false,则用户在对话框输入的总尺寸不符合要求，应弹出对话框提示用户输入有误
	}

	//先计算1260标准段的数据
	int N_1 = GenStandardSegCount(GetLenght(), 1260);
	int n_1 = GenNonstandardUnitCount(GetLenght(), 1260, N_1);
	double k1 = GenK(GetLenght(), 1260, N_1, n_1);

	//计算1380标准段的数据
	int N_2 = GenStandardSegCount(GetLenght(), 1380);
	int n_2 = GenNonstandardUnitCount(GetLenght(), 1380, N_2);
	double k2 = GenK(GetLenght(), 1380, N_2, n_2);

	//对k(栏杆侧边留空间隙)进行判断，如果大于105，那么标准栏杆尺寸就为1380，否则就为1260
	if (k1 <= 105)
	{
		m_B = 1260;
		m_N = N_1;
		m_n = n_1;
		m_K = k1;
	}
	else
	{
		m_B = 1380;
		m_N = N_2;
		m_n = n_2;
		m_K = k2;
	}
	return true;
}

int CRCRailingT1::GenStandardSegCount(double p_lenth, double p_segLength)const		//计算标准栏杆数量，p_lenth为栏杆长，p_segLength为栏杆的标准段长
{
	int nCount = 0;
	if ((int)(p_lenth / p_segLength == 1))
	{
		nCount = 1;
	}
	else if ((int)(p_lenth / p_segLength) - 1 < 0)
	{
		nCount = 0;
	}
	else
	{
		nCount = (int)(p_lenth / p_segLength - 1);
	}

	return nCount;
}

//获取标准栏杆两侧单元花格数量
int CRCRailingT1::GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)	const
{
	//标准栏杆两侧单元花格数量 = 【栏杆总尺寸 - 标准栏杆尺寸 - H(其他空间尺寸)】/ 单元花格尺寸
	int n = (int)((GetLenght() - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - GetH()) / Getb());

	//花格数量要是2的倍数，如果不是，向下取最接近的2的倍数 (即-1)
	if ((((int)(n)) % 2) != 0)
	{
		n--;
	}

	return n;
}

double CRCRailingT1::GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const
{
	//通过标准栏杆尺寸和数量计算出标准栏杆总尺寸,公式：标准栏杆总尺寸 = 标准栏杆尺寸*标准栏杆数量- 40*(栏杆数量-1)
	return p_segLength *p_standardSegCount - GetSmallPillarWidth() * (p_standardSegCount - 1);
}

double CRCRailingT1::GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount) const//侧边留空间隙 = 侧面立柱和结构墙间尺寸/2
{
	double nonStandardLen = p_nonStandardUnitCount * Getb(); //非标准段的单元花格总长度
	double k = (p_lenth - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - nonStandardLen - GetH()) / 2;
	return k;
}

//////////////////////////////////////////////////////////////////////////

double CRCRailingT1::GetStandardRailingTotalLen()const
{
	return GenStandardRailingTotalLen(m_B, m_N);
}
double CRCRailingT1::GetNonstandardLen()const   //获取非标段栏杆长度，含两侧立柱
{
	//花格总尺寸 = 花格数量/2*花格尺寸   /2是只计算一侧的长度
	double nonStandardLen = (m_n/2) * Getb() + (Getb()-GetSmallPillarWidth()) + GetPillarWidth() * 2; //加上一个花格空隙和两侧立柱
	return nonStandardLen;
}


//////////////////////////////////////////////////////////////////////////
CRCRailingBoli::CRCRailingBoli()
{
}

CRCRailingBoli::~CRCRailingBoli()
{
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

