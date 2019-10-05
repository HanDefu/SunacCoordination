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
	virtual ~CRCRailing(void);

	//CRCRailing(const CRCRailing &other);
	//virtual CRCRailing & operator=(const CRCRailing &rhs);

	virtual RCTYPE GetType()const{ return T_RAILING; }
	//virtual CRCRailing * Clone() = 0; //TODO
	
	void SetRailingAtt(const AttrRailing p_railingAtt);
	AttrRailing GetRailingAtt()const { return m_railingAtt; }

	virtual bool SetLength(double p_length);
	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end) = 0;

	//////////////////////////////////////////////////////////////////////////
	virtual double GetLenght()const { return m_railingAtt.m_length; } //获取栏杆总长度
	virtual double Get2K()const { return 2 * m_K; }		//获取栏杆侧边留空间隙
	virtual double GetK()const { return m_K; }		//获取栏杆侧边留空间隙
	virtual double GetB()const { return m_B; }			//获取标准栏杆尺寸
	virtual int GetN()const { return m_N; }
	virtual int Getn()const { return m_n; }

	virtual double GetStandardRailingTotalLen()const = 0;
	virtual double GetNonstandardLen()const = 0;	//获取非标段栏杆长度，含两侧立柱

	virtual double Getb()const = 0;				//单元花格尺寸120
	virtual double GetH() const = 0;			//H固定值280
	virtual double GetPillarWidth()const = 0;	//立柱尺寸40
	virtual double GetSmallPillarWidth()const = 0;//小竖杆尺寸40
	virtual double GetHandRailHeight()const = 0;	//扶手厚度高40

protected:
	virtual bool GenRailing() = 0;

protected:
	//double m_length;	//栏杆总尺寸 譬如5400
	double m_B;			//标准栏杆尺寸 1260或者1380
	int	   m_N;			//标准栏杆段数量 
	int	   m_n;			//标准栏杆两侧单元花格数量（两侧）
	double m_K;			//侧边立柱与结构墙间尺寸（单边）

protected: 
	AttrRailing m_railingAtt;


	//////////////////////////////////////////////////////////////////////////
	
};

typedef std::vector<CRCRailing*> vpRCRailing;


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

	virtual int GenerateRailing(AcGePoint3d start, AcGePoint3d end);

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

//////////////////////////////////////////////////////////////////////////
class CRCRailingBoli : public CRCRailing
{
public:
	CRCRailingBoli();
	~CRCRailingBoli();

private:

};


CRCRailing* CreateRailing(const AttrRailing p_railingAtt);

