#pragma once
#include <vector>
#include "AttrBathroom.h"
#include "..\PrototypeCommonDef.h"
//#include "../PrototypeInfo.h"

using namespace std;

class CBathroomGen
{
public:
	CBathroomGen(AttrBathroom* p_att);
	virtual ~CBathroomGen();

	virtual AcDbObjectId GenBathroom(const AcGePoint3d p_pos, int p_angle); //生成并插入到指定点

	//////////////////////////////////////////////////////////////////////////
	//属性值的设置选项
	virtual vCString GetTaipenOptions();// 获取台盆选项
	virtual CString GetTaipenDefault(); //台盆默认值

	virtual vCString GetMatongOptions();
	virtual CString GetMatongDefault();

	virtual vCString GetGuanxiquOptions();
	virtual CString GetGuanxiquDefault();

	virtual void InitBathroomByDefault();

	virtual bool CheckParameter(CString& errMsg); //插入前检查参数合法性

	//其余的属性值可以通过直接设置AttrBathroom对象的变量实现
	AttrBathroom* GetBathroomAtt(){ return &m_attr; }

protected:
	virtual void SelectTaipen(AcDbObjectId bathroomId, CString taipen);
	virtual void SelectMatong(AcDbObjectId bathroomId, CString matong);
	virtual void SelectGuanxiWidth(AcDbObjectId bathroomId, double width);

	virtual int SetMatongPos(AcDbObjectId bathroomId, double jinShen) { return 0; } //自动计算并设置马桶位置

	virtual double GetXLength() { return min(m_attr.m_width, m_attr.m_height); } //短边位于X方向
	virtual double GetYLength() { return max(m_attr.m_width, m_attr.m_height); } //长边位于Y方向

protected:
	AttrBathroom m_attr;
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

	//U型为静态厨房，不需要检查
	virtual bool CheckParameter(CString& errMsg) { return true; }

	//U型为静态厨房，不用设置
	virtual void SelectTaipen(AcDbObjectId bathroomId, CString taipen) {}
	virtual void SelectMatong(AcDbObjectId bathroomId, CString matong) {}
	virtual void SelectGuanxiWidth(AcDbObjectId bathroomId, double width) {}

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
