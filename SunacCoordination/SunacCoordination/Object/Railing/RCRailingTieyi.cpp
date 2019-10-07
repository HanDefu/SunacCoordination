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
#include "RCRailingTieyi.h"
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Sunac.h"

//////////////////////////////////////////////////////////////////////////
CRCRailingTieyi::CRCRailingTieyi()
{
	m_B1 = 0;
	m_B2 = 0;
}

CRCRailingTieyi::~CRCRailingTieyi()
{
}

CString CRCRailingTieyi::GetStandardBlockName() const
{
	CString sName;
	sName.Format(_T("%s_%d"), m_railingAtt.m_prototypeCode, (int)GetB());
	return sName;
}

CString CRCRailingTieyi::GetNonStandardBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_NonStandard");
}
CString CRCRailingTieyi::GetHandRailBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Handrail");
}


//start 为栏杆的左下角
int CRCRailingTieyi::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	//1. 计算各分段的值
	bool bSuc = GenRailing();
	if (bSuc==false)
		return -1;

	//2 插入到图形
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + m_railingAtt.m_height, 0); //栏杆整体的左上角点
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//扣除扶手的高度
	const double centerY = leftTopPt.y - GetHandRailHeight() - railH / 2;
	const CString fileName = GetPrototypeFilePath();
	const CString sStandardBlockName = GetStandardBlockName();
	const CString sNonStandardBlockName = GetNonStandardBlockName();
	const CString sHandrailBlockName = GetHandRailBlockName();
	
	AcDbObjectIdArray idsOut;
	//2.1 非标段
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), centerY, 0); //左上角点x方向上减去与结构墙间隙，y方向上减去扶手的厚度,然后考虑居中位置
	AcDbObjectId id1 = GenerateRailing_NonStandard(pos1);	
	idsOut.append(id1);

	//2.2 标准段
	AcDbObjectId id2;
	AcGePoint3d pos2 = pos1;
	pos2.x = pos1.x + GetNonstandardLen() - GetPillarWidth();
	AcDbObjectIdArray ids =  GenerateRailing_Standard(pos2);
	idsOut.append(ids);
	

	//2.3 非标段
	AcGePoint3d pos3 = pos2;
	pos3.x = pos2.x + GetStandardRailingTotalLen() - GetPillarWidth();
	AcDbObjectId id3 = GenerateRailing_NonStandard(pos3);
	idsOut.append(id3);

	//2.4 扶手
	AcDbObjectId id4 = GenerateRailing_HandRail(leftTopPt);
	idsOut.append(id4);

	//////////////////////////////////////////////////////////////////////////
	//3 组合为一个块，并给把属性设给当前块  TODO

	acDocManager->unlockDocument(curDoc());

	return 0;
}

AcDbObjectId CRCRailingTieyi::GenerateRailing_NonStandard(AcGePoint3d p_pos)
{
	AcDbObjectId id1 = AcDbObjectId::kNull;

	const double railH = m_railingAtt.m_height - GetHandRailHeight();//扣除扶手的高度
	const CString fileName = GetPrototypeFilePath();
	const CString sNonStandardBlockName = GetNonStandardBlockName();

	id1 = InsertBlockRefFromDwg(fileName, sNonStandardBlockName, ACDB_MODEL_SPACE, p_pos);
	assert(id1 != AcDbObjectId::kNull);

	//设置非标段长度
	DQ_SetDynamicAttribute(id1, _T("L"), GetNonstandardLen());
	DQ_SetDynamicAttribute(id1, _T("Ln"), GetNonstandardLen() - GetPillarWidth() * 2);
	DQ_SetDynamicAttribute(id1, _T("H"), railH);

	return id1;
}


AcDbObjectIdArray CRCRailingTieyi::GenerateRailing_Standard(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//扣除扶手的高度
	const CString fileName = GetPrototypeFilePath();
	const CString sStandardBlockName = GetStandardBlockName();

	AcDbObjectIdArray idsOut;
	AcDbObjectId id2;
	for (int i = 0; i < GetN(); i++)
	{
		id2 = InsertBlockRefFromDwg(fileName, sStandardBlockName, ACDB_MODEL_SPACE, pos);
		assert(id2 != AcDbObjectId::kNull);

		DQ_SetDynamicAttribute(id2, _T("H"), railH);

		idsOut.append(id2);

		pos.x += GetB() - GetPillarWidth(); //减去立柱宽
	}

	return idsOut;
}

AcDbObjectId CRCRailingTieyi::GenerateRailing_HandRail(AcGePoint3d pos)
{
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//扣除扶手的高度
	const CString fileName = GetPrototypeFilePath();
	const CString sHandrailBlockName = GetHandRailBlockName();

	AcDbObjectId id4;
	id4 = InsertBlockRefFromDwg(fileName, sHandrailBlockName, ACDB_MODEL_SPACE, pos);
	assert(id4 != AcDbObjectId::kNull);
	//设置扶手长度
	DQ_SetDynamicAttribute(id4, _T("L"), GetLength());
	DQ_SetDynamicAttribute(id4, _T("H"), railH);
	
	return id4;
}


bool CRCRailingTieyi::GenRailing()  //对栏杆总长进行判断，如果栏杆总长小于1550，返回false
{
	if (GetLength() < 1550)
	{
		return false;    //点击计算按钮后，如果judge()返回false,则用户在对话框输入的总尺寸不符合要求，应弹出对话框提示用户输入有误
	}

	//先计算1260标准段的数据
	int N_1 = GenStandardSegCount(GetLength(), B1());
	int n_1 = GenNonstandardUnitCount(GetLength(), B1(), N_1);
	double k1 = GenK(GetLength(), B1(), N_1, n_1);

	//计算1380标准段的数据
	int N_2 = GenStandardSegCount(GetLength(), B2());
	int n_2 = GenNonstandardUnitCount(GetLength(), B2(), N_2);
	double k2 = GenK(GetLength(), B2(), N_2, n_2);

	//对k(栏杆侧边留空间隙)进行判断，如果大于105，那么标准栏杆尺寸就为1380，否则就为1260
	if (k1 <= 105)
	{
		m_B = B1();
		m_N = N_1;
		m_n = n_1;
		m_K = k1;
	}
	else
	{
		m_B = B2();
		m_N = N_2;
		m_n = n_2;
		m_K = k2;
	}
	return true;
}

int CRCRailingTieyi::GenStandardSegCount(double p_lenth, double p_segLength)const		//计算标准栏杆数量，p_lenth为栏杆长，p_segLength为栏杆的标准段长
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
int CRCRailingTieyi::GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)	const
{
	//标准栏杆两侧单元花格数量 = 【栏杆总尺寸 - 标准栏杆尺寸 - H(其他空间尺寸)】/ 单元花格尺寸
	int n = (int)((GetLength() - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - GetH()) / Getb());

	//花格数量要是2的倍数，如果不是，向下取最接近的2的倍数
	if ((n % 2) != 0)
	{
		n--;
	}

	return n/2; //最终的n取一侧的数量,因此除2
}

double CRCRailingTieyi::GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const
{
	//通过标准栏杆尺寸和数量计算出标准栏杆总尺寸,公式：标准栏杆总尺寸 = 标准栏杆尺寸*标准栏杆数量- 40*(栏杆数量-1)
	return p_segLength *p_standardSegCount - GetPillarWidth() * (p_standardSegCount - 1);
}

double CRCRailingTieyi::GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount) const//侧边留空间隙 = 侧面立柱和结构墙间尺寸/2
{
	double nonStandardLen = p_nonStandardUnitCount * Getb(); //非标准段的单元花格总长度
	double k = (p_lenth - GenStandardRailingTotalLen(p_segLength, p_standardSegCount) - nonStandardLen - GetH()) / 2;
	return k;
}


double CRCRailingTieyi::GetStandardRailingTotalLen()const
{
	return GenStandardRailingTotalLen(m_B, m_N);
}
double CRCRailingTieyi::GetNonstandardLen()const   //获取非标段栏杆长度，含两侧立柱
{
	//花格总尺寸 = 花格数量/2*花格尺寸   /2是只计算一侧的长度
	double nonStandardLen = m_n * Getb() + (Getb()-GetSmallPillarWidth()) + GetPillarWidth() * 2; //加上一个花格空隙和两侧立柱
	return nonStandardLen;
}

//////////////////////////////////////////////////////////////////////////

CRCRailingT1::CRCRailingT1()
{
	m_B1 = 1260;
	m_B2 = 1380;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingT3::CRCRailingT3()
{
	m_B1 = 1250;
	m_B2 = 1320;
}


//////////////////////////////////////////////////////////////////////////