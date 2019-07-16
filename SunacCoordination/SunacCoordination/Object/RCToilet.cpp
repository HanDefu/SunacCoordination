/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCToilet.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCToilet.h"
#include "float.h"
#include <algorithm>

//Constructor
RCToilet::RCToilet(void)
{

}

//Destructor
RCToilet::~RCToilet(void)
{

}


//Constructor
RCToilet::RCToilet(const RCToilet &other):RCDynamicBlock(other)
{

}

//Operator = 
RCToilet & RCToilet::operator=(const RCToilet &rhs)
{
	return *this;
}


void RCToilet::Draw()
{
}

RCToilet * RCToilet::Clone()
{
	RCToilet * newObj = new RCToilet;

	return newObj;
}

