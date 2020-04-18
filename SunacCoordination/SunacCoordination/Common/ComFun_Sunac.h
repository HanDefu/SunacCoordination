#pragma once

#include "ComFun_ACAD_Common.h"
#include "TYRect.h"
#include "../Object/WindowDoor/RCWindow.h"
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

bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, double p_value);
bool DQ_SetDynamicAttribute(AcDbObjectId p_blockRefId, CString p_attributename, CString p_value);

int MD2010_GetAllTypedObjectsInLayer(vAcDbObjectId &allEntites, CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids);

void YT_UpdateBlockReference(AcDbObjectId &entId);


bool TY_GetPoint(AcGePoint3d &ptOut, CString prompt = L"请选择插入点");
bool TY_GetOneRect(TYRect& p_rect);
bool TY_GetTwoPoints(AcGePoint3d &pnt1, AcGePoint3d &pnt2);

bool GetRealInput(const TCHAR* prompt, double defaultVal, int precision, double &ret);
bool GetStringInput(CString prompt, CString &strOut);

//////////////////////////////////////////////////////////////////////////
AcDbObjectId TY_GetExtensionDictionaryID(AcDbObjectId id);
AcDbObjectId TY_CreateExtensionDictionaryID(AcDbObjectId id);
int TY_AddAttributeData(AcDbObjectId Id, AcDbObject *pDataEnt);
int TY_GetAttributeData(AcDbObjectId tkId, AcDbObject *&pDataEnt);

bool TY_IsWindow(AcDbObjectId Id, eViewDir p_view);
bool TY_Iskitchen(AcDbObjectId Id);
bool TY_IsBathroom(AcDbObjectId Id);
bool TY_IsAirCon(AcDbObjectId Id);
bool TY_IsRailing(AcDbObjectId Id, eViewDir p_view);
bool TY_IsSunacObj(AcDbObjectId Id, eRCType p_rcType, eViewDir p_view= E_VIEW_ALL);

//////////////////////////////////////////////////////////////////////////

eRCType TY_GetType(AcDbBlockReference *pBlockReference);


bool TY_IsPairsEqual(vRCPairKeyDValue &A, vRCPairKeyDValue &B);
bool TY_IsPairsEqual(vRCPairKeyIValue &A, vRCPairKeyIValue &B);
bool TY_IsPairsEqual(vRCPairKeyStrValue &A, vRCPairKeyStrValue &B);


CString TY_GetDataFilePath();
CString TY_GetPrototypeFilePath();
CString TY_GetPrototypeImagePath();
CString TY_GetPrototypeFilePath_Local();
CString TY_GetPrototypeImagePath_Local();
CString TY_GetPrototypeFilePath_Web();
CString TY_GetPrototypeImagePath_Web();

char* TY_GetAluminumDatabasePath();
char* TY_GetLocalDataDatabasePath();

vector<pair<CString,CString>> TY_FindFilesInDirecotry(CString fileName,CString directory );
vector<pair<CString,CString>> TY_FindFilesInDirecotryRecursion( CString fileName,CString directory,int recursionCount);

int TYCOM_Mirror(AcDbObjectId entId, AcGePoint3d first, AcGeVector3d dir);
int TYCOM_Rotate(AcDbObjectId entId, AcGePoint3d ptBase, double rotation);
int TYCOM_Move(AcDbObjectId entId, AcGeVector3d offset);
int TYCOM_Transform(AcDbObjectId entId, const AcGeMatrix3d& mat);

//从一个块参照或者动态块参照种隐藏所有的blockrecordname在hideBlockRecordNames中的块
int TY_HideBlockReferencesInBlockReference(AcDbObjectId blkRefId, vCString &hideBlockRecordNames);


//功能: 从其他的DWG图形复制一个块定义到当前图形
//参数: const TCHAR * fileName, Dwg文件名（全路径）
//参数: const TCHAR * blkDefName, 块定义的名称
AcDbObjectId CopyBlockDefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName);
AcDbObjectId CopyBlockDefFromDatabase(AcDbDatabase* pSourceDb, AcDbDatabase* pDestDb, const TCHAR* blkDefName);

bool CloneObjects(AcDbObjectIdArray objList, AcDbObjectIdArray &objListOut);

AcDbObjectId InsertBlockRefFromDwg(const TCHAR* fileName, const TCHAR* blkDefName, const WCHAR *layoutname, AcGePoint3d origin);

bool SelectViewDir(eViewDir& p_viewDir);
vAcDbObjectId SelectWindows_old(eViewDir p_view, bool p_bAllWindow=false); //p_view==E_VIEW_ALL表示不限视图方向
bool IsObjectExsit(AcDbObjectId p_id);

bool IsFileExist(const CString & strFileName);






