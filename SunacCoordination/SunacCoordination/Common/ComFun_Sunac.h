#pragma once

#define  SUNAC_DEBUG

#include "ComFun_ACAD_Common.h"
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





