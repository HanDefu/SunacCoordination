#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos); //生成并插入到指定点

	//////////////////////////////////////////////////////////////////////////
	//属性值的设置选项
	virtual vCString GetTaipenOptions();// 获取台盆选项
	virtual CString GetTaipenDefault(); //台盆默认值

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();

	virtual bool CheckParameter() { return true; } //插入前检查参数合法性

	//////////////////////////////////////////////////////////////////////////
	//设置基本属性
	void SetDoorDir(E_DIRECTION p_doorDir){ m_doorDir = p_doorDir; }
	void SetWindowDir(E_DIRECTION p_winDir){ m_windowDir = p_winDir; }

	//其余的属性值可以通过直接设置AttrBathroom对象的变量实现
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual int SelectTaipen(AcDbObjectId bathroomId, CString taipen);
	virtual int SelectMatong(AcDbObjectId bathroomId, CString matong);
	virtual int SelectGuanxiWidth(AcDbObjectId bathroomId, double width);

	virtual int SetMatongPos(AcDbObjectId bathroomId, double jinShen) { return 0; } //自动计算并设置马桶位置

	virtual void GetRotateAngle(double &angle, AcGeVector3d &offsetV); //处理旋转关系

	virtual double GetXLength(){ return m_attr.m_width; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return m_attr.m_height; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	AttrBathroom m_attr;

	AcDbObjectId m_id; //生成并插入到图上的厨房块ID

	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;
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
	static CBathroomMrg* GetInstance();

	vector<AttrBathroom*> FilterBathroom(EBathroomType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);

	//根据原型创建厨房生成对象
	CBathroomGen* CreateBathroomGenByBathroomType(AttrBathroom* p_attBathroom);

protected:
	//检查尺寸与方向
	vector<AttrBathroom*> FilterBathroomTU(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTL(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTI(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
};