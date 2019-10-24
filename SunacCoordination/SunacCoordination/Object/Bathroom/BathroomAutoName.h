#pragma once
#include <vector>
#include "AttrBathroom.h"

using namespace std;


class CBathroomAutoName
{
public:
	CBathroomAutoName();
	~CBathroomAutoName();

	static CBathroomAutoName* GetInstance();

	//自动获取名字
	CString GetBathroomName(const AttrBathroom& p_att) const;

	//加入卫生间列表中
	void AddBathroomType(const AttrBathroom& p_att);

	//检查当前名称是否合理，如有同名的其它卫生间则返回false
	bool IsNameValid(const AttrBathroom& p_att, CString p_sName) const;

	//将所有卫生间重新自动命名
	void AutoNameAllBathroom();

	//将指定卫生间重命名
	bool RenameBathroom(const AttrBathroom& p_att);


protected:
	vector<AttrBathroom> m_allTypeBathrooms; //所有卫生间

};
