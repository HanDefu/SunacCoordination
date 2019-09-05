/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT "ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT"
#endif

class DLLIMPEXP  AttrObject : public AcDbObject
{
public:

	ACRX_DECLARE_MEMBERS(AttrObject);

	// Constructor / Destructor
	AttrObject();
	AttrObject(const AttrObject &other);
	AttrObject & operator=(const AttrObject &rhs);

	virtual ~AttrObject();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	
	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
private:

public:
	Adesk::Int32 m_version;//文件版本 临时存储

	CString m_id;//原型编号
	CString m_name;//原型名称
	bool m_isJiTuan;//是否集团
	CString m_quyuName;//区域名称
	CString m_type;//原型类型
	bool m_isDynamic;//是否动态块
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)