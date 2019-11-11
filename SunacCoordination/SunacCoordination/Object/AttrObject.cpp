/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrObject.h"
#include "../Common/ComFun_Sunac.h"
#include "../GlobalSetting.h"
#include "../WebIO/WebIO.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrObject, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEOBJECT, ZffCustomObjectDBOBJECT);

AttrObject::AttrObject()
{
	m_version = 0;
	m_isJiTuan = true;
	m_isDynamic = true;
}

AttrObject::~AttrObject()
{
}

AttrObject::AttrObject(const AttrObject &other)
{
	*this = other;
}

AttrObject & AttrObject::operator=(const AttrObject &rhs)
{
	m_version = rhs.m_version;
	m_prototypeCode = rhs.m_prototypeCode;
	//m_name = rhs.m_name;
	m_isJiTuan = rhs.m_isJiTuan;
	m_quyuId = rhs.m_quyuId;
	m_quyuName = rhs.m_quyuName;
	//m_type = rhs.m_type;
	m_isDynamic = rhs.m_isDynamic;
	m_file = rhs.m_file;
	m_instanceCode = rhs.m_instanceCode;
	return *this;
}

Acad::ErrorStatus AttrObject::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	// Call dwgInFields from AcDbObject
	if ((es = AcDbObject::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	}

	filer->readItem(&m_version);

	AcString tempStr;
    filer->readString(tempStr);
	m_prototypeCode = tempStr.kACharPtr();

	filer->readString(tempStr);
	m_instanceCode = tempStr.kACharPtr();

	dwgInFileInfo(filer, m_file);

	filer->readItem(&m_isDynamic);

	filer->readItem(&m_isJiTuan);

	filer->readString(tempStr);
	m_quyuName = tempStr.kACharPtr();

	filer->readString(tempStr);
	m_quyuId = tempStr.kACharPtr();

	return filer->filerStatus();
}

Acad::ErrorStatus AttrObject::dwgInFileInfo(AcDbDwgFiler* filer, CDwgFileInfo& pFileInfo)
{
	AcString tempStr;
	filer->readString(tempStr);
	pFileInfo.fileName = tempStr.kACharPtr();
	filer->readString(tempStr);
	pFileInfo.previewFile = tempStr.kACharPtr();
	filer->readItem(&pFileInfo.id);
	filer->readItem(&pFileInfo.PreviewID);
	return Acad::eOk;
}

Acad::ErrorStatus AttrObject::dwgOutFields(AcDbDwgFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	// Call dwgOutFields from AcDbObject
	if ((es = AcDbObject::dwgOutFields(filer)) != Acad::eOk) {
		return es;
	}

	Adesk::Int32 version = FILE_VERSION;
	filer->writeItem(version);

	filer->writeItem(m_prototypeCode);

	filer->writeItem(m_instanceCode);

	dwgOutFileInfo(filer, m_file);

	filer->writeItem(m_isDynamic);

	filer->writeItem(m_isJiTuan);

	filer->writeItem(m_quyuName);

	filer->writeItem(m_quyuId);

	return filer->filerStatus();
}

Acad::ErrorStatus AttrObject::dwgOutFileInfo(AcDbDwgFiler* filer, const CDwgFileInfo& pFileInfo) const
{
	filer->writeItem(pFileInfo.fileName);
	filer->writeItem(pFileInfo.previewFile);
	filer->writeItem(pFileInfo.id);
	filer->writeItem(pFileInfo.PreviewID);
	return Acad::eOk;
}

bool AttrObject::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	//不用比较version
	if (m_prototypeCode != other->m_prototypeCode)
		return false;

	if (m_instanceCode != other->m_instanceCode)
		return false;

	//if (m_name != other->m_name)
	//	return false;

	if (m_isJiTuan != other->m_isJiTuan)
		return false;

	if (m_quyuName != other->m_quyuName)
		return false;

	//if (m_type != other->m_type)
	//	return false;

	if (m_isDynamic != other->m_isDynamic)
		return false;

	return true;

	/*return (m_id == other->m_id && 
		m_name == other->m_name && 
		m_isJiTuan == other->m_isJiTuan && 
		m_quyuName == other->m_quyuName && 
		m_type == other->m_type && 
		m_isDynamic == other->m_isDynamic
		);*/
}

//int AttrObject::GetFile(CString &filePathName)
//{
//	//首先本地搜索
//	bool has = GSINST->GetLocalFile(m_file.fileName, filePathName);
//	if (has)
//		return 0;
//	
//	////如果本地不存在这个文件 去服务器下载一个文件到本地
//	//int ret = WebIO::DownloadFile(m_prototypeCode, filePathName);
//	//if (ret != 0)
//	//	filePathName = L"";
//
//	return 0;
//}

