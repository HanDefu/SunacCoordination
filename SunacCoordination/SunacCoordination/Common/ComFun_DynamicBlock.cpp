#include "stdafx.h"
#include "ComFun_Str.h"
#include "ComFun_Math.h"
#include "ComFun_Def.h"
#include <acutmem.h>
#include "ComFun_ACAD_Common.h"
#include <dbmline.h>
#include <dbhatch.h>


bool TYCOM_IsDyanamicBlock(AcDbObjectId eId)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return false;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
		pEnt->close();
		return false;
	}
	AcDbEvalVariant var;
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	if(pBlkRef)
	{
		// initialise a AcDbDynBlockReference from the object id of the blockreference
		AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());
		if(pDynBlkRef)
		{
			//Don't forget to close the blockreference here, otherwise you wont be able to modify properties
			bool isb = pDynBlkRef->isDynamicBlock();//是不是动态块？
			if(isb)
			{
				pEnt->close();
				delete pDynBlkRef;
				return true;
			}
			delete pDynBlkRef;
		}
	}
	pEnt->close();
	return false;
}

CString TYCOM_GetDynaBlkName(AcDbObjectId eId)
{
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(eId);

	AcDbObjectId idBlkTblRcd= pDynBlkRef->dynamicBlockTableRecord();
	AcDbObjectPointer<AcDbBlockTableRecord>pBlkTblRcd11(idBlkTblRcd, AcDb::kForRead);
	if (pBlkTblRcd11.openStatus()!=Acad::eOk)
	{
		delete pDynBlkRef;
		return L"";
	}
	ACHAR* szName = NULL;
	Acad::ErrorStatus es = pBlkTblRcd11->getName(szName);
	//Don't forget to delete this reference, otherwise you will have problems.
	delete pDynBlkRef;
	return CString(szName);
}

int TYCOM_GetDynamicBlockName(AcDbBlockReference *pBlkRef, CString &name)
{
	// initialise a AcDbDynBlockReference from the object id of the blockreference
	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pBlkRef->objectId());

	AcDbObjectId idBlkTblRcd= pDynBlkRef->dynamicBlockTableRecord();
	AcDbObjectPointer<AcDbBlockTableRecord>pBlkTblRcd11(idBlkTblRcd, AcDb::kForRead);
	if (pBlkTblRcd11.openStatus()!=Acad::eOk)
	{
		return 1;
	}
	ACHAR* szName = NULL;
	Acad::ErrorStatus es = pBlkTblRcd11->getName(szName);
	name = CString(szName);
	delete pDynBlkRef;
	return 0;
}

int TYCOM_GetDynamicBlockName(AcDbObjectId entId, CString &name)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, entId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return -1;
	}
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	pEnt->close();

	if(pBlkRef)
	{
		TYCOM_GetDynamicBlockName(pBlkRef, name);
		return 0;
	}
	return -2;
}

int TYCOM_CycleAllDynamicBlockReferencesInRect(vAcDbObjectId &vids, AcGePoint3d lb, AcGePoint3d rt)
{
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);

	AcDbEntity *pEnt;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		AcDbBlockReference *pblk = AcDbBlockReference::cast(pEnt);
		if(pblk)
		{
			AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(pblk->objectId());
			bool isb = pDynBlkRef->isDynamicBlock();//是不是动态块？

			if(isb != NULL)
			{
				AcGePoint3d minPt, maxPt;
				JHCOM_GetObjectMinMaxPoint(pEnt->objectId(), minPt, maxPt);
				if(MD2010_CheckPointIsInRect(lb,rt,minPt) && MD2010_CheckPointIsInRect(lb,rt,maxPt))
					vids.push_back(pEnt->objectId());
			}
			delete pDynBlkRef;
		}
		pEnt->close();
	}
	delete pItr; 
	pBlkTblRcd->close();
	return 0;
}

int TYCOM_CycleAllDynamicBlockReferencesInLayer(vAcDbObjectId &vids, CString layer)
{
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);

	AcDbEntity *pEnt;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		AcDbBlockReference *pblk = AcDbBlockReference::cast(pEnt);
		if(pblk)
		{
			if(CString(pblk->layer()) == layer)
				vids.push_back(pEnt->objectId());
		}
		pEnt->close();
	}
	delete pItr; 
	pBlkTblRcd->close();
	return 0;
}

int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, double value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
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
			//look for the relevant property
			if (wcscmp(blkProp.propertyName().kACharPtr(), name) != 0) continue;
			//Get allowed values for property
			AcDbEvalVariantArray evalAry;

			AcDbEvalVariant eval(value);
			if (!blkProp.readOnly())
			{
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

	return 0;
}

int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, CString value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
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
			//look for the relevant property
			if (wcscmp(blkProp.propertyName().kACharPtr(), name) != 0) continue;
			//Get allowed values for property
			AcDbEvalVariantArray evalAry;

			AcDbEvalVariant eval(value);
			if (!blkProp.readOnly())
			{
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

	return 0;
}

int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, Adesk::Int32 value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
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
			//look for the relevant property
			if (wcscmp(blkProp.propertyName().kACharPtr(), name) != 0) continue;
			//Get allowed values for property
			AcDbEvalVariantArray evalAry;

			AcDbEvalVariant eval(value);
			if (!blkProp.readOnly())
			{
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

	return 0;
}

//设置枚举
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, short index)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return -1;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
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
			//look for the relevant property
			if (wcscmp(blkProp.propertyName().kACharPtr(), name) != 0) continue;

			//Get allowed values for property
			AcDbEvalVariantArray evalAry;
			if ((err = blkProp.getAllowedValues(evalAry)) == Acad::eOk)
			{
				if (evalAry.length() >= index)
				{
					AcDbEvalVariant eval = evalAry[index];
					if (!blkProp.readOnly())
					{
						if ((err = blkProp.setValue(eval)) != Acad::eOk)
						{
							acutPrintf(L"\nError setting property value...");
						}
					}
				}
			}
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	return 0;
}

int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, double &value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
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
			CString aa(a.constPtr());
			AcDbEvalVariant va;
			if (aa == key)
			{
				va = blkProp.value();
				value = va.resval.rreal;
				noValue = false;
				break;
			}
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	return noValue;
}

int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, int &value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
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
			CString aa(a.constPtr());
			AcDbEvalVariant va;
			if (aa == key)
			{
				va = blkProp.value();
				value = va.resval.rint;
				noValue = false;
				break;
			}
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	return noValue;
}


int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, bool &value)
{
	int intvalue = 0;
	int ret = TYCOM_GetDynamicBlockData(eId, key, intvalue);
	if (ret != 0)
		return ret;
	if (intvalue > 0)
		value = true;
	else
		value = false;
	
	return 0;
}

int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, CString &value)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
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
			CString aa(a.constPtr());
			AcDbEvalVariant va;
			if (aa == key)
			{
				va = blkProp.value();
				value = va.resval.rstring;
				noValue = false;
				break;
			}
		}
		//Don't forget to delete this reference, otherwise you will have problems.
		delete pDynBlkRef;
	}
	return noValue;
}
