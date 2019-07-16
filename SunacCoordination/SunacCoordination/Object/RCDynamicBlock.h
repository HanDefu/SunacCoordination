/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCDynamicBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCBlock.h"

class RCDynamicBlock : public RCBlock
{
public:
	//Constructors
	RCDynamicBlock(void);
	RCDynamicBlock(const RCDynamicBlock &other);

	//Destructor
	~RCDynamicBlock(void);

	//Operator = 
	RCDynamicBlock & operator=(const RCDynamicBlock &rhs);

protected: 
	
};

typedef std::vector<RCDynamicBlock> vRCDynamicBlock;
typedef std::vector<RCDynamicBlock*> vpRCDynamicBlock;