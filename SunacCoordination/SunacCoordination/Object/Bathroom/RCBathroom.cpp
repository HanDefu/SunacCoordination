/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCBathroom.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCBathroom.h"
#include <float.h>
#include <algorithm>

//Constructor
RCBathroom::RCBathroom(void)
{

}

//Destructor
RCBathroom::~RCBathroom(void)
{

}


//Constructor
RCBathroom::RCBathroom(const RCBathroom &other):RCDynamicBlock(other)
{

}

//Operator = 
RCBathroom & RCBathroom::operator=(const RCBathroom &rhs)
{
	return *this;
}


void RCBathroom::Draw()
{
}

RCBathroom * RCBathroom::Clone()
{
	RCBathroom * newObj = new RCBathroom;

	return newObj;
}

