#include "StdAfx.h"
#include <algorithm>
#include "WindowTable.h"
#include "WindowAutoName.h"
#include "..\..\Common\ComFun_Def.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_RectArray.h"
#include "..\..\Src\DocumentData.h"


//////////////////////////////////////////////////////////////////////////
CWindowAndCount::CWindowAndCount()
{
	nCount = 0;
}
bool WindowAndCountLessFunc(const CWindowAndCount& p_winC1, const CWindowAndCount& p_winC2)
{
	int nRet = p_winC1.winAtt.GetInstanceCode().CompareNoCase(p_winC2.winAtt.GetInstanceCode());

	return nRet < 0;
}

//////////////////////////////////////////////////////////////////////////
bool CWindowCountArray::InitByWindowIds(const vector<CSunacObjInCad>& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		const AttrWindow* pAtt = AttrWindow::GetWinAtt(p_winIds[i].m_winId, true);
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			attTemp.SetMxMirror(p_winIds[i].m_bMxMirror);
			CString sInstanceCode = GetWindowAutoName()->GetWindowName(attTemp);
			attTemp.SetInstanceCode(sInstanceCode);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i].m_winId);
		}
	}

	return InitByWindowAtts(winAtts, winIds);
}

bool CWindowCountArray::InitByWindowIds(const vAcDbObjectId& p_winIds)
{
	vector<AttrWindow>  winAtts;
	vector<AcDbObjectId>  winIds;
	for (UINT i = 0; i < p_winIds.size(); i++)
	{
		RCWindow oneWindow;
		oneWindow.m_id = p_winIds[i];
		oneWindow.InitParameters();

		const AttrWindow* pAtt = oneWindow.GetAttributeConst();
		if (pAtt != NULL)
		{
			AttrWindow attTemp(*pAtt);
			winAtts.push_back(attTemp);

			winIds.push_back(p_winIds[i]);
		}
	}

	return InitByWindowAtts(winAtts, winIds);
}


bool CWindowCountArray::InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids)
{
	bool bSuc = true;
	assert(p_ids.size() == p_winAtts.size());
	bool bUseIds = p_ids.size() == p_winAtts.size();
	for (UINT i = 0; i < p_winAtts.size(); i++)
	{
		CString sInstanceCode = p_winAtts[i].GetInstanceCode(); //原型编号
		bool bFind = false;
		for (UINT n = 0; n < m_winCountArray.size(); n++)
		{
			if (m_winCountArray[n].winAtt.GetInstanceCode().CompareNoCase(sInstanceCode) == 0)
			{
				bFind = true;
				if (p_winAtts[i].GetViewDir() == E_VIEW_TOP) //平面图
				{
					if (0 == p_winAtts[i].m_floorInfo.GetFloorCount()) //若有未设置层数的情况，则返回false
					{
						bSuc = false;
					}

					m_winCountArray[n].nCount += p_winAtts[i].m_floorInfo.GetFloorCount();
					//楼层信息添加到原来的门窗属性中
					m_winCountArray[n].winAtt.m_floorInfo.AddFloors(p_winAtts[i].m_floorInfo.GetFloors());
				}
				else
				{
					m_winCountArray[n].nCount++;
				}

				m_winCountArray[n].objIds.append(p_ids[i]);
				break;
			}
		}

		if (bFind == false)
		{
			CWindowAndCount winNew;
			winNew.winAtt = p_winAtts[i];
			if (p_winAtts[i].GetViewDir() == E_VIEW_TOP) //平面图
			{
				winNew.nCount = p_winAtts[i].m_floorInfo.GetFloorCount();
			}
			else
			{
				winNew.nCount = 1;
			}
			winNew.objIds.append(p_ids[i]);
			m_winCountArray.push_back(winNew);
		}
	}

	//排序
	sort(m_winCountArray.begin(), m_winCountArray.end(), WindowAndCountLessFunc);

	return bSuc;
}

int CWindowCountArray::GetDoorsCount()
{
	int count = 0;
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_winCountArray[i].winAtt.GetPrototypeCode().Find(L"Door") >= 0)
		{
			count++;
		}
	}

	return count;
}

int CWindowCountArray::GetWindowsCount()
{
	int count = 0;
	for (int i = 0; i < GetCount(); i++)
	{
		if (m_winCountArray[i].winAtt.GetPrototypeCode().Find(L"Window") >= 0)
		{
			count++;
		}
	}

	return count;
}

void CSplitWindowDoorArray::SplitWindowDoor(const CWindowAndCount& winAndCount)
{
	const AttrWindow * pWinAtt = &(winAndCount.winAtt);
	if (pWinAtt->GetPrototypeCode().Find(L"NC") >= 0 || pWinAtt->GetInstanceCode().Find(L"NZC") >= 0)
	{
		m_winNC.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"NDC") >= 0 || pWinAtt->GetInstanceCode().Find(L"NDZC") >= 0)
	{
		m_winNDC.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"WC") >= 0 || pWinAtt->GetInstanceCode().Find(L"WZC") >= 0)
	{
		m_winWC.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"TC") >= 0)
	{
		m_winTC.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"TLM") >= 0 && pWinAtt->GetInstanceCode().Find(L"TS") < 0)
	{
		m_doorTLM.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"TSTLM") >= 0)
	{
		m_doorTSTLM.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"WM") >= 0)
	{
		m_doorWM.push_back(winAndCount);
	}
	else if (pWinAtt->GetPrototypeCode().Find(L"WLC") >= 0)
	{
		m_doorWLC.push_back(winAndCount);
	}
}

void CSplitWindowDoorArray::ClearSplitWindowDoor()
{
	m_winNC.clear();
	m_winNDC.clear();
	m_winWC.clear();
	m_winTC.clear();
	m_doorTLM.clear();
	m_doorTSTLM.clear();
	m_doorWM.clear();
	m_doorWLC.clear();
}



