/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include "RCObject.h"
class RCBlock : public RCObject
{
public:
	//Constructors
	RCBlock();
	RCBlock(const RCBlock &other);

	//Destructor
	~RCBlock(void);
	
	//Operator = 
	RCBlock & operator=(const RCBlock &rhs);

public:

};


typedef std::vector<RCBlock> vRCBlock;
typedef std::vector<RCBlock*> vpRCBlock;
