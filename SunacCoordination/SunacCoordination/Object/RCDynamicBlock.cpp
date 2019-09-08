/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCDynamicBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCDynamicBlock.h"
#include "float.h"
#include <algorithm>
#include "../Common/ComFun_DynamicBlock.h"
#include "../Common/ComFun_Sunac.h"

//Constructor
RCDynamicBlock::RCDynamicBlock(void)
{

}

//Destructor
RCDynamicBlock::~RCDynamicBlock(void)
{

}


//Constructor
RCDynamicBlock::RCDynamicBlock(const RCDynamicBlock &other):RCBlock(other)
{
	m_dKeyValues = other.m_dKeyValues;
	m_iKeyValues = other.m_iKeyValues;
	m_strKeyValues = other.m_strKeyValues;
}

//Operator = 
RCDynamicBlock & RCDynamicBlock::operator=(const RCDynamicBlock &other)
{
	m_dKeyValues = other.m_dKeyValues;
	m_iKeyValues = other.m_iKeyValues;
	m_strKeyValues = other.m_strKeyValues;
	return *this;
}

int RCDynamicBlock::SetParameter(CString key, double value)
{
	int error = -1;
	for (int m = 0; m < m_dKeyValues.size(); m++)
	{
        if (key == m_dKeyValues[m].first)
        {
			m_dKeyValues[m].second = value;
			error = 0;
			break;
        }
	}

	return error; 
}

int RCDynamicBlock::SetParameter(CString key, int value)
{
	int error = -1;
	for (int m = 0; m < m_iKeyValues.size(); m++)
	{
		if (key == m_iKeyValues[m].first)
		{
			m_iKeyValues[m].second = value;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::GetParameter(CString key, CString &value)
{
	int error = -1;
	for (int m = 0; m < m_strKeyValues.size(); m++)
	{
		if (key == m_strKeyValues[m].first)
		{
			value = m_strKeyValues[m].second;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::GetParameter(CString key, double &value)
{
	int error = -1;
	for (int m = 0; m < m_dKeyValues.size(); m++)
	{
		if (key == m_dKeyValues[m].first)
		{
			value = m_dKeyValues[m].second;
			error = 0;
			break;
		}
	}

	return error; 
}

int RCDynamicBlock::GetParameter(CString key, int &value)
{
	int error = -1;
	for (int m = 0; m < m_iKeyValues.size(); m++)
	{
		if (key == m_iKeyValues[m].first)
		{
			value = m_iKeyValues[m].second;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::SetParameter(CString key, CString value)
{
	int error = -1;
	for (int m = 0; m < m_strKeyValues.size(); m++)
	{
		if (key == m_strKeyValues[m].first)
		{
			m_strKeyValues[m].second = value;
			error = 0;
			break;
		}
	}
	return error;
}

int RCDynamicBlock::InitParameters()
{
	Acad::ErrorStatus es = acDocManager->lockDocument(curDoc());
	
	if (m_id == 0)
		return -1;

	if(!TYCOM_IsDyanamicBlock(m_id))
		return -1;

	//es = acdbOpenObject(pEnt1, m_id, AcDb::kForWrite);


	m_strKeyValues.clear();
	m_iKeyValues.clear();
	m_dKeyValues.clear();

	//Acad::ErrorStatus es;
	AcDbEntity* pEnt = NULL;
	es = acdbOpenObject(pEnt, m_id, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return 1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		pEnt->close();
		return 1;
	}

	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	// initialise a AcDbDynBlockReference from the object id of the blockreference
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());
	//Don't forget to close the blockreference here, otherwise you wont be able to modify properties
	pEnt->close();


	AcDbObjectId idBlkTblRcd= pDynBlkRef->dynamicBlockTableRecord();
	AcDbObjectPointer<AcDbBlockTableRecord>pBlkTblRcd11(idBlkTblRcd, AcDb::kForRead);
	if (pBlkTblRcd11.openStatus()!=Acad::eOk)
	{
		return 1;
	}

	ACHAR *pName = 0;
	pBlkTblRcd11->getName(pName);
	m_blockRecordName = pName;

	bool noValue = true;
	if (pDynBlkRef)
	{
		AcDbDynBlockReferencePropertyArray blkPropAry;
		pDynBlkRef->getBlockProperties(blkPropAry);
		AcDbDynBlockReferenceProperty blkProp;
		for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
		{
			blkProp = blkPropAry[lIndex1];

			AcString a = blkProp.propertyName();
			CString aa(a.kszPtr());
			AcDbEvalVariant va;
			
			if (blkProp.propertyType() == AcDb::kDwgInt32)
			{
				va = blkProp.value();
				int value = va.resval.rint;
				m_iKeyValues.push_back(RCPairKeyIValue(aa, value));
			}
			else if (blkProp.propertyType() == AcDb::kDwgReal)
			{
				va = blkProp.value();
				double value = va.resval.rreal;
				m_dKeyValues.push_back(RCPairKeyDValue(aa, value));
			}
			else if (blkProp.propertyType() == AcDb::kDwgText)
			{
				va = blkProp.value();
				CString value(va.resval.rstring);
				m_strKeyValues.push_back(RCPairKeyStrValue(aa, value));
			}
			
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	pBlkTblRcd11->close();
	es = acDocManager->unlockDocument(curDoc());
	return noValue;
}

int RCDynamicBlock::RunParameters()
{
	Acad::ErrorStatus es = acDocManager->lockDocument(curDoc());
	if (m_id == 0)
		return -1;

	AcDbEntity* pEnt = NULL;
	es = acdbOpenObject(pEnt, m_id, AcDb::kForRead);
	if (es != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		pEnt->close();
		return -1;
	}
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	// initialise a AcDbDynBlockReference from the object id of the blockreference
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());
	//Don't forget to close the blockreference here, otherwise you wont be able to modify properties
	pEnt->close();
	if (pDynBlkRef)
	{
		AcDbDynBlockReferencePropertyArray blkPropAry;
		pDynBlkRef->getBlockProperties(blkPropAry);
		Acad::ErrorStatus err;
		AcDbDynBlockReferenceProperty blkProp;
		for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
		{
			blkProp = blkPropAry[lIndex1];
			if (blkProp.readOnly())
				continue;

			for (int m = 0; m < m_dKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_dKeyValues[m].first) != 0) continue;

				AcDbEvalVariant eval(m_dKeyValues[m].second);
				es = blkProp.setValue(eval);
				if (es != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}

			for (int m = 0; m < m_iKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_iKeyValues[m].first) != 0) continue;

				AcDbEvalVariant eval(m_iKeyValues[m].second);
				if ((err = blkProp.setValue(eval)) != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}

			for (int m = 0; m < m_strKeyValues.size(); m++)
			{
				//look for the relevant property
				if (wcscmp(blkProp.propertyName().kACharPtr(), m_strKeyValues[m].first) != 0) continue;

				AcDbEvalVariant eval(m_strKeyValues[m].second);
				if ((err = blkProp.setValue(eval)) != Acad::eOk)
				{
					acutPrintf(L"\nError setting property value...");
				}
				break;
			}
			
		}

		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}

	actrTransactionManager->flushGraphics();//必须lock住文档才有效果
	acedUpdateDisplay();
	es = acDocManager->unlockDocument(curDoc());
	return 0;
}

bool RCDynamicBlock::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCDynamicBlock * pObj = dynamic_cast<RCDynamicBlock *>(other);
	if (pObj == 0)
		return false;

	if (!RCBlock::isEqualTo(other))
		return false;

	if (!TY_IsPairsEqual(m_dKeyValues, pObj->m_dKeyValues))
		return false;

	if (!TY_IsPairsEqual(m_iKeyValues, pObj->m_iKeyValues))
		return false;

	if (!TY_IsPairsEqual(m_strKeyValues, pObj->m_strKeyValues))
		return false;
	
	return true;
}

