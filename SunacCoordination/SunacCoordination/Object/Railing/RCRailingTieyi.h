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
	
	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);

	//////////////////////////////////////////////////////////////////////////
	virtual double GetStandardRailingTotalLen()const;
	virtual double GetNonstandardLen()const;				//获取非标段栏杆长度，含两侧立柱

protected:
	virtual bool GenRailing(); //计算各段的长度
	virtual int GenStandardSegCount(double p_lenth, double p_segLength)const; //计算标准栏杆数量，p_lenth为栏杆长，p_segLength为栏杆的标准段长
	virtual int GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)const;	//获取标准栏杆两侧单元单元花格数量
	virtual double GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const;	//获取标准栏杆总尺寸
	virtual double GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount)const;	//获取栏杆侧边留空间隙

protected:
	CString GetStandardBlockName() const;
	CString GetNonStandardBlockName() const;
	CString GetHandRailBlockName() const;
	double B1() const { return m_B1; }
	double B2() const { return m_B2; }

	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_HandRail(AcGePoint3d pos);


protected:
	double m_B1;
	double m_B2;
};

class CRCRailingT1 : public CRCRailingTieyi  //栏杆类, T1-T2都可以用此类
{
public:
	CRCRailingT1();



};

//////////////////////////////////////////////////////////////////////////

class CRCRailingT3 : public CRCRailingT1  
{
public:
	CRCRailingT3();

	virtual double Getb()const{ return 120; }			//单元花格尺寸120
	virtual double GetH() const { return 280; }			//H固定值280
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40
	virtual double GetSmallPillarWidth()const { return 20; }//小竖杆尺寸40
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高40

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingT4 : public CRCRailingTieyi
{
public:
	CRCRailingT4();

	virtual double Getb()const{ return 70; }			//单元花格尺寸
	virtual double GetH() const { return 180; }			//H固定值
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40
	virtual double GetSmallPillarWidth()const { return 20; }//小竖杆尺寸40
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高40

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
};

//////////////////////////////////////////////////////////////////////////
class CRCRailingT5 : public CRCRailingTieyi
{
public:
	CRCRailingT5();

	virtual double Getb()const{ return 105; }			//单元花格尺寸
	virtual double GetH() const { return 260; }			//H固定值
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸
	virtual double GetSmallPillarWidth()const { return 15; }//小竖杆尺寸
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingT6 : public CRCRailingTieyi
{
public:
	CRCRailingT6();

	virtual double Getb()const{ return 215; }			//单元花格尺寸
	virtual double GetH() const { return 480; }			//H固定值
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸
	virtual double GetSmallPillarWidth()const { return 15; }//小竖杆尺寸
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高

protected:
	virtual AcDbObjectId GenerateRailing_NonStandard(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingT7 : public CRCRailingTieyi
{
public:
	CRCRailingT7();

	virtual double Getb()const{ return 206; }			//单元花格尺寸
	virtual double GetH() const { return 468; }			//H固定值
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸
	virtual double GetSmallPillarWidth()const { return 12; }//小竖杆尺寸
	virtual double GetHandRailHeight()const { return 100; };	//扶手厚度高

};

