
#pragma once

#include <vector>
#include "..\RCStandBlock.h"
#include "AttrRailing.h"
#include "RCRailing.h"

//////////////////////////////////////////////////////////////////////////
class CRCRailingBoli : public CRCRailing
{
public:
	virtual double GetNonstandardLen()const{ return 0; }//没有非标准段
	virtual double GetStandardRailingTotalLen()const{ return GetLength(); } //没有非标准段

	virtual int GenerateRailing(AcGePoint3d start, AcDbObjectId &p_railingIdOut);


protected:
	virtual bool GenRailing();
	virtual int GenStandardSegCount(double p_lenth)const = 0;			//计算标准栏杆数量
	virtual double GenStandardRailingLen(double p_lenth)const = 0;		//计算标准栏杆尺寸


	CString GetStandardBlockName() const;
	CString GetLeftBlockName() const;
	CString GetRightBlockName() const;
	CString GetHandRailBlockName() const;

	virtual double GetH() const { return 0; }				//单元之间的间距，默认为0
	virtual double GetHandRailHeight()const { return 33; };	//扶手厚度高

	virtual AcDbObjectId GenerateRailing_Left(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_Right(AcGePoint3d pos);
	virtual AcDbObjectIdArray GenerateRailing_Standard(AcGePoint3d pos);
	virtual AcDbObjectId GenerateRailing_HandRail(AcGePoint3d pos);
};


//////////////////////////////////////////////////////////////////////////
class CRCRailingB1 : public CRCRailingBoli
{
public:
	CRCRailingB1();
	~CRCRailingB1();

	virtual double Get2K()const { return 275; }			//2K固定值275
	virtual double GetK()const { return 275/2.0; }			//2K固定值275

protected:
	virtual int GenStandardSegCount(double p_lenth)const;		//计算标准栏杆数量
	virtual double GenStandardRailingLen(double p_lenth)const;		//计算标准栏杆尺寸



protected:
	virtual double GetH() const { return 150; }		//单元之间的间距
	double GetHandRailHeight()const { return 33; }	//扶手厚度高40


};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB2 : public CRCRailingBoli
{
public:
	CRCRailingB2();
	~CRCRailingB2();

	virtual double GetH() const { return 0; }

protected:
	int GenStandardSegCount(double p_lenth)const;			//计算标准栏杆数量
	double GenStandardRailingLen(double p_lenth)const;		//计算标准栏杆尺寸
};

//////////////////////////////////////////////////////////////////////////
class CRCRailingB4 : public CRCRailingBoli
{
public:
	CRCRailingB4();
	~CRCRailingB4();

	virtual double Get2K()const { return 150; }
	virtual double GetH() const { return 0; }

protected:
	int GenStandardSegCount(double p_lenth)const;			//计算标准栏杆数量
	double GenStandardRailingLen(double p_lenth)const;		//计算标准栏杆尺寸

};