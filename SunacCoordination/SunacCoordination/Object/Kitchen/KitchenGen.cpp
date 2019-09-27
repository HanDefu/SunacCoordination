#include "StdAfx.h"
#include "KitchenGen.h"
#include "RCKitchen.h"
#include "..\..\WebIO\WebIO.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "..\..\Common/ComFun_DynamicBlock.h"


//const CString c_ShuipenBlockNames[5] = { 
//	L"水盆_单盆_600_420",
//	L"水盆_单盆_800_420",
//	L"水盆_双盆_900_450",
//	L"水盆_双盆_1000_450",
//	L"水盆_双盆_1200_450" };
//
//const CString c_BingxiangBlockNames[] = {
//	L"冰箱_单开门_700",
//	L"冰箱_双开门_800",
//	L"冰箱_双开门_1000" };
//
//const CString c_ZhaotaiBlockNames[] = {
//	L"灶台_800",
//	L"灶台_1000"};

CKitchGen::CKitchGen(AttrKitchen* p_att)
: m_attr(*p_att)
{
	m_id = AcDbObjectId::kNull;

	m_doorDir = E_DIR_BOTTOM;
	m_windowDir = E_DIR_TOP;
	m_angle = 0;
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

int CKitchGen::SetAirVentSize(double p_width, double p_height )
{
	//TODO 设置烟道宽度到动态块
	return 0;
}
//////////////////////////////////////////////////////////////////////////
vCString CKitchGen::GetShuipenOptions()// 获取台盆选型
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetShuiPenTypes();
	return options;
}
CString CKitchGen::GetShuipenDefault()
{ 
	return L"单盆600";
}

vCString CKitchGen::GetBinxiangOptions()// 获取冰箱选型
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetBingXiangTypes();
	return options;
}
CString CKitchGen::GetBinxiangDefault()
{
	return _T("对开门800");
}

vCString CKitchGen::GetZhaotaiOptions()// 获取灶台选型
{
	vCString options = WebIO::GetConfigDict()->Kitchen_GetZaoTaiWidths();
	return options;

}
CString CKitchGen::GetZhaotaiDefault()
{
	return _T("800");
}
double CKitchGen::GetRotateAngle() //处理旋转及镜像关系
{
	switch (m_doorDir)
	{
	case E_DIR_TOP:
		return PI;
		break;
	case E_DIR_LEFT:
		return -PI/2;
		break;
	case E_DIR_BOTTOM:
		return 0;
		break;
	case E_DIR_RIGHT:
		return PI / 2;
		break;
	default:
		break;
	}
	return 0;
}

void CKitchGen::InitMirror() //主要针对门窗垂直开情况，门窗垂直原型的窗在门的右侧，若实际为左侧则需要对称
{
	if (abs(m_windowDir - m_doorDir)%2 == 1) //奇数差为门窗垂直开
	{
		if (m_windowDir > m_doorDir)
		{
			m_attr.m_isMirror = false;
		}
		else
		{
			m_attr.m_isMirror = true;
		}
	}
}
AcDbObjectId CKitchGen::GenKitchen(const AcGePoint3d p_pos)
{
	InitMirror();
	const double angle = GetRotateAngle();

	RCKitchen oneKitchen;

	//先插入到原点，最后再做镜像和旋转处理
	AcDbObjectId id = oneKitchen.Insert(m_attr.m_filePathName, p_pos, angle, L"0", 256);
	oneKitchen.InitParameters();
	oneKitchen.SetParameter(L"进深", m_attr.m_height);
	oneKitchen.SetParameter(L"开间", m_attr.m_width);
	oneKitchen.RunParameters();

	SelectShuiPen(id, m_attr.m_shuiPenType);
	SelectBingXiang(id, m_attr.m_bingXiangType);
	SelectZaoTai(id, m_attr.m_zaoTaiType);

	//////////////////////////////////////////////////////////////////////////
	//烟道
	double airVentW = 0;
	double airVentH = 0;
	if (m_attr.m_hasPaiQiDao)
	{
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
	}
	SetAirVentSize(airVentW, airVentH);

	//////////////////////////////////////////////////////////////////////////
	SetDoorPos(id, m_attr.m_width);
	SetZaoTaiPos(id, m_attr.m_width, m_attr.m_height, m_attr.m_zaoTaiType);
	SetShuiPenPos(id, m_attr.m_width, m_attr.m_height,m_attr.m_shuiPenType);

	//////////////////////////////////////////////////////////////////////////
	//先镜像处理
	if (m_attr.m_isMirror)
	{
		AcGePoint3d basePt(p_pos.x + GetXLength() / 2, 0, 0);
		TYCOM_Mirror(oneKitchen.m_id, basePt, AcGeVector3d(0, 1, 0));
	}

	//再角度旋转
	if (angle!=0)
	{
		TYCOM_Rotate(oneKitchen.m_id, p_pos, angle);
	}

	
	//////////////////////////////////////////////////////////////////////////
	//把UI的数据记录在图框的扩展字典中
	AttrKitchen *pAttribute = new AttrKitchen(m_attr);
	oneKitchen.AddAttribute(pAttribute);
	pAttribute->close();

	m_id = id;
	return id;
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
		options.push_back(_T("单盆600"));
		options.push_back(_T("单盆800"));
	}
	else
	{
		options.push_back(_T("双盆900"));
		options.push_back(_T("双盆1000"));
		options.push_back(_T("双盆1200"));
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
		options.push_back(_T("单盆600"));
		options.push_back(_T("单盆800"));
	}
	else
	{
		options.push_back(_T("双盆900"));
		options.push_back(_T("双盆1000"));
		options.push_back(_T("双盆1200"));
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
vCString CKitchGenKL::GetBinxiangOptions()// 获取冰箱选项
{
	vCString options;
	if (m_attr.m_height < 3200) //进深小于3200时单开门
	{
		options.push_back(_T("单开门700"));
	}
	else
	{
		options.push_back(_T("对开门800"));
		options.push_back(_T("对开门1000"));
	}
	return options;
}
CString CKitchGenKL::GetBinxiangDefault()
{
	if (m_attr.m_height < 3200) //进深小于3200时单开门
	{
		return _T("单开门700");
	}
	else
	{
		return _T("对开门800");
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
		//进深≥2900时，灶台右侧距墙面装修完成面， 有多种解释
		//1.灶台右侧距离左侧装修完成面1100
		//2.灶台右侧距离右侧装修完成面1100
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 1100 - ztt);
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
		options.push_back(_T("单盆600"));
		options.push_back(_T("单盆800"));
	}
	else
	{
		options.push_back(_T("双盆900"));
		options.push_back(_T("双盆1000"));
		options.push_back(_T("双盆1200"));
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
vCString CKitchGenKI::GetZhaotaiOptions() 
{
	vCString options;
	if (m_attr.m_height < 3200) //进深小于3200时单开门
	{
		options.push_back(_T("单开门700"));
	}
	else
	{
		options.push_back(_T("对开门800"));
		options.push_back(_T("对开门1000"));
	}
	return options;
}
CString CKitchGenKI::GetZhaotaiDefault()
{
	if (m_attr.m_height < 3350) //进深小于3350时
	{
		return _T("800");
	}
	else
	{
		return _T("900");
	}
}



//KI门窗对开设置水盆的位置
//shuiPen:"单盆600"/"单盆800"/双盆900/双盆1000/双盆1200
int CKitchGenKI::SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType)
{
	acDocManager->lockDocument(curDoc());

	if (shuiPenType == L"单盆600")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", 690.0);
	}
	else if (shuiPenType == L"单盆800")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", 840.0);
	}
	else if (shuiPenType == L"双盆900")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", 840.0);
	}
	else if (shuiPenType == L"双盆100")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", 940.0);
	}
	else if (shuiPenType == L"双盆1200")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", 1040.0);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//zaoTaiType "800"/"900"
int CKitchGenKI::SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (zaoTaiType == L"800")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 600.0);
	}

	if (zaoTaiType == L"900")
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 650.0);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}
//////////////////////////////////////////////////////////////////////////

CKitchGenSTATIC::CKitchGenSTATIC(AttrKitchen* p_att)
:CKitchGen(p_att)
{

}

//////////////////////////////////////////////////////////////////////////
CKitchMrg* CKitchMrg::GetInstance()
{
	static CKitchMrg instance;
	return &instance;
}

CKitchMrg::CKitchMrg()
{

}

CKitchMrg::~CKitchMrg()
{

}

//////////////////////////////////////////////////////////////////////////

vector<AttrKitchen*> CKitchMrg::FilterKitch(EKitchType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	//宽高取整数
	int nWidth = int(p_width + 0.5);
	int nHeight = int(p_height + 0.5);

	//开间为门的方向，因此若门在侧边，需要交换宽高
	if (p_doorDir == E_DIR_LEFT || p_doorDir == E_DIR_RIGHT)
		swap(nWidth, nHeight);

	vector<AttrKitchen*> allKitchOut;
	vector<AttrKitchen*> kitchenStatic = FilterStatic(nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
	allKitchOut.insert(allKitchOut.end(), kitchenStatic.begin(), kitchenStatic.end());

	if (E_KITCH_ALL==p_type)
	{
		vector<AttrKitchen*> kitchenOut1 = FilterKitch_Internal(E_KITCH_U, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		vector<AttrKitchen*> kitchenOut2 = FilterKitch_Internal(E_KITCH_L, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		vector<AttrKitchen*> kitchenOut3 = FilterKitch_Internal(E_KITCH_I, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);

		allKitchOut.insert(allKitchOut.end(), kitchenOut1.begin(), kitchenOut1.end());
		allKitchOut.insert(allKitchOut.end(), kitchenOut2.begin(), kitchenOut2.end());
		allKitchOut.insert(allKitchOut.end(), kitchenOut3.begin(), kitchenOut3.end());
	}
	else
	{
		vector<AttrKitchen*> kitchenOut = FilterKitch_Internal(p_type, nWidth, nHeight, p_doorDir, p_windowDir, p_bHasAirVent);
		allKitchOut.insert(allKitchOut.end(), kitchenOut.begin(), kitchenOut.end());
	}

	//对筛选的原型设置尺寸
	for (UINT i = 0; i < allKitchOut.size();i++)
	{
		allKitchOut[i]->m_width = nWidth;
		allKitchOut[i]->m_height = nHeight;		
	}

	return allKitchOut;
}
vector<AttrKitchen*> CKitchMrg::FilterKitch_Internal(EKitchType p_type, int nWidth, int nHeight, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent)
{
	bool bNotSupport = true;
	bool bDoorWindowOpposite = abs(p_windowDir - p_doorDir) == 2;//门窗对开
	if (E_KITCH_U==p_type)
	{
		if (nWidth > nHeight) //浅U
		{
			if (bDoorWindowOpposite)
			{
				return FilterKUq(nWidth, nHeight, p_bHasAirVent);
			}
			else
			{
				return FilterKUqc(nWidth, nHeight, p_bHasAirVent);
			}
		}
		else //深U
		{
			if (bDoorWindowOpposite)
			{
				return FilterKUs(nWidth, nHeight, p_bHasAirVent);
			}
			else
			{
				bNotSupport = true;//不支持
			}
		}
	}
	else if (E_KITCH_L == p_type)
	{
		if (bDoorWindowOpposite)
		{
			return FilterKL(nWidth, nHeight, p_bHasAirVent);
		}
		else
		{
			bNotSupport = true;//不支持
		}
	}
	else if (E_KITCH_I == p_type)
	{
		if (bDoorWindowOpposite)
		{
			return FilterKI(nWidth, nHeight, p_bHasAirVent);
		}
		else
		{
			bNotSupport = true;//不支持
		}
	}
	else
	{

	}

	return vector<AttrKitchen*>(); //返回空
}


vector<AttrKitchen*> CKitchMrg::FilterStatic(int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent) //静态厨房
{
	bool bDoorWindowOpposite = abs(p_windowDir - p_doorDir) == 2;//门窗对开

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, bDoorWindowOpposite ? _T("门窗对开") : _T("门窗垂直"), _T(""), p_bHasAirVent, false); 
}


vector<AttrKitchen*> CKitchMrg::FilterKUq(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//限定开间与进深的范围
	if (p_width < 2450 || p_width > 3500)
		return attrKitchen;
	if (p_height < 1700 || p_height > 2600)
		return attrKitchen;
	if (p_width <= p_height)
		return attrKitchen;

	//不支持2450*1700
	if (p_width == 2450 && p_height == 1700)
		return attrKitchen;

	//以150递增尺寸
	if ((p_width - 2450) % 150 != 0)
		return attrKitchen;
	if ((p_height - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("门窗对开"), _T("U型"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKUqc(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//限定开间与进深的范围
	if (p_width < 2450 || p_width > 3500)
		return attrKitchen;
	if (p_height < 1700 || p_height > 2450)
		return attrKitchen;
	if (p_width <= p_height)
		return attrKitchen;

	//不支持2450*1700
	if (p_width == 2450 && p_height == 1700)
		return attrKitchen;

	//以150递增尺寸
	if ((p_width - 2450) % 150 != 0)
		return attrKitchen;
	if ((p_height - 1700) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("门窗垂直"), _T("U型"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKUs(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//限定开间与进深的范围
	if (p_width < 2300 || p_width > 2450)
		return attrKitchen;
	if (p_height < 2450 || p_height > 3200)
		return attrKitchen;
	if (p_width > p_height)
		return attrKitchen;

	//以150递增尺寸
	if ((p_width - 2300) % 150 != 0)
		return attrKitchen;
	if ((p_height - 2450) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("门窗对开"), _T("U型"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKL(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//限定开间与进深的范围
	if (p_width < 1700 || p_width > 2000)
		return attrKitchen;
	if (p_height < 2600 || p_height > 3950)
		return attrKitchen;

	//以150递增尺寸
	if ((p_width - 1700) % 150 != 0)
		return attrKitchen;
	if ((p_height - 2600) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("门窗对开"), _T("L型"), p_bHasAirVent, true);
}

vector<AttrKitchen*> CKitchMrg::FilterKI(int p_width, int p_height, bool p_bHasAirVent)
{
	vector<AttrKitchen*> attrKitchen;

	//限定开间与进深的范围
	if (p_width != 1700)
		return attrKitchen;
	if (p_height < 3050 || p_height > 4100)
		return attrKitchen;

	//以150递增尺寸
	if ((p_height - 3050) % 150 != 0)
		return attrKitchen;

	return WebIO::GetInstance()->GetKitchens(p_width, p_height, _T("门窗对开"), _T("I型"), p_bHasAirVent, true);
}

CKitchGen* CKitchMrg::CreateKitchGenByKitchType(AttrKitchen* p_attKitchen)
{
	if (p_attKitchen==NULL)
	{
		assert(false);
		return NULL;
	}
	
	//确保厨房在传入前设置了尺寸
	assert(p_attKitchen->m_width > 0 && p_attKitchen->m_height > 0);


	//或者通过门窗编号创建对应的类
	CKitchGen* pKitchenGen = NULL;

	if (p_attKitchen->m_isDynamic)
	{
		if (p_attKitchen->m_kitchenType==_T("U型"))
		{
			if (p_attKitchen->m_windowDoorPos==_T("门窗对开"))
			{
				CString sName = p_attKitchen->m_name;
				sName.MakeUpper();
				if (sName.Find(_T("KUS"))>=0)
				{
					pKitchenGen = new CKitchGenKUS(p_attKitchen); //深U
				}
				else
				{
					pKitchenGen = new CKitchGenKUQ(p_attKitchen);
				}
			}
			else if (p_attKitchen->m_windowDoorPos == _T("门窗垂直"))
			{
				pKitchenGen = new CKitchGenKUQ_C(p_attKitchen);
			}
			else
			{
				return NULL;
			}
		}
		else if (p_attKitchen->m_kitchenType == _T("L型"))
		{
			pKitchenGen = new CKitchGenKL(p_attKitchen);
		}
		else if (p_attKitchen->m_kitchenType == _T("I型"))
		{
			pKitchenGen = new CKitchGenSTATIC(p_attKitchen);
		}
		else
		{
			assert(false);
		}
	}
	else
	{
		pKitchenGen = new CKitchGenSTATIC(p_attKitchen);
	}


	return pKitchenGen;
}