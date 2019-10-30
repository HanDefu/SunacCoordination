#include "StdAfx.h"
#include "RailingStatistic.h"
#include "../../Common/ComFun_Interactive.h"
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include <dbtable.h>
#include <algorithm>

int CRailingStatistic::SelectRailings()
{
	m_allRailings.clear();
	m_amount.clear();

	vAcDbObjectId ids;
	JHCOM_SelectEnts(ids);

	//查找块定义
	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbEntity* pAttr = NULL;
		if (acdbOpenObject(pAttr, ids[i], AcDb::kForRead) == Acad::eOk)
		{
			AcDbBlockReference* pBlockRef = AcDbBlockReference::cast(pAttr);
			if (pBlockRef != NULL)
				ids[i] = pBlockRef->blockTableRecord();
			pAttr->close();
		}
	}

	int count = 0;
	for (UINT i = 0; i < ids.size(); i++)
	{
		AcDbObject* pAttr = NULL;
		TY_GetAttributeData(ids[i], pAttr);
		AttrRailing* pAttrRailing = AttrRailing::cast(pAttr);
		if (pAttrRailing == NULL)
			continue;
		count++;
		InsertRailing(pAttrRailing);
	}

	return count;
}

void CRailingStatistic::InsertRailing(AttrRailing* pAttr)
{
	for (UINT i = 0; i < m_allRailings.size(); i++)
	{
		if (m_allRailings[i].m_instanceCode == pAttr->m_instanceCode)
		{
			m_amount[i]++;
			return;
		}
	}
	m_allRailings.push_back(*pAttr);
	m_amount.push_back(1);
}

AcDbObjectId CRailingStatistic::InsertTableToCAD(AcGePoint3d insertPos)
{
	const double c_tableCellWidth[] = {20, 80, 30, 20, 50};
	const double c_tableCellHeight = 6;

	sort(m_allRailings.begin(), m_allRailings.end(), RailingCmp);

	Acad::ErrorStatus es;
	AcDbDictionary *pDict = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTableStyleDictionary(pDict, AcDb::kForRead);
	AcDbObjectId idTblStyle;
	es = pDict->getAt(_T("Standard"), idTblStyle);
	pDict->close();

	AcDbTable* pTable = new AcDbTable;
	pTable->setTableStyle(idTblStyle);

	pTable->suppressTitleRow(false);//标题需要保留
	pTable->suppressHeaderRow(true); //表头不需要

	int rowSize = int(m_allRailings.size()) + 2;
	int columSize = 5;
	pTable->setNumColumns(columSize);
	pTable->setNumRows(rowSize);

	//定义行高
	pTable->setVertCellMargin(1);
	for (int i = 0; i < rowSize; i++)
	{
		pTable->setRowHeight(i, c_tableCellHeight);
	}

	//定义列宽
	for (int i = 0; i < rowSize; i++)
	{
		pTable->setColumnWidth(i, c_tableCellWidth[i]);
	}

	for (int nRow = 0; nRow < rowSize; nRow++)
	{
		for (int nColum = 0; nColum < columSize; nColum++)
		{
			pTable->setAlignment(nRow, nColum, AcDb::kMiddleCenter);
		}
	}

	// 设置文字样式
	AcDbObjectId stylerid = MD2010_GetTextStyler(_T("CCFS"));
	if (stylerid != AcDbObjectId::kNull)
	{
		pTable->setTextStyle(stylerid);
	}

	pTable->generateLayout();

	//设置标题
	pTable->setTextString(0, 0, L"栏杆算量表");
	pTable->setTextString(1, 0, L"序号");
	pTable->setTextString(1, 1, L"栏杆编号");
	pTable->setTextString(1, 2, L"长度");
	pTable->setTextString(1, 3, L"数量");
	pTable->setTextString(1, 4, L"备注");
	for (UINT i = 0; i < m_allRailings.size(); i++)
	{
		CString sNum;
		sNum.Format(L"%d", i + 1);
		pTable->setTextString(i + 2, 0, sNum);
		pTable->setTextString(i + 2, 1, m_allRailings[i].m_instanceCode);
		sNum.Format(L"%.0lf", m_allRailings[i].m_length);
		pTable->setTextString(i + 2, 2, sNum);
		sNum.Format(L"%d", m_amount[i]);
		pTable->setTextString(i + 2, 3, sNum);
	}

	pTable->setPosition(insertPos);

	pTable->setRegen();

	return JHCOM_PostToModelSpace(pTable);
}

bool RailingCmp(const AttrRailing& x1, const AttrRailing& x2)
{
	return (x1.m_instanceCode < x2.m_instanceCode);
}
