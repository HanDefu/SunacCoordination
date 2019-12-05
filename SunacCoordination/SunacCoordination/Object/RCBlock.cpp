/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "math.h"
#include "RCBlock.h"
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_ACAD_Common.h"
#include "../Common/ComFun_String.h"
#include "../Common/ComFun_Layer.h"
#include <acdocman.h>

//default constructor
RCBlock::RCBlock()
{
}

//Copy constructor
RCBlock::RCBlock(const RCBlock &other):RCObject(other)
{
	m_blockRecordName = other.m_blockRecordName;
}

//Destructor
RCBlock::~RCBlock(void)
{
}

//Operator = 
RCBlock & RCBlock::operator=(const RCBlock &other)
{
	m_blockRecordName = other.m_blockRecordName;
    return *this;
} 

AcDbObjectId RCBlock::Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	m_blockRecordName = FilePathToFileNameWithoutExtension(fileName);

	acDocManager->lockDocument(curDoc());
	//检查图层是否存在，不存在则创建
	AcDbObjectId layerId = JHCOM_GetLayerID(layerName);
	if (layerId==AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(layerName);
	}

	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, fileName, m_blockRecordName,  m_id, origin, angle, AcGeScale3d(1), layerName, color);
	acDocManager->unlockDocument(curDoc());
	return m_id;
}

//对于已经存在的图块定义的插入
AcDbObjectId RCBlock::Insert(CString layoutname, CString blockDefineName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	CString name;
	MD2010_GetCurrentLayer(name);
	MD2010_SetCurrentLayer(layerName);
	MD2010_InsertBlockReference_Layout(layoutname, blockDefineName, m_id, origin, angle, AcGeScale3d(1,1,1),color);
	MD2010_SetCurrentLayer(name);
	return m_id;
}

bool RCBlock::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCBlock * pObj = dynamic_cast<RCBlock *>(other);
	if (pObj == 0)
		return false;

	if (!RCObject::isEqualTo(other))
		return false;

	//if (m_blockRecordName != pObj->m_blockRecordName)
	//	return false;

	return true;
}