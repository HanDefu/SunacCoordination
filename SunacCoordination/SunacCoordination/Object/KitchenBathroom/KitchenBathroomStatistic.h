#pragma once

#include "../Kitchen/AttrKitchen.h"
#include "../Bathroom/AttrBathroom.h"

struct KitchenBathroomItem
{
	CString name;
	CString type;
	int amount;

	KitchenBathroomItem()
	{
		amount = 0;
	}
};

class CKitchenBathroomStatistic
{
public:
	int SelectKitchenBathroom();
	void InsertKitchen(AttrKitchen* pAttr);
	void InsertBathroom(AttrBathroom* pAttr);

	void InsertKBItem(const KitchenBathroomItem& pItem);
	int FindKBItem(const KitchenBathroomItem& pItem);

	AcDbObjectId InsertTableToCAD(AcGePoint3d insertPos);

protected:
	vector<KitchenBathroomItem> m_allItems;
};

bool KBItemCmp(const KitchenBathroomItem& x1, const KitchenBathroomItem& x2);