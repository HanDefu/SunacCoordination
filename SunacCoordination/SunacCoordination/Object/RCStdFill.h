/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCStdFill.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"

class RCStdFill : public RCStandBlock
{
public:
	//Constructors
	RCStdFill(void);
	RCStdFill(const RCStdFill &other);

	//Destructor
	~RCStdFill(void);

	//Operator = 
	RCStdFill & operator=(const RCStdFill &rhs);

	virtual RCTYPE GetType()const{return T_WATERPROOF;}

	virtual RCStdFill * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCStdFill> vRCStdFill;
typedef std::vector<RCStdFill*> vpRCStdFill;