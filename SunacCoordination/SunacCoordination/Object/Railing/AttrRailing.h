
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
	AttrRailing(const AttrRailing &other);
	virtual ~AttrRailing();

	virtual AttrRailing & operator=(const AttrRailing &rhs);
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��
	virtual eRCType GetType() {return RAILING;}

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS


public:
    double m_height;		//���˸߶�
	double m_fanKanHeight;	//�����߶�
	double m_length;	//���˳�
	eRailingType m_railingType; //��������
};