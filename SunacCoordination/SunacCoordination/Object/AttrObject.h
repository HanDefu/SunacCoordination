/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../Sunac_def.h"
#include "PrototypeInfo.h"

#pragma warning(disable: 4275 4251)

#ifdef _CUSTOMOBJECTDB_
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP 
#endif

#ifndef ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT
#define ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT "ZFFCUSTOMOBJECTDB_DBXSERVICE_OBJECT"
#endif

//需要存储的都要放这个系列下
class DLLIMPEXP  AttrObject : public AcDbObject
{
public:
	ACRX_DECLARE_MEMBERS(AttrObject);

	AttrObject();
	virtual ~AttrObject();

	AttrObject(const AttrObject &other);
	virtual AttrObject & operator=(const AttrObject &rhs);

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	
	virtual eRCType GetType(){return TYPENUM;}
	virtual bool isEqualTo(AttrObject*other);//基础数据一致
	
	//得到从服务器下载到本地的一个文件（含路径）
	//或者是本地本身就存在的一个文件路径
	//成功返回0 否则返回-1
	int GetFile(CString &filePathName);

	void SetInstanceCode(CString  bianHao) { m_instanceCode = bianHao; }
	CString& GetInstanceCode() { return m_instanceCode; }

	virtual CProBase* GetPrototype();
	
public:
	Adesk::Int32 m_version;//文件版本 临时存储

	CString m_prototypeCode;//原型编号
	//CString m_name;			//原型名称， YUAN 1006注释，所有原型名称用原型编号
	CString m_type;		//原型类型
	bool m_isDynamic;	//是否动态块

	bool m_isJiTuan;//是否集团
	CString m_quyuId;//区域ID
	CString m_quyuName;//区域名称

	CString m_fileName;//原型文件名,包含.dwg，但不包含路径
	
	CString m_instanceCode;//实例编号
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)