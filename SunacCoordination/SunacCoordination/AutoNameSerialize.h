#pragma once

//需要在初始化的initApp时候调用CAutoNameSerialize::rxInit();

const CString AutoNameSerialize_DBXSERVICE =  _T("AutoNameSerialize_DBXSERVICE");

const CString DWG_AutoName_KEY = TEXT("AutoName");

class CAutoNameSerialize : public AcDbObject
{
public:
	CAutoNameSerialize();
	virtual ~CAutoNameSerialize();

	ACRX_DECLARE_MEMBERS(CAutoNameSerialize);
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler)const;

protected:
	Adesk::UInt32 m_version;
};

void InitSerialize();