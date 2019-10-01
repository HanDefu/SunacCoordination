#include "StdAfx.h"
#include "BathroomCom.h"
#include "..\..\Common/ComFun_Sunac.h"
#include "..\..\Common/ComFun_DynamicBlock.h"

int Bathroom_SelectTaiPen(AcDbObjectId BathroomId, CString taiPen)
{
	vCString hideBlockRecordNames;
	if (taiPen == L"Sunac_台盆_650")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taiPen == L"Sunac_台盆_750")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taiPen == L"Sunac_台盆_800")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taiPen == L"Sunac_台盆_900")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_1000");
	}
	else if (taiPen == L"Sunac_台盆_1000")
	{
		hideBlockRecordNames.push_back(L"Sunac_台盆_650");
		hideBlockRecordNames.push_back(L"Sunac_台盆_750");
		hideBlockRecordNames.push_back(L"Sunac_台盆_800");
		hideBlockRecordNames.push_back(L"Sunac_台盆_900");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(BathroomId, hideBlockRecordNames);

	return 0;
}

int Bathroom_SelectBingXiang(AcDbObjectId BathroomId, CString maTong)
{
	vCString hideBlockRecordNames;
	if (maTong == L"Sunac_马桶800")
	{
		hideBlockRecordNames.push_back(L"Sunac_马桶750");
	}
	else if (maTong == L"Sunac_马桶750")
	{
		hideBlockRecordNames.push_back(L"Sunac_马桶800");
	}
	else
		return -1;


	TY_HideBlockReferencesInBlockReference(BathroomId, hideBlockRecordNames);

	return 0;
}

int TI3_SetMaTongPos(AcDbObjectId BathroomId, double yLen)
{
	if (yLen > 2600)
		TYCOM_SetDynamicBlockValue(BathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(BathroomId, L"马桶距墙Y", 400.0);

	return 0;
}

vCString TI3_GetTaiPens(AcDbObjectId BathroomId, double yLen, double maTongWidth)
{
	if (maTongWidth <= 0)
		maTongWidth = 750;

	double y1 = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"排气道立管Y总尺寸", y1);

	double y2 = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"排气道墙厚", y2);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"装修厚度", zxt);

	double lastY = yLen - y1 - y2 - zxt * 2 - maTongWidth;


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_台盆_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_台盆_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_台盆_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_台盆_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_台盆_1000");
	
	return taiPens;
}


int TI3_G_SetHuanXiQuY(AcDbObjectId BathroomId, double yLen)
{
	if (fabs(yLen - 950) > TOL && fabs(yLen - 1050) > TOL )
		return -1;
	TYCOM_SetDynamicBlockValue(BathroomId, L"盥洗区Y", yLen);
	return 0;
}

//yLen y方向总长（长边尺寸）
int TI4_SetMaTongPos(AcDbObjectId BathroomId, double yLen)
{
	if (yLen > 3200)
		TYCOM_SetDynamicBlockValue(BathroomId, L"马桶距墙Y", 450.0);
	else
		TYCOM_SetDynamicBlockValue(BathroomId, L"马桶距墙Y", 400.0);

	return 0;
}

vCString TI4_GetTaiPens(AcDbObjectId BathroomId, double yLen, double maTongWidth)
{
	if (maTongWidth <= 0)
		maTongWidth = 750;

	double y1 = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"排气道立管Y总尺寸", y1);

	double y2 = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"排气道墙厚", y2);

	double zxt = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"装修厚度", zxt);

	double lastY = yLen - y1 - y2 - zxt * 2 - maTongWidth - 650;//650为洗衣机宽度


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_台盆_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_台盆_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_台盆_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_台盆_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_台盆_1000");

	return taiPens;
}

int TI4_G_SetHuanXiQuY(AcDbObjectId BathroomId, double yLen)
{
	if (fabs(yLen - 950) > TOL && fabs(yLen - 1050) > TOL )
		return -1;
	TYCOM_SetDynamicBlockValue(BathroomId, L"盥洗区Y", yLen);
	return 0;
}

vCString TL4_GetTaiPens(AcDbObjectId BathroomId, double yLen, double linYuFangY)
{
	if (linYuFangY <= 0)
		linYuFangY = 900;

	double zxt = 0;
	TYCOM_GetDynamicBlockData(BathroomId, L"装修厚度", zxt);

	double lastY = yLen - zxt * 2 - 650;//650为洗衣机宽度


	vCString taiPens;
	if (lastY > 650 )
		taiPens.push_back(L"Sunac_台盆_650");

	if (lastY > 750 )
		taiPens.push_back(L"Sunac_台盆_750");

	if (lastY > 800 )
		taiPens.push_back(L"Sunac_台盆_800");

	if (lastY > 900 )
		taiPens.push_back(L"Sunac_台盆_900");

	if (lastY > 1000 )
		taiPens.push_back(L"Sunac_台盆_1000");

	return taiPens;
}