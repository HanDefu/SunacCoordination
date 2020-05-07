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
#include "../../Common/ComFun_Layer.h"
#include "../../Common/ComFun_Math.h"
#include "../../Tool/DocLock.h"
#include "RCRailingTieyi.h"
#include "RCRailingBoli.h"
#include "../../Common/ComFun_Interactive.h"
 
CRCRailing::CRCRailing(void)
{
	m_B = 1260;			//标准栏杆尺寸 1260或者1380
	m_N = 1;			//标准栏杆数量 
	m_K = 0;

	m_bSimple = true;
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
bool CRCRailing::CheckLength()
{
	return (m_railingAtt.m_length >= GetMinWidth());
}
bool CRCRailing::CheckHeight()
{
	return (m_railingAtt.m_height >= GetMinHeight());
}
//start 为栏杆的左下角
int CRCRailing::GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut)
{
	if (CheckLength()==false || CheckHeight()==false)
	{
		return -1;
	}

	//1. 计算各分段的值
	bool bSuc = GenRailing();
	if (bSuc == false)
		return -1;

	CDocLock docLock; //锁定文档用，勿删

	CString sRailingDefName = m_railingAtt.GetInstanceCode();
	if (sRailingDefName.IsEmpty())
	{
		assert(false);
		sRailingDefName.Format(_T("%s_%d_%d"), m_railingAtt.m_prototypeCode, (int)(m_railingAtt.m_length), (int)(m_railingAtt.m_height));
	}

	CString oldLayerName;
	MD2010_GetCurrentLayer(oldLayerName);
	CString sRalingLayerName = _T("Sunac_Railing");

	if (!IsSampleDraw())
	{
		sRalingLayerName = _T("Sunac_Railing_Detail");
		sRailingDefName.Format(_T("%s_%d_%d_%s"), m_railingAtt.m_prototypeCode, (int)(m_railingAtt.m_length), (int)(m_railingAtt.m_height), _T("Detail"));
	}
	
	if (JHCOM_GetLayerID(sRalingLayerName)==AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sRalingLayerName);
	}
	MD2010_SetCurrentLayer(sRalingLayerName);

	//若之前有，则直接插入
	AcDbObjectId railingBlockDef = MD2010_GetBlockDefID(sRailingDefName);
	if (railingBlockDef == AcDbObjectId::kNull)
	{
		AcDbObjectId blkDefId = CreateRailingBlockDefine(sRailingDefName);
	}

	MD2010_InsertBlockReference_ModelSpace(sRailingDefName, p_railingIdOut, start);
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	TY_AddAttributeData(p_railingIdOut, pAttRailing);
	pAttRailing->close();
	MD2010_SetCurrentLayer(oldLayerName);

	//针对USC坐标处理
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(p_railingIdOut, mat);
	}

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

void CRCRailing::CreateRailingTop(AcGePoint3d p_pnt1, AcGePoint3d p_pnt2)
{
	CDocLock lockEnt;

	CString sRailingDefName = m_railingAtt.GetInstanceCode();

	int nRailingTopLength = 0;
	bool bSul = CheckRailingStartEndPt(p_pnt1, p_pnt2, nRailingTopLength);
	if (bSul == false)
		return;

	sRailingDefName.Format(_T("%s_%d_%d_P"), m_railingAtt.m_prototypeCode, nRailingTopLength, (int)(m_railingAtt.m_height));

	CString oldLayerName;
	MD2010_GetCurrentLayer(oldLayerName);
	CString sRalingLayerName = _T("Sunac_Railing");

	if (JHCOM_GetLayerID(sRalingLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sRalingLayerName);
	}
	MD2010_SetCurrentLayer(sRalingLayerName);

	AcDbObjectId railingTopId = GenerateRailingTop(sRailingDefName, p_pnt1, p_pnt2);

	MD2010_SetCurrentLayer(oldLayerName);

	// 将多段线添加到模型空间
	MD2010_InsertBlockReference_ModelSpace(sRailingDefName, railingTopId, p_pnt1);
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	TY_AddAttributeData(railingTopId, pAttRailing);
	pAttRailing->close();
}

AcDbObjectId CRCRailing::GenerateRailingTop(CString p_blockName, AcGePoint3d p_pnt1, AcGePoint3d p_pnt2)
{
	// 获得当前图形数据库的块表
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);

	// 创建新的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = new AcDbBlockTableRecord();

	if (pBlkTbl->has(p_blockName))
	{
		pBlkTblRcd->setName(p_blockName + L"_1");
	}
	else
	{
		pBlkTblRcd->setName(p_blockName);
	}

	// 将块表记录添加到块表中
	AcDbObjectId blkDefId;
	pBlkTbl->add(blkDefId, pBlkTblRcd);
	pBlkTbl->close();

	AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;

	const double halfWidth = 40; //栏杆半厚度
	
	if (p_pnt1.y == p_pnt2.y)
	{
		ptLeftBottom.set(p_pnt1.x, p_pnt1.y - halfWidth);
		ptRightBottom.set(p_pnt2.x, p_pnt2.y - halfWidth);
		ptRightTop.set(p_pnt2.x, p_pnt2.y + halfWidth);
		ptLeftTop.set(p_pnt1.x, p_pnt1.y + halfWidth);
	}
	else if (p_pnt1.x == p_pnt2.x)
	{
		ptLeftBottom.set(p_pnt1.x - halfWidth, p_pnt1.y);
		ptRightBottom.set(p_pnt1.x + halfWidth, p_pnt1.y);
		ptRightTop.set(p_pnt2.x + halfWidth, p_pnt2.y);
		ptLeftTop.set(p_pnt2.x - halfWidth, p_pnt2.y);
	}
	else
		return false;

	// 创建对应的多段线
	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(1, ptRightBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(2, ptRightTop, 0, 0.1, 0.1);
	pPoly->addVertexAt(3, ptLeftTop, 0, 0.1, 0.1);
	pPoly->setClosed(Adesk::kTrue);

	pPoly->setColorIndex(1);

	AcDbObjectId entId;
	pBlkTblRcd->appendAcDbEntity(entId, pPoly);
	pBlkTblRcd->setOrigin(p_pnt1);

	pPoly->close();
	pBlkTblRcd->close();

	return entId;
}

bool CRCRailing::CheckRailingStartEndPt(AcGePoint3d& p_pnt1, AcGePoint3d& p_pnt2, int& p_length)
{
	int nError = 20; //选择两个点的间距误差
	bool bSul = true;

	if (fabs(p_pnt2.x - p_pnt1.x) > nError && fabs(p_pnt2.y - p_pnt1.y) > nError)
	{
		AfxMessageBox(_T("选择的两个点尽量保持在同一水平线或垂直线上\n"));
		bSul = false;
	}
	else if (fabs(p_pnt2.y - p_pnt1.y) <= nError && fabs(p_pnt2.x - p_pnt1.x) > nError)
	{
		p_pnt2.y = p_pnt1.y;
		p_length = (int)fabs(p_pnt2.x - p_pnt1.x);
		CheckRailingLength(p_length);
		if (p_pnt2.x - p_pnt1.x > 0)
			p_pnt2.x = p_pnt1.x + p_length;
		else
			p_pnt2.x = p_pnt1.x - p_length;
	}
	else if (fabs(p_pnt2.x - p_pnt1.x) <= nError && fabs(p_pnt2.y - p_pnt1.y) > nError)
	{
		p_pnt2.x = p_pnt1.x;
		p_length = (int)fabs(p_pnt2.y - p_pnt1.y);
		CheckRailingLength(p_length);
		if (p_pnt2.y - p_pnt1.y > 0)
			p_pnt2.y = p_pnt1.y + p_length;
		else
			p_pnt2.y = p_pnt1.y - p_length;
	}
	else
	{
		AfxMessageBox(_T("选择的两个点间距过小，请重新选择\n"));
		bSul = false;
	}

	return bSul;
}

void CRCRailing::CheckRailingLength(int& p_length)
{
	const int nAlignSize = 50;
	int nRemainder = p_length % nAlignSize;
	if (nRemainder % nAlignSize < (nAlignSize/2))
	{
		p_length -= nRemainder;
	}
	else
	{
		p_length += nAlignSize - nRemainder;
	}
}

//////////////////////////////////////////////////////////////////////////
//栏杆详图
void CRCRailing::DrawRailingDetail()
{
	CDocLock lockEnt;

	//第一步：选择需要统计的栏杆
	vAcDbObjectId RailingIds;
	JHCOM_SelectEnts(RailingIds);
	if (RailingIds.size() == 0)
		return;

	//第二步  选择栏杆详图插入点
	AcGePoint3d pnt;
	bool bSuc = TY_GetPoint(pnt, L"请选择栏杆详图插入点");
	if (bSuc == false)
		return;

	//第三步：读取栏杆数据并且分类汇总
	CRailingCountArray railCountArray;
	railCountArray.InitByRailingIds(RailingIds);

	////////////////////////////////////////////////////////////////////////////
	AcDbObjectIdArray hatchIds;

	for (int i = 0; i < railCountArray.GetCount(); i++)
	{
		const AttrRailing& railAtt = railCountArray.GetRailing(i).railAtt;
		CRCRailing* pRailing = CreateRailing(railAtt);
		pRailing->SetSimpleDraw(false);
		pRailing->GenerateRailing(pnt, RailingIds[i]);
		pnt.set(pnt.x, pnt.y+ 2200, 0);

		DrawRailingWhiteWall(railAtt, RailingIds[i]);

		AcDbObjectId concreteId = DrawRailingFill(railAtt, RailingIds[i]);//绘制外围墙壁多段线

		hatchIds.append(concreteId);
		CreateHatch(hatchIds, true);//创建填充
		hatchIds.removeAll();

		DrawRailingYellowWall(railAtt, RailingIds[i]);

		CreateRailingDetailDim(railAtt, RailingIds[i], pRailing);
	}

	return;
}

void CRCRailing::CreateRailingDetailDim(const AttrRailing& railAtt, AcDbObjectId m_id, CRCRailing* pRailing)
{
	CDocLock lockEnt;

	if (m_id == 0)
		return;

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sRailingLayerName = L"Sunac_dim";
	if (JHCOM_GetLayerID(sRailingLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sRailingLayerName, 7);
	}

	MD2010_SetCurrentLayer(sRailingLayerName);

	const double railingTotalLength = railAtt.m_length;
	const double railingTotalHeight = railAtt.m_height;

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + railingTotalLength;
	rect.m_rt.y = rect.m_lb.y + railingTotalHeight;
	rect.m_rt.z = 0;

	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();
	const AcGePoint3d  leftTopPt = rect.GetLT();
	const AcGePoint3d  leftBottomPt = rect.GetLB();

	//上下标注间距
	int offset = 250;
	double nDimLength = 0;

	AcGePoint3d start = leftTopPt;
	AcGePoint3d end = leftTopPt;
	AcGePoint3d mid = leftTopPt;

	//先标注横排
	//标注左侧白色多段线到墙壁距离
	double nWhiteToWall = 50;
	end.x += nWhiteToWall;
	mid.y += offset;
	CreateDimension(start, end, mid, nWhiteToWall);

	//标注白色多段线到立柱左侧距离
	start.set(start.x + nWhiteToWall, start.y - 700, start.z);
	mid.y -= 700;
	end = start;
	if (railAtt.m_railingType == E_RAILING_TIEYI)
		nDimLength = pRailing->GetK() - nWhiteToWall;
	else
		nDimLength = pRailing->GetK() - nWhiteToWall - pRailing->GetPillarWidth() / 2;
	end.x += nDimLength;
	CreateDimension(start, end, mid, fabs(nDimLength));

	//标注白色多段线到立柱中心距离
	start.y += 700;
	mid.y += 700 + offset;
	end = start;
	if (railAtt.m_railingType == E_RAILING_TIEYI)
		nDimLength = pRailing->GetK() - nWhiteToWall + pRailing->GetPillarWidth() / 2;
	else
		nDimLength = pRailing->GetK() - nWhiteToWall;//玻璃栏杆k加上了立柱宽
	end.x += nDimLength; //立柱宽
	CreateDimension(start, end, mid, fabs(nDimLength));
	
	//非标段长度
	if (railAtt.m_railingType == E_RAILING_TIEYI)
	{
		start = end;
		mid = start;
		mid.y += offset;
		nDimLength = pRailing->GetNonstandardLen() - pRailing->GetPillarWidth();
		end.x += nDimLength;
		CreateDimension(start, end, mid, nDimLength);
	}

	//标准段长度
	for (int i = 0; i < pRailing->GetN(); i++)
	{
		start = end;
		mid = start;
		mid.y += offset;
		if (railAtt.m_railingType == E_RAILING_TIEYI)
		{
			nDimLength = pRailing->GetB() - pRailing->GetPillarWidth();
		}
		else
		{
			nDimLength = pRailing->GetB();
			if (railAtt.m_prototypeCode.Find(_T("B1")) >= 0 && i > 0)
			{
				start.x += 150;
				end = start;
			}
		}
		end.x += nDimLength;
		CreateDimension(start, end, mid, nDimLength);
	}

	//右侧非标段
	if (railAtt.m_railingType == E_RAILING_TIEYI)
	{
		start = end;
		mid = start;
		mid.y += offset;
		nDimLength = pRailing->GetNonstandardLen() - pRailing->GetPillarWidth();
		end.x += nDimLength;
		CreateDimension(start, end, mid, nDimLength);
	}

	//标注右侧白色多段线到墙壁距离
	start = rightTopPt;
	mid = start;
	end = start;
	mid.y += offset;
	end.x -= nWhiteToWall;
	CreateDimension(start, end, mid, nWhiteToWall);

	//标注白色多段线到立柱右侧距离
	start.set(start.x - nWhiteToWall, start.y - 700, start.z);
	mid.y -= 700;
	end = start;
	if (railAtt.m_railingType == E_RAILING_TIEYI)
		nDimLength = pRailing->GetK() - nWhiteToWall;
	else
		nDimLength = pRailing->GetK() - nWhiteToWall - pRailing->GetPillarWidth() / 2;
	end.x -= nDimLength;
	CreateDimension(start, end, mid, fabs(nDimLength));

	//标注白色多段线到立柱中心距离
	start.y += 700;
	mid.y += 700 + offset;
	end = start;
	if (railAtt.m_railingType == E_RAILING_TIEYI)
		nDimLength = pRailing->GetK() - nWhiteToWall + pRailing->GetPillarWidth() / 2;
	else
		nDimLength = pRailing->GetK() - nWhiteToWall;//玻璃栏杆k加上了立柱宽
	end.x -= nDimLength; //立柱宽
	CreateDimension(start, end, mid, fabs(nDimLength));

	//标注栏杆总长度
	start = leftTopPt;
	mid = start;
	mid.y += offset * 2;
	end = rightTopPt;
	nDimLength = railingTotalLength;
	CreateDimension(start, end, mid, nDimLength);

	//标注竖排
	//标注墙壁尺寸
	start.set(rightBottomPt.x, rightBottomPt.y - 100, rightBottomPt.z);
	mid = start;
	mid.x += offset * 2 + 100;
	end = start;
	nDimLength = 100;
	end.y += nDimLength;
	CreateDimension(start, end, mid, nDimLength);

	//标注竖排白色多段线到墙壁距离
	start = end;
	mid = start;
	mid.x += offset + 100;
	end.y += nWhiteToWall;
	CreateDimension(start, end, mid, nWhiteToWall);

	//标注总高度
	start = rightBottomPt;
	mid = start;
	mid.x += offset * 2 + 100;
	end = rightTopPt;
	nDimLength = railingTotalHeight;
	CreateDimension(start, end, mid, nDimLength);

	//创建直线，标注F.L
	start.set(rightBottomPt.x + offset * 2 + 100, rightBottomPt.y - 100, rightBottomPt.z);
	end = start;
	end.x += 350;
	JHCOM_CreateLine(start, end);
	
	start.x += 100;
	start.y += 30;
	/*JHCOM_CreateText(start,
		AcGeVector3d(0, 0, 1),
		150, 0,
		L"F.L");*/
	AcDbText *pText = new AcDbText(start, L"F.L", AcDbObjectId::kNull, 150, 0);
	pText->setNormal(AcGeVector3d(0, 0, 1));
	pText->setPosition(start);
	AcDbObjectId textId = MD2010_GetTextStylerID(L"_TCH_DIM_T3");
	pText->setTextStyle(textId);
	pText->setWidthFactor(0.5647);
	pText->setColorIndex(7);
	JHCOM_PostToModelSpace(pText);
	pText->close();

	MD2010_SetCurrentLayer(oldLayerName);
}

AcDbObjectId CRCRailing::DrawRailingWhiteWall(const AttrRailing& railAtt, AcDbObjectId m_id)
{
	CDocLock lockEnt;

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sWindowDoorLayerName = L"Sunac_wall";
	if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sWindowDoorLayerName, 7);
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	const double railingTotalLength = railAtt.m_length;
	const double railingTotalHeight = railAtt.m_height;

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + railingTotalLength;
	rect.m_rt.y = rect.m_lb.y + railingTotalHeight;
	rect.m_rt.z = 0;

	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();
	const AcGePoint3d  leftTopPt = rect.GetLT();
	const AcGePoint3d  leftBottomPt = rect.GetLB();

	AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;

	// 创建对应的多段线
	ptLeftBottom.set(leftBottomPt.x + 50, leftBottomPt.y + 50);
	ptRightBottom.set(rightBottomPt.x - 50, rightBottomPt.y + 50);
	ptRightTop.set(rightTopPt.x - 50, rightTopPt.y);
	ptLeftTop.set(leftTopPt.x + 50, leftTopPt.y);

	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftTop, 0, 0.1, 0.1);
	pPoly->addVertexAt(1, ptLeftBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(2, ptRightBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(3, ptRightTop, 0, 0.1, 0.1);
	pPoly->setColorIndex(7);

	// 将多段线添加到模型空间
	AcDbObjectId polyId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly);

	pPoly->close();

	MD2010_SetCurrentLayer(oldLayerName);

	return polyId;
}

AcDbObjectId CRCRailing::DrawRailingYellowWall(const AttrRailing& railAtt, AcDbObjectId m_id)
{
	CDocLock lockEnt;

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sWindowDoorLayerName = L"Sunac_wall";
	if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sWindowDoorLayerName, 2);
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	const double railingTotalLength = railAtt.m_length;
	const double railingTotalHeight = railAtt.m_height;

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + railingTotalLength;
	rect.m_rt.y = rect.m_lb.y + railingTotalHeight;
	rect.m_rt.z = 0;

	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();
	const AcGePoint3d  leftTopPt = rect.GetLT();
	const AcGePoint3d  leftBottomPt = rect.GetLB();

	AcGePoint2d ptLeftBottom, ptRightBottom, ptRightTop, ptLeftTop;

	ptLeftBottom.set(leftBottomPt.x, leftBottomPt.y);
	ptRightBottom.set(rightBottomPt.x, rightBottomPt.y);
	ptRightTop.set(rightTopPt.x, rightTopPt.y);
	ptLeftTop.set(leftTopPt.x, leftTopPt.y);

	// 创建对应的多段线
	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftTop, 0, 0.1, 0.1);
	pPoly->addVertexAt(1, ptLeftBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(2, ptRightBottom, 0, 0.1, 0.1);
	pPoly->addVertexAt(3, ptRightTop, 0, 0.1, 0.1);
	pPoly->setColorIndex(2);

	// 将多段线添加到模型空间
	AcDbObjectId polyId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly);

	pPoly->close();

	MD2010_SetCurrentLayer(oldLayerName);
	return polyId;
}

AcDbObjectId CRCRailing::DrawRailingFill(const AttrRailing& railAtt, AcDbObjectId m_id)
{
	CDocLock lockEnt;

	CString oldLayerName;

	MD2010_GetCurrentLayer(oldLayerName);

	CString sWindowDoorLayerName = L"Sunac_fill";
	if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(sWindowDoorLayerName, 8);
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	const double railingTotalLength = railAtt.m_length;
	const double railingTotalHeight = railAtt.m_height;

	TYRect rect;
	DQ_GetBlockReferenceInsertPoint(m_id, rect.m_lb);
	rect.m_rt.x = rect.m_lb.x + railingTotalLength;
	rect.m_rt.y = rect.m_lb.y + railingTotalHeight;
	rect.m_rt.z = 0;

	const AcGePoint3d  rightBottomPt = rect.GetRB();
	const AcGePoint3d  rightTopPt = rect.GetRT();
	const AcGePoint3d  leftTopPt = rect.GetLT();
	const AcGePoint3d  leftBottomPt = rect.GetLB();

	AcGePoint2d ptLeftBottom1, ptRightBottom1, ptRightTop1, ptLeftTop1, ptLeftBottom2, ptRightBottom2, ptRightTop2, ptLeftTop2;

	ptLeftBottom1.set(leftBottomPt.x, leftBottomPt.y);
	ptRightBottom1.set(rightBottomPt.x, rightBottomPt.y);
	ptRightTop1.set(rightTopPt.x, rightTopPt.y);
	ptLeftTop1.set(leftTopPt.x, leftTopPt.y);
	ptLeftBottom2.set(ptLeftBottom1.x - 100, ptLeftBottom1.y - 100);
	ptRightBottom2.set(ptRightBottom1.x + 100, ptRightBottom1.y - 100);
	ptRightTop2.set(ptRightTop1.x + 100, ptRightTop1.y);
	ptLeftTop2.set(ptLeftTop1.x - 100, ptLeftTop1.y);

	// 创建对应的多段线
	AcDbPolyline *pPoly = new AcDbPolyline(4);
	pPoly->addVertexAt(0, ptLeftTop1, 0, 0.1, 0.1);
	pPoly->addVertexAt(1, ptLeftBottom1, 0, 0.1, 0.1);
	pPoly->addVertexAt(2, ptRightBottom1, 0, 0.1, 0.1);
	pPoly->addVertexAt(3, ptRightTop1, 0, 0.1, 0.1);
	pPoly->addVertexAt(4, ptRightTop2, 0, 0.1, 0.1);
	pPoly->addVertexAt(5, ptRightBottom2, 0, 0.1, 0.1);
	pPoly->addVertexAt(6, ptLeftBottom2, 0, 0.1, 0.);
	pPoly->addVertexAt(7, ptLeftTop2, 0, 0.1, 0.1);
	pPoly->setClosed(Adesk::kTrue);
	pPoly->setColorIndex(252);

	// 将多段线添加到模型空间
	AcDbObjectId polyId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPoly);

	pPoly->close();

	MD2010_SetCurrentLayer(oldLayerName);

	return polyId;
}

AcDbObjectId CRCRailing::CreateHatch(const AcDbObjectIdArray &loopIds, bool bAssociative)
{
	Acad::ErrorStatus es;
	AcDbHatch *pHatch = new AcDbHatch();

	//设置填充平面
	AcGeVector3d normal(0, 0, 1);
	pHatch->setNormal(normal); //设置填充平面的法向矢量
	pHatch->setElevation(0);
	pHatch->setPatternScale(20);//比例
	pHatch->setColorIndex(252);
	pHatch->setPatternSpace(20);

	//设置关联性
	pHatch->setAssociative(true);

	//设置填充图案
	pHatch->setPattern(AcDbHatch::kPreDefined, L"SACNCR");

	//添加填充边界
	es = pHatch->appendLoop(AcDbHatch::kExternal, loopIds);

	//显示填充对象
	es = pHatch->evaluateHatch();

	//添加到模型空间
	AcDbObjectId hatchId = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pHatch);

	//如果是关联性的填充，将填充对象与边界绑定，使其能获得边界对象修改的通知
	if (bAssociative)
	{
		AcDbObjectIdArray assocEntIds;
		pHatch->getAssocObjIds(assocEntIds);
		for (int i = 0; i < assocEntIds.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, assocEntIds[i], AcDb::kForWrite) == Acad::eOk)
			{
				//添加一个永久反应器
				pEnt->addPersistentReactor(hatchId);
				pEnt->close();
			}
		}

		pHatch->close();
	}
	return hatchId;
}


bool m_isSetDimTextStyle = true; //是否设置标注文字样式
AcDbObjectId CRCRailing::CreateDimension(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, double size)
{
	if (m_isSetDimTextStyle == true)
	{
		SetRailingDetailTextStyle(L"_TCH_DIM_T3");
		m_isSetDimTextStyle = false;
	}

	CreateDimensionStyle();
	
	//获取所创建的标注尺寸的id
	AcDbObjectId dimStyleId = GetDimstylerID(L"_TCH_ARCH&&50");

	if (JHCOM_PointDistance(start, end) <= TOL * 10000)//小于1的不标注
		return 0;

	CString entryName;
	entryName.Format(_T("%d"), (int)size);
	AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start, end, dimlinpnt, entryName, dimStyleId);
	pDim->setColorIndex(7);

	AcDbObjectId dimID = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pDim);

	pDim->close();

	return dimID;
}

void CRCRailing::CreateDimensionStyle()
{
	// 获得要创建的标注样式名称
	CString styleName = L"_TCH_ARCH&&50";

	// 获得当前图形的标注样式表
	AcDbDimStyleTable *pDimStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStyleTbl, AcDb::kForWrite);
	if (pDimStyleTbl->has(styleName))
	{
		pDimStyleTbl->close();//已经存在
		return;
	}

	// 创建新的标注样式表记录
	AcDbDimStyleTableRecord *pDimStyleTblRcd = NULL;
	pDimStyleTblRcd = new AcDbDimStyleTableRecord();

	// 设置标注样式的特性
	pDimStyleTblRcd->setName(styleName); // 样式名称
	//pDimStyleTblRcd->setDimscale(50); //全局比例
	pDimStyleTblRcd->setDimasz(37.5); // 箭头大小
	pDimStyleTblRcd->setDimexo(100); //尺寸界线偏移
	pDimStyleTblRcd->setDimdli(30); //尺寸线间距
	pDimStyleTblRcd->setDimexe(50); //超出尺寸线的距离
	pDimStyleTblRcd->setDimzin(0); //消零
	pDimStyleTblRcd->setDimtzin(2); //角度消零
	pDimStyleTblRcd->setDimtad(1); // 文字位于标注线的上方
	pDimStyleTblRcd->setDimtxt(127.5); // 标注文字的高度
	pDimStyleTblRcd->setDimgap(62.5); //设置标注文字周围的距离
	pDimStyleTblRcd->setDimtmove(2);  //设置标注文字的移动规则
	pDimStyleTblRcd->setDimtih(false); //文字方向
	pDimStyleTblRcd->setDimtix(true); //文字方向
	pDimStyleTblRcd->setDimblk(L"_ARCHTICK");//设置箭头的形状为建筑标记,设置尺寸线末尾的阴影部分显示
	
	AcDbObjectId textId = MD2010_GetTextStylerID(L"_TCH_DIM_T3");
	pDimStyleTblRcd->setDimtxsty(textId);

	// 将标注样式表记录添加到标注样式表中
	pDimStyleTbl->add(pDimStyleTblRcd);
	pDimStyleTblRcd->close();
	pDimStyleTbl->close();
}

AcDbObjectId CRCRailing::GetDimstylerID(CString dimname)
{
	AcDbDimStyleTable *pDimStylTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStylTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pDimStylTbl->has(dimname))
	{
		pDimStylTbl->close();
		return 0;
	}

	// 获得指定层表记录的指针
	AcDbDimStyleTableRecord *pDimStylerTblRcd = NULL;
	pDimStylTbl->getAt(dimname, pDimStylerTblRcd, AcDb::kForWrite);

	AcDbObjectId ids = pDimStylerTblRcd->objectId();
	pDimStylerTblRcd->close();
	pDimStylTbl->close();

	return ids;
}

void CRCRailing::SetRailingDetailTextStyle(CString dimname)
{
	AcDbTextStyleTable *pTextStylTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStylTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pTextStylTbl->has(dimname))
	{
		pTextStylTbl->close();
		CreateRailingDetailTextStyle(dimname);
		return;
	}

	// 获得指定层表记录的指针
	AcDbTextStyleTableRecord *pTextStylerTblRcd = NULL;
	pTextStylTbl->getAt(dimname, pTextStylerTblRcd, AcDb::kForWrite);

	pTextStylerTblRcd->setFileName(L"simplex.shx");
	pTextStylerTblRcd->setBigFontFileName(L"gbcbig.shx");
	pTextStylerTblRcd->setXScale(0.5647);

	pTextStylerTblRcd->close();
	pTextStylTbl->close();

	return;
}

void CRCRailing::CreateRailingDetailTextStyle(CString dimname)
{
	// 获得字体样式表
	AcDbTextStyleTable *pTextStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTbl, AcDb::kForWrite);

	// 创建新的字体样式表记录
	AcDbTextStyleTableRecord *pTextStyleTblRcd = NULL;
	pTextStyleTblRcd = new AcDbTextStyleTableRecord();

	// 设置字体样式表记录的名称
	pTextStyleTblRcd->setName(dimname);

	// 设置字体文件名称
	pTextStyleTblRcd->setFileName(L"simplex.shx");
	pTextStyleTblRcd->setBigFontFileName(L"gbcbig.shx");

	// 设置高宽比例
	pTextStyleTblRcd->setXScale(0.5647);

	// 将新的记录添加到字体样式表
	pTextStyleTbl->add(pTextStyleTblRcd);
	pTextStyleTblRcd->close();
	pTextStyleTbl->close();
}



AcDbObjectId CRCRailing::CreateWipeOut()
{
	AcGePoint3dArray pnts; //左侧、右侧、底侧各留20的量以免将墙体线也遮挡
	pnts.append(AcGePoint3d(20, 20, 0));
	pnts.append(AcGePoint3d(m_railingAtt.m_length - 20, 20, 0));
	pnts.append(AcGePoint3d(m_railingAtt.m_length - 20, m_railingAtt.m_height, 0));
	pnts.append(AcGePoint3d(20, m_railingAtt.m_height, 0));
	pnts.append(AcGePoint3d(20, 20, 0));

	return TYCOM_CreateWipeOut(pnts);
}