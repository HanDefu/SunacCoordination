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
#include <acdocman.h>

//default constructor
RCBlock::RCBlock()
{
}

//Copy constructor
RCBlock::RCBlock(const RCBlock &other):RCObject(other)
{
}

//Destructor
RCBlock::~RCBlock(void)
{
}

//Operator = 
RCBlock & RCBlock::operator=(const RCBlock &rhs)
{
    return *this;
} 

AcDbObjectId RCBlock::Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	WCHAR blockname[256] = L"";
	CF_STR_get_file_name(fileName, blockname);
	CF_STR_get_file_name_2(blockname, blockname);
	CString blockDefName(blockname);
	acDocManager->lockDocument(curDoc());
	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, fileName, blockDefName,  m_id, origin, angle, AcGeScale3d(1), layerName, color);
	acDocManager->unlockDocument(curDoc());
	return 0;
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

	return true;
}