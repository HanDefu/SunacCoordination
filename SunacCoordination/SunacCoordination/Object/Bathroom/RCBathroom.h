/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCBathroom.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include <vector>
#include "..\RCDynamicBlock.h"
#include "AttrBathroom.h"

class RCBathroom : public RCDynamicBlock
{
public:
	//Constructors
	RCBathroom(void);
	RCBathroom(const RCBathroom &other);

	//Destructor
	~RCBathroom(void);

	//Operator = 
	RCBathroom & operator=(const RCBathroom &rhs);

	virtual RCTYPE GetType()const{return T_Bathroom;}

	virtual RCBathroom * Clone();
	virtual void Draw();

	AttrBathroom * GetAttribute();
	void AddAttribute(AttrBathroom * attr);

protected: 
	AttrBathroom * m_pAttribute;
};

typedef std::vector<RCBathroom> vRCBathroom;
typedef std::vector<RCBathroom*> vpRCBathroom;