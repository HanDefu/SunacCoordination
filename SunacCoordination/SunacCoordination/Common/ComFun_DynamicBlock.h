#pragma once
#include <dbdynblk.h>
#include <dbobjptr.h>

bool TYCOM_IsDyanamicBlock(AcDbObjectId id);

CString TYCOM_GetDynaBlkName(AcDbObjectId eId);
int TYCOM_GetDynamicBlockName(AcDbBlockReference *pBlkRef, CString &name);
int TYCOM_GetDynamicBlockName(AcDbObjectId entId, CString &name);


//----------------设置动态块属性--------------//
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, double value);
//设置int和bool类型的
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, Adesk::Int32 value);
//设置枚举类型的动态块属性
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, short index);
int TYCOM_SetDynamicBlockValue(AcDbObjectId eId, CString name, CString value);

//----------------读取动态块属性--------------//
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, double &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, int &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, bool &value);
int TYCOM_GetDynamicBlockData(AcDbObjectId eId, CString key, CString &value);

//遍历
int TYCOM_CycleAllDynamicBlockReferencesInRect(vAcDbObjectId &vids, AcGePoint3d lb, AcGePoint3d rt);
int TYCOM_CycleAllDynamicBlockReferencesInLayer(vAcDbObjectId &vids, CString layer);





