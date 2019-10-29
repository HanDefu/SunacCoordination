/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON)
#define ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "..\AttrObject.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON "ZFFCUSTOMOBJECTDB_DBXSERVICE_AIRCON"
#endif


typedef enum eLengNingShuiGuanPos//冷凝水管位置
{
	AIRCON_CEBIAN,//侧边
	AIRCON_HOUBIAN//后边
}eLingNingShuiGuanPos;


class DLLIMPEXP  AttrAirCon : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrAirCon);

	AttrAirCon();
	virtual ~AttrAirCon();

	AttrAirCon(const AttrAirCon &other);
	AttrAirCon & operator=(const AttrAirCon &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	virtual eRCType GetType() {return AIRCON;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
	virtual bool IsPrototypeEqual(const AttrAirCon& p_att);

	CString AutoInstanceCode(); //空调自动编号只考虑匹数，仅用于统计，不用于查找块

public:
	double m_power;			//空调匹数
	CString m_pipePos;		//空调冷凝水管位置
	bool m_hasRainPipe;		//空调是否有雨水立管穿过
	CString m_rainPipePos;	//空调雨水管位置

	int m_airW;		//对应空调数据表格
	int m_airH;
	int m_airD;
};

#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_AIRCON)