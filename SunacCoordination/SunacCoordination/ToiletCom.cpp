#include "StdAfx.h"
#include "ToiletCom.h"
#include "Common/ComFun_Sunac.h"
#include "Common/ComFun_DynamicBlock.h"

int Toilet_SelectTaiPen(AcDbObjectId toiletId, CString taiPen)
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


	TY_HideBlockReferencesInBlockReference(toiletId, hideBlockRecordNames);

	return 0;
}

int Toilet_SelectBingXiang(AcDbObjectId toiletId, CString maTong)
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


	TY_HideBlockReferencesInBlockReference(toiletId, hideBlockRecordNames);

	return 0;
}

