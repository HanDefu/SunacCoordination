#include "StdAfx.h"
#include "AutoNameSerialize.h"
#include "Object/WindowDoor/WindowAutoName.h"
#include "Object/Kitchen/KitchenAutoName.h"
#include "Object/Bathroom/BathroomAutoName.h"
#include "Src/DocumentData.h"

static const Adesk::UInt32  c_nAutoNameVersion = 1; //测试用，每次修改序列化内容+1，若版本号不一致，则不读取

void InitSerialize()
{
	AcDbDictionary *pNameObjDict = NULL;
	AcDbObjectId dictObjId;

	acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNameObjDict, AcDb::kForWrite);

	CAutoNameSerialize* pAutoNameSerialize = new CAutoNameSerialize();
	pNameObjDict->setAt(DWG_AutoName_KEY, pAutoNameSerialize, dictObjId);
	pAutoNameSerialize->close();

	pNameObjDict->close();
}

ACRX_DXF_DEFINE_MEMBERS(CAutoNameSerialize, AcDbObject,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, AutoNameSerialize, "Sunac");

//需要在初始化的initApp时候调用CAutoNameSerialize::rxInit();  

CAutoNameSerialize::CAutoNameSerialize()
{
	m_version = 1;
}
CAutoNameSerialize::~CAutoNameSerialize()
{
}


Acad::ErrorStatus CAutoNameSerialize::dwgInFields(AcDbDwgFiler* pFiler)
{
	//1. 调用父类的函数
	assertWriteEnabled();
	Acad::ErrorStatus es = AcDbObject::dwgInFields(pFiler);
	if (es != Acad::eOk) 
		return es;

	//2. 读取数据版本号
	pFiler->readItem(&m_version);
	
	//3. 读取数据
	GetWindowAutoName()->ReadFromDwg(pFiler);
	GetKitchenAutoName()->ReadFromDwg(pFiler);
	GetBathroomAutoName()->ReadFromDwg(pFiler);
	
	//4. 结束
	return pFiler->filerStatus();
}

Acad::ErrorStatus CAutoNameSerialize::dwgOutFields(AcDbDwgFiler* pFiler)const
{
	//1. 调用父类的函数
	assertReadEnabled();
	Acad::ErrorStatus es= AcDbObject::dwgOutFields(pFiler);
	if (es  != Acad::eOk)
		return es;

	//2. 写入数据版本号
	Adesk::UInt32 nVersion = c_nAutoNameVersion;
	pFiler->writeItem(nVersion);

	//3. 写入数据
	GetWindowAutoName()->WriteToDwg(pFiler);
	GetKitchenAutoName()->WriteToDwg(pFiler);
	GetBathroomAutoName()->WriteToDwg(pFiler);
	
	//4. 结束
	return pFiler->filerStatus();
}
