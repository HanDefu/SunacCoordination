#pragma once
#include <vector>
#include "AttrKitchen.h"

using namespace std;


class CDocumentData;

class CKitchenAutoName
{
	friend CDocumentData;
	CKitchenAutoName();
public:
	~CKitchenAutoName();


	//自动获取名字
	CString GetKitchenName(const AttrKitchen& p_att) const;

	//加入厨房列表中
	void AddKitchenType(const AttrKitchen& p_att);

	//检查当前名称是否合理，如有同名的其它厨房则返回false
	bool IsNameValid(const AttrKitchen& p_att, CString p_sName) const;

	//将所有厨房重新自动命名
	void AutoNameAllKitchen();

	//将指定厨房重命名
	bool RenameKitchen(const AttrKitchen& p_att);

	Acad::ErrorStatus ReadFromDwg(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus WriteToDwg(AcDbDwgFiler* pFiler);

protected:
	vector<AttrKitchen> m_allTypeKitchens; //所有厨房

};
