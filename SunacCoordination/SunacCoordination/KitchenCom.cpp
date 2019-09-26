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

int Kitchen_SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang)
{
	vCString hideBlockRecordNames;
	if (bingXiang == L"单开门700")
	{
		//hideBlockRecordNames.push_back(L"灶台_900");
		//hideBlockRecordNames.push_back(L"灶台_900");
	}
	else if (bingXiang == L"对开门800")
	{
		//hideBlockRecordNames.push_back(L"灶台_800");
		//hideBlockRecordNames.push_back(L"灶台_900");
	}
	else if (bingXiang == L"对开门1000")
	{
		//hideBlockRecordNames.push_back(L"灶台_800");
		//hideBlockRecordNames.push_back(L"灶台_900");
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