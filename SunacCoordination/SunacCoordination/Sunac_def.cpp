#include "StdAfx.h"
#include <algorithm>
#include "Sunac_def.h"
#include "Common/ComFun_String.h"

CFloorInfo::CFloorInfo()
{
	m_floorHeight = 2900;
}


bool CFloorInfo::SetFloorHeight(double p_height)
{ 
	if (p_height>100 &&p_height<10000)
	{
		m_floorHeight = p_height;
		return true;
	} 

	return false;
}

bool CFloorInfo::SetFloors(CString p_sFloors)
{
	if (p_sFloors != p_sFloors.SpanIncluding(_T("0123456789, -")))
	{
		return false;
	}

	vector<int> floors;
	std::vector<CString> strs = YT_SplitCString(p_sFloors, L',');
	for (UINT i = 0; i < strs.size(); i++)
	{
		int nPos = strs[i].Find(_T('-'));
		if (nPos >= 0) //区间
		{
			CString str1 = strs[i].Left(nPos);
			CString str2 = strs[i].Mid(nPos+1);
			int nStart = _ttoi(str1);
			int nEnd = _ttoi(str2);
			if (nStart>nEnd || nStart==0)
			{
				return false;
			}

			for (int n = nStart; n <= nEnd; n++)
			{
				floors.push_back(n);
			}
		}
		else
		{
			int nFloor = _ttoi(strs[i]);
			if (nFloor == 0)
			{
				return false;
			}

			floors.push_back(nFloor);
		}
	}

	//去除重复元素
	sort(floors.begin(), floors.end());
	floors.erase(unique(floors.begin(), floors.end()), floors.end());

	m_sFloors = p_sFloors;
	m_nAllFloors = floors;

	return true;
}