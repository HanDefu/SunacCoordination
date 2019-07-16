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
#include "float.h"
#include <algorithm>

//Constructor
RCKitchen::RCKitchen(void)
{

}

//Destructor
RCKitchen::~RCKitchen(void)
{

}


//Constructor
RCKitchen::RCKitchen(const RCKitchen &other):RCDynamicBlock(other)
{

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

