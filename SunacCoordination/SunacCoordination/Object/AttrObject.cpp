/////////////////////////////////////////////
// ZffDwgScale custom object.
#include "StdAfx.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "AttrObject.h"
#include "../Common/ComFun_Sunac.h"
#include "../GlobalSetting.h"
#include "../WebIO.h"


//{{AFX_ARX_MACRO
ACRX_DXF_DEFINE_MEMBERS(AttrObject, AcDbObject,
						 AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
                        AcDbProxyObject::kNoOperation,
						 ZFFDWGSCALEOBJECT, ZffCustomObjectDBOBJECT);

AttrObject::AttrObject()
{
	m_version = 0;
	m_yxid = L"";
    m_name = L"";
	m_isJiTuan = false;
	m_quyuName = L"";
	m_type = L""; 
	m_isDynamic = true;
	m_instBianHao = L"";
}

AttrObject::~AttrObject()
{
	// TODO: clean up.

}

AttrObject::AttrObject(const AttrObject &other)
{
	m_version = other.m_version;
	m_yxid = other.m_yxid;
	m_name = other.m_name;
	m_isJiTuan = other.m_isJiTuan;
	m_quyuName = other.m_quyuName;
	m_type = other.m_type; 
	m_isDynamic = other.m_isDynamic;
	m_instBianHao = other.m_instBianHao;
}

AttrObject & AttrObject::operator=(const AttrObject &rhs)
{
	m_version = rhs.m_version;
	m_yxid = rhs.m_yxid;
	m_name = rhs.m_name;
	m_isJiTuan = rhs.m_isJiTuan;
	m_quyuName = rhs.m_quyuName;
	m_type = rhs.m_type; 
	m_isDynamic = rhs.m_isDynamic;
	m_instBianHao = rhs.m_instBianHao;
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

	ACHAR *tempStr = new ACHAR[SUNAC_COMMON_STR_LEN];
    filer->readItem(&tempStr);
	m_yxid = CString(tempStr);

	filer->readItem(&tempStr);
	m_instBianHao = CString(tempStr);


	filer->readItem(&tempStr);
	m_name = CString(tempStr);

	filer->readItem(&m_isJiTuan);

	filer->readItem(&tempStr);
	m_quyuName = CString(tempStr);

	filer->readItem(&tempStr);
	m_type = CString(tempStr);

	filer->readItem(&m_isDynamic);

	delete [] tempStr;
	return filer->filerStatus();
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
	filer->writeItem(m_yxid);
	filer->writeItem(m_instBianHao);
	filer->writeItem(m_name);
	filer->writeItem(m_isJiTuan);
	filer->writeItem(m_quyuName);
	filer->writeItem(m_type);
	filer->writeItem(m_isDynamic);

	return filer->filerStatus();
}

bool AttrObject::isEqualTo(AttrObject*other)
{
	if (other == 0)
		return false;

	//不用比较version
	if (m_yxid != other->m_yxid)
		return false;

	if (m_instBianHao != other->m_instBianHao)
		return false;

	if (m_name != other->m_name)
		return false;

	if (m_isJiTuan != other->m_isJiTuan)
		return false;

	if (m_quyuName != other->m_quyuName)
		return false;

	if (m_type != other->m_type)
		return false;

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

int AttrObject::GetFile(CString &filePathName)
{
	//首先本地搜索
	bool has = GSINST->GetLocalFile(m_name,filePathName);
	if (has)
		return 0;
	
	//如果本地不存在这个文件 去服务器下载一个文件到本地
	int ret = WebIO::DownLoadFile(m_yxid, filePathName);
	if (ret != 0)
		filePathName = L"";

	return ret;
}

