
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

	virtual int Getn()const { return m_n; }

	virtual double Getb()const{ return 120; }			//单元花格尺寸120
	virtual double GetH() const { return 280; }			//H固定值280
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40
	virtual double GetSmallPillarWidth()const { return 20; }//小竖杆尺寸40
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高40
	
	//////////////////////////////////////////////////////////////////////////
	virtual double GetStandardRailingTotalLen()const;
	virtual double GetNonstandardLen()const;				//获取非标段栏杆长度，含两侧立柱

	virtual double GetMinWidth()const { return 0; }
	virtual double GetMinHeight()const { return 400; }

protected:
	virtual bool GenRailing(); //计算各段的长度
	virtual int GenStandardSegCount(double p_lenth, double p_segLength)const; //计算标准栏杆数量，p_lenth为栏杆长，p_segLength为栏杆的标准段长
	virtual int GenNonstandardUnitCount(double p_lenth, double p_segLength, int p_standardSegCount)const;	//获取标准栏杆两侧单元单元花格数量
	virtual double GenStandardRailingTotalLen(double p_segLength, int p_standardSegCount)const;	//获取标准栏杆总尺寸
	virtual double GenK(double p_lenth, double p_segLength, int p_standardSegCount, int p_nonStandardUnitCount)const;	//获取栏杆侧边留空间隙

	virtual AcDbObjectId CreateRailingBlockDefine(CString sRailingDefName);

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

	int	   m_n;			//标准栏杆两侧单元花格数量
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

	virtual double Getb()const{ return 70; }			//单元花格尺寸
	virtual double GetH() const { return 180; }			//H固定值280
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸40
	virtual double GetSmallPillarWidth()const { return 20; }//小竖杆尺寸40
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高40

};

//////////////////////////////////////////////////////////////////////////
class CRCRailingT4 : public CRCRailingTieyi
{
public:
	CRCRailingT4();

	virtual double Getb()const{ return 114; }			//单元花格尺寸
	virtual double GetH() const { return 280; }			//H固定值
	virtual double GetPillarWidth()const { return 40; }	//立柱尺寸
	virtual double GetSmallPillarWidth()const { return 14; }//小竖杆尺寸
	virtual double GetHandRailHeight()const { return 40; };	//扶手厚度高

	virtual double GetMinHeight()const { return 800; }

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


	virtual double GetMinHeight()const { return 500; }

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

	virtual double GetMinHeight()const { return 800; }
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

	virtual double GetMinHeight()const { return 1000; }
};

