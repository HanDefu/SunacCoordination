#pragma once

//需要在初始化的initApp时候调用CDocumentDataSerialize::rxInit();

const CString cDocmentDataSerialize_DBXSERVICE =  _T("SunacStandard_DBXSERVICE");
const CString cSunacDocmentDataKey = TEXT("SunacDocName");

class CDocumentDataSerialize : public AcDbObject
{
public:
	static void InitSerialize();

	CDocumentDataSerialize();
	virtual ~CDocumentDataSerialize();

	ACRX_DECLARE_MEMBERS(CDocumentDataSerialize);
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler)const;

protected:
	Adesk::UInt32 m_version;
};
