#include "stdafx.h"
#include "PrototypeInfo.h"


CProBase::CProBase()
{
	m_bIsDynamic = true; //默认动态
}


CProBase::~CProBase()
{
}


void CProBase::AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
		for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
			AddSize(i, j);
}

void CProBase::AddSize(int p_xLen, int p_yLen)
{
	PrototypeSize tempSize;
	tempSize.xLen = p_xLen;
	tempSize.yLen = p_yLen;
	m_sizeList.push_back(tempSize);
}

void CProBase::DeleteSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
		for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
			DeleteSize(i, j);
}

void CProBase::DeleteSize(int p_xLen, int p_yLen)
{
	for (UINT i = 0; i < m_sizeList.size();)
	{
		if (m_sizeList[i].xLen == p_xLen && m_sizeList[i].yLen == p_yLen)
			m_sizeList.erase(m_sizeList.begin() + i);
		else
			i++;
	}
}

bool CProBase::MatchPrototype(int p_xLen, int p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos)
{
	if ((p_doorPos - m_doorPos) % 2 != 0)
		swap(p_xLen, p_yLen);

	bool bFind = false;
	for (UINT i = 0; i < m_sizeList.size(); i++)
	{
		if (m_sizeList[i].xLen == p_xLen && m_sizeList[i].yLen == p_yLen)
			bFind = true;
	}
	if (!bFind)
		return false;

	E_DIRECTION newWindowPos = E_DIRECTION((p_windowPos - p_doorPos + m_doorPos + 4) % 4);
	if ((newWindowPos - m_windowPos) % 2 != 0)
		return false;

	return true;
}

eWindowDoorPos CProBase::GetWindowDoorPos() const
{
	if (abs(m_doorPos - m_windowPos) == 2)
		return DUIKAI;
	return CHUIZHIKAI;
}

bool CProBase::GetRotateAngle(E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, int& p_angle, bool& p_needMirror)
{
	E_DIRECTION newWindowPos = E_DIRECTION((p_windowPos - p_doorPos + m_doorPos + 4) % 4);

	if ((newWindowPos - m_windowPos) % 2 != 0)
		return false;
	else if (newWindowPos == m_windowPos)
		p_needMirror = false;
	else
		p_needMirror = true;

	p_angle = ((p_doorPos - m_doorPos + 4) % 4) * 90;

	return true;
}


CProMrg* CProMrg::GetInstance()
{
	static CProMrg instance;
	return &instance;
}

CProBase* CProMrg::GetPrototypeByFileName(CString p_sFileName)
{
	//暂时只有厨卫
	CProBase* pRet = NULL;

	pRet = GetProKitchenByFileName(p_sFileName);
	if (pRet != NULL)
		return pRet;

	pRet = GetProBathroomByFileName(p_sFileName);
	return pRet;
}

CProBathroom* CProMrg::GetProBathroomByFileName(CString p_sFileName)
{
	for (UINT i = 0; i < m_allProBathrooms.size(); i++)
	{
		if (m_allProBathrooms[i].m_sFileName == p_sFileName)
			return &m_allProBathrooms[i];
	}
	return NULL;
}

void CProMrg::AddProBathroom(const CProBathroom& p_proBathroom)
{
	for (UINT i = 0; i < m_allProBathrooms.size(); i++)
	{
		//如果原型已经存在，更新原型
		if (m_allProBathrooms[i].m_sFileName == p_proBathroom.m_sFileName)
		{
			m_allProBathrooms[i] = p_proBathroom;
			return;
		}
	}
	m_allProBathrooms.push_back(p_proBathroom);
}

CProKitchen* CProMrg::GetProKitchenByFileName(CString p_sFileName)
{
	for (UINT i = 0; i < m_allProKitchens.size(); i++)
	{
		if (m_allProKitchens[i].m_sFileName == p_sFileName)
			return &m_allProKitchens[i];
	}
	return NULL;
}

void CProMrg::AddProKitchen(const CProKitchen& p_proKitchen)
{
	for (UINT i = 0; i < m_allProKitchens.size(); i++)
	{
		//如果原型已经存在，更新原型
		if (m_allProKitchens[i].m_sFileName == p_proKitchen.m_sFileName)
		{
			m_allProKitchens[i] = p_proKitchen;
			return;
		}
	}
	m_allProKitchens.push_back(p_proKitchen);
}

void CProBathroom::AddSize(int p_xLen, int p_yLen)
{
	//卫生间X边长不能大于Y边长
	if (p_xLen > p_yLen)
		return;
	CProBase::AddSize(p_xLen, p_yLen);
}
