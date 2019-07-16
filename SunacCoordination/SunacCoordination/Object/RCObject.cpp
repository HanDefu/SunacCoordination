/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCObject.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/


#include "StdAfx.h"
#include "RCObject.h"

RCObject::RCObject(void)
{
	m_userData = 0;
}

RCObject::RCObject(const RCObject&other)
{
	m_name = other.m_name;
	m_userData = other.m_userData;
}

RCObject::~RCObject(void)
{
}

