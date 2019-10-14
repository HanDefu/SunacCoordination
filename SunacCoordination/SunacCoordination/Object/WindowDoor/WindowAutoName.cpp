#include "StdAfx.h"
#include "WindowAutoName.h"


//////////////////////////////////////////////////////////////////////////
CWindowAutoName::CWindowAutoName()
{
}

CWindowAutoName::~CWindowAutoName()
{
}

CString CWindowAutoName::GetWinodwName(const AttrWindow& p_att)
{
	CString sWindowName;
	//1.寻找当前相同的窗型（原型名称，窗户尺寸、窗户属性完全相同


	//2若找到则直接返回找到的窗型的编号名称


	//3.1若没有找到则在窗户编号库中找到和当前窗户原型名称、窗户尺寸相同的窗型

	//3.2 最后的编号取找到的窗型的编号+1

	//3.3 将当前窗型加到窗型库中

	return sWindowName;
}
