#pragma once

#include "ComFun_ACAD_Common.h"
#include "TYRect.h"
#include "../Object/RCWindow.h"
#include <utility>

using namespace std;

template<typename T> 
int TYCOM_vFind(T &B, std::vector<T>&A)
{
	for(int i = 0; i < (int)A.size(); i++)
	{
		if(A[i] == B)
			return i;
	}
	return -1;
}

int vFind(RCPairKeyDValue &A, vRCPairKeyDValue &B);
int vFind(RCPairKeyIValue &A, vRCPairKeyIValue &B);
int vFind(RCPairKeyStrValue &A, vRCPairKeyStrValue &B);
int vFind(RCWindow &oneWindow, vRCWindow &allWindows);


//判断是不是一个直线（线段）
bool DQ_IsALine(AcDbObjectId entId);
//判断是不是一个块参照
bool DQ_IsBlockReference(AcDbObjectId id);
//让用户选择多个（框选）对象
void DQ_SelectMany(vAcDbObjectId &ids);

//得到图块的插入点
int DQ_GetBlockReferenceInsertPoint(AcDbObjectId id, AcGePoint3d &pnt);

//得到曲线和点的最近点
int DQ_GetCurveClosedPointToOneGivenPoint(AcDbObjectId curvre, AcGePoint3d givenPnt, AcGePoint3d &closedPnt);



//块参照有没有 attributename属性
bool DQ_HasAttribute_BlkDef(AcDbObjectId blockDefId, CString attributename);
//块参照有没有 attributename属性
bool DQ_HasAttribute_BlkRef(AcDbObjectId blockRefId, CString tagText);

//读取块参照属性
bool DQ_GetAttributeValue( AcDbObjectId blockRefId, CString attributename, CString &value );
//读取块参照属性
bool DQ_GetAttributeValue( AcDbBlockReference *pBlkRef, const TCHAR* attributename, CString &value );

int MD2010_GetAllTypedObjectsInLayer(vAcDbObjectId &allEntites, CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids);

void YT_UpdateBlockReference(AcDbObjectId &entId);

void TY_GetAllWindowFiles(vCString &files);
void TY_GetAllKitchenFiles(vCString &files);
void TY_GetAllTieYiLanGanFiles(vCString &files);
void TY_GetAllBoLiLanGanFiles(vCString &files);

AcGePoint3d TY_GetPoint(CString prompt = L"请选择插入点");
TYRect TY_GetOneRect();
int TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2);

AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id);
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt);
int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt);

bool TY_IsWindow(AcDbObjectId Id);
bool TY_Iskitchen(AcDbObjectId Id);
bool TY_IsToilet(AcDbObjectId Id);
bool TY_IsAirCon(AcDbObjectId Id);
bool TY_IsDoor(AcDbObjectId Id);
bool TY_IsRailing(AcDbObjectId Id);

bool TY_IsPairsEqual(vRCPairKeyDValue &A, vRCPairKeyDValue &B);
bool TY_IsPairsEqual(vRCPairKeyIValue &A, vRCPairKeyIValue &B);
bool TY_IsPairsEqual(vRCPairKeyStrValue &A, vRCPairKeyStrValue &B);


CString TY_GetLocalFilePath();

vector<pair<CString,CString>> TY_FindFilesInDirecotry(CString fileName,CString directory );
vector<pair<CString,CString>> TY_FindFilesInDirecotryRecursion( CString fileName,CString directory,int recursionCount);
int TYCOM_Mirror(AcDbObjectId entId, AcGePoint3d first, AcGeVector3d dir);

int TYCOM_Rotate(AcDbObjectId entId, AcGePoint3d ptBase, double rotation);

int TYCOM_Move(AcDbObjectId entId, AcGeVector3d offset);

//从一个块参照或者动态块参照种隐藏所有的blockrecordname在hideBlockRecordNames中的块
int TY_HideBlockReferencesInBlockReference(AcDbObjectId blkRefId, vCString &hideBlockRecordNames);





