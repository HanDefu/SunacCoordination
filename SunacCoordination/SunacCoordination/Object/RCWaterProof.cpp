/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCWaterProof.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCWaterProof.h"
#include "float.h"
#include <algorithm>

//Constructor
RCWaterProof::RCWaterProof(void)
{

}

//Destructor
RCWaterProof::~RCWaterProof(void)
{

}


//Constructor
RCWaterProof::RCWaterProof(const RCWaterProof &other):RCStandBlock(other)
{

}

//Operator = 
RCWaterProof & RCWaterProof::operator=(const RCWaterProof &rhs)
{
	return *this;
}


void RCWaterProof::Draw()
{
}

RCWaterProof * RCWaterProof::Clone()
{
	RCWaterProof * newObj = new RCWaterProof;

	return newObj;
}

