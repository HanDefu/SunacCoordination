/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCAirCondition.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCAirCondition.h"
#include "float.h"
#include <algorithm>
#include "../Common/ComFun_Sunac.h"

//Constructor
RCAirCondition::RCAirCondition(void)
{

}

//Destructor
RCAirCondition::~RCAirCondition(void)
{

}


//Constructor
RCAirCondition::RCAirCondition(const RCAirCondition &other):RCStandBlock(other)
{

}

//Operator = 
RCAirCondition & RCAirCondition::operator=(const RCAirCondition &rhs)
{
	return *this;
}


void RCAirCondition::Draw()
{
}

RCAirCondition * RCAirCondition::Clone()
{
	RCAirCondition * newObj = new RCAirCondition;

	return newObj;
}

void RCAirCondition::AddAttribute(AttrAirCon * attr)
{
	if (attr != 0)
		TY_AddAttributeData(m_id, attr);
}
