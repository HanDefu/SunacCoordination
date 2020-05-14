/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCKitchen.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCKitchen.h"
#include <float.h>
#include <algorithm>
#include "../../Common/ComFun_Sunac.h"

//Constructor
RCKitchen::RCKitchen(void)
{
	m_pAttribute = 0;
}

//Destructor
RCKitchen::~RCKitchen(void)
{

}


//Constructor
RCKitchen::RCKitchen(const RCKitchen &other):RCDynamicBlock(other)
{
	m_pAttribute = other.m_pAttribute;
}

//Operator = 
RCKitchen & RCKitchen::operator=(const RCKitchen &rhs)
{
	return *this;
}


void RCKitchen::Draw()
{
}

RCKitchen * RCKitchen::Clone()
{
	RCKitchen * newObj = new RCKitchen;

	return newObj;
}

AttrKitchen * RCKitchen::GetAttribute()
{
	AcDbObject * pDataEnt = 0;
	TY_GetAttributeData(m_id, pDataEnt, false);
	AttrKitchen * pAttribute = dynamic_cast<AttrKitchen *>(pDataEnt);
	return pAttribute;
}

void RCKitchen::AddAttribute(AttrKitchen * attr)
{
	if (attr != 0)
		TY_AddAttributeData(m_id, attr);
}

