/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCLanGan.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"

class RCLanGan : public RCStandBlock
{
public:
	//Constructors
	RCLanGan(void);
	RCLanGan(const RCLanGan &other);

	//Destructor
	~RCLanGan(void);

	//Operator = 
	RCLanGan & operator=(const RCLanGan &rhs);

	virtual RCTYPE GetType()const{return T_STDFACADE;}

	virtual RCLanGan * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCLanGan> vRCLanGan;
typedef std::vector<RCLanGan*> vpRCLanGan;