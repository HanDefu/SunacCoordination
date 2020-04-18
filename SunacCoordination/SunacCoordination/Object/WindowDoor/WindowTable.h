#pragma once
#include "WindowSelect.h"


class CWindowAndCount
{
public:
	CWindowAndCount();

public:
	AttrWindow winAtt;
	int nCount;

	AcDbObjectIdArray objIds;
};

class CWindowCountArray
{
	bool InitByWindowIds(const vAcDbObjectId& p_winIds);
public:
	bool InitByWindowIds(const vector<CSunacObjInCad>& p_winIds);
	bool InitByWindowAtts(const vector<AttrWindow>& p_winAtts, const vector<AcDbObjectId>& p_ids);

	int GetCount()const { return (int)(m_winCountArray.size()); }

	//门的数量
	int GetDoorsCount();

	//窗的数量
	int GetWindowsCount();

	const CWindowAndCount& GetWindow(int p_index) { return m_winCountArray[p_index]; }

private:
	vector<CWindowAndCount> m_winCountArray;
};

class CSplitWindowDoorArray
{
public:
	void SplitWindowDoor(int p_numWindowDoor, const CWindowAndCount& winAndCount); //将m_winCountArray拆分成各个类型的门窗(NC、NDC...)
	void ClearSplitWindowDoor(); //清空m_winNC等vector

	int GetWindowNCCount()const { return (int)(m_winNC.size()); }
	int GetWindowNDCCount()const { return (int)(m_winNDC.size()); }
	int GetWindowWCCount()const { return (int)(m_winWC.size()); }
	int GetWindowTCCount()const { return (int)(m_winTC.size()); }
	int GetDoorTLMCount()const { return (int)(m_doorTLM.size()); }
	int GetDoorTSTLMCount()const { return (int)(m_doorTSTLM.size()); }
	int GetDoorWMCount()const { return (int)(m_doorWM.size()); }
	int GetDoorWLCCount()const { return (int)(m_doorWLC.size()); }

	const CWindowAndCount& GetWindowNC(int p_index) { return m_winNC[p_index]; }
	const CWindowAndCount& GetWindowNDC(int p_index) { return m_winNDC[p_index]; }
	const CWindowAndCount& GetWindowWC(int p_index) { return m_winWC[p_index]; }
	const CWindowAndCount& GetWindowTC(int p_index) { return m_winTC[p_index]; }
	const CWindowAndCount& GetDoorTLM(int p_index) { return m_doorTLM[p_index]; }
	const CWindowAndCount& GetDoorTSTLM(int p_index) { return m_doorTSTLM[p_index]; }
	const CWindowAndCount& GetDoorWM(int p_index) { return m_doorWM[p_index]; }
	const CWindowAndCount& GetDoorWLC(int p_index) { return m_doorWLC[p_index]; }

private:
	vector<CWindowAndCount> m_winNC;
	vector<CWindowAndCount> m_winNDC;
	vector<CWindowAndCount> m_winWC;
	vector<CWindowAndCount> m_winTC;
	vector<CWindowAndCount> m_doorTLM;
	vector<CWindowAndCount> m_doorTSTLM;
	vector<CWindowAndCount> m_doorWM;
	vector<CWindowAndCount> m_doorWLC;
};