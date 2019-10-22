#pragma once
#include <vector>
#include "AttrKitchen.h"

using namespace std;


class CKitchenAutoName
{
public:
	CKitchenAutoName();
	~CKitchenAutoName();

	static CKitchenAutoName* GetInstance();

	//自动获取名字，并加入到当前窗型库中
	CString GetKitchenName(const AttrKitchen& p_att) const; 

	void AddKitchenType(const AttrKitchen& p_att);

	//用于用户自命名的情况,检查当前名称是否合理，合理则加入到窗型库中，否则返回false
	bool IsUserNameValid(const AttrKitchen& p_att, CString p_sName);
	
	void AutoNameAllKitchen(); //遍历当前图纸中的所有的窗户，对其进行自动命名
	void ReNameKitchen(const AttrKitchen& p_att);	//对图纸中当前原型相同的窗户进行命名


protected:
	vector<AttrKitchen> m_allTypeKitchens; //所有窗型

};
