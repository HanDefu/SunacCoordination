#pragma once
#include <vector>
#include "AttrKitchen.h"
#include "..\PrototypeCommonDef.h"

using namespace std;

class CKitchGen
{
public:
	CKitchGen(AttrKitchen* p_att);
	virtual ~CKitchGen();

	virtual AcDbObjectId GenKitchen(const AcGePoint3d p_pos); //生成并插入到指定点

	//////////////////////////////////////////////////////////////////////////
	//属性值的设置选项
	virtual vCString GetShuipenOptions();// 获取水盆选项
	virtual CString GetShuipenDefault(); //水盆默认值

	virtual vCString GetBinxiangOptions();// 获取冰箱选项
	virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// 获取灶台选项
	virtual CString GetZhaotaiDefault();


	//////////////////////////////////////////////////////////////////////////
	//设置基本属性
	void SetDoorDir(E_DIRECTION p_doorDir){ m_doorDir = p_doorDir; }
	void SetWindowDir(E_DIRECTION p_winDir){ m_windowDir = p_winDir; }

	//其余的属性值可以通过直接设置AttrKitchen对象的变量实现
	AttrKitchen* GetKitchenAtt(){ return &m_attr; }

protected:
	//kitchenId厨房块参照的id
	//shuiPen:"单盆600"/"单盆800"/双盆900/双盆1000/双盆1200
	virtual int SelectShuiPen(AcDbObjectId kitchenId, CString shuiPen);

	//kitchenId厨房块参照的id shuiPen:"800"/"900"
	virtual int SelectZaoTai(AcDbObjectId kitchenId, CString zaoTai);

	//kitchenId厨房块参照的id
	//bingXiang:"冰箱_单开门_700"/"冰箱_对开门_800"/"冰箱_对开门_1000"
	virtual int SelectBingXiang(AcDbObjectId kitchenId, CString bingXiang);

	//设置门位置关系
	virtual int SetDoorPos(AcDbObjectId kitchenId, double kaiJian) { return 0; }
	//设置灶台的位置 
	virtual int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType){ return 0; }
	//设置水盆的位置
	virtual int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType){ return 0; }

	virtual void GetRotateAngle(double &angle, AcGeVector3d &offsetV); //处理旋转关系
	//virtual void InitMirror(); //主要针对门窗垂直开情况，门窗垂直原型的窗在门的右侧，若实际为左侧则需要对称 //改为对话框内判断

	virtual double GetXLength(){ return m_attr.m_width; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return m_attr.m_height; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	AttrKitchen m_attr;

	AcDbObjectId m_id; //生成并插入到图上的厨房块ID

	E_DIRECTION m_doorDir;
	E_DIRECTION m_windowDir;

	double m_angle;
};
//////////////////////////////////////////////////////////////////////////
class CKitchGenKUQ : public CKitchGen	//浅U，门窗对开
{
public:
	CKitchGenKUQ(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// 获取水盆选项
	virtual CString GetShuipenDefault(); //水盆默认值

	//冰箱和灶台按默认的处理
	//virtual vCString GetBinxiangOptions();// 获取冰箱选项
	//virtual CString GetBinxiangDefault();
	//virtual vCString GetZhaotaiOptions();// 获取灶台选项
	//virtual CString GetZhaotaiDefault();

protected:
	int SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
};

class CKitchGenKUQ_C : public CKitchGen//浅U，门窗垂直开
{
public:
	CKitchGenKUQ_C(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// 获取水盆选项
	virtual CString GetShuipenDefault(); //水盆默认值

	//冰箱和灶台按默认的处理
	//virtual vCString GetBinxiangOptions();// 获取冰箱选项
	//virtual CString GetBinxiangDefault();
	//virtual vCString GetZhaotaiOptions();// 获取灶台选项
	//virtual CString GetZhaotaiDefault();


protected:
	int SetDoorPos(AcDbObjectId kitchenId, double kaiJian);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);

};

class CKitchGenKUS : public CKitchGen////深U，门窗对开
{
public:
	CKitchGenKUS(AttrKitchen* p_att);

	//都按默认处理
	//virtual vCString GetShuipenOptions();// 获取水盆选项
	//virtual CString GetShuipenDefault(); //水盆默认值

	//virtual vCString GetBinxiangOptions();// 获取冰箱选项
	//virtual CString GetBinxiangDefault();

	//virtual vCString GetZhaotaiOptions();// 获取灶台选项
	//virtual CString GetZhaotaiDefault();

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};

class CKitchGenKL : public CKitchGen////深U，门窗对开
{
public:
	CKitchGenKL(AttrKitchen* p_att);

	//virtual vCString GetShuipenOptions();// 获取水盆选项
	//virtual CString GetShuipenDefault(); //水盆默认值

	virtual vCString GetBinxiangOptions();// 获取冰箱选项
	virtual CString GetBinxiangDefault();

	//virtual vCString GetZhaotaiOptions();// 获取灶台选项
	//virtual CString GetZhaotaiDefault();
	virtual double GetXLength(){ return m_attr.m_height; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return m_attr.m_width; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);
	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};

class CKitchGenKI : public CKitchGen////深U，门窗对开
{
public:
	CKitchGenKI(AttrKitchen* p_att);

	virtual vCString GetShuipenOptions();// 获取水盆选项
	virtual CString GetShuipenDefault(); //水盆默认值

	//virtual vCString GetBinxiangOptions();// 获取冰箱选项
	//virtual CString GetBinxiangDefault();

	virtual vCString GetZhaotaiOptions();// 获取灶台选项
	virtual CString GetZhaotaiDefault();

	virtual double GetXLength(){ return m_attr.m_height; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向
	virtual double GetYLength(){ return m_attr.m_width; } //获得x方向的长度，width是面宽，height是进深，但有时候width并非x方向

protected:
	int SetShuiPenPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString shuiPenType);

	int SetZaoTaiPos(AcDbObjectId kitchenId, double kaiJian, double jinShen, CString zaoTaiType);

};
class CKitchGenSTATIC: public CKitchGen////静态厨房
{
public:
	CKitchGenSTATIC(AttrKitchen* p_att);



};


//////////////////////////////////////////////////////////////////////////
class CKitchMrg
{
	CKitchMrg();
public:
	static CKitchMrg *GetInstance();
	~CKitchMrg();


	//根据搜索条件返回原型
	vector<AttrKitchen*> FilterKitch(EKitchType p_type, double p_width, double p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);

	//根据原型创建厨房生成对象
	CKitchGen* CreateKitchGenByKitchType(AttrKitchen* p_attKitchen);

protected:
	vector<AttrKitchen*> FilterKitch_Internal(EKitchType p_type, int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterStatic(int p_width, int p_height, E_DIRECTION p_doorDir, E_DIRECTION p_windowDir, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUq(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUqc(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKUs(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKL(int p_width, int p_height, bool p_bHasAirVent);
	vector<AttrKitchen*> FilterKI(int p_width, int p_height, bool p_bHasAirVent);

};