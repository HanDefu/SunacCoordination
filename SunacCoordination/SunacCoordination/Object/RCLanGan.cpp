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
#include "RCLanGan.h"
#include "float.h"
#include <algorithm>

//Constructor
RCLanGan::RCLanGan(void)
{

}

//Destructor
RCLanGan::~RCLanGan(void)
{

}


//Constructor
RCLanGan::RCLanGan(const RCLanGan &other):RCStandBlock(other)
{

}

//Operator = 
RCLanGan & RCLanGan::operator=(const RCLanGan &rhs)
{
	return *this;
}


void RCLanGan::Draw()
{
}

RCLanGan * RCLanGan::Clone()
{
	RCLanGan * newObj = new RCLanGan;

	return newObj;
}

