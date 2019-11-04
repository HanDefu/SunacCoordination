
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
	m_minX = m_minY = 10000;
	m_maxX = m_maxY = 0;
}


CKitchenBathroomProp::~CKitchenBathroomProp()
{

}



void CKitchenBathroomProp::AddSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	/*for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
		for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
			AddSize(i, j);*/
	AddSize(p_minXLen, p_minYLen);
	AddSize(p_maxXLen, p_maxYLen);
}

void CKitchenBathroomProp::AddSize(int p_xLen, int p_yLen)
{
	/*PrototypeSize tempSize;
	tempSize.xLen = p_xLen;
	tempSize.yLen = p_yLen;
	m_sizeList.push_back(tempSize);*/
	m_maxX = max(m_maxX, p_xLen);
	m_minX = min(m_minX, p_xLen);
	m_maxY = max(m_maxY, p_yLen);
	m_minY = min(m_minY, p_yLen);
}

void CKitchenBathroomProp::DeleteSizeRange(int p_minXLen, int p_minYLen, int p_maxXLen, int p_maxYLen, int p_step /*= 150*/)
{
	for (int i = p_minXLen; i <= p_maxXLen; i += p_step)
	for (int j = p_minYLen; j <= p_maxYLen; j += p_step)
		DeleteSize(i, j);
}

void CKitchenBathroomProp::DeleteSize(int p_xLen, int p_yLen)
{
	/*for (UINT i = 0; i < m_sizeList.size();)
	{
		if (m_sizeList[i].xLen == p_xLen && m_sizeList[i].yLen == p_yLen)
			m_sizeList.erase(m_sizeList.begin() + i);
		else
			i++;
	}*/
	PrototypeSize tempSize;
	tempSize.xLen = p_xLen;
	tempSize.yLen = p_yLen;
	m_sizeList.push_back(tempSize);
}

bool CKitchenBathroomProp::MatchPrototype(int p_xLen, int p_yLen, E_DIRECTION p_doorPos, E_DIRECTION p_windowPos)
{
	if ((p_doorPos - m_doorPos) % 2 != 0)
		swap(p_xLen, p_yLen);

	if (p_xLen > m_maxX || p_xLen < m_minX)
		return false;
	if (p_yLen > m_maxY || p_yLen < m_minY)
		return false;

	//bool bFind = false;
	for (UINT i = 0; i < m_sizeList.size(); i++)
	{
		if (m_sizeList[i].xLen == p_xLen && m_sizeList[i].yLen == p_yLen)
			//bFind = true;
			return false;
	}
	//if (!bFind)
	//	return false;

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

Acad::ErrorStatus CKitchenBathroomProp::ReadFromDwg(AcDbDwgFiler* filer)
{
	filer->readItem((Adesk::UInt32*)&m_doorPos);
	filer->readItem((Adesk::UInt32*)&m_windowPos);
	Adesk::UInt32 size;
	filer->readItem(&size);
	m_sizeList.resize(size);
	for (UINT i = 0; i < m_sizeList.size(); i++)
	{
		filer->readItem(&m_sizeList[i].xLen);
		filer->readItem(&m_sizeList[i].yLen);
	}

	filer->readItem(&m_minX);
	filer->readItem(&m_maxX);
	filer->readItem(&m_minY);
	filer->readItem(&m_maxY);

	return Acad::eOk;
}

Acad::ErrorStatus CKitchenBathroomProp::WriteToDwg(AcDbDwgFiler* filer) const
{
	filer->writeItem((Adesk::UInt32)m_doorPos);
	filer->writeItem((Adesk::UInt32)m_windowPos);
	filer->writeItem((Adesk::UInt32)m_sizeList.size());
	for (UINT i = 0; i < m_sizeList.size(); i++)
	{
		filer->writeItem(m_sizeList[i].xLen);
		filer->writeItem(m_sizeList[i].yLen);
	}

	filer->writeItem(m_minX);
	filer->writeItem(m_maxX);
	filer->writeItem(m_minY);
	filer->writeItem(m_maxY);

	return Acad::eOk;
}

