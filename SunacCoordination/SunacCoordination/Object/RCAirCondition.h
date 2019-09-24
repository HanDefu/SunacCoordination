/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCAirCondition.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"
#include "AttrAirCon.h"

class RCAirCondition : public RCStandBlock
{
public:
	//Constructors
	RCAirCondition(void);
	RCAirCondition(const RCAirCondition &other);

	//Destructor
	~RCAirCondition(void);

	//Operator = 
	RCAirCondition & operator=(const RCAirCondition &rhs);

	virtual RCTYPE GetType()const{return T_AIRCONDITION;}

	void AddAttribute(AttrAirCon * attr);

	virtual RCAirCondition * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCAirCondition> vRCAirCondition;
typedef std::vector<RCAirCondition*> vpRCAirCondition;