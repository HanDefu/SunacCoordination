/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCStdFill.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCStdFill.h"
#include "float.h"
#include <algorithm>

//Constructor
RCStdFill::RCStdFill(void)
{

}

//Destructor
RCStdFill::~RCStdFill(void)
{

}


//Constructor
RCStdFill::RCStdFill(const RCStdFill &other):RCStandBlock(other)
{

}

//Operator = 
RCStdFill & RCStdFill::operator=(const RCStdFill &rhs)
{
	return *this;
}


void RCStdFill::Draw()
{
}

RCStdFill * RCStdFill::Clone()
{
	RCStdFill * newObj = new RCStdFill;

	return newObj;
}

