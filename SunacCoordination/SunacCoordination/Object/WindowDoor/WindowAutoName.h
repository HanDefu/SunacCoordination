#pragma once
#include <vector>
#include "AttrWindow.h"

using namespace std;


class CWindowAutoName
{
	CWindowAutoName();
public:
	~CWindowAutoName();

	static CWindowAutoName* GetInstance();

	//自动获取名字
	CString GetWindowName(const AttrWindow& p_att) const;

	//加入窗型列表中
	void AddWindowType(const AttrWindow& p_att);

	//检查当前名称是否合理，如有同名的其它窗型则返回false
	bool IsNameValid(const AttrWindow& p_att, CString p_sName) const;

	//将所有窗型重新自动命名
	void AutoNameAllWindow();

	//将指定窗型重命名
	bool RenameWindow(const AttrWindow& p_att);


protected:
	vector<AttrWindow> m_allTypeWindows; //所有窗型

};
