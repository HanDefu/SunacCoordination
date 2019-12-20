#include "StdAfx.h"
#include "DocumentDataSerialize.h"
#include "Object/WindowDoor/WindowAutoName.h"
#include "Object/Kitchen/KitchenAutoName.h"
#include "Object/Bathroom/BathroomAutoName.h"
#include "Src/DocumentData.h"

void CDocumentDataSerialize::InitSerialize()
{
	AcDbDictionary *pNameObjDict = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);
	if (es!=Acad::eOk)
	{
		return;
	}

	AcDbObjectId dictObjId = AcDbObjectId::kNull;
	es = pNameObjDict->getAt(cSunacDocmentDataKey, dictObjId);
	if (es!=Acad::eOk)
	{
		CDocumentDataSerialize* pAutoNameSerialize = new CDocumentDataSerialize();
		pNameObjDict->setAt(cSunacDocmentDataKey, pAutoNameSerialize, dictObjId);
		pAutoNameSerialize->close();


		CDocumentData* pCurDocData = CDocumentFactory::Instance().GetCurDocData();
		if (pCurDocData!=NULL)
		{
			pCurDocData->m_bLoad = true; //新添加的也标记数据为
		}
	}

	pNameObjDict->close();
}

 //ACRX_DXF_DEFINE_MEMBERS(CLASS_NAME,PARENT_CLASS,DWG_VERSION,\
 //             MAINTENANCE_VERSION,PROXY_FLAGS,DXF_NAME,APP) \

ACRX_DXF_DEFINE_MEMBERS(CDocumentDataSerialize, AcDbObject,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, SunacSerialize, "SunacCAD");

//需要在初始化的initApp时候调用CDocumentDataSerialize::rxInit();  

CDocumentDataSerialize::CDocumentDataSerialize()
{
	m_version = 1;
}
CDocumentDataSerialize::~CDocumentDataSerialize()
{
}


Acad::ErrorStatus CDocumentDataSerialize::dwgInFields(AcDbDwgFiler* pFiler)
{
	CDocumentData* pCurDocData = CDocumentFactory::Instance().GetCurDocData();
	if (pCurDocData == NULL || pCurDocData->m_bLoad) //当前文档的数据只在打开文档时加载一次，对于拷贝和插入文件作为块时进入此函数的情况不读取dwg的文档数据
	{
		return pFiler->filerStatus();
	}
	
	//1. 调用父类的函数
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) 
		return es;

	//2. 读取数据版本号
	pFiler->readItem(&m_version);
	
	//3. 读取数据
	pCurDocData->ReadFromDwg(pFiler, m_version);
	
	//4. 结束
	return pFiler->filerStatus();
}

Acad::ErrorStatus CDocumentDataSerialize::dwgOutFields(AcDbDwgFiler* pFiler)const
{
	//1. 调用父类的函数
	assertReadEnabled();
	Acad::ErrorStatus es= AcDbObject::dwgOutFields(pFiler);
	if (es  != Acad::eOk)
		return es;

	//2. 写入数据版本号
	Adesk::UInt32 nVersion = FILE_VERSION;
	pFiler->writeItem(nVersion);

	//3. 写入数据
	CDocumentFactory::Instance().GetCurDocData()->WriteToDwg(pFiler);
	
	//4. 结束
	return pFiler->filerStatus();
}
