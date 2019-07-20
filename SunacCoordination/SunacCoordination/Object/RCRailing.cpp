/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCLanGan.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCRailing.h"
#include "float.h"
#include <algorithm>
#include "../Common/ComFun_Str.h"
#include "../Common/ComFun_ACAD_Common.h"
#include "../Common/ComFun_ACad.h"
#include <acdocman.h>

//Constructor
RCRailing::RCRailing(void)
{

}

//Destructor
RCRailing::~RCRailing(void)
{

}


//Constructor
RCRailing::RCRailing(const RCRailing &other):RCStandBlock(other)
{

}

//Operator = 
RCRailing & RCRailing::operator=(const RCRailing &rhs)
{
	return *this;
}


void RCRailing::Draw()
{
}

RCRailing * RCRailing::Clone()
{
	RCRailing * newObj = new RCRailing;

	return newObj;
}

int RCRailing::InsertRailing(AcGePoint3d start, AcGePoint3d end, CString fileName)
{
	int cnt = GetTickCount();
	double xOffset = 0;
	for (int i = 0; i < 3; i++ )
	{
		CString blockDefName;
		blockDefName.Format(L"%d", cnt+i);
		acDocManager->lockDocument(curDoc());
		AcDbObjectId m_id;
		AcGePoint3d origin = start;
		origin.x += xOffset;
		MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, fileName, blockDefName,  m_id, origin, 0, AcGeScale3d(1), 0, 256);
		acDocManager->unlockDocument(curDoc());

		AcGePoint3d min,max;
		JHCOM_GetObjectMinMaxPoint(m_id, min,max);
		xOffset = xOffset + max.x - min.x;
	}
	return 0;
}



