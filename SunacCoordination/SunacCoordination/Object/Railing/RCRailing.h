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
#include "..\RCStandBlock.h"
#include "AttrRailing.h"

class CRCRailing : public RCStandBlock
{
public:
	CRCRailing(void);
	CRCRailing(const CRCRailing &other);
	virtual ~CRCRailing(void);

	virtual CRCRailing & operator=(const CRCRailing &rhs);

	virtual RCTYPE GetType()const{ return T_RAILING; }
	virtual CRCRailing * Clone() = 0;
	
	void SetRailingAtt(const AttrRailing p_railingAtt);
	AttrRailing GetRailingAtt()const { return m_railingAtt; }

	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end) = 0;


protected: 
	AttrRailing m_railingAtt;
	
};

typedef std::vector<CRCRailing*> vpRCRailing;


//////////////////////////////////////////////////////////////////////////
class CRCRailingTieyi : public CRCRailing
{
public:
	CRCRailingTieyi();
	~CRCRailingTieyi();


	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end);

private:

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingBoli : public CRCRailing
{
public:
	CRCRailingBoli();
	~CRCRailingBoli();

private:

};


CRCRailing* CreateRailing(eRailingType p_railingType, CString p_sPrototypeCode);

