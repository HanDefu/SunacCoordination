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
#include "RCRailing.h"

//////////////////////////////////////////////////////////////////////////
class CRCRailingTieyi : public CRCRailing
{
public:
	CRCRailingTieyi();
	~CRCRailingTieyi();

	virtual double Getb()const{ return 120; }			//单元花格尺寸120
	virtual double GetH() const { return 280; }			//H固定值280
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40
	virtual double GetSmallPillarWidth()const { return 20; }//小竖杆尺寸40
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高40


private:

};

class CRCRailingT1 : public CRCRailingTieyi  //栏杆类
{
public:
	CRCRailingT1();

	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetStandardRailingTotalLen()const;
	virtual double GetNonstandardLen()const;				//获取非标段栏杆长度，含两侧立柱


protected:
	virtual bool GenRailing();
	int GenStandardSegCount(double p_lenth, double p_segLength)const;		//计算标准栏杆数量，p_lenth为栏杆长，p_segLength为栏杆的标准段长
	int GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)const;					//获取标准栏杆两侧单元单元花格数量
	double GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const;	//获取标准栏杆总尺寸
	double GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount)const;							//获取栏杆侧边留空间隙

};

