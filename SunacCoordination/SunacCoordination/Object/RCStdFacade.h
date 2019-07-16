/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCStdFacade.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCStandBlock.h"

class RCStdFacade : public RCStandBlock
{
public:
	//Constructors
	RCStdFacade(void);
	RCStdFacade(const RCStdFacade &other);

	//Destructor
	~RCStdFacade(void);

	//Operator = 
	RCStdFacade & operator=(const RCStdFacade &rhs);

	virtual RCTYPE GetType()const{return T_STDFACADE;}

	virtual RCStdFacade * Clone();
	virtual void Draw();

protected: 
	
};

typedef std::vector<RCStdFacade> vRCStdFacade;
typedef std::vector<RCStdFacade*> vpRCStdFacade;