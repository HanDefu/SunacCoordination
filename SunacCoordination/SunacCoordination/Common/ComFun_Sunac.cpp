#include "stdafx.h"
#include "ComFun_Str.h"
#include "ComFun_Math.h"
#include "ComFun_Sunac.h"
#include "ComFun_Def.h"
#include <acutmem.h>
#include "ComFun_ACAD_Common.h"
#include <dbmline.h>
#include <dbhatch.h>
#include <acedads.h>
#include <dbidmap.h>
#include "../Common/TYRect.h"
#include "../Object\WindowDoor\AttrWindow.h"
#include "../Object\AirCondition\AttrAirCon.h"
#include "../Object\Kitchen\AttrKitchen.h"
#include "../Object\Bathroom\AttrBathroom.h"
#include "../Object/Railing/AttrRailing.h"
#include "../Tool/DocLock.h"

bool DQ_IsALine(AcDbObjectId entId)
{
	if (entId == 0)
	{
		return false;
	}

	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return false;

	AcDbLine *pLine = AcDbLine::cast(pEntity);
	pEntity->close();
	if (pLine == 0)
		return false;

	return true;
}

bool DQ_IsBlockReference(AcDbObjectId id)
{
	if (id == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}
	pEnt->close();
	return true;
}

int DQ_GetBlockReferenceInsertPoint(AcDbObjectId id, AcGePoint3d &pnt)
{
	if (id == 0)
		return -1;

	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, id, AcDb::kForRead);
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return -1;
	}
	pnt = pBR->position();
	pEnt->close();
	return 0;
}

//得到曲线和点的最近点
int DQ_GetCurveClosedPointToOneGivenPoint(AcDbObjectId curvre, AcGePoint3d givenPnt, AcGePoint3d &closedPnt)
{
	AcDbEntity* pEnt = NULL;

	if (acdbOpenObject(pEnt, curvre, AcDb::kForRead) != Acad::eOk)
	{
		if (pEnt)
			pEnt->close();
		return -1;
	}

	AcDbCurve * pCrv = AcDbCurve::cast(pEnt);
	if (pCrv == 0)
	{
		return -2;
	}
	AcGePoint3d result;
	pCrv->getClosestPointTo(givenPnt, closedPnt);
	pEnt->close();
	return 0;
}

bool DQ_GetAttributeValue( AcDbBlockReference *pBlkRef, const TCHAR* tag, CString &value )
{
	// 创建块表记录的属性遍历器
	bool bFound = false;
	AcDbObjectIterator *pItr = pBlkRef->attributeIterator();
	if (pItr)
	{
		while (!pItr->done())
		{
			AcDbObjectId attId = pItr->objectId();
			AcDbAttribute *pAtt = NULL;
			if (pBlkRef->openAttribute(pAtt, attId, AcDb::kForRead) == Acad::eOk)
			{
				// 块参照中的属性Tag永远都会是大写字母，因此传入的参数需要进行换转
				TCHAR *szTag = pAtt->tag();
				CString strTag = tag;
				strTag.MakeUpper();
				if (_tcscmp(strTag, szTag) == 0)
				{
					TCHAR *szValue = pAtt->textString();	
					value = szValue;
					acutDelString(szValue);
					bFound = true;
				}
				acutDelString(szTag);

				pAtt->close();
			}			

			pItr->step();
		}
		delete pItr;

		return bFound;
	}
	else
	{
		return false;
	}
}


bool DQ_HasAttribute_BlkDef(AcDbObjectId blockDefId, CString tag)
{

	AcDbBlockTableRecord *pBlkDefRcd = NULL;
	bool bRet = false;
	Acad::ErrorStatus es = acdbOpenObject(pBlkDefRcd, blockDefId, AcDb::kForRead);

	if (es == Acad::eOk)
	{
		// 块定义中是否包含了对应的属性
		if (pBlkDefRcd->hasAttributeDefinitions())
		{
			AcDbBlockTableRecordIterator *pItr = NULL;
			pBlkDefRcd->newIterator(pItr);
			AcDbEntity *pEnt = NULL;

			for (pItr->start(); !pItr->done(); pItr->step())
			{
				pItr->getEntity(pEnt, AcDb::kForRead);

				// 检查是否是属性定义
				AcDbAttributeDefinition *pAttDef = AcDbAttributeDefinition::cast(pEnt);
				if (pAttDef != NULL)
				{
					TCHAR* pszTag = pAttDef->tag();
					CString strTag = pszTag;
					acutDelString(pszTag);
					if (strTag.CompareNoCase(tag) == 0)
					{
						pEnt->close();
						pBlkDefRcd->close();
						return true;
					}										
				}
				pEnt->close();
			}
			delete pItr;
		}
		else
		{
			bRet = false;
		}

		pBlkDefRcd->close();
	}	

	return bRet;
}


bool DQ_HasAttribute_BlkRef(AcDbObjectId blockRefId, CString tagText)
{
	if (blockRefId == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, blockRefId, AcDb::kForRead);
	if (pEnt == 0)
		return false;
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}

	pEnt->close();

	AcDbObjectId recID = pBR->blockTableRecord();

	return DQ_HasAttribute_BlkDef(recID,tagText);

}


void DQ_SelectMany(vAcDbObjectId &ids)
{
	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	int rt = acedSSGet(NULL, NULL, NULL, NULL, ssname); // 提示用户选择对象
	if (rt == RTNORM)
	{
		Adesk::Int32 length;
		acedSSLength(ssname, &length);
		for (int i=0;i<length;i++)
		{
			acedSSName(ssname,i, ent);
			acdbGetObjectId(id, ent);
			ids.push_back(id);
		}
		acedSSFree(ssname);
	}
}

bool DQ_GetAttributeValue( AcDbObjectId blockRefId, CString tag, CString &value )
{
	if (blockRefId == 0)
	{
		return false;
	}
	AcDbEntity * pEnt;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, blockRefId, AcDb::kForRead);
	if (pEnt == 0)
		return false;
	// Make sure its a block reference
	AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
	if (pBR == NULL)
	{
		pEnt->close();
		return false;
	}
	pEnt->close();

	return DQ_GetAttributeValue( pBR, tag, value );
}

bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, double p_value)
{
	if (p_blockRefId==AcDbObjectId::kNull)
		return false;

	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(p_blockRefId);
	if (pDynBlkRef == NULL)
		return false;

	AcDbDynBlockReferencePropertyArray blkPropAry;
	pDynBlkRef->getBlockProperties(blkPropAry);

	Acad::ErrorStatus err = Acad::eInvalidInput;
	for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
	{
		AcDbDynBlockReferenceProperty blkProp = blkPropAry[lIndex1];
		if (blkProp.readOnly())
			continue;

		if (wcscmp(blkProp.propertyName().kACharPtr(), p_attributename) == 0)
		{
			AcDbEvalVariant eval(p_value);
			err = blkProp.setValue(eval);
			break;
		}
	}

	delete pDynBlkRef;

	return err == Acad::eOk;
}

bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, CString p_value)
{
	if (p_blockRefId == AcDbObjectId::kNull)
		return false;

	AcDbDynBlockReference* pDynBlkRef = new AcDbDynBlockReference(p_blockRefId);
	if (pDynBlkRef == NULL)
		return false;

	CDocLock docLock;

	AcDbDynBlockReferencePropertyArray blkPropAry;
	pDynBlkRef->getBlockProperties(blkPropAry);

	Acad::ErrorStatus err = Acad::eInvalidInput;
	for (long lIndex1 = 0L; lIndex1 < blkPropAry.length(); ++lIndex1)
	{
		AcDbDynBlockReferenceProperty blkProp = blkPropAry[lIndex1];
		if (blkProp.readOnly())
			continue;

		if (wcscmp(blkProp.propertyName().kACharPtr(), p_attributename) == 0)
		{
			AcDbEvalVariant eval(p_value);
			err = blkProp.setValue(eval);
			break;
		}
	}

	delete pDynBlkRef;

	return err == Acad::eOk;
}
//遍历操作
int MD2010_GetAllTypedObjectsInLayer(vAcDbObjectId &allEntites, CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids)
{
	// 判断是否存在名称为“测试”的图层
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForRead);
	if (!pLayerTbl->has(layname))
	{
		//acutPrintf("\n当前图形中未包含\"测试\"图层!");
		pLayerTbl->close();
		return 0;
	}

	AcDbObjectId layerId; // “测试”图层的ID
	pLayerTbl->getAt(layname, layerId);
	pLayerTbl->close();


	for (UINT i = 0; i < allEntites.size(); i++)
	{
		AcDbEntity *pEnt = 0;
		// 打开图形数据库中的对象
		acdbOpenObject(pEnt, allEntites[i], AcDb::kForRead);
		if (pEnt == 0)
			continue;

		// 是否在“测试”图层上
		if (pEnt->layerId() == layerId)
		{
			switch(type)
			{
			case ACDB_POINT:
				{
					// 是否是点
					AcDbPoint *prealobj = AcDbPoint::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_LINE:
				{
					// 是否是直线
					AcDbLine *prealobj = AcDbLine::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_CYCLE:
				{
					// 是否是圆
					AcDbCircle *prealobj = AcDbCircle::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_ARC:
				{
					// 是否是圆
					AcDbArc *prealobj = AcDbArc::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_2dPLINE:
				{
					// 是否是2维多段线
					AcDb2dPolyline *prealobj = AcDb2dPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_MLINES:
				{
					// 是否是多线段平行
					AcDbMline *prealobj = AcDbMline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_SPLINE:
				{
					// 是否是样条曲线
					AcDbSpline *prealobj = AcDbSpline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_ELLIPSE: 
				{
					// 是否是椭圆
					AcDbEllipse *prealobj = AcDbEllipse::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_TEXT: 
				{
					// 是否是文字
					AcDbText *prealobj = AcDbText::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_MTEXT:
				{
					// 是否是多段文字
					AcDbMText  *prealobj = AcDbMText ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_0PLINE:
				{
					// 是否是多段线
					AcDbPolyline *prealobj = AcDbPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
					    vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_CURVE:
				{
					// 是否是曲线
					AcDbCurve  *prealobj = AcDbCurve ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			case ACDB_HATCH:
				{
					// 是否是曲线
					AcDbHatch  *prealobj = AcDbHatch ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(allEntites[i]);
					}
					break;
				}
			default:
				break;
			}
		}
		// 注意需要关闭实体
		pEnt->close();
	}
	return 0;
}

void YT_UpdateBlockReference(AcDbObjectId &entId)
{
	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, entId, AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return ;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
		pEnt->close();
		return ;
	}
	AcDbEvalVariant var;
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	pBlkRef->recordGraphicsModified(true) ;
	//pBlkRef->draw();
	pBlkRef->close();
}

bool TY_GetPoint(AcGePoint3d &ptOut, CString prompt)
{
	ads_point pt;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,prompt,pt)!=RTNORM) //第一角点选择
	{
		ptOut = AcGePoint3d(0, 0, 0);
		return false;
	}

	ptOut = AcGePoint3d(pt[0], pt[1], pt[2]);
	return true;
}

bool TY_GetOneRect(TYRect& p_rect)
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"\n请选择第一角点\n",pt)!=RTNORM) //第一角点选择
	{
		return false;
	}

	if(acedGetCorner(pt,L"\n请选择对角点\n",result)!=RTNORM)//框选
	{
		return false;
	}

	AcGePoint3d pnt1(pt[0],pt[1],pt[2]);
	AcGePoint3d pnt2(result[0],result[1],result[2]);

	TYRect rect;
	rect.Add(pnt1);
	rect.Add(pnt2);

	p_rect = rect;
	return true;
}

bool TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2)
{
	ads_point pt,result;
	acedInitGet(32,NULL);
	if(acedGetPoint(NULL,L"请选择第一个点\n",pt)!=RTNORM) //第一角点选择
	{
		return false;
	}

	if(acedGetPoint(pt,L"请选择第二个点\n",result)!=RTNORM) //第一角点选择
	{
		return false;
	}

	pnt1 = AcGePoint3d(pt[0],pt[1],pt[2]);
	pnt2 = AcGePoint3d(result[0],result[1],result[2]);

	return true;
}

bool GetRealInput(const TCHAR* prompt, double defaultVal, int precision, double &ret)
{
	CString strPrompt = prompt;
	strPrompt.TrimRight();
	strPrompt.TrimRight(TEXT(":"));
	CString strDefaultValue;
	strDefaultValue.Format(TEXT("<%%.%df>:"), precision);	// 得到类似“%.2f”的格式字符串
	strPrompt.Format(strPrompt + strDefaultValue, defaultVal);

	ret = defaultVal;
	int rc = acedGetReal(strPrompt, &ret);
	if (rc == RTNORM || rc == RTNONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool GetStringInput(CString prompt, CString &strOut)
{
	int rcc = 0;
	ACHAR num[256] = L"\0";
	rcc = acedGetString(0, prompt, num);
	if (rcc == RTCAN)
		return false;
	else if (rcc != RTCAN&&rcc != RTNORM)
		return false;

	strOut = CString(num);
	return true;
}


AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id)
{
	acDocManager->lockDocument(curDoc());

	AcDbObjectId dicId = AcDbObjectId::kNull;

	AcDbObject *pObj = NULL;
	if(acdbOpenObject(pObj, id, AcDb::kForWrite)==Acad::eOk)
	{
		Acad::ErrorStatus es = pObj->createExtensionDictionary();
		if (es == Acad::eOk || es == Acad::eAlreadyInDb)
		{
			dicId = pObj->extensionDictionary();
		}
		pObj->close();
	}

	acDocManager->unlockDocument(curDoc());

	return dicId;
}

//把UI的信息数据写入图框
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt)
{
	if (pDataEnt == 0)
		return -2;

	AcDbObjectId dicID = TY_GetExtensionDictionaryID(Id);
	if (dicID == 0)
		return -1;


	//注意这里有时候加入不进去， 是因为没有注册
	Acad::ErrorStatus es = acDocManager->lockDocument(curDoc());

	AcDbDictionary *pDict = NULL;
	if (acdbOpenObject(pDict, dicID, AcDb::kForWrite) == Acad::eOk)
	{
		AcDbObjectId patternID = NULL;
		es =pDict->setAt(SUNAC_ATTRIBUTE_ENTITY, pDataEnt, patternID);
		pDict->close();
		pDataEnt->close();

		acDocManager->unlockDocument(curDoc());
		return 0;
	}
	else
	{
		acDocManager->unlockDocument(curDoc());
		return -3;
	}
}

int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt)
{
	AcDbObjectId m_dicID = TY_GetExtensionDictionaryID(tkId);
	if (m_dicID == 0)
		return -1;

	AcDbDictionary *pDict = NULL;
	if(acdbOpenObject(pDict, m_dicID, AcDb::kForRead)==Acad::eOk)
	{
		Acad::ErrorStatus es =pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		pDict->close();
		if(pDataEnt == NULL)
			return 68;
		pDataEnt->close();
	}
	return 0;
}

bool TY_IsWindow(AcDbObjectId Id, eViewDir p_view)
{
	AcDbObject * pDataEnt = 0;

	TY_GetAttributeData(Id, pDataEnt);
	AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
	if (pWindow == NULL)
		return false;

	if (p_view != E_VIEW_ALL)
	{
		return pWindow->GetViewDir() == p_view;
	}

	return true;
}

eRCType TY_GetType(AcDbBlockReference *pBlockReference)
{
	if (pBlockReference == 0)
		return TYPENUM;
	AcDbObject * pDataEnt = 0;
	AcDbObjectId dictid = pBlockReference->extensionDictionary();

	AcDbDictionary *pDict = NULL;
	if(acdbOpenObject(pDict, dictid, AcDb::kForRead)==Acad::eOk)
	{
		Acad::ErrorStatus es =pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		pDict->close();
		pDataEnt->close();
		if(pDataEnt == NULL)
			return TYPENUM;

		AttrWindow * pWindow = dynamic_cast<AttrWindow *>(pDataEnt);
		if (pWindow != 0)
			return WINDOW;

		AttrKitchen * pKit = dynamic_cast<AttrKitchen *>(pDataEnt);
		if (pKit != 0)
			return KITCHEN;

		AttrBathroom * pBath= dynamic_cast<AttrBathroom *>(pDataEnt);
		if (pBath != 0)
			return Bathroom;

		AttrRailing * pRail = dynamic_cast<AttrRailing *>(pDataEnt);
		if (pRail != 0)
			return RAILING;

		AttrAirCon * pAir = dynamic_cast<AttrAirCon *>(pDataEnt);
		if (pAir != 0)
			return AIRCON;

		return TYPENUM;
	}
	else
		return TYPENUM;
}

bool TY_Iskitchen(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrKitchen * pKitchen = dynamic_cast<AttrKitchen *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsRailing(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrRailing * pKitchen = dynamic_cast<AttrRailing *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsAirCon(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrAirCon * pKitchen = dynamic_cast<AttrAirCon *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}

bool TY_IsBathroom(AcDbObjectId Id)
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(Id, pDataEnt);
	AttrBathroom * pKitchen = dynamic_cast<AttrBathroom *>(pDataEnt);
	if (pKitchen != 0)
		return true;
	return false;
}



int vFind(RCPairKeyDValue &A, vRCPairKeyDValue &B)
{
	int ret = -1;
	for (UINT i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			fabs(A.second - B[i].second) < TOL)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCPairKeyIValue &A, vRCPairKeyIValue &B)
{
	int ret = -1;
	for (UINT i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			A.second == B[i].second)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCPairKeyStrValue &A, vRCPairKeyStrValue &B)
{
	int ret = -1;
	for (UINT i = 0; i < B.size(); i++)
	{
		if (A.first == B[i].first  &&
			A.second == B[i].second)
		{
			return i;
		}
	}
	return ret;
}

int vFind(RCWindow &oneWindow, vRCWindow &allWindows)
{
	for (UINT i = 0; i < allWindows.size(); i++)
	{
		if (oneWindow.isEqualTo(&(allWindows[i])))
		{
			return i;
		}
	}

	return -1;
}

bool TY_IsPairsEqual(vRCPairKeyDValue &A, vRCPairKeyDValue &B)
{
	if (A.size() != B.size())
		return false;

	for (UINT i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

bool TY_IsPairsEqual(vRCPairKeyIValue &A, vRCPairKeyIValue &B)
{
	if (A.size() != B.size())
		return false;

	for (UINT i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

bool TY_IsPairsEqual(vRCPairKeyStrValue &A, vRCPairKeyStrValue &B)
{
	if (A.size() != B.size())
		return false;

	for (UINT i = 0; i < A.size(); i++)
	{
		int index = vFind(A[i],B);
		if (index == -1)
			return false;
	}
	return true;
}

CString TY_GetDataFilePath()
{
	CString appPath = MD2010_GetAppPath();
	return appPath + L"\\Sunac2019\\Data\\";
}

CString TY_GetPrototypeFilePath()
{
#ifdef WORK_LOCAL//本地模式
	return TY_GetPrototypeFilePath_Local();
#else
	return TY_GetPrototypeFilePath_Web();
#endif
}

CString TY_GetPrototypeImagePath()
{
#ifdef WORK_LOCAL//本地模式
	return TY_GetPrototypeImagePath_Local();
#else
	return TY_GetPrototypeImagePath_Web();
#endif
}
CString TY_GetPrototypeFilePath_Local()
{
	CString sPath = MD2010_GetAppPath()+ L"\\Sunac2019\\LocalMode\\";
	return sPath;
}
CString TY_GetPrototypeImagePath_Local()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Image\\";
	return sPath;
}
CString TY_GetPrototypeFilePath_Web()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Files";
	if (!JHCom_FileExist(sPath))//判断Files文件夹是否存在，如果不存在则创建
	{
		CreateDirectory(sPath, NULL);
	}
	return sPath + L"\\";
}
CString TY_GetPrototypeImagePath_Web()
{
	CString sPath = MD2010_GetAppPath() + L"\\Sunac2019\\Files";
	if (!JHCom_FileExist(sPath))//判断Files文件夹是否存在，如果不存在则创建
	{
		CreateDirectory(sPath, NULL);
	}
	return sPath + L"\\";
}

char* TY_GetAluminumDatabasePath()
{
	CString TempPath = MD2010_GetAppPath() + L"\\Sunac2019\\Data\\AluminumData.db";
	int n = TempPath.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, TempPath, n, NULL, 0, NULL, NULL);
	char * DatabasePath = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, TempPath, n, DatabasePath, len, NULL, NULL);
	DatabasePath [len] = '\0';
	return DatabasePath;
}

char* TY_GetLocalDataDatabasePath()
{
	CString TempPath = MD2010_GetAppPath() + L"\\Sunac2019\\Data\\LocalData.db";
	int n = TempPath.GetLength();
	int len = WideCharToMultiByte(CP_ACP, 0, TempPath, n, NULL, 0, NULL, NULL);
	char * DatabasePath = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, TempPath, n, DatabasePath, len, NULL, NULL);
	DatabasePath [len] = '\0';
	return DatabasePath;
}

/************************************************************************
说明：
    在文件夹中查找文件（仅在指定文件夹查找，不递归）
参数：
    fileName:要查找的文件名。支持通配符*
    directory:要查找的文件所在的目录路径
返回值：
    pair<string,string>类型的数组，map<文件名，文件路径>
************************************************************************/
vector<pair<CString,CString>> TY_FindFilesInDirecotry(CString fileName,CString directory )
{
    vector<pair<CString,CString>> files;
 
    if(directory.Right(1) != "\\") //保证目录是以\结尾的
    {  
        directory += "\\";
    }
    directory += fileName;
    CFileFind finder;
    BOOL success = finder.FindFile(directory);
    while(success)
    {
        success = finder.FindNextFile();
        CString name = finder.GetFileName();
        CString path = finder.GetFilePath();
        pair<CString,CString> fileMap(name,path);
        files.push_back(fileMap);
    }
    finder.Close();
    return files;
}

/************************************************************************
说明：
    在文件夹中递归查找文件
参数：
    fileName:要查找的文件名。支持通配符*
    directory:要查找的文件所在的目录路径
    recursionCount:递归查找的深度，默认为-1（不限制深度）,0表示只搜索给定的directory目录
返回值：
    pair<string,string>类型的数组，map<文件名，文件路径>
************************************************************************/
vector<pair<CString,CString>> TY_FindFilesInDirecotryRecursion( CString fileName,CString directory,int recursionCount)
{
    vector<pair<CString,CString>> files;
 
    int curRecursionCount = recursionCount;
 
    //先对参数传进来的文件夹查找
    vector<pair<CString,CString>> localFiles =  TY_FindFilesInDirecotry(fileName,directory);
    files.insert(files.end(),localFiles.begin(),localFiles.end());
 
    //然后对此文件夹下面的子文件夹递归查找
    CFileFind finder;
    if(directory.Right(1) != "\\") //保证目录是以\结尾的
    {  
        directory += "\\";
    }
    directory += "*.*";
 
    BOOL res = finder.FindFile(directory);
    while (res)
    {
        res = finder.FindNextFile();
        CString path = finder.GetFilePath();
        CString name = finder.GetFileName();
        curRecursionCount = recursionCount;  //查找同级目录的时候将当前深度设置为给定值
        if(finder.IsDirectory() && !finder.IsDots())
        {
            if(curRecursionCount >= 1 || curRecursionCount == -1 )
            {
                if(curRecursionCount >= 1)
                {
                    curRecursionCount--;
                }
                localFiles = TY_FindFilesInDirecotryRecursion(fileName,path,curRecursionCount); //如果目标是个文件夹，则利用嵌套来遍历
                files.insert(files.end(),localFiles.begin(),localFiles.end());
            }
        }
    }
    finder.Close();
    return files;
}

int TYCOM_Mirror(AcDbObjectId entId, AcGePoint3d first, AcGeVector3d dir)
{
	CDocLock docLock;
	AcDbEntity *pEntity = 0;
	Acad::ErrorStatus  es = acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	if (pEntity == 0)
		return es;

	AcGeMatrix3d mx;
	mx.setToMirroring(AcGeLine3d(first,dir));

	AcDbEntity *newEnt = 0;
	pEntity->transformBy(mx);

	pEntity->close();
	return 0;
}

//旋转
int TYCOM_Rotate(AcDbObjectId entId, AcGePoint3d ptBase, double rotation)
{
	CDocLock docLock;
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	Acad::ErrorStatus  es = acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	if (pEntity == 0)
		return es;

	AcGeMatrix3d xform;
	AcGeVector3d vec(0, 0, 1);
	xform.setToRotation(rotation, vec, ptBase);

	AcDbEntity *newEnt = 0;
	pEntity->transformBy(xform);

	pEntity->close();

	return 0;
}

int TYCOM_Move(AcDbObjectId entId, AcGeVector3d offset)
{
	CDocLock docLock;
	AcGeMatrix3d xform;
	xform.setToTranslation(offset);

	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(xform);
		pEnt->close();
	}
	return 0;
}

int TYCOM_Transform(AcDbObjectId entId, const AcGeMatrix3d& mat)
{
	if (entId == AcDbObjectId::kNull)
		return 0;

	CDocLock docLock;

	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, entId, AcDb::kForWrite, false);
	if (es == Acad::eOk)
	{
		es = pEnt->transformBy(mat);
		pEnt->close();
	}
	return 0;
}

double GePlineLength(AcDbObjectId eId)
{

	AcDbEntity* pEnt = NULL;
	if (acdbOpenObject(pEnt, eId, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"\nError opening entity.");
		if (pEnt)
			pEnt->close();
		return 0;
	}
	if (pEnt->isA() != AcDbBlockReference::desc())
	{
		acutPrintf(L"\nMust select a block insert.");
		pEnt->close();
		return 0;
	}

	double lem = 0;
	AcDbBlockReference *pBlkRef = AcDbBlockReference::cast(pEnt);
	AcDbVoidPtrArray entitySet;
	pBlkRef->explode(entitySet);
	vAcDbObjectId arrayids111,arrayids222;
	for(int i = 0; i < entitySet.length(); i++)
	{
		AcDbEntity *pent = AcDbEntity::cast((AcRxObject*)entitySet[i]);
		AcDb::Visibility  vv = pent->visibility();
		if(vv == AcDb::kVisible)
		{
			AcDbPolyline *ppline = AcDbPolyline ::cast((AcRxObject*)entitySet[i]);
			//UINT color = ppline->entityColor();
			if(ppline!=NULL)
			{
				double endParm = 0;
				Acad::ErrorStatus es = ppline->getEndParam(endParm);
				if(es == Acad::eOk)
				{
					double temp = 0;
					ppline->getDistAtParam(endParm, temp);
					if(temp > lem)
						lem = temp;
				}
			}
		}
	}
	pEnt->close();
	return lem;
}

int TY_HideBlockReferencesInBlockReference(AcDbObjectId blkRefId, vCString &hideBlockRecordNames)
{
	vAcDbObjectId vids;
	ACHAR *brecname = L"";
	MD2010_GetBlockReference_Record_name(blkRefId, brecname);
	MD2010_CycleBlockEntites(brecname, vids);

	vAcDbObjectId blkrefs;
	for (UINT i = 0; i < vids.size(); i++)
	{
		if(MD2010_GetBlockReference_Record_name(vids[i], brecname) == 0)
		{
			CString internalRecordName(brecname);
			int index =JHCOM_vFind(internalRecordName,hideBlockRecordNames);
			if (index >= 0)
			{
				JHCOM_ShowObject(vids[i],false,false);
			}
		}
	}

	TYCOM_FlushBlockDisplay(blkRefId);
	return 0;
}



AcDbObjectId CopyBlockDefFromDatabase(AcDbDatabase* pSourceDb, AcDbDatabase* pDestDb, const TCHAR* blkDefName)
{
	Acad::ErrorStatus es;
	AcDbObjectId blockRefId = AcDbObjectId::kNull;

	// 打开外部图形数据库的块表，寻找给定名称的块表记录
	AcDbBlockTable* pBlkTable = NULL;
	es = pSourceDb->getBlockTable(pBlkTable, AcDb::kForRead);
	assert(es == Acad::eOk);
	if (es != Acad::eOk)
	{
		acutPrintf(TEXT("\n 复制目标源时，打开块表记录失败."));
		return blockRefId;
	}

	bool bRet = false;
	if (pBlkTable->has(blkDefName))	// 不存在指定的图块
	{
		AcDbObjectId destBlkDefId;		// 指定图块的块表记录Id
		es = pBlkTable->getAt(blkDefName, destBlkDefId);

		// 把指定的图块输出到一个临时图形数据库
		AcDbDatabase* pTempDb = NULL;	// 注意：这里千万不能new
		es = pSourceDb->wblock(pTempDb, destBlkDefId);
		assert(es == Acad::eOk);

		// 把该临时图形数据库作为块插入到当前dwg
		es = pDestDb->insert(blockRefId, blkDefName, pTempDb, false);
		assert(es == Acad::eOk);
		delete pTempDb;
		pTempDb = NULL;
	}
	else
	{
		acutPrintf(TEXT("\n给定的源数据中, 不存在指定名称的块表记录."));
	}
	pBlkTable->close();

	return blockRefId;
}

AcDbObjectId CopyBlockDefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName)
{
	CDocLock docLock;

	AcDbObjectId blockRefId = AcDbObjectId::kNull;

	// 使用_SH_DENYNO参数打开图形(只读打开)，允许其它用户读写该文件
	AcDbDatabase* pSourceDwg = new AcDbDatabase(false);
#if (defined ARX_2018) || (defined ARX_2017)|| (defined ARX_2019)
	Acad::ErrorStatus es = pSourceDwg->readDwgFile(fileName, AcDbDatabase::kForReadAndAllShare);
#else
	Acad::ErrorStatus es = pSourceDwg->readDwgFile(fileName, _SH_DENYNO);
#endif //  ARX_2018

	
	if (es != Acad::eOk)
	{
		delete pSourceDwg;
		pSourceDwg = NULL;

		acutPrintf(TEXT("\n读入dwg图形错误, 图形名称: %s"), fileName);
		return blockRefId;
	}

	AcDbDatabase* pCurDb = acdbHostApplicationServices()->workingDatabase();
	blockRefId = CopyBlockDefFromDatabase(pSourceDwg, pCurDb, blkDefName);

	delete pSourceDwg;
	pSourceDwg = NULL;

	return blockRefId;
}

bool CloneObjects(AcDbObjectIdArray objList, AcDbObjectIdArray &objListCloned)
{
	Acad::ErrorStatus es;
	objListCloned.removeAll();

	AcDbBlockTable *pBlockTable;
	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbObjectId  modelSpaceId = AcDbObjectId::kNull;
	pBlockTable->getAt(ACDB_MODEL_SPACE, modelSpaceId);
	pBlockTable->close();
	
	AcDbIdMapping idMap;
	es = acdbHostApplicationServices()->workingDatabase()->deepCloneObjects(objList, modelSpaceId, idMap);
	if (es!=Acad::eOk)
	{
		return false;
	}
	
	AcDbIdMappingIter iter(idMap);
	for (iter.start(); !iter.done(); iter.next())
	{
		AcDbIdPair idPair;
		iter.getMap(idPair);
		if (idPair.isCloned() && idPair.isPrimary())
		{
			objListCloned.append(idPair.value());
		}
	}

	return true;
}

AcDbObjectId InsertBlockRefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName, const WCHAR *layoutname, AcGePoint3d origin)
{
	AcDbObjectId blockRefId = CopyBlockDefFromDwg(fileName, blkDefName);
	if (blockRefId == AcDbObjectId::kNull)
	{
		return AcDbObjectId::kNull;
	}
	
	AcDbObjectId entId = AcDbObjectId::kNull;
	int nSuc = MD2010_InsertBlockReference_Layout(layoutname, blkDefName, entId, origin);
	return entId;
}


bool SelectViewDir(eViewDir& p_viewDir)
{
	CString sDir;

	bool bSuc = false;
	do
	{
		bSuc = GetStringInput(_T("\n需要选择的门窗视图方向[平面图(T) 立面图(F)]<T>:"), sDir);
		if (bSuc == false)
			return false;

		sDir.Trim();
		sDir.MakeUpper();
		if (sDir.IsEmpty())
		{
			p_viewDir = E_VIEW_TOP;
			break;
		}

		if (sDir.Find(_T('T')) >= 0 || sDir.Find(_T("平面")) >= 0)
		{
			p_viewDir = E_VIEW_TOP;
			break;
		}
		else if (sDir.Find(_T('F')) >= 0 || sDir.Find(_T("立面")) >= 0)
		{
			p_viewDir = E_VIEW_FRONT;
			break;
		}

	} while (bSuc);

	return bSuc;
}

vAcDbObjectId SelectWindows(eViewDir p_view)
{
	vAcDbObjectId vIds;//当前选择的ids

	acutPrintf(L"\n请选择门窗");

	//TODO 选择全部和自动过滤

	ads_name sset;
	acedSSGet(NULL, NULL, NULL, NULL, sset);

	Adesk::Int32 length = 0;
	acedSSLength(sset, &length);
	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(sset, i, ent);

		AcDbObjectId objId = 0;
		acdbGetObjectId(objId, ent);
		if (objId != 0 && TY_IsWindow(objId, p_view ))
		{
			vIds.push_back(objId);
		}
	}
	acedSSFree(sset);

	if (vIds.size() == 0)
	{
		acutPrintf(L"\n未选择到门窗\n");
	}
	else
	{
		CString info;
		info.Format(L"\n共选择了%d个门窗\n", vIds.size());
		acutPrintf(info);
	}
	return vIds;
}


vAcDbObjectId SelectAllWindows(eViewDir p_view)
{
	//TODO 利用acedSSGet 参数进行过滤
	vAcDbObjectId outIds;

	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	int rt = acedSSGet(_T("A"), NULL, NULL, NULL, ssname); // 提示用户选择对象
	if (rt == RTNORM)
	{
		long length;
		acedSSLength(ssname, &length);
		for (int i = 0; i < length; i++)
		{
			acedSSName(ssname, i, ent);
			acdbGetObjectId(id, ent);
			
			if (id != AcDbObjectId::kNull && TY_IsWindow(id, p_view))//检查是否是门窗
			{
				outIds.push_back(id);
			}
		}
		acedSSFree(ssname);
	}

	return outIds;
}