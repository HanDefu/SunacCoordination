/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCStandBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCBlock.h"

class RCStandBlock : public RCBlock
{
public:
	//Constructors
	RCStandBlock(void);
	RCStandBlock(const RCStandBlock &other);

	//Destructor
	~RCStandBlock(void);

	//Operator = 
	RCStandBlock & operator=(const RCStandBlock &rhs);

protected: 
	
};

typedef std::vector<RCStandBlock> vRCStandBlock;
typedef std::vector<RCStandBlock*> vpRCStandBlock;