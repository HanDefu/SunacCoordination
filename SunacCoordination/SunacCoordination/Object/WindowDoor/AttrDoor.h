/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_DOOR)
#define ARX__ZFFDWGSCALE_H__20140205_113541_DOOR

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

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_DOOR
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_DOOR "ZFFCUSTOMOBJECTDB_DBXSERVICE_DOOR"
#endif

class DLLIMPEXP  AttrDoor : public AttrObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrDoor);

	// Constructor / Destructor
	AttrDoor();
	virtual ~AttrDoor();

	AttrDoor(const AttrDoor &other);
	AttrDoor & operator=(const AttrDoor &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	virtual eRCType GetType() {return DOOR;}
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
private:

public:
	CString m_frontViewFile;	//原型立面文件, 展开图用基类的m_fileName
	CString	m_topViewFile;		//原型俯视图文件
	CString m_leftViewFile;		//原型侧视图文件

	double m_minWid;//宽度尺寸最小值
	double m_maxWid;//宽度尺寸最大值

	CString m_DoorType;//门类型

	vSRCDimData m_dimData;
private:

};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_DOOR)