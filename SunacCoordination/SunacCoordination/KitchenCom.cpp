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
	else if (shuiPen == L"单盆700")
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
}

//kuq 对开 自动设置门的位置
int Kitchen_KUq_DuiKai_SetDoorPos(AcDbObjectId kitchenId, double kaiJian)
{
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

	return 0;
}

//kuq 对开 自动设置灶台的位置
int Kitchen_KUq_DuiKai_SetZaoTaiPos(AcDbObjectId kitchenId, double jinshen, CString zaoTaiType)
{
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

	return 0;
}