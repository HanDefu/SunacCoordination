#include "StdAfx.h"
#include "BathroomGen.h"
#include "RCBathroom.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_DynamicBlock.h"
#include "..\..\Common/ComFun_Sunac.h"


CBathroomGen::CBathroomGen(AttrBathroom* p_att)
: m_attr(*p_att)
{
	m_id = AcDbObjectId::kNull;

	m_doorDir = E_DIR_BOTTOM;
	m_windowDir = E_DIR_TOP;
}

CBathroomGen::~CBathroomGen()
{

}

int CBathroomGen::SelectTaipen(AcDbObjectId bathroomId, CString taipen)
{
	vCString hideBlockRecordNames;
	if (taipen == L"Sunac_台盆_650")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taipen == L"Sunac_台盆_750")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taipen == L"Sunac_台盆_800")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taipen == L"Sunac_台盆_900")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taipen == L"Sunac_台盆_1000")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);

	return 0;
}

int CBathroomGen::SelectMatong(AcDbObjectId bathroomId, CString matong)
{
	vCString hideBlockRecordNames;
	if (matong == L"Sunac_马桶800")
	{
		hideBlockRecordNames.push_back(L"Sunac_马桶750");
	}
	else if (matong == L"Sunac_马桶750")
	{
		hideBlockRecordNames.push_back(L"Sunac_马桶800");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);

	return 0;
}

int CBathroomGen::SelectGuanxiWidth(AcDbObjectId bathroomId, double width)
{
	bool isG = (m_attr.m_fileName.Right(6) == _T("_g.dwg"));
	//只有干湿分离的卫生间才有盥洗区
	if (isG)
	{
		if (fabs(width - 950) > TOL && fabs(width - 1050) > TOL )
			return -1;
		TYCOM_SetDynamicBlockValue(bathroomId, L"盥洗区Y", width);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
vCString CBathroomGen::GetTaipenOptions()
{
	return WebIO::GetConfigDict()->Bathroom_GetTaiPenWidths();
}
CString CBathroomGen::GetTaipenDefault()
{ 
	return L"650";
}

vCString CBathroomGen::GetMatongOptions()
{
	return WebIO::GetConfigDict()->Bathroom_GetMaTongTypes();
}
CString CBathroomGen::GetMatongDefault()
{
	return _T("750");
}

vCString CBathroomGen::GetGuanxiquOptions()
{
	//只有干湿分离的卫生间才有盥洗区
	bool isG = (m_attr.m_fileName.Right(6) == _T("_g.dwg"));
	if (!isG)
		return vCString(0);
	return WebIO::GetConfigDict()->Bathroom_GetGuanXiWidths();

}
CString CBathroomGen::GetGuanxiquDefault()
{
	return _T("950");
}
void CBathroomGen::GetRotateAngle(double &angle, AcGeVector3d &offsetX) //处理旋转及镜像关系
{
	angle = 0;
	offsetX = AcGeVector3d(0, 0, 0);
	const double xLen = GetXLength();
	const double yLen = GetYLength();

	switch (m_doorDir)
	{
	case E_DIR_TOP:
		angle = PI;
		offsetX = AcGeVector3d(xLen, yLen, 0);
		break;
	case E_DIR_LEFT:
		angle = -PI / 2;
		offsetX = AcGeVector3d(0, xLen, 0);
		break;
	case E_DIR_BOTTOM:
		angle = 0;
		offsetX = AcGeVector3d(0, 0, 0);
		break;
	case E_DIR_RIGHT:
		angle = PI / 2;
		offsetX = AcGeVector3d(yLen, 0, 0);
		break;
	default:
		break;
	}
}

AcDbObjectId CBathroomGen::GenBathroom(const AcGePoint3d p_pos)
{
	double angle = 0;
	AcGeVector3d offsetXY = AcGeVector3d(0, 0, 0);
	GetRotateAngle(angle, offsetXY);

	RCBathroom oneBathroom;

	//先插入到原点，最后再做镜像和旋转处理
	AcDbObjectId id = oneBathroom.Insert(m_attr.m_fileName, p_pos, 0, L"0", 256);
	oneBathroom.InitParameters();
	oneBathroom.SetParameter(L"进深", m_attr.m_height);
	oneBathroom.SetParameter(L"开间", m_attr.m_width);
	//////////////////////////////////////////////////////////////////////////
	//烟道
	if (m_attr.m_hasPaiQiDao)
	{
		double airVentW = 0;
		double airVentH = 0;
		if (m_attr.m_isGuoBiao) //国标
		{
			airVentW = m_attr.m_airVentOffsetX + c_airVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_airVentSize[m_attr.m_floorRange];
		}
		else
		{
			airVentW = m_attr.m_airVentW;
			airVentH = m_attr.m_airVentH;
		}
		assert(airVentW > 0 && airVentH > 0);
		oneBathroom.SetParameter(L"排气道X尺寸", airVentW);
		oneBathroom.SetParameter(L"排气道Y尺寸", airVentH);
	}
	//////////////////////////////////////////////////////////////////////////
	oneBathroom.RunParameters();

	SelectTaipen(id, m_attr.m_taipenWidth);
	SelectMatong(id, m_attr.m_matongWidth);
	SelectGuanxiWidth(id, m_attr.m_guanXiWidth);

	SetMatongPos(id, GetYLength());

	//////////////////////////////////////////////////////////////////////////
	//先镜像处理
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + GetXLength() / 2, 0, 0);
		TYCOM_Mirror(id, basePt, AcGeVector3d(0, 1, 0));
	}

	//再角度旋转
	if (angle!=0)
	{
		TYCOM_Rotate(id, p_pos, angle);

		//旋转后定义点不再是左下角，需要平移
		TYCOM_Move(id, offsetXY);
	}


	//////////////////////////////////////////////////////////////////////////
	//把UI的数据记录在图框的扩展字典中
	AttrBathroom *pAttribute = new AttrBathroom(m_attr);
	oneBathroom.AddAttribute(pAttribute);
	pAttribute->close();

	m_id = id;
	return id;
}

int CBathroomGenKI::SetMatongPos(AcDbObjectId bathroomId, double yLen)
{
	CString type = m_attr.m_BathroomType;
	if (type == _T("I3"))
		return SetMatongPos_I3(bathroomId, yLen);
	else
		return SetMatongPos_I4(bathroomId, yLen);
}

int CBathroomGenKI::SetMatongPos_I3(AcDbObjectId bathroomId, double yLen)
{
	if (yLen > 2600)
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 400.0);

	return 0;
}

int CBathroomGenKI::SetMatongPos_I4(AcDbObjectId bathroomId, double yLen)
{
	if (yLen > 3200)
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 400.0);

	return 0;
}

CBathroomMrg* CBathroomMrg::GetInstance()
{
	static CBathroomMrg instance;
	return &instance;
}

vector<AttrBathroom*> CBathroomMrg::FilterBathroom(EBathroomType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	vector<AttrBathroom*> allBathroomOut;

	if (p_type == E_BATHROOM_U)
		return FilterBathroomTU(p_width, p_height, p_doorDir, p_windowDir, p_bHasAirVent);
	if (p_type == E_BATHROOM_L)
		return FilterBathroomTL(p_width, p_height, p_doorDir, p_windowDir, p_bHasAirVent);
	if (p_type == E_BATHROOM_I)
		return FilterBathroomTI(p_width, p_height, p_doorDir, p_windowDir, p_bHasAirVent);

	return allBathroomOut;
}

CBathroomGen* CBathroomMrg::CreateBathroomGenByBathroomType(AttrBathroom* p_attBathroom)
{
	if (p_attBathroom == NULL)
	{
		assert(false);
		return NULL;
	}

	//确保卫生间在传入前设置了尺寸
	assert(p_attBathroom->m_width > 0 && p_attBathroom->m_height > 0);


	//或者通过门窗编号创建对应的类
	CBathroomGen* pBathroomGen = NULL;

	if (p_attBathroom->m_isDynamic)
	{
		if (p_attBathroom->m_BathroomType.Left(1) == _T("U"))
		{
			pBathroomGen = new CBathroomGenKU(p_attBathroom);
		}
		else if (p_attBathroom->m_BathroomType.Left(1) == _T("L"))
		{
			pBathroomGen = new CBathroomGenKL(p_attBathroom);
		}
		else if (p_attBathroom->m_BathroomType.Left(1) == _T("I"))
		{
			pBathroomGen = new CBathroomGenKI(p_attBathroom);
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		//pBathroomGen = new CKitchGenSTATIC(p_attBathroom);
	}


	return pBathroomGen;
}

vector<AttrBathroom*> CBathroomMrg::FilterBathroomTU(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	vector<AttrBathroom*> attrBathroom;

	bool bOpposite = (abs(p_doorDir - p_windowDir) == 2);
	bool bLeftRight = (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT);

	int width = int(p_width);
	int height = int(p_height);

	//U型尺寸固定且不重复
	if (width == 1850 && height == 2000 && bOpposite && bLeftRight)
		return WebIO::GetInstance()->GetBathrooms(width, height, _T("对开"), _T("U"), p_bHasAirVent);
	if (width == 1600 && height == 2450 && !bOpposite && bLeftRight)
		return WebIO::GetInstance()->GetBathrooms(width, height, _T("垂直开"), _T("U"), p_bHasAirVent);
	if (width == 1850 && height == 2750 && !bOpposite && bLeftRight)
		return WebIO::GetInstance()->GetBathrooms(width, height, _T("垂直开"), _T("U"), p_bHasAirVent);
	if (width == 2000 && height == 2750 && !bOpposite && bLeftRight)
		return WebIO::GetInstance()->GetBathrooms(width, height, _T("垂直开"), _T("U"), p_bHasAirVent);

	return attrBathroom;
}

vector<AttrBathroom*> CBathroomMrg::FilterBathroomTL(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	vector<AttrBathroom*> attrBathroom;

	bool bOpposite = (abs(p_doorDir - p_windowDir) == 2);
	bool bLeftRight = (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT);

	int width = int(p_width + 0.5);
	int height = int(p_height + 0.5);

	if (width < 1700 || width > 2150)
		return attrBathroom;
	if (height < 1850 || height > 2750)
		return attrBathroom;
	if ((width - 1700) % 150 != 0)
		return attrBathroom;
	if ((height - 1850) % 150 != 0)
		return attrBathroom;
	//不支持以下尺寸
	if (width == 1700 && height == 1850)
		return attrBathroom;
	if (width == 1700 && height == 2000)
		return attrBathroom;
	if (width == 2000 && height == 2600)
		return attrBathroom;
	if (width == 2000 && height == 2750)
		return attrBathroom;
	if (width == 2150 && height == 2600)
		return attrBathroom;
	if (width == 2150 && height == 2750)
		return attrBathroom;

	//L型均为上下垂直开
	if (bOpposite || bLeftRight)
		return attrBathroom;

	return WebIO::GetInstance()->GetBathrooms(width, height, _T("垂直开"), _T("L"), p_bHasAirVent);
}

vector<AttrBathroom*> CBathroomMrg::FilterBathroomTI(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	vector<AttrBathroom*> attrBathroom;

	bool bOpposite = (abs(p_doorDir - p_windowDir) == 2);
	bool bLeftRight = (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT);

	int width = int(p_width + 0.5);
	int height = int(p_height + 0.5);

	if (width != 1600 && width != 1700 && width != 1850)
		return attrBathroom;
	if (height < 2450 || height > 3650)
		return attrBathroom;
	if ((height - 2450) % 150 != 0)
		return attrBathroom;

	//I型均为上下对开
	if (!bOpposite || bLeftRight)
		return attrBathroom;

	return WebIO::GetInstance()->GetBathrooms(width, height, _T("对开"), _T("I"), p_bHasAirVent);
}
