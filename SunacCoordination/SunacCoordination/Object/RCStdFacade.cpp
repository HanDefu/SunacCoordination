/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCStdFacade.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCStdFacade.h"
#include "float.h"
#include <algorithm>

//Constructor
RCStdFacade::RCStdFacade(void)
{

}

//Destructor
RCStdFacade::~RCStdFacade(void)
{

}


//Constructor
RCStdFacade::RCStdFacade(const RCStdFacade &other):RCStandBlock(other)
{

}

//Operator = 
RCStdFacade & RCStdFacade::operator=(const RCStdFacade &rhs)
{
	return *this;
}


void RCStdFacade::Draw()
{
}

RCStdFacade * RCStdFacade::Clone()
{
	RCStdFacade * newObj = new RCStdFacade;

	return newObj;
}

