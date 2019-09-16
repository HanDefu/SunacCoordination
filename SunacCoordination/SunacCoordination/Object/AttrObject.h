/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../Sunac_def.h"

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

	// Constructor / Destructor
	AttrObject();
	AttrObject(const AttrObject &other);
	AttrObject & operator=(const AttrObject &rhs);

	virtual ~AttrObject();

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	//}}AFX_ARX_METHODS
	
	virtual eRCType GetType(){return TYPENUM;}

	virtual bool isEqualTo(AttrObject*other = 0);//基础数据一致
	
	//得到从服务器下载到本地的一个文件（含路径）
	//或者是本地本身就存在的一个文件路径
	//成功返回0 否则返回-1
	int GetFile(CString &filePathName);

	void SetBianHao(CString  bianHao) { m_instBianHao = bianHao; }
	CString& GetBianHao() { return m_instBianHao; }

private:
	
public:
	Adesk::Int32 m_version;//文件版本 临时存储

	CString m_yxid;//原型编号
	CString m_name;//原型名称
	bool m_isJiTuan;//是否集团
	CString m_quyuName;//区域名称
	CString m_type;//原型类型
	bool m_isDynamic;//是否动态块

	CString m_filePathName;//本地完整路径
	
	CString m_instBianHao;//自定义名称 非块名 实例的编号
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)