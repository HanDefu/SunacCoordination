/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCKitchen.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCDynamicBlock.h"

class RCKitchen : public RCDynamicBlock
{
public:
	//Constructors
	RCKitchen(void);
	RCKitchen(const RCKitchen &other);

	//Destructor
	~RCKitchen(void);

	//Operator = 
	RCKitchen & operator=(const RCKitchen &rhs);

	virtual RCTYPE GetType()const{return T_KITCHEN;}

	virtual RCKitchen * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCKitchen> vRCKitchen;
typedef std::vector<RCKitchen*> vpRCKitchen;