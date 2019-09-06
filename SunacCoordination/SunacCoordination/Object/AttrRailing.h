/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_RAILING)
#define ARX__ZFFDWGSCALE_H__20140205_113541_RAILING

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "AttrObject.h"
#include "../Sunac_def.h"
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

	// Constructor / Destructor
	AttrRailing();
	virtual ~AttrRailing();

	AttrRailing(const AttrRailing &other);
	AttrRailing & operator=(const AttrRailing &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return RAILING;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
private:


public:
    double m_height;
	double m_fanKanHeight;
	double m_dongKouWidht;
	eRailingType m_railingType;
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_RAILING)