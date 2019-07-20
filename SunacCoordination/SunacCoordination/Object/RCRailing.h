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

class RCRailing : public RCStandBlock
{
public:
	//Constructors
	RCRailing(void);
	RCRailing(const RCRailing &other);

	//Destructor
	~RCRailing(void);

	//Operator = 
	RCRailing & operator=(const RCRailing &rhs);

	virtual RCTYPE GetType()const{return T_STDFACADE;}

	virtual RCRailing * Clone();
	virtual void Draw();

	int InsertRailing(AcGePoint3d start, AcGePoint3d end, CString fileName);

protected: 
	
};

typedef std::vector<RCRailing> vRCLanGan;
typedef std::vector<RCRailing*> vpRCLanGan;