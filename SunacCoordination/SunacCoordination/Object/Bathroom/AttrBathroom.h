/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)
#define ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../AttrObject.h"
#include "../../Sunac_def.h"
#include "../PrototypeCommonDef.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom "ZFFCUSTOMOBJECTDB_DBXSERVICE_Bathroom"
#endif

class DLLIMPEXP  AttrBathroom : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrBathroom);

	// Constructor / Destructor
	AttrBathroom();
	virtual ~AttrBathroom();

	AttrBathroom(const AttrBathroom &other);
	AttrBathroom & operator=(const AttrBathroom &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return Bathroom;}
	virtual bool isEqualTo(AttrObject*other = 0);//��������һ��

public:
	bool m_isGuoBiao;//������
	int m_floorRange;//¥��ѡ��
	double m_airVentW;//����������
	double m_airVentH;//����������
	bool m_isMirror;//����

	bool m_hasPaiQiDao;//�Ƿ���������
	eWindowDoorPos m_windowDoorPos;//�Ŵ�λ�ù�ϵ

	double m_guanXiWidth;//��ϴ������

	CString m_BathroomType; //������
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_Bathroom)