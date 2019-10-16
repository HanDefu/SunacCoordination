/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This file is the head file of class RCBlock.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/
#pragma once

#include "RCObject.h"
class RCBlock : public RCObject
{
public:
	RCBlock();
	RCBlock(const RCBlock &other);
	virtual ~RCBlock(void);
	
	virtual bool isEqualTo(RCObject*other = 0);//基础数据一致
	virtual RCBlock & operator=(const RCBlock &rhs);

	virtual AcDbObjectId Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color);
	virtual AcDbObjectId RCBlock::AirInsert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color);

	AcDbObjectId RCBlock::Insert(CString layoutname, CString blockDefineName, AcGePoint3d origin, double angle, CString layerName, int color);
	

	CString GetBlockRecordName(){return m_blockRecordName;}

public:
	CString m_blockRecordName;//
};


typedef std::vector<RCBlock> vRCBlock;
typedef std::vector<RCBlock*> vpRCBlock;
