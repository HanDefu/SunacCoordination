/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCXianJiao.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"

class RCXianJiao : public RCStandBlock
{
public:
	//Constructors
	RCXianJiao(void);
	RCXianJiao(const RCXianJiao &other);

	//Destructor
	~RCXianJiao(void);

	//Operator = 
	RCXianJiao & operator=(const RCXianJiao &rhs);

	virtual RCTYPE GetType()const{return T_STDFACADE;}

	virtual RCXianJiao * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCXianJiao> vRCXianJiao;
typedef std::vector<RCXianJiao*> vpRCXianJiao;