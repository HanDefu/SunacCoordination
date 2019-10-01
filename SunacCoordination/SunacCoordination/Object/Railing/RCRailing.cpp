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
#include <float.h>
#include <algorithm>
#include <acdocman.h>
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_ACad.h"
 
CRCRailing::CRCRailing(void)
{

}
 
CRCRailing::~CRCRailing(void)
{

}

CRCRailing::CRCRailing(const CRCRailing &other):RCStandBlock(other)
{

}

CRCRailing& CRCRailing::operator=(const CRCRailing &rhs)
{
	//TODO
	return *this;
}

void CRCRailing::SetRailingAtt(const AttrRailing p_railingAtt)
{
	m_railingAtt = p_railingAtt;
}


//////////////////////////////////////////////////////////////////////////
CRCRailingTieyi::CRCRailingTieyi()
{
}

CRCRailingTieyi::~CRCRailingTieyi()
{
}


int CRCRailingTieyi::GenerateRailing(AcGePoint3d start, AcGePoint3d end)
{
	//int cnt = GetTickCount();
	//double xOffset = 0;
	//for (int i = 0; i < 3; i++)
	//{
	//	CString blockDefName;
	//	blockDefName.Format(L"%d", cnt + i);
	//	acDocManager->lockDocument(curDoc());
	//	AcDbObjectId m_id;
	//	AcGePoint3d origin = start;
	//	origin.x += xOffset;
	//	MD2010_InsertBlockFromPathName(ACDB_MODEL_SPACE, fileName, blockDefName, m_id, origin, 0, AcGeScale3d(1), 0, 256);
	//	acDocManager->unlockDocument(curDoc());

	//	AcGePoint3d min, max;
	//	JHCOM_GetObjectMinMaxPoint(m_id, min, max);
	//	xOffset = xOffset + max.x - min.x;
	//}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
CRCRailingBoli::CRCRailingBoli()
{
}

CRCRailingBoli::~CRCRailingBoli()
{
}




//////////////////////////////////////////////////////////////////////////

CRCRailing* CreateRailing(eRailingType p_railingType, CString p_sPrototypeCode)
{
	return NULL; //TODO
}

