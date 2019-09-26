#include "StdAfx.h"
#include "KitchenCom.h"
#include "Common/ComFun_Sunac.h"
#include "Common/ComFun_DynamicBlock.h"

int Kitchen_SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen)
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

int Kitchen_SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai)
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

int Kitchen_SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang)
{
	vCString hideBlockRecordNames;
	if (bingXiang == L"冰箱_单开门_700")
	{
		hideBlockRecordNames.push_back(L"冰箱_对开门_800");
		hideBlockRecordNames.push_back(L"冰箱_对开门_1000");
	}
	else if (bingXiang == L"冰箱_对开门_800")
	{
		hideBlockRecordNames.push_back(L"冰箱_单开门_700");
		hideBlockRecordNames.push_back(L"冰箱_对开门_1000");
	}
	else if (bingXiang == L"冰箱_对开门_1000")
	{
		hideBlockRecordNames.push_back(L"冰箱_单开门_700");
		hideBlockRecordNames.push_back(L"冰箱_对开门_800");
	}
	else
		return -1;

	TY_HideBlockReferencesInBlockReference(kitchenId, hideBlockRecordNames);

	return 0;
}

//kuq 对开 自动设置门的位置
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", 700.0);
	}
	else
	{
		//1600是门宽
		double value = (kaiJian - 1600)/2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 对开 自动设置灶台的位置
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType)
{
	acDocManager->lockDocument(curDoc());

	if (jinshen < 2000)
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

		double value = (jinshen - zxt * 2 - qt - pqdY - ztt/2)/2;

		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙Y", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 对开 自动设置门的位置
int Kitchen_KUq_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgx = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管X尺寸", lgx);

	double value  = (kaiJian - 350)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", value-qt-zxt-lgx);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 垂直开 自动设置门的位置
int Kitchen_KUq_ChuiZhiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
	acDocManager->lockDocument(curDoc());

	if (kaiJian < 3000)
	{
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", 700.0);
	}
	else
	{
		//1600是门宽
		double value = (kaiJian - 1600)/2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"门垛右长", value);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 垂直开 自动设置灶台的位置
int Kitchen_KUq_ChuiZhiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian)
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

		double value  = (kaiJian - 350)/2;
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", value-qt-zxt-pqdX);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kuq 垂直开 自动设置水盆的位置
int Kitchen_KUq_ChuiZhiKai_SetShuiPenPos(AcDbObjectId kitchenId, double jinshen)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double lgy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"立管Y尺寸", lgy);


	double value  = (jinshen - qt - zxt*2 - lgy)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙Y", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//kus 垂直开 自动设置水盆的位置
int Kitchen_KUs_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian)
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


	double value  = (kaiJian - qt*2 - zxt*2 - lgx - pqdx)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙X", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

int Kitchen_KUs_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinShen)
{
	acDocManager->lockDocument(curDoc());

	double qt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"墙厚", qt);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"装修厚度", zxt);

	double pqdy = 0;
	TYCOM_GetDynamicBlockData(kitchenId, L"排气道Y尺寸", pqdy);


	double value  = (jinShen - qt - zxt*2  - pqdy)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙Y", value);

	acDocManager->unlockDocument(curDoc());
	return 0;
}

//KL门窗对开设置水盆的位置
int Kitchen_KL_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian)
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


	double value  = (kaiJian - qt*2 - zxt*2 - lgy - pqdy)/2;
	TYCOM_SetDynamicBlockValue(kitchenId, L"水盆距墙Y", value);

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//KL门窗对开设置灶台的位置 
int Kitchen_KL_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinShen, CString zaoTaiType)
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
		TYCOM_SetDynamicBlockValue(kitchenId, L"灶台距墙X", 1100-ztt);
	}

	acDocManager->unlockDocument(curDoc());

	return 0;
}

//KI门窗对开设置水盆的位置
//shuiPen:"单盆600"/"单盆800"/双盆900/双盆1000/双盆1200
int Kitchen_KI_DuiKai_SetShuiPenPos(AcDbObjectId kitchenId, double jinShen, CString shuiPenType)
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
int Kitchen_KI_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, CString zaoTaiType)
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