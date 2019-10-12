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

AcDbObjectId CRCRailingBoli::CreateRailingBlockDefine(CString sRailingDefName)
{
	AcGePoint3d start = AcGePoint3d::kOrigin;
	//2 插入到图形
	acDocManager->lockDocument(curDoc());

	const AcGePoint3d leftTopPt = AcGePoint3d(start.x, start.y + m_railingAtt.m_height, 0); //栏杆整体的左上角点
	const double railH = m_railingAtt.m_height - GetHandRailHeight();//扣除扶手的高度
	const double centerY = leftTopPt.y - GetHandRailHeight() - railH / 2;


	AcDbObjectIdArray idsOut;
	//2.1 左侧段
	AcGePoint3d pos1 = AcGePoint3d(leftTopPt.x + GetK(), centerY, 0); //左上角点x方向上减去与结构墙间隙，y方向上减去扶手的厚度,然后考虑居中位置


	//2.2 标准段
	AcDbObjectId id2;
	AcDbObjectIdArray ids = GenerateRailing_Standard(pos1);
	idsOut.append(ids);


	//2.4 扶手
	AcDbObjectId id4 = GenerateRailing_HandRail(leftTopPt);
	idsOut.append(id4);

	//////////////////////////////////////////////////////////////////////////
	//3 组合为一个块
	AcDbObjectId blkDefId;
	int nRet = MD2010_CreateBlockDefine_ExistEnts(sRailingDefName, idsOut, start, blkDefId);

	//删除原来的块
	JHCOM_DeleteCadObjectArray(idsOut);

	//4 将栏杆属性赋值给此栏杆实例
	AttrRailing* pAttRailing = new AttrRailing(m_railingAtt);
	pAttRailing->SetInstanceCode(sRailingDefName);
	TY_AddAttributeData(blkDefId, pAttRailing);

	acDocManager->unlockDocument(curDoc());

	return blkDefId;
}
CString CRCRailingBoli::GetStandardBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Standard");
}

CString CRCRailingBoli::GetLeftBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Left");
}
CString CRCRailingBoli::GetRightBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Right");
}
CString CRCRailingBoli::GetHandRailBlockName() const
{
	return m_railingAtt.m_prototypeCode + _T("_Handrail");
}

AcDbObjectIdArray CRCRailingBoli::GenerateRailing_Standard(AcGePoint3d pos)
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
		DQ_SetDynamicAttribute(id2, _T("L"), GetB());

		if (i==0 && (i==GetN()-1))
		{
			DQ_SetDynamicAttribute(id2, _T("可见性"), _T("左右"));
		}
		else if (i == 0)
		{
			DQ_SetDynamicAttribute(id2, _T("可见性"), _T("左侧"));
		}
		else if ((i == GetN() - 1))
		{
			DQ_SetDynamicAttribute(id2, _T("可见性"), _T("右侧"));
		}
		else
		{
			DQ_SetDynamicAttribute(id2, _T("可见性"), _T("中心"));
		}


		idsOut.append(id2);

		pos.x += GetB() + GetH(); 
	}

	return idsOut;
}

AcDbObjectId CRCRailingBoli::GenerateRailing_HandRail(AcGePoint3d pos)
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