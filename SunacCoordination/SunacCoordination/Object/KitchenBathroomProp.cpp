
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "KitchenBathroomProp.h"
#include "../Common/ComFun_Sunac.h"



CKitchenBathroomProp::CKitchenBathroomProp()
{
	m_doorPos = E_DIR_BOTTOM;
	m_windowPos = E_DIR_TOP;
}


CKitchenBathroomProp::~CKitchenBathroomProp()
{

}



void CKitchenBathroomProp::AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
		for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
			AddSize(i, j);
}

void CKitchenBathroomProp::AddSize(int p_xLen, int p_yLen)
{
	PrototypeSize tempSize;
	tempSize.xLen = p_xLen;
	tempSize.yLen = p_yLen;
	m_sizeList.push_back(tempSize);
}

void CKitchenBathroomProp::DeleteSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
	for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
		DeleteSize(i, j);
}

void CKitchenBathroomProp::DeleteSize(int p_xLen, int p_yLen)
{
	for (UINT i = 0; i < m_sizeList.size();)
	{
		if (m_sizeList[i].xLen == p_xLen && m_sizeList[i].yLen == p_yLen)
			m_sizeList.erase(m_sizeList.begin() + i);
		else
			i++;
	}
}

bool CKitchenBathroomProp::MatchPrototype(int p_xLen, int p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos)
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

eWindowDoorPos CKitchenBathroomProp::GetWindowDoorPos() const
{
	if (abs(m_doorPos - m_windowPos) == 2)
		return DUIKAI;
	return CHUIZHIKAI;
}

bool CKitchenBathroomProp::GetRotateAngle(E_DIRECTION p_doorPos, E_DIRECTION p_windowPos, int& p_angle, bool& p_needMirror)
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

