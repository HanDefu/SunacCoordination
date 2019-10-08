#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"
#include "../PrototypeInfo.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos, double p_angle); //生成并插入到指定点

	//////////////////////////////////////////////////////////////////////////
	//属性值的设置选项
	virtual vCString GetTaipenOptions();// 获取台盆选项
	virtual CString GetTaipenDefault(); //台盆默认值

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();

	virtual bool CheckParameter() { return true; } //插入前检查参数合法性

	//其余的属性值可以通过直接设置AttrBathroom对象的变量实现
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual int SelectTaipen(AcDbObjectId bathroomId, CString taipen);
	virtual int SelectMatong(AcDbObjectId bathroomId, CString matong);
	virtual int SelectGuanxiWidth(AcDbObjectId bathroomId, double width);

	virtual int SetMatongPos(AcDbObjectId bathroomId, double jinShen) { return 0; } //自动计算并设置马桶位置

	virtual double GetXLength(){ return m_attr.m_width; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return m_attr.m_height; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	AttrBathroom m_attr;

	AcDbObjectId m_id; //生成并插入到图上的厨房块ID
};

class CBathroomGenKI : public CBathroomGen
{
public:
	CBathroomGenKI(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	virtual int SetMatongPos(AcDbObjectId bathroomId, double yLen);

protected:
	int SetMatongPos_I3(AcDbObjectId bathroomId, double yLen);
	int SetMatongPos_I4(AcDbObjectId bathroomId, double yLen);
};

class CBathroomGenKU : public CBathroomGen
{
public:
	CBathroomGenKU(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomGenKL : public CBathroomGen
{
public:
	CBathroomGenKL(AttrBathroom* p_att) : CBathroomGen(p_att) {}

	//int SetMatongPos(AcDbObjectId bathroomId, double yLen);
};

class CBathroomMrg
{
public:
	static CBathroomGen* CreateBathroomByAttribute(AttrBathroom* p_attr);
};
