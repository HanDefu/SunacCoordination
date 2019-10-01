/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCDoor.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCDoor.h"
#include <float.h>
#include <algorithm>

//Constructor
RCDoor::RCDoor(void)
{

}

//Destructor
RCDoor::~RCDoor(void)
{

}


//Constructor
RCDoor::RCDoor(const RCDoor &other):RCDynamicBlock(other)
{

}

//Operator = 
RCDoor & RCDoor::operator=(const RCDoor &rhs)
{
	return *this;
}


void RCDoor::Draw()
{
}

RCDoor * RCDoor::Clone()
{
	RCDoor * newObj = new RCDoor;

	return newObj;
}

