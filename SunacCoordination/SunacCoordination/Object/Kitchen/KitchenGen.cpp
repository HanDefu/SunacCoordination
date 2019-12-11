#include "StdAfx.h"
#include "KitchenGen.h"
#include "RCKitchen.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "..\..\Common/ComFun_DynamicBlock.h"
#include "KitchenAutoName.h"
#include "..\..\Src\DocumentData.h"


CKitchGen::CKitchGen(AttrKitchen* p_att)
: m_attr(*p_att)
{
}

CKitchGen::~CKitchGen()
{
}

int CKitchGen::SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen)
{
	vCString hideBlockRecordNames;
	if (shuiPen == L"单盆600")
	{
		hideBlockRecordNames.push_back(L"水盆_单盆_800_420");
		hideBlockRecordNames.push_back(L"水盆_双盆_900_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1000_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1200_450");
	}
	else if (shuiPen == L"单盆800")
	{
		hideBlockRecordNames.push_back(L"水盆_单盆_600_420");
		hideBlockRecordNames.push_back(L"水盆_双盆_900_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1000_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1200_450");
	}
	else if (shuiPen == L"双盆900")
	{
		hideBlockRecordNames.push_back(L"水盆_单盆_600_420");
		hideBlockRecordNames.push_back(L"水盆_单盆_800_420");
		hideBlockRecordNames.push_back(L"水盆_双盆_1000_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1200_450");
	}
	else if (shuiPen == L"双盆1000")
	{
		hideBlockRecordNames.push_back(L"水盆_单盆_600_420");
		hideBlockRecordNames.push_back(L"水盆_单盆_800_420");
		hideBlockRecordNames.push_back(L"水盆_双盆_900_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1200_450");
	}
	else if (shuiPen == L"双盆1200")
	{
		hideBlockRecordNames.push_back(L"水盆_单盆_600_420");
		hideBlockRecordNames.push_back(L"水盆_单盆_800_420");
		hideBlockRecordNames.push_back(L"水盆_双盆_900_450");
		hideBlockRecordNames.push_back(L"水盆_双盆_1000_450");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
}

int CKitchGen::SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai)
{
	vCString hideBlockRecordNames;
	if (zaoTai == L"800")
	{
		hideBlockRecordNames.push_back(L"灶台_900");
	}
	else if (zaoTai == L"900")
	{
		hideBlockRecordNames.push_back(L"灶台_800");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
}

int CKitchGen::SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang)
{
	vCString hideBlockRecordNames;
	if (bingXiang == L"单开门700")
	{
		hideBlockRecordNames.push_back(L"冰箱_对开门_800");
		hideBlockRecordNames.push_back(L"冰箱_对开门_1000");
	}
	else if (bingXiang == L"对开门800")
	{
		hideBlockRecordNames.push_back(L"冰箱_单开门_700");
		hideBlockRecordNames.push_back(L"冰箱_对开门_1000");
	}
	else if (bingXiang == L"对开门1000")
	{
		hideBlockRecordNames.push_back(L"冰箱_单开门_700");
		hideBlockRecordNames.push_back(L"冰箱_对开门_800");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
}

double CKitchGen::GetXLength()
{
	CKitchenBathroomProp* pProKitchen = &m_attr.m_prop;
	if (pProKitchen->m_doorPos == E_DIR_BOTTOM || pProKitchen->m_doorPos == E_DIR_TOP)
		return m_attr.m_width;
	else
		return m_attr.m_height;
}

double CKitchGen::GetYLength()
{
	if (m_attr.m_width == GetXLength())
		return m_attr.m_height;
	else
		return m_attr.m_width;
}

//////////////////////////////////////////////////////////////////////////
vCString CKitchGen::GetShuipenOptions()// 获取台盆选型
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetShuiPenTypes();
	return options;
}

CString CKitchGen::GetShuipenDefault()
{ 
	return L"单盆600";
}

vCString CKitchGen::GetBinxiangOptions()// 获取冰箱选型
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetBingXiangTypes();
	return options;
}

CString CKitchGen::GetBinxiangDefault()
{
	return L"对开门800";
}

vCString CKitchGen::GetZhaotaiOptions()// 获取灶台选型
{
	vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetZaoTaiWidths();
	return options;

}

CString CKitchGen::GetZhaotaiDefault()
{
	return L"800";
}

void CKitchGen::InitKitchenByDefault()
{
	vCString shuipenOptions = GetShuipenOptions();
	vCString bingxiangOptions = GetBinxiangOptions();
	vCString zaotaiOptions = GetZhaotaiOptions();
	CString& shuipen = GetKitchenAtt()->m_shuiPenType;
	CString& bingxiang = GetKitchenAtt()->m_bingXiangType;
	CString& zaotai = GetKitchenAtt()->m_zaoTaiType;

	if (TYCOM_vFind(shuipen, shuipenOptions) == -1)
		shuipen = GetShuipenDefault();
	if (TYCOM_vFind(bingxiang, bingxiangOptions) == -1)
		bingxiang = GetBinxiangDefault();
	if (TYCOM_vFind(zaotai, zaotaiOptions) == -1)
		zaotai = GetZhaotaiDefault();

	GetKitchenAtt()->m_instanceCode = GetKitchenAutoName()->GetKitchenName(*GetKitchenAtt());
}

AcDbObjectId CKitchGen::GenKitchen(const AcGePoint3d p_pos, int p_angle)
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

	RCKitchen oneKitchen;

	//先插入到原点，最后再做镜像和旋转处理
	AcDbObjectId id = oneKitchen.Insert(TY_GetPrototypeFilePath() + m_attr.GetFileName(), p_pos, 0, L"Sunac_Kitchen", 256);
	oneKitchen.InitParameters();
	oneKitchen.SetParameter(L"开间", m_attr.m_width);
	oneKitchen.SetParameter(L"进深", m_attr.m_height);
	//////////////////////////////////////////////////////////////////////////
	//烟道
	if (m_attr.m_hasPaiQiDao)
	{
		double airVentW = 0;
		double airVentH = 0;
		if (m_attr.m_isGuoBiao) //国标
		{
			airVentW = m_attr.m_airVentOffsetX + c_kitchenAirVentSize[m_attr.m_floorRange];
			airVentH = m_attr.m_airVentOffsetY + c_kitchenAirVentSize[m_attr.m_floorRange];
		}
		else
		{
			airVentW = m_attr.m_airVentW;
			airVentH = m_attr.m_airVentH;
		}
		assert(airVentW > 0 && airVentH > 0);
		oneKitchen.SetParameter(L"排气道X尺寸", airVentW);
		oneKitchen.SetParameter(L"排气道Y尺寸", airVentH);
		oneKitchen.SetParameter(L"排气道偏移X", m_attr.m_airVentOffsetX);
		oneKitchen.SetParameter(L"排气道偏移Y", m_attr.m_airVentOffsetY);

		oneKitchen.SetParameter(L"可见性", airVentH >= 540 ? L"大烟道" : L"小烟道"); //540为600扣去墙尺寸60
	}
	//////////////////////////////////////////////////////////////////////////
	oneKitchen.RunParameters();

	SelectShuiPen(id, m_attr.m_shuiPenType);
	SelectBingXiang(id, m_attr.m_bingXiangType);
	SelectZaoTai(id, m_attr.m_zaoTaiType);

	SetDoorPos(id, m_attr.m_width);
	SetZaoTaiPos(id, m_attr.m_width, m_attr.m_height, m_attr.m_zaoTaiType);
	SetShuiPenPos(id, m_attr.m_width, m_attr.m_height,m_attr.m_shuiPenType);

	//////////////////////////////////////////////////////////////////////////
	//先镜像处理
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + GetXLength() / 2, 0, 0);
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
	AttrKitchen *pAttribute = new AttrKitchen(m_attr);
	oneKitchen.AddAttribute(pAttribute);
	pAttribute->close();

	//针对USC坐标处理
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}

	return id;
}

	//烟道
bool CKitchGen::GetPaiqidaoSize(double & p_width, double &p_height)
{
	if (m_attr.m_hasPaiQiDao)
	{
		if (m_attr.m_isGuoBiao) //国标
		{
			p_width = m_attr.m_airVentOffsetX + c_kitchenAirVentSize[m_attr.m_floorRange];
			p_height = m_attr.m_airVentOffsetY + c_kitchenAirVentSize[m_attr.m_floorRange];
		}
		else
		{
			p_width = m_attr.m_airVentW;
			p_height = m_attr.m_airVentH;
		}
		return true;
	}
	else
	{
		p_width = 0; 
		p_height = 0;
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////

CKitchGenKUQ::CKitchGenKUQ(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKUQ::GetShuipenOptions()// 获取台盆选型
{
	vCString options;
	if (m_attr.m_width < 2900)
	{
		options.push_back(L"单盆600");
		options.push_back(L"单盆800");
	}
	else
	{
		options.push_back(L"双盆900");
		options.push_back(L"双盆1000");
		options.push_back(L"双盆1200");
	}
	return options;
}

CString CKitchGenKUQ::GetShuipenDefault()
{
	if (m_attr.m_width < 2900)
		return L"单盆600";
	else
		return L"双盆900";
}

vCString CKitchGenKUQ::GetBinxiangOptions()
{
	vCString options;
	
	options.push_back(L"对开门800");
	options.push_back(L"对开门1000");

	return options;
}

CString CKitchGenKUQ::GetBinxiangDefault()
{
	return L"对开门800";
}

//kuq 对开 自动设置门的位置
int CKitchGenKUQ::SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", 700.0);
	}
	else
	{
		//1600是门宽
		double value = (kaiJian - 1600) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 对开 自动设置灶台的位置
int CKitchGenKUQ::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (jinShen < 2000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙Y", 200.0);
	}
	else
	{
		double pqdY = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"排气道Y尺寸", pqdY);

		double qt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

		double ztt = 800;
		if (zaoTaiType == L"900")
			ztt = 900;

		double value = (jinShen - zxt * 2 - qt - pqdY - ztt / 2) / 2;

		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙Y", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 对开 自动设置门的位置
int CKitchGenKUQ::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管X尺寸", lgx);

	double value = (kaiJian - 350) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", value - qt - zxt - lgx);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKUQ_C::CKitchGenKUQ_C(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKUQ_C::GetShuipenOptions()// 获取台盆选型
{
	vCString options;
	if (m_attr.m_height < 2150) //进深小于2150时单盆
	{
		options.push_back(L"单盆600");
		options.push_back(L"单盆800");
	}
	else
	{
		options.push_back(L"双盆900");
		options.push_back(L"双盆1000");
		options.push_back(L"双盆1200");
	}
	return options;
}

CString CKitchGenKUQ_C::GetShuipenDefault()
{
	if (m_attr.m_height < 2150) //进深小于2150时单盆
		return L"单盆600";
	else
		return L"双盆900";
}

vCString CKitchGenKUQ_C::GetBinxiangOptions()
{
	vCString options;

	options.push_back(L"对开门800");
	options.push_back(L"对开门1000");

	return options;
}

CString CKitchGenKUQ_C::GetBinxiangDefault()
{
	return L"对开门800";
}

//kuq 垂直开 自动设置门的位置
int CKitchGenKUQ_C::SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", 700.0);
	}
	else
	{
		//1600是门宽
		double value = (kaiJian - 1600) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 垂直开 自动设置灶台的位置
int CKitchGenKUQ_C::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 2900)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 750.0);
	}
	else
	{
		double pqdX = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"排气道X尺寸", pqdX);

		double qt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

		double value = (kaiJian - 350) / 2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", value - qt - zxt - pqdX);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 垂直开 自动设置水盆的位置
int CKitchGenKUQ_C::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管Y尺寸", lgy);


	double value = (jinShen - qt - zxt * 2 - lgy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙Y", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKUS::CKitchGenKUS(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKUS::GetBinxiangOptions()
{
	vCString options;

	options.push_back(L"对开门800");
	options.push_back(L"对开门1000");

	return options;
}

CString CKitchGenKUS::GetBinxiangDefault()
{
	return L"对开门800";
}

//kus 垂直开 自动设置水盆的位置
int CKitchGenKUS::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管X尺寸", lgx);

	double pqdx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"排气道X尺寸", pqdx);


	double value = (kaiJian - qt * 2 - zxt * 2 - lgx - pqdx) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

int CKitchGenKUS::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double pqdy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"排气道Y尺寸", pqdy);


	double value = (jinShen - qt - zxt * 2 - pqdy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙Y", value);

	acDocManager->unlockDocument(curDoc());
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CKitchGenKL::CKitchGenKL(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKL::GetShuipenOptions()// 获取水盆选项
{
	double len = m_attr.m_width - 50 - 600 - 300; //50是装修面宽， 600是台面宽， 300是下水管空间

	vCString strs;
	strs.push_back(L"单盆600");
	strs.push_back(L"单盆800");

	 if (len>1000)
	 {
		 strs.push_back(L"双盆900");
	 }
	 else if (len>1100)
	 {
		 strs.push_back(L"双盆1000");
	 }
	 else
	 {
		 vCString options = WebIO::GetInstance()->GetConfigDict()->Kitchen_GetShuiPenTypes();
		 return options;
	 }
	 return strs;
}

CString CKitchGenKL::GetShuipenDefault() //水盆默认值
{
	return L"单盆600";
}

vCString CKitchGenKL::GetBinxiangOptions()// 获取冰箱选项
{
	vCString options;
	if (m_attr.m_height < 3200) //进深小于3200时单开门
	{
		options.push_back(L"单开门700");
	}
	else
	{
		options.push_back(L"对开门800");
		options.push_back(L"对开门1000");
	}
	return options;
}

CString CKitchGenKL::GetBinxiangDefault()
{
	if (m_attr.m_height < 3200) //进深小于3200时单开门
	{
		return L"单开门700";
	}
	else
	{
		return L"对开门800";
	}
}

//KL门窗对开设置水盆的位置
int CKitchGenKL::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管Y尺寸", lgy);

	double pqdy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"排气道Y尺寸", pqdy);


	double value = (kaiJian - qt * 2 - zxt * 2 - lgy - pqdy) / 2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙Y", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//KL门窗对开设置灶台的位置 
int CKitchGenKL::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	double ztt = 800;
	if (zaoTaiType == L"900")
		ztt = 900;

	if (jinShen < 2900)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 900.0);
	}
	else
	{
		double zxt = 0;
		TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);


		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", jinShen - 2 * zxt - 1100 - ztt);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenKI::CKitchGenKI(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

vCString CKitchGenKI::GetShuipenOptions()// 获取台盆选型
{
	vCString options;
	if (m_attr.m_height < 3350) //进深小于3350时单盆
	{
		options.push_back(L"单盆600");
		options.push_back(L"单盆800");
	}
	else
	{
		options.push_back(L"双盆900");
		options.push_back(L"双盆1000");
		options.push_back(L"双盆1200");
	}
	return options;
}

CString CKitchGenKI::GetShuipenDefault()
{
	if (m_attr.m_height < 3350) //进深小于3350时单盆
		return L"单盆600";
	else
		return L"双盆900";
}

vCString CKitchGenKI::GetBinxiangOptions()
{
	//没有冰箱
	vCString options;
	return options;
}

CString CKitchGenKI::GetBinxiangDefault()
{
	return L"";
}

vCString CKitchGenKI::GetZhaotaiOptions() 
{
	vCString options;
	if (m_attr.m_height <= 3350)
	{
		options.push_back(L"800");
	}
	else
	{
		options.push_back(L"900");
	}
	return options;
}

CString CKitchGenKI::GetZhaotaiDefault()
{
	if (m_attr.m_height <= 3350)
	{
		return L"800";
	}
	else
	{
		return L"900";
	}
}

bool CKitchGenKI::CheckParameter(CString& errMsg)
{
	//检查水盆和灶台的间距大于550
	double shuipenOffset = GetShuipenOffset(m_attr.m_shuiPenType);
	double zhaotaiOffset = GetZaotaiOffset(m_attr.m_shuiPenType);

	double shuipenLen = GetShuipenLen(m_attr.m_shuiPenType);
	double zhaotaiLen = GetZaotaiLen(m_attr.m_shuiPenType);

	double paiqidaoWidth = 0;
	double paiqidaoHeight = 0;
	GetPaiqidaoSize(paiqidaoWidth, paiqidaoHeight);

	double zhuangxiuLen = 25; //装修面宽
	double qiangWidth = 60; //墙厚

	double dist = m_attr.m_height - shuipenOffset - zhaotaiOffset - shuipenLen / 2 - zhaotaiLen / 2 - paiqidaoWidth - zhuangxiuLen * 2 - qiangWidth;
	if (dist<500)
	{
		errMsg = _T("不能满足灶台和水盆距离大于500的要求");
		return false;
	}

	return true;
}

//KI门窗对开设置水盆的位置
//shuiPen:"单盆600"/"单盆800"/双盆900/双盆1000/双盆1200
int CKitchGenKI::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	double offset = GetShuipenOffset(shuiPenType);
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", offset);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//zaoTaiType "800"/"900"
int CKitchGenKI::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	double offset = GetZaotaiOffset(zaoTaiType);
	TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", offset);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

double CKitchGenKI::GetShuipenOffset(const CString shuiPenType)
{
	if (shuiPenType == L"单盆600")
	{
		return 690;
	}
	else if (shuiPenType == L"单盆800")
	{
		return 840.0;
	}
	else if (shuiPenType == L"双盆900")
	{
		return 840.0;
	}
	else if (shuiPenType == L"双盆1000")
	{
		return 940.0;
	}
	else if (shuiPenType == L"双盆1200")
	{
		return 1040.0;
	}

	return 690;
}


double CKitchGenKI::GetShuipenLen(const CString shuiPenType)
{
	if (shuiPenType == L"单盆600")
	{
		return 600;
	}
	else if (shuiPenType == L"单盆800")
	{
		return 800;
	}
	else if (shuiPenType == L"双盆900")
	{
		return 900;
	}
	else if (shuiPenType == L"双盆1000")
	{
		return 1000;
	}
	else if (shuiPenType == L"双盆1200")
	{
		return 1200;
	}

	return 600;
}

double CKitchGenKI::GetZaotaiOffset(const CString zaoTaiType)
{
	if (zaoTaiType == L"800")
	{
		return 600.0;
	}
	else if (zaoTaiType == L"900")
	{
		return 650.0;
	}

	return 600;
}
double CKitchGenKI::GetZaotaiLen(const CString zaoTaiType)
{
	if (zaoTaiType == L"800")
	{
		return 800;
	}
	else if (zaoTaiType == L"900")
	{
		return 900;
	}

	return 800;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenSTATIC::CKitchGenSTATIC(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

CKitchGen* CKitchMrg::CreateKitchGenByKitchType(AttrKitchen* p_attr)
{
	CString prototypeCode = p_attr->m_prototypeCode.MakeUpper();
	if ((prototypeCode.Left(3) == L"KUQ") && (prototypeCode.Find(L"_C") == -1))
		return new CKitchGenKUQ(p_attr);
	else if (prototypeCode.Left(3) == L"KUQ")
		return new CKitchGenKUQ_C(p_attr);
	else if (prototypeCode.Left(3) == L"KUS")
		return new CKitchGenKUS(p_attr);
	else if (prototypeCode.Left(2) == L"KL")
		return new CKitchGenKL(p_attr);
	else if (prototypeCode.Left(2) == L"KI")
		return new CKitchGenKI(p_attr);
	else
		return NULL;
}
