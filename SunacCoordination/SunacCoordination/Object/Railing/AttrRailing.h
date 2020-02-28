
#pragma once

#include "dbmain.h"
#include "../AttrObject.h"
#include "../../Sunac_def.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING "ZFFCUSTOMOBJECTDB_DBXSERVICE_RAILING"
#endif

class DLLIMPEXP  AttrRailing : public AttrObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrRailing);

	AttrRailing();
	virtual ~AttrRailing();

	//AttrRailing(const AttrRailing &other);
	//virtual AttrRailing & operator=(const AttrRailing &rhs);

	virtual bool isEqualTo(AttrObject*other);//基础数据一致
	virtual bool IsPrototypeEqual_test(const AttrRailing& p_att);
	virtual eRCType GetType() {return RAILING;}

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS

	CString AutoInstanceCode(); //自动编号

	void SetFloorInfo(CFloorInfo p_info){ m_floorInfo = p_info; }

public:
    double m_height;		//栏杆高度
	double m_fanKanHeight;	//反坎高度
	double m_length;	//栏杆长
	eRailingType m_railingType; //栏杆类型
	CFloorInfo m_floorInfo; //楼层信息
};

class CRailingAndCount
{
public:
	CRailingAndCount(){}

public:
	AttrRailing railAtt;
	int nCount;

	AcDbObjectIdArray objIds;
};

class CRailingCountArray
{
public:
	CRailingCountArray() {}

	void InitByRailingIds(const vAcDbObjectId& p_railingIds);
	void InitByRailingAtts(const vector<AttrRailing>& p_railingAtts, const vector<AcDbObjectId>& p_ids);

	int GetCount()const { return (int)(m_railCountArray.size()); }

	const CRailingAndCount& GetRailing(int p_index){ return m_railCountArray[p_index]; }

private:
	vector<CRailingAndCount> m_railCountArray;
};

