/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCWindow.h"
#include "float.h"
#include <algorithm>

//Constructor
RCWindow::RCWindow(void)
{

}

//Destructor
RCWindow::~RCWindow(void)
{

}


//Constructor
RCWindow::RCWindow(const RCWindow &other):RCDynamicBlock(other)
{

}

//Operator = 
RCWindow & RCWindow::operator=(const RCWindow &rhs)
{
	return *this;
}


void RCWindow::Draw()
{
}

RCWindow * RCWindow::Clone()
{
	RCWindow * newObj = new RCWindow;

	return newObj;
}

