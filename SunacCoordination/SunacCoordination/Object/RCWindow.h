/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "RCDynamicBlock.h"

class RCWindow : public RCDynamicBlock
{
public:
	//Constructors
	RCWindow(void);
	RCWindow(const RCWindow &other);

	//Destructor
	~RCWindow(void);

	//Operator = 
	RCWindow & operator=(const RCWindow &rhs);

	virtual RCTYPE GetType()const{return T_WINDOW;}

	virtual RCWindow * Clone();
	virtual void Draw();

protected: 
	
private:
	
};

typedef std::vector<RCWindow> vRCWindow;
typedef std::vector<RCWindow*> vpRCWindow;