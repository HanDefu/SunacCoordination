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


	//////////////////////////////////////////////////////////////////////////
	//设置基本属性
	void SetDoorDir(E_DIRECTION p_doorDir){ m_doorDir = p_doorDir; }
	void SetWindowDir(E_DIRECTION p_winDir){ m_windowDir = p_winDir; }

	//其余的属性值可以通过直接设置AttrBathroom对象的变量实现
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual int SelectTaipen(AcDbObjectId bathroomId, CString taipen);

	virtual int SelectMatong(AcDbObjectId bathroomId, CString matong);

	virtual int SelectGuanxiqu(AcDbObjectId bathroomId, CString guanxiqu);

	virtual int SetDoorPos(AcDbObjectId bathroomId, double kaiJian) = 0;

	virtual int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth) = 0;

	virtual int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType) = 0;

	virtual void GetRotateAngle(double &angle, AcGeVector3d &offsetV); //处理旋转关系

	virtual double GetXLength(){ return 0; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return 0; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	AttrBathroom m_attr;

	AcDbObjectId m_id; //生成并插入到图上的厨房块ID

	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;

	double m_angle;
};

class CBathroomGenKI : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomGenKU : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomGenKL : public CBathroomGen
{
	//CBathroomGenKI(AttrBathroom* p_att);

	int SetTaipenPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString taipenWidth);

	int SetMatongPos(AcDbObjectId bathroomId, double kaiJian, double jinShen, CString matongType);
};

class CBathroomMrg
{
public:
	static CBathroomMrg* GetInstance();

	vector<AttrBathroom*> FilterBathroom(EBathroomType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);

protected:
	//检查尺寸与方向
	vector<AttrBathroom*> FilterBathroomTU(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTL(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrBathroom*> FilterBathroomTI(double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
};