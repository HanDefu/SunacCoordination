#include "StdAfx.h"
#include "BathroomGen.h"
#include "RCBathroom.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_DynamicBlock.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "BathroomAutoName.h"


CBathroomGen::CBathroomGen(AttrBathroom* p_att)
: m_attr(*p_att)
{
}

CBathroomGen::~CBathroomGen()
{
}

void CBathroomGen::SelectTaipen(AcDbObjectId bathroomId, CString taipen)
{
	vCString hideBlockRecordNames;

	if (taipen != L"650")
		hideBlockRecordNames.push_back(L"Sunac_台盆650");
	if (taipen != L"750")
		hideBlockRecordNames.push_back(L"Sunac_台盆750");
	if (taipen != L"800")
		hideBlockRecordNames.push_back(L"Sunac_台盆800");
	if (taipen != L"900")
		hideBlockRecordNames.push_back(L"Sunac_台盆900");
	if (taipen != L"1000")
		hideBlockRecordNames.push_back(L"Sunac_台盆1000");

	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);
}

void CBathroomGen::SelectMatong(AcDbObjectId bathroomId, CString matong)
{
	vCString hideBlockRecordNames;
	if (matong != L"750")
		hideBlockRecordNames.push_back(L"Sunac_马桶750");
	if (matong != L"800")
		hideBlockRecordNames.push_back(L"Sunac_马桶800");

	TY_HideBlockReferencesInBlockReference(bathroomId, hideBlockRecordNames);
}

void CBathroomGen::SelectGuanxiWidth(AcDbObjectId bathroomId, double width)
{
	acDocManager->lockDocument(curDoc());

	bool isG = (m_attr.GetFileName().Right(6) == _T("_g.dwg"));
	//只有干湿分离的卫生间才有盥洗区
	if (isG)
		TYCOM_SetDynamicBlockValue(bathroomId, L"盥洗区Y", width);

	acDocManager->unlockDocument(curDoc());
}

//////////////////////////////////////////////////////////////////////////
vCString CBathroomGen::GetTaipenOptions()
{
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetTaiPenWidths();
}
CString CBathroomGen::GetTaipenDefault()
{ 
	return L"650";
}

vCString CBathroomGen::GetMatongOptions()
{
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetMaTongTypes();
}
CString CBathroomGen::GetMatongDefault()
{
	return _T("750");
}

vCString CBathroomGen::GetGuanxiquOptions()
{
	//只有干湿分离的卫生间才有盥洗区
	if (m_attr.m_prototypeCode.Find(L"_g") == -1)
		return vCString(0);
	return WebIO::GetInstance()->GetConfigDict()->Bathroom_GetGuanXiWidths();

}
CString CBathroomGen::GetGuanxiquDefault()
{
	return _T("950");
}

void CBathroomGen::InitBathroomByDefault()
{
	if (GetBathroomAtt()->m_taipenWidth.IsEmpty())
		GetBathroomAtt()->m_taipenWidth = GetTaipenDefault();
	if (GetBathroomAtt()->m_matongWidth.IsEmpty())
		GetBathroomAtt()->m_matongWidth = GetMatongDefault();
	if (m_attr.m_prototypeCode.Find(L"_g") == -1)
		GetBathroomAtt()->m_guanXiWidth = 0;
	else if (GetBathroomAtt()->m_guanXiWidth == 0)
		GetBathroomAtt()->m_guanXiWidth = _ttof(GetGuanxiquDefault());
	GetBathroomAtt()->m_instanceCode = CBathroomAutoName::GetInstance()->GetBathroomName(*GetBathroomAtt());
}

bool CBathroomGen::CheckParameter(CString& errMsg)
{
	//检查台盆是否超出盥洗区
	bool isG = (m_attr.GetFileName().Right(6) == _T("_g.dwg"));
	if (!isG)
		return true;
	int taipenWidth = _ttoi(m_attr.m_taipenWidth);
	int guanXiWidth = int(m_attr.m_guanXiWidth + 0.5);
	if (taipenWidth > guanXiWidth)
	{
		errMsg = L"台盆宽度不能超过盥洗区宽度";
		return false;
	}
	return true;
}

AcDbObjectId CBathroomGen::GenBathroom(const AcGePoint3d p_pos, int p_angle)
{
	AcGeVector3d offsetXY;
	const double xLen = GetXLength();
	const double yLen = GetYLength();
	switch (p_angle)
	{
	case 90:
		offsetXY = AcGeVector3d(yLen, 0, 0);
		break;
	case 180:
		offsetXY = AcGeVector3d(xLen, yLen, 0);
		break;
	case 270:
		offsetXY = AcGeVector3d(0, xLen, 0);
		break;
	default:
		offsetXY = AcGeVector3d(0, 0, 0);
	}

	RCBathroom oneBathroom;

	//先插入到原点，最后再做镜像和旋转处理
	AcDbObjectId id = oneBathroom.Insert(TY_GetPrototypeFilePath() + m_attr.GetFileName(), p_pos, 0, L"0", 256);
	oneBathroom.InitParameters();
	oneBathroom.SetParameter(L"X边长", xLen);
	oneBathroom.SetParameter(L"Y边长", yLen);
	//////////////////////////////////////////////////////////////////////////
	//烟道
	if (m_attr.m_hasPaiQiDao)
	{
		double airVentW = 0;
		double airVentH = 0;
		if (m_attr.m_isGuoBiao) //国标
		{
			airVentW = m_attr.m_airVentOffsetX + c_bathroomAirVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_bathroomAirVentSize[m_attr.m_floorRange];
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

	SetMatongPos(id, yLen);

	//////////////////////////////////////////////////////////////////////////
	//先镜像处理
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + xLen / 2, 0, 0);
		TYCOM_Mirror(id, basePt, AcGeVector3d(0, 1, 0));
	}

	//再角度旋转
	if (p_angle!=0)
	{
		TYCOM_Rotate(id, p_pos, p_angle * PI / 180);

		//旋转后定义点不再是左下角，需要平移
		TYCOM_Move(id, offsetXY);
	}

	//////////////////////////////////////////////////////////////////////////
	//把UI的数据记录在图框的扩展字典中
	AttrBathroom *pAttribute = new AttrBathroom(m_attr);
	oneBathroom.AddAttribute(pAttribute);
	pAttribute->close();

	return id;
}

int CBathroomGenKI::SetMatongPos(AcDbObjectId bathroomId, double yLen)
{
	CString type = m_attr.m_sBathroomType;
	if (type.Left(3) == _T("TI3"))
		return SetMatongPos_I3(bathroomId, yLen);
	else
		return SetMatongPos_I4(bathroomId, yLen);
}

int CBathroomGenKI::SetMatongPos_I3(AcDbObjectId bathroomId, double yLen)
{
	acDocManager->lockDocument(curDoc());

	if (yLen > 2600)
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 400.0);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

int CBathroomGenKI::SetMatongPos_I4(AcDbObjectId bathroomId, double yLen)
{
	acDocManager->lockDocument(curDoc());

	if (yLen > 3200)
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(bathroomId, L"马桶距墙Y", 400.0);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

CBathroomGen* CBathroomMrg::CreateBathroomByAttribute(AttrBathroom* p_attr)
{
	if (p_attr->m_prototypeCode.Left(2) == _T("TI"))
		return new CBathroomGenKI(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("TL"))
		return new CBathroomGenKL(p_attr);
	else if (p_attr->m_prototypeCode.Left(2) == _T("TU"))
		return new CBathroomGenKU(p_attr);
	else
		return NULL;
}
