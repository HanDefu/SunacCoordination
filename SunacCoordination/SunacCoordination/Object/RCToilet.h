/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCToilet.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCDynamicBlock.h"

class RCToilet : public RCDynamicBlock
{
public:
	//Constructors
	RCToilet(void);
	RCToilet(const RCToilet &other);

	//Destructor
	~RCToilet(void);

	//Operator = 
	RCToilet & operator=(const RCToilet &rhs);

	virtual RCTYPE GetType()const{return T_TOILET;}

	virtual RCToilet * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCToilet> vRCToilet;
typedef std::vector<RCToilet*> vpRCToilet;