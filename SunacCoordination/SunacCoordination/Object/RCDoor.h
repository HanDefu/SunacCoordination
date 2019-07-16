/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCDoor.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCDynamicBlock.h"

class RCDoor : public RCDynamicBlock
{
public:
	//Constructors
	RCDoor(void);
	RCDoor(const RCDoor &other);

	//Destructor
	~RCDoor(void);

	//Operator = 
	RCDoor & operator=(const RCDoor &rhs);

	virtual RCTYPE GetType()const{return T_DOOR;}

	virtual RCDoor * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCDoor> vRCDoor;
typedef std::vector<RCDoor*> vpRCDoor;