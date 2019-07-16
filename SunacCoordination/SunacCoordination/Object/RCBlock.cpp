/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "math.h"
#include "RCBlock.h"

//default constructor
RCBlock::RCBlock()
{
}

//Copy constructor
RCBlock::RCBlock(const RCBlock &other):RCObject(other)
{
}

//Destructor
RCBlock::~RCBlock(void)
{
}

//Operator = 
RCBlock & RCBlock::operator=(const RCBlock &rhs)
{
    return *this;
} 