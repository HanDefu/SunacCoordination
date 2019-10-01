/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCXianJiao.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCXianJiao.h"
#include <float.h>
#include <algorithm>

//Constructor
RCXianJiao::RCXianJiao(void)
{

}

//Destructor
RCXianJiao::~RCXianJiao(void)
{

}


//Constructor
RCXianJiao::RCXianJiao(const RCXianJiao &other):RCStandBlock(other)
{

}

//Operator = 
RCXianJiao & RCXianJiao::operator=(const RCXianJiao &rhs)
{
	return *this;
}


void RCXianJiao::Draw()
{
}

RCXianJiao * RCXianJiao::Clone()
{
	RCXianJiao * newObj = new RCXianJiao;

	return newObj;
}

