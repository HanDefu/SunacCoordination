/////////////////////////////////////////////
// ZffDwgScale custom object.

#if !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)
#define ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "../Sunac_def.h"
#include "AcString.h"

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
	virtual AttrObject& operator=(const AttrObject &rhs);
	void Clone(const AttrObject& p_src); //克隆只克隆数据，不克隆关联关系，如m_instanceCodeId

	//{{AFX_ARX_METHODS(ZffDwgScale)
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const;
	virtual Acad::ErrorStatus dwgOutFileInfo(AcDbDwgFiler* pFiler, const CDwgFileInfo& pFileInfo) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	virtual Acad::ErrorStatus dwgInFileInfo(AcDbDwgFiler* pFiler, CDwgFileInfo& pFileInfo);
	//}}AFX_ARX_METHODS
	
	virtual eRCType GetType(){return S_TYPENUM;}
	virtual bool isEqualTo(AttrObject*other);//基础数据一致

	CString GetFileName()const{ return m_file.fileName; }
	void SetFileName(CString p_str){ m_file.fileName = p_str; }

	void SetInstanceCode(CString  bianHao);
	void SetInstanceCodeObjectId(AcDbObjectId p_id);
	CString GetInstanceCode()const { return m_instanceCode; }
	AcDbObjectId GetInstanceCodeObjectId()const { return m_instanceCodeId; }
	
	//20200324 版本6：楼层信息从AttrWindow移到基类，以便支持所有的类型
	CFloorInfo GetFloorInfo()const { return m_floorInfo; }
	void SetFloorInfo(CFloorInfo p_info) { m_floorInfo = p_info;  close(); }

	virtual eViewDir GetViewDir()const { return m_viewDir; }
	virtual void SetViewDir(eViewDir p_view) { } //默认不能改变视图方向

public:
	Adesk::Int32 m_version;		//文件版本 序列化时使用

	//原型属性
	CString m_prototypeCode;//原型编号 //所有原型名称用原型编号
	CDwgFileInfo m_file;	//原型文件名,包含.dwg，但不包含路径
	bool m_isDynamic;		//是否动态块
	bool m_isJiTuan;		//是否集团
	CString m_quyuId;			//区域ID
	CString m_quyuName;			//区域名称

	//////////////////////////////////////////////////////////////////////////
	//实例属性
	CString m_instanceCode;			//实例编号
	AcDbObjectId m_instanceCodeId; //实例编号在图上显示的文字ID  201912


	CFloorInfo m_floorInfo; //楼层信息 YUAN 20200324楼层信息从AttrWindow移到基类，以便支持所有的类型

	eViewDir m_viewDir;//视图方向，平面图、立面图、侧视图 YUAN 20200324楼层信息从AttrWindow移到基类，以便支持所有的类型
};



#endif // !defined(ARX__ZFFDWGSCALE_H__20140205_113541_OBJECT)