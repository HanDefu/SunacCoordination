/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCWaterProof.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"

class RCWaterProof : public RCStandBlock
{
public:
	//Constructors
	RCWaterProof(void);
	RCWaterProof(const RCWaterProof &other);

	//Destructor
	~RCWaterProof(void);

	//Operator = 
	RCWaterProof & operator=(const RCWaterProof &rhs);

	virtual RCTYPE GetType()const{return T_WATERPROOF;}

	virtual RCWaterProof * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCWaterProof> vRCWaterProof;
typedef std::vector<RCWaterProof*> vpRCWaterProof;