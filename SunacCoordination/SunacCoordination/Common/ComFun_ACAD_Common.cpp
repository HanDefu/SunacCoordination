#include "StdAfx.h"
#include "ComFun_ACAD_Common.h"
#include <dbidmap.h>
#include <AcDbLMgr.h>
#include <acaplmgr.h>
#include <aced.h>
#include <dbplotsetval.h>
#include <acdocman.h>
#include <dbmline.h>
#include <dbspline.h>
#include <dbelipse.h>
#include <acutmem.h>
#include <dbAnnotationScale.h >
#include <AcString.h>
#include <dbObjectContextCollection.h>
#include <dbObjectContextManager.h>
#include <dbhatch.h>
#include <genurb2d.h>
#include <gecurv2d.h>
#include <geent2d.h>
#include <gelnsg2d.h>
#include <adscodes.h>
#include <genurb2d.h>
#include <geell2d.h>
#include "ComFun_Math.h"
#include "ComFun_Str.h"
#include "ComFun_ACAD_Geometry.h"
#include <dbObjectContextCollection.h>
#include <dbObjectContextManager.h>
#include <acdb.h>
#include <dbObjectContextInterface.h>
#include "ComFun_ACad.h"
#include "../Common/ComFun_Convert.h"
#include "../Common/ComFun_Sunac.h"

AcDbObjectId MD2010_PostModalToBlockTable(const ACHAR* entryName, AcDbEntity *pent)
{
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);
	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(entryName,pBlockTableRecord,AcDb::kForWrite);
	
	//保护和转换
	if (pBlockTableRecord == NULL && wcscmp(entryName, L"*Model_Space") == 0)
	{
        pBlockTable->getAt(L"模型",pBlockTableRecord,AcDb::kForWrite);
	}
	if (pBlockTableRecord == NULL && wcscmp(entryName, L"模型") == 0)
	{
		pBlockTable->getAt(L"*Model_Space",pBlockTableRecord,AcDb::kForWrite);
	}

	if(pBlockTableRecord == NULL)
	{
		pBlockTable->close();
		return 0;
	}

	AcDbObjectId entID;
	Acad::ErrorStatus es = pBlockTableRecord->appendAcDbEntity(entID,pent);

	pBlockTable->close();
	pBlockTableRecord->close();

	return entID;
}

AcDbObjectId MD2010_CreateLine(const AcGePoint3d &ptStart, const AcGePoint3d &ptEnd,
							   const ACHAR* entryName, Adesk::UInt8 color)
{
    AcDbLine *pLine = new AcDbLine(ptStart,ptEnd);
	pLine->setColorIndex(color);
	AcDbObjectId lineID = MD2010_PostModalToBlockTable(entryName, pLine);
	pLine->close();
	return lineID;
}

AcDbObjectId MD2010_CreateLine(double startx, double starty, double startz,
							   double endx, double endy, double endz,
							   const ACHAR* entryName, Adesk::UInt8 color)
{
	AcGePoint3d ptStart(startx,starty,startz),ptEnd(endx,endy,endz);
	AcDbLine *pLine = new AcDbLine(ptStart,ptEnd);
	pLine->setColorIndex(color);
	AcDbObjectId lineID = MD2010_PostModalToBlockTable(entryName, pLine);
	pLine->close();
	return lineID;
}

AcDbObjectId MD2010_CreateLine(double startx, double starty,
							   double endx, double endy,
							   const ACHAR* entryName, Adesk::UInt8 color)
{
	AcGePoint3d ptStart(startx,starty,0),ptEnd(endx,endy,0);
	AcDbLine *pLine = new AcDbLine(ptStart,ptEnd);
	pLine->setColorIndex(color);
	AcDbObjectId lineID = MD2010_PostModalToBlockTable(entryName, pLine);
	pLine->close();
	return lineID;
}

AcDbObjectId MD2010_CreateArc(const AcGePoint3d &center, double radius, double startangle, double endangle,
							  const ACHAR* entryName)
{
	AcDbArc *pArc = new AcDbArc(center, radius, startangle, endangle);
	AcDbObjectId arcID = MD2010_PostModalToBlockTable(entryName, pArc);
	pArc->close();
	return arcID;
}

AcDbObjectId MD2010_CreateArc(double centerx, double centery, double centerz,
							  double radius, double startangle, double endangle,
							  const ACHAR* entryName)
{
	AcGePoint3d center(centerx, centery, centerz);
	AcDbArc *pArc = new AcDbArc(center, radius, startangle, endangle);
	AcDbObjectId arcID = MD2010_PostModalToBlockTable(entryName, pArc);
	pArc->close();
	return arcID;
}

AcDbObjectId MD2010_CreateArc(double centerx, double centery,
							  double radius, double startangle, double endangle,
							  const ACHAR* entryName)
{
	AcGePoint3d center(centerx, centery, 0);
	AcDbArc *pArc = new AcDbArc(center, radius, startangle, endangle);
	AcDbObjectId arcID = MD2010_PostModalToBlockTable(entryName, pArc);
	pArc->close();
	return arcID;
}

AcDbObjectId MD2010_CreateCycle(AcGePoint3d center, double radius,AcGeVector3d dir,
								const ACHAR* entryName)
{
	AcDbCircle *pCircle = new AcDbCircle(center, dir, radius);
	AcDbObjectId ID = MD2010_PostModalToBlockTable(entryName, pCircle);
	pCircle->close();
	return ID;
}

AcDbObjectId MD2010_CreateCycle(double centerx, double centery, double centerz, double radius,
								const ACHAR* entryName)
{
	AcGePoint3d center(centerx, centery, 0);
	AcGeVector3d vec(0,0,1);
	AcDbCircle *pCircle = new AcDbCircle(center, vec, radius);
	AcDbObjectId ID = MD2010_PostModalToBlockTable(entryName, pCircle);
	pCircle->close();
	return ID;
}

AcDbObjectId MD2010_CreatePolyLine(vAcGePoint3d &pnts,
							   const ACHAR* entryName, Adesk::UInt8 color)
{
	AcDbPolyline *pPolyLine = new AcDbPolyline();
	pPolyLine->setColorIndex(color);
	for (int i = 0; i < (int)pnts.size(); i++)
	{
		AcGePoint2d temp = AcGePoint2d(pnts[i].x, pnts[i].y);
		pPolyLine->addVertexAt(i, temp);
	}
	
	AcDbObjectId lineID = MD2010_PostModalToBlockTable(entryName, pPolyLine);
	pPolyLine->close();
	return lineID;
}

int MD2010_SetEntityColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{
	AcDbEntity *pent = NULL;
	acdbOpenAcDbEntity(pent,entId,AcDb::kForWrite);
	pent->setColorIndex(colorIndex);
	pent->close();
	return Acad::eOk;
}

Adesk::UInt16 MD2010_GetEntityColor(AcDbObjectId entId)
{
    if (entId == 0)
        return 0;
	AcDbEntity *pent = NULL;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pent,entId,AcDb::kForRead);
	if (es != Acad::eOk)
	    return 0;
	Adesk::UInt16 index = pent->colorIndex();
	pent->close();
	return index;
}

int MD2010_SetEntityLayer(AcDbObjectId objID, CString strLayerName)
{
    AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForWrite);
	pent->setLayer(strLayerName);//use cstring directly! cstring-->ACHAR
	pent->close();
	return Acad::eOk;
}

CString MD2010_GetEntityLayer(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
    ACHAR* layname = pent->layer();
	CString strLayerName(layname);//use ACHAR directly! ACHAR-->cstring
	pent->close();
	return strLayerName;
}

AcDbObjectId MD2010_GetEntityLayerID(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	AcDbObjectId id = pent->layerId();
	pent->close();
	return id;
}

int MD2010_SetLineType(AcDbObjectId objID, CString strLinetype)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForWrite);
	pent->setLinetype(strLinetype);
	pent->close();
	return Acad::eOk;
}

CString MD2010_GetLineType(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	CString strlinetype(pent->linetype());
	pent->close();
	return strlinetype;
}

AcDbObjectId MD2010_GetLineTypeID(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	AcDbObjectId id = pent->linetypeId();
	pent->close();
	return id;
}

int MD2010_SetLineWeight(AcDbObjectId objID, AcDb::LineWeight weight )
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForWrite);
	pent->setLineWeight(weight);
	pent->close();
	return Acad::eOk;
}

AcDb::LineWeight MD2010_GetLineWeight(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	AcDb::LineWeight weight = pent->lineWeight();
	pent->close();
	return weight;
}

AcDbObjectId MD2010_AddAlignedDimension(AcGePoint3d start,AcGePoint3d end, AcGePoint3d dimlinpnt, const WCHAR * entryname,const ACHAR* newLayer)
{
	//AcGePoint3d aas =AcGePoint3d(0,0,0);
	if (JHCOM_PointDistance(start, end) <= TOL*10000)//小于1的不标注
	    return 0;
	
	AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start,end,dimlinpnt);
	
	//pDim->setColorIndex(color);extend
	AcDbObjectId dimID = MD2010_PostModalToBlockTable(entryname, pDim);
	pDim->setLayer(newLayer);
	pDim->close();
	return dimID;
}

AcDbObjectId MD2010_AddAlignedDimension2(AcGePoint3d start, AcGePoint3d end, AcGePoint3d dimlinpnt, const ACHAR* newLayer, int colorIndex, double textHeight, double lineTextSpace)
{
	//AcGePoint3d aas =AcGePoint3d(0,0,0);
	if (JHCOM_PointDistance(start, end) <= TOL*10000)//小于1的不标注
		return 0;

	AcDbAlignedDimension  *pDim = new AcDbAlignedDimension(start,end,dimlinpnt);

	//pDim->setColorIndex(color);extend
	AcDbObjectId dimID = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pDim);
	pDim->setLayer(newLayer);
	pDim->setColorIndex(colorIndex);
	pDim->setDimtxt(textHeight);//设置文字高度
	pDim->setDimgap(lineTextSpace);

	pDim->close();
	return dimID;
}

int MD2010_SetTransparency(AcDbObjectId objID,AcCmTransparency trans)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	pent->setTransparency(trans);
	pent->close();
	return Acad::eOk;
}

AcCmTransparency MD2010_GetTransparency(AcDbObjectId objID)
{
	AcDbEntity *pent;
	acdbOpenAcDbEntity(pent,objID,AcDb::kForRead);
	AcCmTransparency trans = pent->transparency();
	pent->close();
	return trans;
}

//布局
int MD2010_SetModelSpaceCurrentLayout()
{
	//这个函数找到当前active布局,基本上就是当前布局,currentlayoyt和activelayout一样?
	/*const WCHAR *pcur = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);*/

	//中文版的是这个,用 模型==Model ==MODEL 都可以找到模型layout,我们用通用的Model
	/*AcDbLayout * pmodl =      
		acdbHostApplicationServices()->layoutManager()->findLayoutNamed(L"模型");*/
	
	//注意凡是找到的指针类型的,都需要关闭,说明这个已经打开了. 关闭可以进行两次,close函数内部进行了判断,如果已经
	//关闭了,就不再进行一次
	/*if (pmodl) pmodl->close();
	     if (pmodl) pmodl->close();*/

#if (defined ARX_2018) || (defined ARX_2019)
	AcDbObjectId modlid = acdbHostApplicationServices()->layoutManager()->findLayoutNamed(L"Model");
#else
	AcDbLayout * pmodl2 =
		acdbHostApplicationServices()->layoutManager()->findLayoutNamed(L"Model");
	AcDbObjectId modlid = pmodl2->objectId();
#endif
	
	
	acdbHostApplicationServices()->layoutManager()
		->setCurrentLayoutId(modlid);
	return 0;
}

int MD2010_SetCurrentLayout(const ACHAR * layoutname)
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	AcDbLayout * pLayout = NULL;
	const WCHAR *pcur = pAcApLayoutManager->findActiveLayout(true);
	if (wcscmp(pcur, layoutname) == 0)
	{
		return 0;
	}

#if (defined ARX_2018) || (defined ARX_2019)
	if (0 == pAcApLayoutManager->findLayoutNamed(layoutname))
		return 1;
#else
	pLayout = pAcApLayoutManager->findLayoutNamed(layoutname);
	if (NULL == pLayout)
		return 1;
#endif 

	pAcApLayoutManager->setCurrentLayout(layoutname);
	pAcApLayoutManager->updateCurrentPaper(Adesk::kTrue);
	pAcApLayoutManager->updateLayoutTabs();

	if(pLayout != 0)
	    pLayout->close();

	return 0;
}

const ACHAR * MD2010_GetCurrentLayout()
{
    return acdbHostApplicationServices()->layoutManager()->findActiveLayout(true);
}

//should call MD2010_UpdateLayout after this function
int MD2010_CopyLayoutWithSameConfig(const WCHAR * tname, const WCHAR* newname)
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	AcDbLayout * pLayout = NULL;
	if (0 == pAcApLayoutManager->findLayoutNamed(tname))
	    return 1;
	Acad::ErrorStatus es = pAcApLayoutManager->copyLayout(tname,newname);
	pLayout->close();

	return es;
}

bool MD2010_CheckLayoutExist(const WCHAR * tname)
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	AcDbLayout * pLayout = NULL;

	bool flag;
	if (NULL == pAcApLayoutManager->findLayoutNamed(tname))
	    flag= false;
	else 
		flag= true;
	if (flag)
	    pLayout->close();
	return flag;
}

int MD2010_DeleteLayout(const WCHAR * tname)
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	AcDbLayout * pLayout = NULL;
	if (0 == pAcApLayoutManager->findLayoutNamed(tname))
	    return 1;
	pAcApLayoutManager->deleteLayout(tname);
	pLayout->close();
	return 0;
}

void MD2010_UpdateLayout()
{
	AcApLayoutManager *pAcApLayoutManager = (AcApLayoutManager *)(acdbHostApplicationServices()->layoutManager());
	pAcApLayoutManager->updateCurrentPaper(Adesk::kTrue);
	pAcApLayoutManager->updateLayoutTabs();
	return;
}

AcDbObjectId MD2010_CreateNewLayer(CString newname)
{
	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl,AcDb::kForWrite);
	if (pLayerTbl->has(newname))
	{
		pLayerTbl->close();
		return 0;
	}
	//创建新的层表记录
	AcDbLayerTableRecord *pLayerTableRecord = new AcDbLayerTableRecord;
	pLayerTableRecord->setName(newname);

	// 将新建的层表记录添加到层表中
	AcDbObjectId id;
	pLayerTbl->add(id,pLayerTableRecord);

	//
	pLayerTableRecord->close();
	pLayerTbl->close();

	return id;
}

int MD2010_DeleteLayer(CString layerName)
{
	// 获得当前图形的层表
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);
	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}
	// 获得指定层表记录的指针
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);
	pLayerTblRcd->erase(); // 为其设置“删除”标记
	pLayerTblRcd->close();
	pLayerTbl->close();

	return 0;
}

AcDbObjectId MD2010_GetLayerID(CString layerName)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}

	// 获得指定层表记录的指针
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForRead);

	AcDbObjectId ids = pLayerTblRcd->objectId();
	pLayerTblRcd->close();
	pLayerTbl->close();

	return ids;
}

CString MD2010_GetLayerName(AcDbObjectId id)
{
	AcDbObject* pObj;
	acdbOpenAcDbObject(pObj, id, AcDb::kForRead);
	AcDbLayerTableRecord *pLayerTableRecord = NULL;

	// 是否是层类型
	pLayerTableRecord = AcDbLayerTableRecord::cast(pObj);
	if (pLayerTableRecord == NULL)
	    return L"";
	WCHAR * name;
	pLayerTableRecord->getName(name);
	pLayerTableRecord->close();
	return name;
}

int MD2010_GetAllLayers(vCString &vstr, vAcDbObjectId &vids)
{
	// 获得层表指针
	AcDbLayerTable *pLayerTbl;
	AcDbLayerTableRecord *pLayerTblRcd;
	AcDbObjectId id;
	int num = 0;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 使用遍历器访问每一条层表记录
	AcDbLayerTableIterator *pItr;
	pLayerTbl->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getRecord(pLayerTblRcd, AcDb::kForRead);
		// 输出图层的信息
		WCHAR *layerName;
		pLayerTblRcd->getName(layerName);
		vstr.push_back(layerName);
		id = pLayerTblRcd->objectId();
		vids.push_back(id);
		pLayerTblRcd->close();
		num++;
	}
	delete pItr;
	pLayerTbl->close();
	return num;
}

int MD2010_GetLayerCount()
{
	// 获得层表指针
	AcDbLayerTable *pLayerTbl;
	int num = 0;
	acdbHostApplicationServices()->workingDatabase()
		->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 使用遍历器访问每一条层表记录
	AcDbLayerTableIterator *pItr;
	pLayerTbl->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	    num++;
	delete pItr;
	pLayerTbl->close();
	return num;
}


int MD2010_SetCurrentLayer(CString name)
{
	AcDbObjectId id = MD2010_GetLayerID(name);
	if (id == 0)
		return -1;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->setClayer(id);
	return 0;
}

AcDbObjectId MD2010_GetCurrentLayer(CString &name)
{
	AcDbObjectId id;
	id = acdbHostApplicationServices()->workingDatabase()->clayer();
	name = MD2010_GetLayerName(id);
	return id;
}

int MD2010_SetLayerProperty(CString layerName,
							bool bFlagColor,AcCmColor newcolor, 
							bool bFlagLT,AcDbObjectId lintypeid,
							bool bFlagLW,AcDb::LineWeight linw,
							bool bFlagFroze,bool froze, 
							bool bFlagOF,bool onoff, 
							bool bVPFroze,bool vpfroze, 
							bool bLock ,bool lock)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pLayerTbl->has(layerName))
	{
		pLayerTbl->close();
		return 0;
	}

	// 获得指定层表记录的指针
	AcDbLayerTableRecord *pLayerTblRcd;
	pLayerTbl->getAt(layerName, pLayerTblRcd, AcDb::kForWrite);

	AcCmColor oldcolor = pLayerTblRcd->color();

	if (bFlagColor && oldcolor != newcolor)
	    pLayerTblRcd->setColor(newcolor);
	if (bFlagLT && lintypeid != pLayerTblRcd->linetypeObjectId())
		pLayerTblRcd->setLinetypeObjectId(lintypeid);
	if (bFlagLW && linw != pLayerTblRcd->lineWeight())
		pLayerTblRcd->setLineWeight(linw);
	if (bFlagFroze && froze != pLayerTblRcd->isFrozen())
	    pLayerTblRcd->setIsFrozen(froze);
	if (bFlagOF && onoff == pLayerTblRcd->isOff())
		pLayerTblRcd->setIsOff(!onoff);
	if (bVPFroze && vpfroze != pLayerTblRcd->VPDFLT())
		pLayerTblRcd->setVPDFLT(vpfroze);
	if (bLock && lock != pLayerTblRcd->isLocked())
		pLayerTblRcd->setIsLocked(lock);

	pLayerTblRcd->close();
	pLayerTbl->close();

	return 0;
}


//遍历操作
int MD2010_CycleAllTypedObjectsInLayer(CString layname, eACDBOBJECTTYPE type, vAcDbObjectId &vids)
{
	// 判断是否存在名称为“测试”的图层
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForRead);
	if (!pLayerTbl->has(layname))
	{
		//acutPrintf("\n当前图形中未包含\"测试\"图层!");
		pLayerTbl->close();
		return 0;
	}

	AcDbObjectId layerId; // “测试”图层的ID
	pLayerTbl->getAt(layname, layerId);
	pLayerTbl->close();

	// 获得当前数据库的块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	// 创建块表记录遍历器
	AcDbBlockTableRecordIterator *pItr; // 块表记录遍历器
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt; // 遍历的临时实体指针
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		// 利用遍历器获得每一个实体
		pItr->getEntity(pEnt, AcDb::kForRead);
		// 是否在“测试”图层上
		if (pEnt->layerId() == layerId)
		{
			switch(type)
			{
			case ACDB_POINT:
				{
					// 是否是点
					AcDbPoint *prealobj = AcDbPoint::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
						//pLine->setColorIndex(1); // 将直线的颜色修改为红色
					}
					break;
				}
			case ACDB_LINE:
				{
					// 是否是直线
					AcDbLine *prealobj = AcDbLine::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
						//pLine->setColorIndex(1); // 将直线的颜色修改为红色
					}
					break;
				}
			case ACDB_CYCLE:
				{
					// 是否是圆
					AcDbCircle *prealobj = AcDbCircle::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_ARC:
				{
					// 是否是圆
					AcDbArc *prealobj = AcDbArc::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_2dPLINE:
				{
					// 是否是2维多段线
					AcDb2dPolyline *prealobj = AcDb2dPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_MLINES:
				{
					// 是否是多线段平行
					AcDbMline *prealobj = AcDbMline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_SPLINE:
				{
					// 是否是样条曲线
					AcDbSpline *prealobj = AcDbSpline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_ELLIPSE: 
				{
					// 是否是椭圆
					AcDbEllipse *prealobj = AcDbEllipse::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_TEXT: 
				{
					// 是否是文字
					AcDbText *prealobj = AcDbText::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_MTEXT:
				{
					// 是否是多段文字
					AcDbMText  *prealobj = AcDbMText ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_0PLINE:
				{
					// 是否是多段线
					AcDbPolyline *prealobj = AcDbPolyline::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
						//pLine->setColorIndex(1); // 将直线的颜色修改为红色
					}
					break;
				}
			case ACDB_CURVE:
				{
					// 是否是曲线
					AcDbCurve  *prealobj = AcDbCurve ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			case ACDB_HATCH:
				{
					// 是否是曲线
					AcDbHatch  *prealobj = AcDbHatch ::cast(pEnt);
					if (prealobj != NULL)
					{
						vids.push_back(prealobj->objectId());
						num++;
					}
					break;
				}
			default:
				break;
			}
		}
		// 注意需要关闭实体
		pEnt->close();
	}
	delete pItr; // 遍历器使用完毕之后一定要删除！
	pBlkTblRcd->close();
	return num;
}

//遍历操作
int MD2010_CycleAllTypedObjectsInAllLayer(eACDBOBJECTTYPE type, vAcDbObjectId &vids)
{
	// 获得当前数据库的块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	// 创建块表记录遍历器
	AcDbBlockTableRecordIterator *pItr; // 块表记录遍历器
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt; // 遍历的临时实体指针
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		// 利用遍历器获得每一个实体
		pItr->getEntity(pEnt, AcDb::kForRead);
		switch(type)
		{
		case ACDB_POINT:
			{
				// 是否是点
				AcDbPoint *prealobj = AcDbPoint::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
					//pLine->setColorIndex(1); // 将直线的颜色修改为红色
				}
				break;
			}
		case ACDB_LINE:
			{
				// 是否是直线
				AcDbLine *prealobj = AcDbLine::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
					//pLine->setColorIndex(1); // 将直线的颜色修改为红色
				}
				break;
			}
		case ACDB_CYCLE:
			{
				// 是否是圆
				AcDbCircle *prealobj = AcDbCircle::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_ARC:
			{
				// 是否是圆
				AcDbArc *prealobj = AcDbArc::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_2dPLINE:
			{
				// 是否是2维多段线
				AcDb2dPolyline *prealobj = AcDb2dPolyline::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_0PLINE:
			{
				// 是否是多段线
				AcDbPolyline *prealobj = AcDbPolyline::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
					//pLine->setColorIndex(1); // 将直线的颜色修改为红色
				}
				break;
			}
		case ACDB_MLINES:
			{
				// 是否是多线段平行
				AcDbMline *prealobj = AcDbMline::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_SPLINE:
			{
				// 是否是样条曲线
				AcDbSpline *prealobj = AcDbSpline::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_ELLIPSE: 
			{
				// 是否是椭圆
				AcDbEllipse *prealobj = AcDbEllipse::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_TEXT: 
			{
				// 是否是文字
				AcDbText *prealobj = AcDbText::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_MTEXT:
			{
				// 是否是多段文字
				AcDbMText  *prealobj = AcDbMText ::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		case ACDB_CURVE:
			{
				// 是否是曲线
				AcDbCurve  *prealobj = AcDbCurve ::cast(pEnt);
				if (prealobj != NULL)
				{
					vids.push_back(prealobj->objectId());
					num++;
				}
				break;
			}
		default:
			break;
		}
		// 注意需要关闭实体
		pEnt->close();
	}
	delete pItr; // 遍历器使用完毕之后一定要删除！
	pBlkTblRcd->close();
	return num;
}

int MD2010_CycleAllBlockReferencesInLayer(CString layname, vAcDbObjectId &vids)
{
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForRead);
	if (!pLayerTbl->has(layname))
	{
		//acutPrintf("\n当前图形中未包含\"测试\"图层!");
		pLayerTbl->close();
		return 0;
	}

	AcDbObjectId layerId; // “测试”图层的ID
	pLayerTbl->getAt(layname, layerId);
	pLayerTbl->close();

	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		if (pEnt->layerId() == layerId)
		{
			AcDbBlockReference *pblockreference = AcDbBlockReference::cast(pEnt);
			if (pblockreference != NULL)
			{
				vids.push_back(pblockreference->objectId());
				num++;
			}
		}
		pEnt->close();
	}
	delete pItr; 
	pBlkTblRcd->close();
	return num;
}

int MD2010_CycleAllBlockReferencesInAllLayer(vAcDbObjectId &vids)
{

	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		AcDbBlockReference *pblockreference = AcDbBlockReference::cast(pEnt);
		if (pblockreference != NULL)
		{
			vids.push_back(pblockreference->objectId());
			num++;
		}
		pEnt->close();
	}
	delete pItr; 
	pBlkTblRcd->close();
	return num;
}

int MD2010_CycleAllEntitesInLayer(CString layname, vAcDbObjectId &vids)
{
	// 判断是否存在名称为“测试”的图层
	AcDbLayerTable *pLayerTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getSymbolTable(pLayerTbl, AcDb::kForRead);
	if (!pLayerTbl->has(layname))
	{
		//acutPrintf("\n当前图形中未包含\"测试\"图层!");
		pLayerTbl->close();
		return 0;
	}

	AcDbObjectId layerId; // “测试”图层的ID
	pLayerTbl->getAt(layname, layerId);
	pLayerTbl->close();

	// 获得当前数据库的块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	// 创建块表记录遍历器
	AcDbBlockTableRecordIterator *pItr; // 块表记录遍历器
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt; // 遍历的临时实体指针
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		// 利用遍历器获得每一个实体
		pItr->getEntity(pEnt, AcDb::kForRead);
		// 是否在“测试”图层上
		if (pEnt->layerId() == layerId)
		{
			vids.push_back(pEnt->objectId());
		}
		// 注意需要关闭实体
		pEnt->close();
	}
	delete pItr; // 遍历器使用完毕之后一定要删除！
	pBlkTblRcd->close();
	return num;
}

int MD2010_CycleAllEntites(vAcDbObjectId &vids)
{
	// 获得当前数据库的块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	// 创建块表记录遍历器
	AcDbBlockTableRecordIterator *pItr; // 块表记录遍历器
	pBlkTblRcd->newIterator(pItr);

	int num = 0;
	AcDbEntity *pEnt; // 遍历的临时实体指针
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		// 利用遍历器获得每一个实体
		pItr->getEntity(pEnt, AcDb::kForRead);
		vids.push_back(pEnt->objectId());
        // 注意需要关闭实体
		pEnt->close();
	}
	delete pItr; // 遍历器使用完毕之后一定要删除！
	pBlkTblRcd->close();
	return num;
}

int MD2010_CycleAllBlockReferencesInRect(vAcDbObjectId &vids, AcGePoint3d lb, AcGePoint3d rt)
{
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);

	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForRead);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);

	AcDbEntity *pEnt;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		AcDbBlockReference *pblk = AcDbBlockReference::cast(pEnt);
		if(pblk)
		{
			AcGePoint3d minPt, maxPt;
			JHCOM_GetObjectMinMaxPoint(pEnt->objectId(), minPt, maxPt);
			if(MD2010_CheckPointIsInRect(lb,rt,minPt) && MD2010_CheckPointIsInRect(lb,rt,maxPt))
				vids.push_back(pEnt->objectId());
		}
		pEnt->close();
	}
	delete pItr; 
	pBlkTblRcd->close();
	return 0;
}

//普通计算函数
int MD2010_LoopCountCurve1(AcDbCurve **pCurve, int num)//没完成
{
	AcGePoint3d d3s1, d3e1,d3s2, d3e2;
	int num2 =0;
    //AcDbCurve **pCurve1 ;//= new AcDbCurve[num];
	AcGePoint3d temps, tempe;
	//pCurve1[0] = pCurve[0];
	pCurve[0]->getEndPoint(tempe);
	return 0;
	
}
int MD2010_LoopCountCurve1(vAcDbObjectId vids)//没完成
{
	if (vids.size() == 0)
		return 0;
	int numloop = 0, numid = 0;
	AcGePoint3d geStart, geEnd;

	AcDbEntity *pEnt = NULL;
	/*for (int i = 0; i < vids.size(); i++)
	{
		acdbOpenAcDbObject(pEnt,vids.at(i),AcDb::kForRead);
		pCurve = AcDbCurve ::cast(pEnt);
		if (pCurve == NULL)
			continue;
		numid++;
		pEnt->close();
	}*/
	//AcDbCurve **pCurve;// = new AcDbCurve[numid];
	//numloop +=MD2010_LoopCountCurve(pCurve,numid);
	//delete [] pCurve;
	return numloop;
}

int MD2010_LoopCount1(vAcDbObjectId vids)//找到所有闭循环个数 
{
	if (vids.size() == 0)
	    return 0;

	int numloop = 0;
	vAcDbObjectId vlastids;
	AcGePoint3d ge3d;

	AcDbEntity *pEnt = NULL;
	AcDbCurve *pCurve = NULL;
    /*for (int ii = 0; ii < vids.size();++ii)
    {
		acdbOpenAcDbObject(pEnt,vids.at(ii),AcDb::kForRead);
		pCurve = AcDbCurve ::cast(pEnt);
		if (pCurve == NULL)
		    continue;
		if (pCurve->isClosed())
		{
		    numloop++;
			continue;
		}
		vlastids.push_back(vids.at(ii));
		pEnt->close();
    }
    numloop +=MD2010_LoopCountPoints(vlastids);*/
	return numloop;
}

int MD2010_LoopCountCurve(AcDbCurve **pCurve, int num, AcGePoint3d st,vAcGePoint3d &rvec)//没完成 给一点找到一圈
{
	AcGePoint3d tempe = st;
	AcGePoint3d d3s, d3e;
	if (num>1024) return 0;
	AcDbCurve *pCurvetemp[1024];
	int j = 0;

	if (num == 0)//已经找完,所有的连在一起,含一个曲线都没有的情况
	{
		return 1;//暂时return 1没意义
	}

	for (; j < num; j++)
	{
		pCurve[j]->getStartPoint(d3s);
		pCurve[j]->getEndPoint(d3e);
		if (/*tempe == d3e */JHCOM_isPoint3dEqual(tempe, d3e))
		{
			rvec.push_back(d3e);
			tempe = d3s;
			break;
		}
		if ( /*tempe == d3s*/JHCOM_isPoint3dEqual(tempe, d3s))
		{
			rvec.push_back(d3s);
			tempe = d3e;
			break;
		}
	}//找到一个相邻的点,放进来,

	//如果这次没有找到,说明这一次递归已经完成,找到了终点
	if (j == num)
	{
		return 0;
	}

	//把剩下的曲线收集起来,
	int lastcurvenum = 0;
	for (int i =0; i < num; i++)
	{
		if (i != j)
			pCurvetemp[lastcurvenum++] = pCurve[i];
	}

	MD2010_LoopCountCurve(pCurvetemp, lastcurvenum, tempe,rvec);
	return 0;
}


bool MD2010_CheckLoopPoints(vAcGePoint3d v3d)
{
	AcGePoint3d temp,temp1;
	int num = 0;
	int size = (int)v3d.size();
    for (int i =0; i < size; i++)
    {
		temp  = v3d.at(i);
		for (int j = 0; j < size; j++)
		{
            temp1 = v3d.at(j);
			if (/*temp == temp1*/JHCOM_isPoint3dEqual(temp, temp1))
			{
				num++;
			}
		}
		if(num != 2) return false;
		num = 0;
    }
	return true;
}


bool MD2010_IsOneLoop(vAcDbObjectId vids)
{
	if (vids.size() == 0)
	    return false;

	int numloop = 0;
	AcGePoint3d ge3d;
	vAcGePoint3d vd3;
	int size = (int)vids.size();

	AcDbObject *pEnt = NULL;
	AcDbCurve *pCurve = NULL;
	AcDbObjectId pids[5102];
	AcDbCurve* pCurves[5102];
	int kk =0;

	//第一步,判断有多少个封闭的曲线,如果大于1 则返回
	//并且把不封闭的所有点找出来放在vector中,
	//所有不封闭曲线个数是kk
    for (int ii = 0; ii < size;++ii)
    {
		acdbOpenAcDbObject(pEnt,vids.at(ii),AcDb::kForRead);
		pCurve = AcDbCurve ::cast(pEnt);
		if (pCurve == NULL)
		{
			pEnt->close();
		    continue;
		}
		if (pCurve->isClosed())
		{
		    numloop++;
			if (numloop > 1)
			{
				pEnt->close();
				return false;//如果封闭曲线个数已经大于一个,那么直接返回不唯一
			}
			pEnt->close();
			continue;
		}
		pCurve->getStartPoint(ge3d);
		vd3.push_back(ge3d);
		pCurve->getEndPoint(ge3d);
		vd3.push_back(ge3d);
		pids[kk++] = pEnt->objectId();
		pEnt->close();
    }

	//第二步,如果有一个封闭的,也有不封闭的,那么不是唯一的 返回false
	if(numloop == 1 && kk > 0) return false;
	  //如果只有一个开的曲线，直接判断首尾点
	if (numloop == 0 && kk == 1)
	{
		if (JHCOM_isPoint3dEqual(vd3[0], vd3[1]))
		{
			return true;
		}
		else
			return false;
	}

	if (numloop == 1 && kk ==0) return true;//如果封闭的为1,没有不封闭的,返回true


	//如果都是0,返回false,这个可以不判断的,因为进入MD2010_IsOneLoop,第一个就判断了vids.size()
	if (numloop ==0 && kk == 0) return false;

	//否则,进入第三步,即numloop == 0,kk>0 情况
	//检查是不是所有的点都是重复的相等,都是两个两个的一样,否则返回false
	bool re = MD2010_CheckLoopPoints(vd3);
	if (re == false) return false;


	//如果上面的条件满足了,进入第四步,首先打开所有的曲线
	for (int nn = 0; nn < kk; nn++)
	{
		acdbOpenAcDbObject(pEnt,pids[nn],AcDb::kForRead);
		pCurves[nn] = AcDbCurve ::cast(pEnt);
		pEnt = NULL;
	}
    //第四步,2,对所有的上面找到的曲线的点,我们只需要那任一个点(现在取第一个),找一圈,
	//如果找到的点的个数不等于曲线的个数,那么返回false
	vAcGePoint3d vec;
	MD2010_LoopCountCurve(pCurves, kk, vd3.at(0), vec);
	if (vec.size() != kk/*vd3.size()*/)//注意是曲线的个数,不是所有点的个数,因为每一个曲线会放进来一个点
	{
		return false;
	}

	for (int nn = 0; nn < kk; nn++)
		pCurves[nn]->close(); 

	//经过了上面的四步检查,返回true
	return true;
}

int MD2010_CheckRect(const WCHAR * layername)
{
	vAcDbObjectId curveIds;
	MD2010_CycleAllTypedObjectsInLayer( layername, ACDB_CURVE, curveIds);
	//if (curveIds.size() != 4) return 0;
	if (!MD2010_IsOneLoop( curveIds))
	{
		return false;
	}
	return true;
}

bool MD2010_CheckRect_2(AcDbObjectId curveid, Adesk::UInt16 colorIndex)
{
	AcDbObject *pEnt = NULL;

	acdbOpenAcDbObject(pEnt,curveid,AcDb::kForRead);
	AcDbPolyline *polyLine = AcDbPolyline ::cast(pEnt);
	if (polyLine == NULL)
	{
		polyLine->close();
		return false;
	}

	if(polyLine->numVerts() != 4) 
	{
		polyLine->close();
		return false;
	}

	if(polyLine->colorIndex() != colorIndex)
	{
		if (colorIndex == 7 && polyLine->color().isByLayer())//白色切bylayer返回true
		{
			polyLine->close();
			return 1;
		}
		else
		{
			polyLine->close();
			return false;
		}
	}
	polyLine->close();
	return 1;
}

int MD2010_GetLinePoints(AcDbObjectId lineid, AcGePoint3d& start, AcGePoint3d& end)
{
	AcDbObject *pEnt = NULL;

	acdbOpenAcDbObject(pEnt,lineid,AcDb::kForRead);
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
		return 1;
	start = pLine->startPoint();
	end = pLine->endPoint();
    pLine->close();
	return 0;
}

int MD2010_GetCurrentMode()//1模型空间
{
	struct resbuf rb;
	acedGetVar(L"TILEMODE", &rb);
	return rb.resval.rint;
}
//此函数存在问题，如果AcDbObjectIdArray中的实体已经加入到模型空间，则无法加入到新的块定义中
int MD2010_CreateBlockDefine_old(const WCHAR* blockname, AcDbObjectIdArray &vids, AcGePoint3d ori, AcDbObjectId& blkDefId)
{
	Acad::ErrorStatus es;

	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);

	AcDbBlockTableRecord *pBlkTblRcd = new AcDbBlockTableRecord();
	es =pBlkTbl->add(blkDefId, pBlkTblRcd);
	pBlkTbl->close();

	es = pBlkTblRcd->setName(blockname);

	AcDbObject *pObj = NULL;
	AcDbObjectId id = 0;
	for(int i = 0; i < vids.length(); i++)
	{
		es = acdbOpenAcDbObject(pObj, vids.at(i),AcDb::kForRead);
		AcDbEntity *pEnt = AcDbEntity ::cast(pObj);
		if (pEnt == NULL)
		{
			if (pObj!=NULL)
			{
				pObj->close();
			}
			continue;
		}

		es = pBlkTblRcd->appendAcDbEntity(id, pEnt);
		pEnt->close();
	}
    es = pBlkTblRcd->setOrigin(ori);
	es = pBlkTblRcd->close();

	return 0;
}

Acad::ErrorStatus defineNewBlock(LPCTSTR blkName, AcDbBlockTableRecord*& newBlkRec, AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
	ASSERT(db != NULL);

	AcDbBlockTable* blkTbl;
	Acad::ErrorStatus es = db->getSymbolTable(blkTbl, AcDb::kForWrite);
	if (es != Acad::eOk)
		return es;

	// if this block already exists, erase its contents first
	if (blkTbl->getAt(blkName, newBlkRec, AcDb::kForWrite) == Acad::eOk) {
		newBlkRecId = newBlkRec->objectId();
		AcDbBlockTableRecordIterator* iter;
		es = newBlkRec->newIterator(iter);
		if (es != Acad::eOk) 
		{
			//ArxDbgUtils::rxErrorMsg(es);
			newBlkRec->close();
		}
		else {
			AcDbEntity* ent;
			for (; !iter->done(); iter->step()) {
				if (iter->getEntity(ent, AcDb::kForWrite) == Acad::eOk) {
					ent->erase();
					ent->close();
				}
			}
			delete iter;
		}
	}
	else {    // create a new block table record and add it to the block table
		newBlkRec = new AcDbBlockTableRecord;
		newBlkRec->setPathName(_T(""));    // constructor doesn't do it properly

		es = newBlkRec->setName(blkName);
		if (es == Acad::eOk)
			es = blkTbl->add(newBlkRecId, newBlkRec);

		if (es != Acad::eOk) {
			//ArxDbgUtils::rxErrorMsg(es);
			delete newBlkRec;
		}
	}

	blkTbl->close();    // doesn't need to be open anymore
	return es;
}


Acad::ErrorStatus defineNewBlock(LPCTSTR blkName, AcDbObjectId& newBlkRecId, AcDbDatabase* db)
{
	AcDbBlockTableRecord* newBlkRec;
	Acad::ErrorStatus es = defineNewBlock(blkName, newBlkRec, newBlkRecId, db);
	if (es == Acad::eOk)
		newBlkRec->close();
	return es;
}


#include <dbxutil.h>
#include <dbidmap.h>
void getUcsToWcsMatrix(AcGeMatrix3d& m, AcDbDatabase* db)
{
	ASSERT(db != NULL);

	if (!acdbUcsMatrix(m, db)) {
		m.setToIdentity();
		ASSERT(0);
	}
}

AcGePoint3d ucsToWcs(const AcGePoint3d& pt)
{
	AcDbDatabase* db = acdbHostApplicationServices()->workingDatabase();
	ASSERT(db != NULL);

	AcGeMatrix3d m;

	getUcsToWcsMatrix(m, db);
	return m * pt;
}

/****************************************************************************
**
**	ArxDbgUtils::getUcsToWcsOriginMatrix
**		move an object form one coordinate system to another.
**
**	NOTE:    The basePt must be passed in as a WCS point!
**
**	**jma
**
*************************************/

void getUcsToWcsOriginMatrix(AcGeMatrix3d& m, const AcGePoint3d& wcsBasePt, AcDbDatabase* db)
{
	ASSERT(db != NULL);

    AcGeMatrix3d tmpMat;

	if (acdbUcsMatrix(tmpMat, db)) {
		AcGePoint3d origin;
		AcGeVector3d xDir, yDir, zDir;

		tmpMat.getCoordSystem(origin, xDir, yDir, zDir);
		origin += wcsBasePt.asVector();

	    m.setToAlignCoordSys(origin, xDir, yDir, zDir,
            AcGePoint3d::kOrigin, AcGeVector3d::kXAxis,
            AcGeVector3d::kYAxis, AcGeVector3d::kZAxis);
	}
	else {
		ASSERT(0);
        m = AcGeMatrix3d::kIdentity;
	}
}

Acad::ErrorStatus cloneAndXformObjects(AcDbDatabase* db, AcDbObjectIdArray& entsToClone,
								       AcDbObjectId& ownerBlockId,
								       const AcGeMatrix3d& xformMat, bool debugSpeak)
{
	ASSERT(db != NULL);

	AcDbIdMapping idMap;
	Acad::ErrorStatus es = db->deepCloneObjects(entsToClone, ownerBlockId, idMap);
	if (es != Acad::eOk) 
	{
		//ArxDbgUtils::rxErrorMsg(es);
		return es;
	}

	AcDbIdMappingIter iter(idMap);
	for (iter.start(); !iter.done(); iter.next()) 
	{
		AcDbIdPair idPair;
		if (!iter.getMap(idPair))
			return Acad::eInvalidInput;

		if (idPair.isCloned()) 
		{
			AcDbEntity* clonedEnt;
			es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
			if (es == Acad::eOk) 
			{
				if (idPair.isPrimary()) 
				{
					//if (debugSpeak)
						//acutPrintf(_T("\nCloned And Transformed: %s"), ArxDbgUtils::objToClassStr(clonedEnt));
					clonedEnt->transformBy(xformMat);
				}
				//else if (debugSpeak)
					//acutPrintf(_T("\nCloned: %s"), ArxDbgUtils::objToClassStr(clonedEnt));

				clonedEnt->close();
			}
			//else
				//ArxDbgUtils::rxErrorMsg(es);
		}
	}
	return Acad::eOk;
}

int MD2010_CreateBlockDefine_ExistEnts(const WCHAR* blockname, AcDbObjectIdArray &vids, AcGePoint3d ori, AcDbObjectId& blkDefId)
{
    if (MD2010_CheckBlockDefExist(blockname))
        return 1;
    Acad::ErrorStatus es = defineNewBlock( blockname, blkDefId, acdbHostApplicationServices()->workingDatabase());
    if (es != Acad::eOk)
        return 1;

	// must transform entities to WCS origin to be correct block def
	AcGeMatrix3d xformMat;
	getUcsToWcsOriginMatrix(xformMat, ucsToWcs(ori), acdbHostApplicationServices()->workingDatabase());
	 
	if (cloneAndXformObjects(acdbHostApplicationServices()->workingDatabase(), vids, blkDefId, xformMat, true) != Acad::eOk)
		 return 1;

	 return 0;
}
bool MD2010_CheckBlockDefExist(const WCHAR * blockname)
{
	AcDbBlockTable *pBlkTbl;
	/*char blkName[40];
	CString strBlkDef;
	strBlkDef.Format("%s", blkName);*/
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	if (!pBlkTbl->has(blockname))
	{
		pBlkTbl->close();
		return false; 
	}
	pBlkTbl->close();
	return true;
}

AcDbObjectId MD2010_GetBlockDefID(const WCHAR * blockname)
{
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);
	if (!pBlkTbl->has(blockname))
	{
		pBlkTbl->close();
		return 0; 
	}

	AcDbObjectId blkDefId;
	pBlkTbl->getAt(blockname, blkDefId);
    pBlkTbl->close();
	
	return blkDefId;
}

int MD2010_InsertBlockReference_ModelSpace(const WCHAR * blockname, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale)
{
	AcDbBlockTable *pBlkTbl;

	acdbHostApplicationServices()->workingDatabase() ->getBlockTable(pBlkTbl, AcDb::kForRead);
	if (!pBlkTbl->has(blockname))
	{
		acutPrintf(L"\n no blockdef %s\n", blockname);
		pBlkTbl->close();
		return 1; 
	}

	acDocManager->lockDocument(curDoc());
	// 获得用户指定的块表记录
	AcDbObjectId blkDefId;
	Acad::ErrorStatus es = pBlkTbl->getAt(blockname, blkDefId);

	// 创建块参照对象
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(origin, blkDefId);
	// 将块参照添加到模型空间
	AcDbBlockTableRecord *pBlkTblRcd;
	es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	//AcDbObjectId entId;
	es = pBlkTblRcd->appendAcDbEntity(entId, pBlkRef);

	//修改参数
    es = pBlkRef->setScaleFactors(scale);
	es = pBlkRef->setRotation(angle);

	// 关闭数据库的对象
	es = pBlkRef->close();
	es =pBlkTblRcd->close();
	es =pBlkTbl->close();


	acDocManager->unlockDocument(curDoc());

	return 0;
}

int MD2010_InsertBlockReference_Layout(const WCHAR * layoutname, const WCHAR * blockname, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, int color)
{
	AcDbBlockTable *pBlkTbl;

	if (MD2010_SetCurrentLayout(layoutname) == 1)
	{
		if (wcscmp(layoutname, ACDB_MODEL_SPACE) == 0)
		{
			if (MD2010_SetCurrentLayout(L"模型") == 1)
				return 1;
		}
	}		

	Acad::ErrorStatus es1 = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForRead);
	if (!pBlkTbl->has(blockname))
	{
		acutPrintf(L"no blockdef %s\n", blockname);
		pBlkTbl->close();
		return 1; 
	}

	Acad::ErrorStatus es;
	// 获得用户指定的块表记录
	AcDbObjectId blkDefId;
	es = pBlkTbl->getAt(blockname, blkDefId);

	// 创建块参照对象
	AcDbBlockReference *pBlkRef = new AcDbBlockReference(origin, blkDefId);
	// 将块参照添加到模型空间
	AcDbBlockTableRecord *pBlkTblRcd;
	if (wcscmp(layoutname, L"模型") != 0 && wcscmp(layoutname, ACDB_MODEL_SPACE) != 0)
	{
		es = pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
			AcDb::kForWrite);
	}
	else
		es = pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	
	//AcDbObjectId entId;
	es = pBlkTblRcd->appendAcDbEntity(entId, pBlkRef);

	//修改参数
	es = pBlkRef->setScaleFactors(scale);
	es = pBlkRef->setRotation(angle);

	AcCmColor cmc;
	if (color == -2)
	{
		cmc.setColorMethod(AcCmEntityColor::kByBlock);
		pBlkRef->setColor(cmc);
	}
	else if (color == -1)
	{
		cmc.setColorMethod(AcCmEntityColor::kByLayer);
		pBlkRef->setColor(cmc);
	}
	else
	{
		pBlkRef->setColorIndex(color);
	}

	// 关闭数据库的对象
	es = pBlkRef->close();
	es = pBlkTblRcd->close();
	es = pBlkTbl->close();

	return 0;
}

void MD2010_GetBlockRecordname(AcDbObjectId id, ACHAR *&pName)
{
	AcDbObject *pEnt = NULL;
	AcDbObjectId recordid;
	AcDbBlockTableRecord *precord = NULL;

	acdbOpenAcDbObject(pEnt, id,AcDb::kForRead);
	precord = AcDbBlockTableRecord ::cast(pEnt);
	if (precord == NULL)
	{
		pEnt->close();
		return;
	}

	precord->getName(pName);
	pEnt->close();
	return;
}

int MD2010_GetBlockReference_Record_name(AcDbObjectId id, ACHAR *&pName)
{
	AcDbObject *pEnt = NULL,*pEntR = NULL;
	AcDbBlockReference *pbloref = NULL;
	AcDbObjectId recordid=0;
	AcDbBlockTableRecord *precord = NULL;

	acdbOpenAcDbObject(pEnt, id,AcDb::kForRead);
	pbloref = AcDbBlockReference ::cast(pEnt);
	if (pbloref == NULL)
	{
		pEnt->close();
		return -1;
	}
	recordid = pbloref->blockTableRecord();
	if (recordid == 0)
	{
		pEnt->close();
		return -1;
	}

	acdbOpenAcDbObject(pEntR, recordid,AcDb::kForRead);
	precord = AcDbBlockTableRecord ::cast(pEntR);
	if (precord == NULL)
	{
		if(pEntR)
		    pEntR->close();

		if (pEnt)
			pEnt->close();
		return -2;
	}

	precord->getName(pName);
	pEnt->close();
	pEntR->close();
	return 0;
}



int MD2010_CycleBlockEntites(const WCHAR * blockname, vAcDbObjectId &vids)
{
    AcDbObjectId blkDefId = MD2010_GetBlockDefID(blockname); 
	if(blkDefId == 0) return 0;

	int count = 0;
	AcDbBlockTableRecord *pBlkDefRcd = 0;
    acdbOpenObject(pBlkDefRcd, blkDefId, AcDb::kForRead);
	if (pBlkDefRcd == 0)
		return -1;

	AcDbBlockTableRecordIterator *pItr;
	pBlkDefRcd->newIterator(pItr);
	AcDbEntity *pEnt = 0;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		vids.push_back(pEnt->objectId());
		count++;
		pEnt->close(); 
	}
	delete pItr;
	pBlkDefRcd->close();
	return count;
}

int TYCOM_CycleBlockReferenceEntites(AcDbObjectId blockReferenceId,vAcDbObjectId &ids)
{
	ACHAR *brecname = L"";
	vAcDbObjectId vidsToCheck;
	int ret = MD2010_GetBlockReference_Record_name(blockReferenceId, brecname);
	if (ret == 0)
	{
		MD2010_CycleBlockEntites(brecname, ids);
	}
	return 0;
}

//bool 
int TYCOM_DeepCycleBlockReferences(AcDbObjectId inputId, eViewDir viewDir, bool(*IsWindowFunction)(AcDbObjectId, eViewDir), vAcDbObjectId &outputIds)
{
	if (inputId && IsWindowFunction(inputId, viewDir))
	{
		outputIds.push_back(inputId);
		return 1;
	}

	if (!DQ_IsBlockReference(inputId))
		return 0;

	ACHAR *brecname = L"";
	vAcDbObjectId vidsToCheck;
	int ret = MD2010_GetBlockReference_Record_name(inputId, brecname);
	if (ret == 0)
	{
		MD2010_CycleBlockEntites(brecname, vidsToCheck);
		for (int i = 0; i < vidsToCheck.size(); i++)
		{
			TYCOM_DeepCycleBlockReferences(vidsToCheck[i], viewDir, IsWindowFunction, outputIds);
		}
	}
	return 0;
}

int MD2010_CycleBlockReferencesInBlockDef(const WCHAR * blockname, vAcDbObjectId &vids)
{
	AcDbObjectId blkDefId = MD2010_GetBlockDefID(blockname);
	if (blkDefId == 0) return 0;

	int count = 0;
	AcDbBlockTableRecord *pBlkDefRcd;
	acdbOpenObject(pBlkDefRcd, blkDefId, AcDb::kForRead);

	AcDbBlockTableRecordIterator *pItr = 0;
	pBlkDefRcd->newIterator(pItr);
	AcDbEntity *pEnt = 0;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		if (pEnt)
		{
			AcDbBlockReference * pBR = AcDbBlockReference::cast(pEnt);
			if (pBR)
			{
				vids.push_back(pEnt->objectId());
				count++;
			}
			pEnt->close();	
		}
	}
	delete pItr;

	return count;
}

HBITMAP MD2010_BlockIconToBMP(const AcDbObjectId &blkId, HDC hdc)
{
	Acad::ErrorStatus es;
	AcDbBlockTableRecord *pBlkTblRcd = NULL; // 块表记录的指针
	AcArray<Adesk::UInt8> icon; // 保存预览图标的数组
	// 获得保存块表记录的预览图标的数组
	try
	{
		es = acdbOpenObject(pBlkTblRcd, blkId, AcDb::kForRead);
		if (es != Acad::eOk)
			throw 1;
		if (!pBlkTblRcd->hasPreviewIcon()) // 如果块定义不包含预览图标
		{
			pBlkTblRcd->close();
			return NULL;
		}
		es = pBlkTblRcd->getPreviewIcon(icon);
		if (es != Acad::eOk)
			throw 2;
		es = pBlkTblRcd->close();
		if (es != Acad::eOk)
			throw 3;
	}
	catch (...)
	{
		pBlkTblRcd->close();
		return NULL;
	}
	// 由icon数组获得可显示的位图
	BITMAPINFOHEADER ih; // 位图信息头
	memcpy(&ih, icon.asArrayPtr(), sizeof(ih));
	size_t memsize = sizeof(BITMAPINFOHEADER) + ((size_t)(1<<ih.biBitCount)
		* sizeof(RGBQUAD));
	LPBITMAPINFO bi = (LPBITMAPINFO)malloc(memsize); // 位图信息
	memcpy(bi, icon.asArrayPtr(), memsize);
	HBITMAP hbm = CreateDIBitmap(hdc, &ih, CBM_INIT,
		icon.asArrayPtr() + memsize, bi, DIB_RGB_COLORS);
	free(bi);
	return hbm;
}

int MD2010_CycleAllBlockDefs(vAcDbObjectId &vids, vCString &vstr)
{
	int count = 0;
	AcDbBlockTable *pBlkTbl;
	AcDbBlockTableRecord *pBlkTblRcd;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	
	AcDbBlockTableIterator *pItr;
	pBlkTbl->newIterator(pItr);
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getRecord(pBlkTblRcd, AcDb::kForRead);
		vids.push_back(pBlkTblRcd->objectId());
		ACHAR* pname;
		pBlkTblRcd->getName(pname);
		CString strName(pname);
        acutDelString(pname);

		vstr.push_back(strName);
		count++;
		pBlkTblRcd->close();
	}
	delete pItr;
	pBlkTbl->close();
	return count;
}

int MD2010_GetBlockDefReferences(const WCHAR * blockname, vAcDbObjectId &vids)
{
	/*HBITMAP hBitmap = BlockIconToBMP(blkTblRcdId, dc.GetSafeHdc());
	m_bitmap.SetBitmap(hBitmap);*/

	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	//AcDbObjectId blkTblRcdId;
	//Acad::ErrorStatus es = pBlkTbl->getAt(strBlkDefName, blkTblRcdId);
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(blockname, pBlkTblRcd, AcDb::kForRead);

	// 获得块参照的数量
	AcDbBlockReferenceIdIterator *pItr;
	pBlkTblRcd->newBlockReferenceIdIterator(pItr);
	int number = 0;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		AcDbObjectId id = 0;
		pItr->getBlockReferenceId(id);
		vids.push_back(id);
		number++;
	}
	pBlkTblRcd->close();
	pBlkTbl->close();

	return number;
	//UpdateData(FALSE); GOOD
}

bool MD2010_CheckBlockHasAttribute(const WCHAR * blockname)
{
    AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	//AcDbObjectId blkTblRcdId;
	//Acad::ErrorStatus es = pBlkTbl->getAt(strBlkDefName, blkTblRcdId);
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(blockname, pBlkTblRcd, AcDb::kForRead);
	bool tf = pBlkTblRcd->hasAttributeDefinitions();
	pBlkTblRcd->close();
	pBlkTbl->close();
	return tf;
}

AcDbObjectId MD2010_InsertBlockDefineFromPathName(const WCHAR *pathname,CString blockName)
{
	WCHAR blockname[256] = L"";
	CF_STR_get_file_name(pathname, blockname);
	CF_STR_get_file_name_2(blockname, blockname);
	if(blockName.GetLength() == 0)
		blockName = blockname;

	//寻找是否已经定义
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pBlkTable;
	AcDbObjectId blockId ;
	pCurDb->getBlockTable(pBlkTable, AcDb::kForRead);
	if(!pBlkTable->has(blockName))
	{
		pBlkTable->close();
		AcDbDatabase *pDwg =new AcDbDatabase (Adesk::kFalse) ;
		Acad::ErrorStatus es  = pDwg->readDwgFile(pathname);

		es = pCurDb->insert (blockId, blockName, pDwg);
		delete pDwg ;
		if ( es != Acad::eOk )
		{
			acutPrintf (L"\n插入块错误.") ;
			return 0;
		}
	}
	else
		pBlkTable->getAt(blockName, blockId);

	pBlkTable->close();

	return blockId;
}


int MD2010_InsertBlockFromPathName(const WCHAR *layoutname,const WCHAR *pathname, CString blockDefineName, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, const WCHAR *layername, int color)
{
	CString name;
	MD2010_GetCurrentLayer(name);
	MD2010_SetCurrentLayer(layername);
	MD2010_InsertBlockDefineFromPathName(pathname,blockDefineName);
	MD2010_InsertBlockReference_Layout(layoutname, blockDefineName,entId, origin, angle, scale,color);
	MD2010_SetCurrentLayer(name);

	/*WCHAR dir[MAX_PATH];
	GetModuleFileName(NULL,dir,MAX_PATH);*/
	return 0;
}

AcDbObjectId MD2010_CreateViewport(const WCHAR *layoutname, AcGePoint3d center, double w, double h)
{
	// 指定当前布局
	Acad::ErrorStatus es =
		acdbHostApplicationServices()->layoutManager()
		->setCurrentLayout(layoutname);
	if (es != Acad::eOk)
	{
		return 0;
	}

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的视口对象
	AcDbViewport *pViewport = new AcDbViewport();
	pViewport->setCenterPoint(center);
	pViewport->setHeight(h);
	pViewport->setWidth(w);
	
	AcDbObjectId layid = MD2010_GetLayerID(L"Defpoints");
	if (layid != NULL)
	    pViewport->setLayer(layid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId viewportId;
	pBlkTblRcd->appendAcDbEntity(viewportId, pViewport);

	pViewport->close();
	pBlkTblRcd->close();

	AcDbViewport *pVP;
	acdbOpenObject(pVP, viewportId, AcDb::kForWrite);
	pViewport->setOn();
	acedSetCurrentVPort(pVP);
	/*bool ison = pViewport->isOn();
	AcDbObjectId ids = MD2010_GetLayerID(L"00_WallNew");
	AcDbObjectIdArray idarray;
	idarray.append(ids);
	pViewport->freezeLayersInViewport(idarray);*/
	pVP->close();

	return viewportId;
}

int MD2010_SetCurrentViewport(const WCHAR *layoutname,AcDbObjectId viewid )
{
	// 指定当前布局
	Acad::ErrorStatus es =
		acdbHostApplicationServices()->layoutManager()
		->setCurrentLayout(layoutname);
	if (es != Acad::eOk)
	{
		return 0;
	}

	AcDbViewport *pVP;
	acdbOpenObject(pVP, viewid, AcDb::kForWrite);
	es = pVP->setOn();
	es = acedSetCurrentVPort(pVP);
	es = pVP->close();
	return 0;
}

int MD2010_GetViewportIDs(const WCHAR *layoutname,vAcDbObjectId &vids  )
{
	// 指定当前布局
	Acad::ErrorStatus es =
		acdbHostApplicationServices()->layoutManager()
		->setCurrentLayout(layoutname);
	if (es != Acad::eOk)
	{
		return 0;
	}

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	AcDbBlockTableRecordIterator *pItr;
	pBlkTblRcd->newIterator(pItr);
	AcDbEntity *pEnt;
	AcDbViewport *pvp;
	int count = 0;
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		pvp = AcDbViewport::cast(pEnt);
		if (pvp != NULL)
		{
			vids.push_back(pEnt->objectId());
		}
		count++;
		pEnt->close(); 
	}
	delete pItr;
	pBlkTblRcd->close();
	return count;
}

int MD2010_SetViewportViewCenter(AcDbObjectId id, AcGePoint2d  p2d)
{
	AcDbViewport *pvp1;
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);
	pvp1 = AcDbViewport::cast(pEnt);
	if (pvp1==NULL)
	{
		return 0;
	}
	Acad::ErrorStatus es = pvp1->setOn();
	 //es = acedSetCurrentVPort(pvp1);
	es = pvp1->setViewCenter(p2d);
	// pvp1->setStandardScale(AcDbViewport::k1_100);
	AcGePoint2d     DD=   pvp1->viewCenter();
	 double     AA=   pvp1->viewHeight();
	//pvp1->setViewHeight(10000);
	pvp1->close();
	return 0;
}

int MD2010_SetViewportStandardScale(AcDbObjectId id, AcDbViewport::StandardScaleType scale)
{
	AcDbViewport *pvp1;
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);
	pvp1 = AcDbViewport::cast(pEnt);
	if (pvp1==NULL)
	{
		return 1;
	}

	AcDbViewport::StandardScaleType standsc;
	standsc = scale;
	pvp1->setStandardScale(standsc);
	pvp1->close();
    return 0;
}

int MD2010_SetViewportCustormScale(AcDbObjectId id, double scale)
{
	AcDbViewport *pvp1;
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);
	pvp1 = AcDbViewport::cast(pEnt);
	if (pvp1==NULL)
	{
		return 0;
	}

	AcDbViewport::StandardScaleType standsc;
	standsc = AcDbViewport::kCustomScale;
	pvp1->setStandardScale(standsc);
	pvp1->setCustomScale(scale);
	pvp1->close();
	return 0;
}

int MD2010_AddAnnotationScale(const WCHAR *scalename, double punits, double drawunits)
{
	AcDbAnnotationScale annotationScale;
	annotationScale.setName(scalename);
	//annotationScale.setName(_T("1:16"));
	annotationScale.setPaperUnits(1);
	annotationScale.setDrawingUnits(22);

	AcDbDatabase *dwg = curDoc()->database();
	AcDbObjectContextManager *contextManager = dwg->objectContextManager();
	AcDbObjectContextCollection* const contextCollection = contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);
	Acad::ErrorStatus es = contextCollection->addContext(&annotationScale);
	//dwg->setCannoscale(&annotationScale);//设置模型空间注释比例
	return 0;
}

AcDbAnnotationScale *MD2010_GetAnnotationScale(const WCHAR *scalename)
{
	AcDbDatabase *dwg = curDoc()->database();
	AcDbObjectContextManager *contextManager = dwg->objectContextManager();
	AcDbObjectContextCollection* const contextCollection = contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);

	AcDbAnnotationScale *annoScale = NULL;

	AcDbObjectContextCollectionIterator* pIter = contextCollection->newIterator();
	for (pIter->start(); !pIter->done(); pIter->next()) 
	{
		AcString strName;
		AcDbObjectContext *objectContext = NULL;
		// get the context object for this iteration
		Acad::ErrorStatus es = pIter->getContext(objectContext);
		// if we have what we want
		//PrintObjectContext(objectContext);
		annoScale = AcDbAnnotationScale::cast(objectContext);
		if (annoScale == NULL)
		{
			continue;
		}
		annoScale->getName(strName);
		if (wcscmp(strName.constPtr(), scalename) == 0)
		{
			//pvp1->setAnnotationScale(annoScale);
			break;
		}
		// remember to delete because getContext allocates the memory
		delete objectContext;
		annoScale = NULL;
	}
	delete pIter;

	return annoScale;
}

int MD2010_SetViewportAnnotationScale(const WCHAR *scalename, AcDbObjectId viewportid/*, const WCHAR *layoutname = NULL*/)//待定
{  //设置视口注释比例
	AcDbViewport *pVP;
	AcDbAnnotationScale *annotationScale;

	/*
	// 指定当前布局
	Acad::ErrorStatus es =
	acdbHostApplicationServices()->layoutManager()
	->setCurrentLayout(layoutname);
	if (es != Acad::eOk)
	{
	return 0;
	}
	*///待定

	annotationScale = MD2010_GetAnnotationScale(scalename);

	acdbOpenObject(pVP, viewportid, AcDb::kForWrite);
	//pVP->setOn();
	//acedSetCurrentVPort(pVP);
	pVP->setAnnotationScale(annotationScale);
	pVP->close();
	delete annotationScale;
    return 0;
}

int MD2010_SetModelSpaceAnnotationScale(const WCHAR *scalename)
{
	AcDbAnnotationScale *annotationScale;
	annotationScale = MD2010_GetAnnotationScale(scalename);

	AcDbDatabase *dwg = curDoc()->database();
	AcDbObjectContextManager *contextManager = dwg->objectContextManager();
	AcDbObjectContextCollection* const contextCollection = contextManager->contextCollection(ACDB_ANNOTATIONSCALES_COLLECTION);
	Acad::ErrorStatus es = contextCollection->addContext(annotationScale);
	dwg->setCannoscale(annotationScale);//设置模型空间注释比例
	delete annotationScale;
	return 0;
}

int MD2010_SetViewportUnFreeLayer(AcDbObjectId viewportid, vAcDbObjectId ids)
{
	AcDbObjectIdArray idarray;
	AcDbObject *pEnt = NULL;
    acdbOpenAcDbObject(pEnt,viewportid,AcDb::kForWrite);
	AcDbViewport *pvp = AcDbViewport::cast(pEnt);
	if(pvp == NULL) 
		return 0;
	vCString vstr;
	vAcDbObjectId valllayerids;
    MD2010_GetAllLayers(vstr, valllayerids);
	for(int i = 0; i < (int)valllayerids.size(); i++)
	{
		if(vfind(valllayerids[i], ids))
			continue;
		idarray.append(valllayerids[i]);
	}

	pvp->setOn();
	acedSetCurrentVPort(pvp);
	pvp->freezeLayersInViewport(idarray);
	pEnt->close();
	return 0;
}

int MD2010_SetViewportFreeLayer(AcDbObjectId viewportid, vAcDbObjectId ids)
{
	AcDbObjectIdArray idarray;
	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,viewportid,AcDb::kForWrite);
	AcDbViewport *pvp = AcDbViewport::cast(pEnt);
	if(pvp == NULL) 
	{
		pEnt->close();
		return 0;
	}

    //AcDbObjectIdArray idarray;
	for (int i = 0; i < (int)ids.size();i++)
	{
	    if (ids[i] != NULL)
		    idarray.append(ids[i]);
	}
	pvp->setOn();
	acedSetCurrentVPort(pvp);
	Acad::ErrorStatus es = pvp->freezeLayersInViewport(idarray);
	pEnt->close();
	return 0;
}

int MD2010_SetViewportLock(AcDbObjectId viewportid, bool lock)
{
	AcDbObjectIdArray idarray;
	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,viewportid,AcDb::kForWrite);
	if (pEnt == NULL) return 0;
	AcDbViewport *pvp = AcDbViewport::cast(pEnt);
	if(pvp == NULL) 
	{
		pEnt->close();
		return 0;
	}

	pvp->setOn();
	acedSetCurrentVPort(pvp);
	if (lock)
	    pvp->setLocked();
	else
		pvp->setUnlocked();
	//pvp->freezeLayersInViewport(idarray);
	pEnt->close();
	return 0;
}

AcDbObjectId MD2010_AddTextToPaperSpace(const WCHAR *layoutname, const WCHAR *text, double height,
										Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d center)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}
	
	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbText *pText = new AcDbText(center, text, AcDbObjectId::kNull, height);
	pText->setHorizontalMode(AcDb::kTextCenter);
	pText->setVerticalMode(AcDb::kTextVertMid);
	pText->setAlignmentPoint(center);
	pText->setColorIndex(color);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}

AcDbObjectId MD2010_AddTextToPaperSpace_LEFT(const WCHAR *layoutname, const WCHAR *text, double height,
										Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d left)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}
	
	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbText *pText = new AcDbText(left, text, AcDbObjectId::kNull, height);
	pText->setHorizontalMode(AcDb::kTextLeft);
	pText->setVerticalMode(AcDb::kTextVertMid);
	pText->setAlignmentPoint(left);
	pText->setColorIndex(color);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}

AcDbObjectId MD2010_GetTextStyler(const WCHAR *stylername)
{
	// 获得字体样式表
	AcDbTextStyleTable *pTextStyleTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getTextStyleTable(pTextStyleTbl, AcDb::kForRead);
	
	if (!pTextStyleTbl->has(stylername))
	{
		pTextStyleTbl->close();
		return 0;
	}

	AcDbObjectId recordId;
	pTextStyleTbl->getAt (stylername, recordId);
	pTextStyleTbl->close();
	return recordId;
}

CString MD2010_GetTextString(AcDbObjectId entId)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return "";

	AcDbText *ptext = AcDbText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return "";

	return CString(ptext->textString());
}

int MD2010_GetTextString(AcDbObjectId entId, CString& str)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return -1;

	AcDbText *ptext = AcDbText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return -1;

	str = CString(ptext->textString());
	return 0;
}

int MD2010_GetTextPosition(AcDbObjectId entId, AcGePoint3d &pt)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return -1;

	AcDbText *ptext = AcDbText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return -1;

	pt = ptext->position();
	return 0;
}

CString MD2010_GetMTextString(AcDbObjectId entId)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return "";

	AcDbMText *ptext = AcDbMText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return "";

	return CString(ptext->contents());
}

int MD2010_GetMTextString(AcDbObjectId entId, CString &str)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return -1;

	AcDbMText *ptext = AcDbMText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return -1;

	str = CString(ptext->contents());

	return 0;
}

int MD2010_GetMTextPosition(AcDbObjectId entId, AcGePoint3d &pt)
{
	AcDbEntity *pEntity = 0;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return -1;

	AcDbMText *ptext = AcDbMText::cast(pEntity);
	pEntity->close();
	if (ptext == 0)
		return -1;

	pt = ptext->location();
	return 0;
}

AcDbObjectId MD2010_GetDimStyler(const WCHAR *stylername)
{
    // 获得字体样式表
    AcDbDimStyleTable *pDimStyleTbl;
    acdbHostApplicationServices()->workingDatabase()
        ->getDimStyleTable(pDimStyleTbl, AcDb::kForRead);

    if (!pDimStyleTbl->has(stylername))
    {
        pDimStyleTbl->close();
        return 0;
    }

    AcDbObjectId recordId;
    pDimStyleTbl->getAt (stylername, recordId);
    pDimStyleTbl->close();
    return recordId;
}

AcDbObjectId MD2010_AddMTextToPaperSpace(const WCHAR *layoutname, const WCHAR *text, double height,
										Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d center)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}

	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbMText *pText = new AcDbMText();
	//pText->setHorizontalMode(AcDb::kTextCenter);
	//pText->setVerticalMode(AcDb::kTextVertMid);
	//pText->setAlignmentPoint(center);
	pText->setColorIndex(color);
	pText->setContents(text);
	pText->setHeight(height);
	pText->setLocation(center);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}

AcDbObjectId MD2010_AddMTextToPaperSpace_LEFTUP(const WCHAR *layoutname, const WCHAR *text, double height,
											 Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d left, double width)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}

	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbMText *pText = new AcDbMText();
	pText->setAttachment(AcDbMText::kTopLeft);
	//pText->setVerticalMode(AcDb::kTextVertMid);
	//pText->setAlignmentPoint(left);
	pText->setColorIndex(color);
	pText->setContents(text);
	pText->setTextHeight(height);
	pText->setLocation(left);
	pText->setWidth(width);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}

AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLEMID(const WCHAR *layoutname, const WCHAR *text, double height,
												Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}

	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbMText *pText = new AcDbMText();
	pText->setAttachment(AcDbMText::kMiddleCenter);
	//pText->setVerticalMode(AcDb::kTextVertMid);
	//pText->setAlignmentPoint(AcDb::kTextLeft);
	pText->setColorIndex(color);
	pText->setContents(text);
	pText->setTextHeight(height);
	pText->setLocation(mid);
	pText->setWidth(width);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}

AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLELEFT(const WCHAR *layoutname, const WCHAR *text, double height,
                                                   Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width)
{
    const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
        ->findActiveLayout(true);

    if (wcscmp(activename,layoutname) != 0)
    {
        // 指定当前布局
        Acad::ErrorStatus es =
            acdbHostApplicationServices()->layoutManager()
            ->setCurrentLayout(layoutname);
        if (es != Acad::eOk)
        {
            return 0;
        }
    }

    //MD2010_SetCurrentLayout(layoutname);

    // 获得块表
    AcDbBlockTable *pBlkTbl;
    acdbHostApplicationServices()->workingDatabase()
        ->getBlockTable(pBlkTbl, AcDb::kForRead);
    // 获得图纸空间的块表记录
    AcDbBlockTableRecord *pBlkTblRcd;
    pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
        AcDb::kForWrite);
    pBlkTbl->close();

    // 创建新的文字对象
    AcDbMText *pText = new AcDbMText();
    pText->setAttachment(AcDbMText::kMiddleLeft);
    //pText->setVerticalMode(AcDb::kTextVertMid);
    //pText->setAlignmentPoint(left);
    pText->setColorIndex(color);
    int n = pText->setContents(text);
    pText->setTextHeight(height);
    pText->setLocation(mid);
    pText->setWidth(width);

    AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
    pText->setTextStyle(stylerid);

    // 将新的布局对象添加到图纸空间块表记录中
    AcDbObjectId textId;
    Acad::ErrorStatus es = pBlkTblRcd->appendAcDbEntity(textId, pText);

    pText->close();
    pBlkTblRcd->close();

    return textId;
}

AcDbObjectId MD2010_AddMTextToPaperSpace_MIDDLERIGHT(const WCHAR *layoutname, const WCHAR *text, double height,
													Adesk::UInt8 color, const WCHAR *textstyler, AcGePoint3d mid, double width)
{
	const WCHAR *activename = acdbHostApplicationServices()->layoutManager()
		->findActiveLayout(true);

	if (wcscmp(activename,layoutname) != 0)
	{
		// 指定当前布局
		Acad::ErrorStatus es =
			acdbHostApplicationServices()->layoutManager()
			->setCurrentLayout(layoutname);
		if (es != Acad::eOk)
		{
			return 0;
		}
	}

	//MD2010_SetCurrentLayout(layoutname);

	// 获得块表
	AcDbBlockTable *pBlkTbl;
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlkTbl, AcDb::kForRead);
	// 获得图纸空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd;
	pBlkTbl->getAt(ACDB_PAPER_SPACE, pBlkTblRcd,
		AcDb::kForWrite);
	pBlkTbl->close();

	// 创建新的文字对象
	AcDbMText *pText = new AcDbMText();
	pText->setAttachment(AcDbMText::kMiddleRight);
	//pText->setVerticalMode(AcDb::kTextVertMid);
	//pText->setAlignmentPoint(left);
	pText->setColorIndex(color);
	int n = pText->setContents(text);
	pText->setTextHeight(height);
	pText->setLocation(mid);
	pText->setWidth(width);

	AcDbObjectId stylerid = MD2010_GetTextStyler(textstyler);
	pText->setTextStyle(stylerid);

	// 将新的布局对象添加到图纸空间块表记录中
	AcDbObjectId textId;
	Acad::ErrorStatus es = pBlkTblRcd->appendAcDbEntity(textId, pText);

	pText->close();
	pBlkTblRcd->close();

	return textId;
}


void MD2010_UpdateOneEntity(AcDbObjectId entId)
{
	AcDbEntity *pEnt;
	if(acdbOpenAcDbEntity(pEnt,entId,AcDb::kForRead)==Acad::eOk)
	{
		pEnt->recordGraphicsModified(); 
		pEnt->downgradeOpen(); 
		pEnt->draw(); 
		pEnt->close();
	}
}

AcDbObjectId MD2010_AddHatchPoints(vAcGePoint3d &pnts, const ACHAR* entryName) 
{
    AcDbHatch* pHatch = new AcDbHatch();
    Acad::ErrorStatus es;
    // Set hatch plane
    //
    AcGeVector3d normal(0.0, 0.0, 1.0);
    pHatch->setNormal(normal);
    pHatch->setElevation(0.0);

    // Set non associative hatch
    //
    pHatch->setAssociative(Adesk::kFalse);

    // Set hatch pattern to SolidFill type
    //
    pHatch->setPattern(AcDbHatch::kPreDefined, L"SOLID");

    // Set hatch style to kNormal
    //
    pHatch->setHatchStyle(AcDbHatch::kNormal);

    // Construct hatch external boundary
    //
    AcGePoint2dArray vertexPts;
    int len = (int)pnts.size();
    for (int i = 0; i < len; i++)
    {
        AcGePoint2d pt = AcGePoint2d(pnts[i].x, pnts[i].y);
        vertexPts.append(pt);
    }
    /*vertexPts.setPhysicalLength(0).setLogicalLength(5);
    vertexPts[0].set(2.0, 2.0);
    vertexPts[1].set(8.0, 2.0);
    vertexPts[2].set(8.0, 8.0);
    vertexPts[3].set(2.0, 8.0);
    vertexPts[4].set(2.0, 2.0);*/
    
    AcGeDoubleArray vertexBulges;
    vertexBulges.setPhysicalLength(0).setLogicalLength(len);
    for (int i = 0; i < len; i++)
        vertexBulges[i] = 0.0;

    // Append an external loop (rectangle) to hatch boundary

    es = pHatch->appendLoop(AcDbHatch::kDefault, vertexPts, vertexBulges); 

    /*
    // Construct a circle
    //
    AcGePoint2d cenPt(5.0, 5.0);
    double TWOPI = 2.0 * 3.1415926535897932;
    AcGeCircArc2d *cirArc = new AcGeCircArc2d();
    cirArc->setCenter(cenPt);
    cirArc->setRadius(1.0);
    cirArc->setAngles(0.0, TWOPI);

    // Append an internal circular loop to hatch boundary 
    //
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;
    edgeTypes.append(AcDbHatch::kCirArc);
    edgePtrs.append((void*)cirArc);
    pHatch->appendLoop(AcDbHatch::kDefault, edgePtrs, edgeTypes);*/
    

    // Elaborate solid fill
    //
    es = pHatch->evaluateHatch();

    // Post hatch entity to database
    //
    AcDbObjectId newId;
    newId = MD2010_PostModalToBlockTable(entryName, pHatch);
    pHatch->close();

    return newId;
}

AcDbObjectId MD2010_AddHatchPoints_newwall(vAcGePoint3d &pnts, const ACHAR* entryName) 
{
    AcDbHatch* pHatch = new AcDbHatch();
    Acad::ErrorStatus es;
    // Set hatch plane
    //
    AcGeVector3d normal(0.0, 0.0, 1.0);
    pHatch->setNormal(normal);
    pHatch->setElevation(0.0);

    // Set non associative hatch
    //
    pHatch->setAssociative(Adesk::kFalse);

    pHatch->setPatternScale(20);

    // Set hatch style to kNormal
    //
    pHatch->setHatchStyle(AcDbHatch::kNormal);
    
    // Set hatch pattern to SolidFill type
    //
    pHatch->setPattern(AcDbHatch::kPreDefined, L"ANSI31");

    // Construct hatch external boundary
    //
    AcGePoint2dArray vertexPts;
    int len = (int)pnts.size();
    for (int i = 0; i < len; i++)
    {
        AcGePoint2d pt = AcGePoint2d(pnts[i].x, pnts[i].y);
        vertexPts.append(pt);
    }
    /*vertexPts.setPhysicalLength(0).setLogicalLength(5);
    vertexPts[0].set(2.0, 2.0);
    vertexPts[1].set(8.0, 2.0);
    vertexPts[2].set(8.0, 8.0);
    vertexPts[3].set(2.0, 8.0);
    vertexPts[4].set(2.0, 2.0);*/
    
    AcGeDoubleArray vertexBulges;
    vertexBulges.setPhysicalLength(0).setLogicalLength(len);
    for (int i = 0; i < len; i++)
        vertexBulges[i] = 0.0;

    // Append an external loop (rectangle) to hatch boundary

    es = pHatch->appendLoop(AcDbHatch::kDefault, vertexPts, vertexBulges); 

    /*
    // Construct a circle
    //
    AcGePoint2d cenPt(5.0, 5.0);
    double TWOPI = 2.0 * 3.1415926535897932;
    AcGeCircArc2d *cirArc = new AcGeCircArc2d();
    cirArc->setCenter(cenPt);
    cirArc->setRadius(1.0);
    cirArc->setAngles(0.0, TWOPI);

    // Append an internal circular loop to hatch boundary 
    //
    AcGeIntArray edgeTypes;
    AcGeVoidPointerArray edgePtrs;
    edgeTypes.append(AcDbHatch::kCirArc);
    edgePtrs.append((void*)cirArc);
    pHatch->appendLoop(AcDbHatch::kDefault, edgePtrs, edgeTypes);*/
    

    // Elaborate solid fill
    //
    es = pHatch->evaluateHatch();

    // Post hatch entity to database
    //
    AcDbObjectId newId;
    newId = MD2010_PostModalToBlockTable(entryName, pHatch);
    pHatch->close();

    return newId;
}


//添加原有墙体的hatch
AcDbObjectId MD2010_AddHatchRectangle(AcGePoint3d lb, AcGePoint3d rt, const ACHAR* entryName) 
{
    AcGePoint3d a = AcGePoint3d(rt.x, lb.y, 0);
    AcGePoint3d b = AcGePoint3d(lb.x, rt.y, 0);
    
    AcGePoint3d center = AcGePoint3d((lb.x + rt.x)/2, (lb.y + rt.y)/2, 0);
    vAcGePoint3d pnts;
    pnts.push_back(lb);
    pnts.push_back(a);
    pnts.push_back(rt);
    pnts.push_back(b);
    pnts.push_back(center);
    return MD2010_AddHatchPoints(pnts, entryName);
}

//添加新建墙体的hatch
AcDbObjectId MD2010_AddHatchRectangle_newwall(AcGePoint3d lb, AcGePoint3d rt, const ACHAR* entryName) 
{
    AcGePoint3d a = AcGePoint3d(rt.x, lb.y, 0);
    AcGePoint3d b = AcGePoint3d(lb.x, rt.y, 0);

    AcGePoint3d center = AcGePoint3d((lb.x + rt.x)/2, (lb.y + rt.y)/2, 0);
    vAcGePoint3d pnts;
    pnts.push_back(lb);
    pnts.push_back(a);
    pnts.push_back(rt);
    pnts.push_back(b);
    pnts.push_back(lb);
    return MD2010_AddHatchPoints_newwall(pnts, entryName);
}

AcDbObjectId MD2010_AddHatchRectangle(AcGePoint3d a, AcGePoint3d b,AcGePoint3d c, AcGePoint3d d, const ACHAR* entryName) 
{
    AcGePoint3d center = AcGePoint3d((a.x + c.x + b.x + d.x)/4, (a.y + c.y + b.y + d.y)/4, 0);
    vAcGePoint3d pnts;
    pnts.push_back(a);
    pnts.push_back(b);
    pnts.push_back(c);
    pnts.push_back(d);
    pnts.push_back(center);
    return MD2010_AddHatchPoints(pnts, entryName);
}

int MD2010_GetHatchLinesPts(vSHatchLines &outs, vAcGePoint3d &pts)
{
    for (int i = 0; i < (int)outs.size(); i++)
    {
		if (outs[i].isPline == 1)
		{
			for (int kk = 0; kk < outs[i].vertices.length(); kk++)
			{
				AcGePoint2d temp = outs[i].vertices[kk];
				AcGePoint3d t3d(temp.x, temp.y, 0);
				pts.push_back(t3d);
			}
		}
		else
		{
            AcGePoint2d start, end;
			for (int j=0; j<outs[i].edgeTypesAry.length(); j++)
			{
				if(outs[i].edgeTypesAry[j]==AcDbHatch::kLine)//直线
				{
					AcGeLineSeg2d *LnSeg=(AcGeLineSeg2d *)outs[i].edgeptrAry[j];
                    start = LnSeg->startPoint();
					end = LnSeg->endPoint();
					pts.push_back(AcGePoint3d(start.x, start.y, 0));
					pts.push_back(AcGePoint3d(end.x, end.y, 0));
					//acutPrintf("\n直线");
				}
				else if(outs[i].edgeTypesAry[j]==AcDbHatch::kCirArc)//圆弧
				{
					AcGeCircArc2d *CirArc=(AcGeCircArc2d *)outs[i].edgeptrAry[j];
					start = CirArc->startPoint();
					end = CirArc->endPoint();
					pts.push_back(AcGePoint3d(start.x, start.y, 0));
					pts.push_back(AcGePoint3d(end.x, end.y, 0));
					//acutPrintf("\n圆弧");
					//可以根据数学圆弧构造相应的AcDb圆弧，取决于你（以下同）
				}
				else if(outs[i].edgeTypesAry[j]==AcDbHatch::kEllArc)//椭圆弧
				{
					AcGeEllipArc2d *EllArc=(AcGeEllipArc2d *)(outs[i].edgeptrAry[j]);
					start = EllArc->startPoint();
					end = EllArc->endPoint();
					pts.push_back(AcGePoint3d(start.x, start.y, 0));
					pts.push_back(AcGePoint3d(end.x, end.y, 0));
					//acutPrintf("\n椭圆弧");
				}
				else if(outs[i].edgeTypesAry[j]==AcDbHatch::kSpline)//NURBS曲线
				{
					AcGeNurbCurve2d *Spline=(AcGeNurbCurve2d *)(outs[i].edgeptrAry[j]);
					start = Spline->startPoint();
					end = Spline->endPoint();
					pts.push_back(AcGePoint3d(start.x, start.y, 0));
					pts.push_back(AcGePoint3d(end.x, end.y, 0));
					//acutPrintf("\nNURBS曲线");
				}
			}
		}
	}

	return 0;
}

int MD2010_GetHatchLines(AcDbObjectId Id, vSHatchLines &outs)
{
	if(Id == NULL)
		return 1;
	AcDbEntity *pEnt = NULL;
	acdbOpenObject(pEnt, Id, AcDb::kForRead);     
	if(pEnt == NULL) return 1;

	AcDbHatch *pHatch = AcDbHatch::cast(pEnt);        
	if(pHatch == NULL)
	{
		pEnt->close();
		return 1;
	}        

	Acad::ErrorStatus es;
	Adesk::Int32 LoopType;

	//获得填充边界的数目
	int LoopNum = pHatch->numLoops();        
	for (int i=0; i<LoopNum; i++)
	{
		SHatchLines hatchlines;
		//获取边界类型
		LoopType = pHatch->loopTypeAt(i);
		//如果边界是多义线
		if (LoopType & AcDbHatch::kPolyline)
		{
			//取得边界定义数据(polyline)的顶点数组和凸度数组，它们是一一对应的
			es = pHatch->getLoopAt(i, LoopType, hatchlines.vertices, hatchlines.bulges);
			hatchlines.isPline = 1;
			//acutPrintf("\n多义线");
			//是不是根据这些顶点数组和凸度数组构造一条AcDb多义线取决于你     
			/*AcDbPolyline *pPolyLine = new AcDbPolyline(); 
			for (int kk = 0; kk < vertices.length(); kk++)
			{
				AcGePoint2d temp = vertices[kk];
				pPolyLine->addVertexAt(i, temp, bulges[kk]);
			}
			AcDbObjectId plineID = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pPolyLine);
			pPolyLine->close();
			hatchlines.plineid = plineID;*/
		}
		else
		{
			//几乎可以取得除polyline外的所有边界定义数据
			//第三个参数返回值是无值指针数组
			//第四个参数返回值是组成边界的每一条边的类型
			//它们也是一一对应的关系
			hatchlines.isPline = 0;
			es = pHatch->getLoopAt(i, LoopType, hatchlines.edgeptrAry, hatchlines.edgeTypesAry);

			//下面这一段暂时保留
			//遍历，因为每一条边界又可能由多种AcGe曲线构成
			/*for (int j=0; j<edgeTypesAry.length(); j++)
			{
				if(edgeTypesAry[j]==AcDbHatch::kLine)//直线
				{
					AcGeLineSeg2d *LnSeg=(AcGeLineSeg2d *)edgeptrAry[j];
					
					//acutPrintf("\n直线");
				}
				else if(edgeTypesAry[j]==AcDbHatch::kCirArc)//圆弧
				{
					AcGeCircArc2d *CirArc=(AcGeCircArc2d *)edgeptrAry[j];
					//acutPrintf("\n圆弧");
					//可以根据数学圆弧构造相应的AcDb圆弧，取决于你（以下同）
				}
				else if(edgeTypesAry[j]==AcDbHatch::kEllArc)//椭圆弧
				{
					AcGeEllipArc2d *EllArc=(AcGeEllipArc2d *)edgeptrAry[j];
					//acutPrintf("\n椭圆弧");
				}
				else if(edgeTypesAry[j]==AcDbHatch::kSpline)//NURBS曲线
				{
					AcGeNurbCurve2d *Spline=(AcGeNurbCurve2d *)edgeptrAry[j];
					//acutPrintf("\nNURBS曲线");
				}
			} */       
		}
		outs.push_back(hatchlines);
	}        
	pEnt->close();  

	return 0;
}

//给一个标注样式的名字,得到ID
AcDbObjectId MD2010_GetDimstylerID(CString dimname)
{
    AcDbDimStyleTable *pDimStylTbl = NULL;
    acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStylTbl, AcDb::kForRead);

    // 判断是否包含指定名称的层表记录
    if (!pDimStylTbl->has(dimname))
    {
        pDimStylTbl->close();
        return 0;
    }

    // 获得指定层表记录的指针
    AcDbDimStyleTableRecord *pDimStylerTblRcd = NULL;
    pDimStylTbl->getAt(dimname, pDimStylerTblRcd, AcDb::kForWrite);

    AcDbObjectId ids = pDimStylerTblRcd->objectId();
    pDimStylerTblRcd->close();
    pDimStylTbl->close();

    return ids;
}

//给一个文字样式的名字,得到ID
AcDbObjectId MD2010_GetTextStylerID(CString dimname)
{
	AcDbTextStyleTable *pTextStylTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStylTbl, AcDb::kForRead);

	// 判断是否包含指定名称的层表记录
	if (!pTextStylTbl->has(dimname))
	{
		pTextStylTbl->close();
		return 0;
	}

	// 获得指定层表记录的指针
	AcDbTextStyleTableRecord *pTextStylerTblRcd = NULL;
	pTextStylTbl->getAt(dimname, pTextStylerTblRcd, AcDb::kForWrite);

	AcDbObjectId ids = pTextStylerTblRcd->objectId();
	pTextStylerTblRcd->close();
	pTextStylTbl->close();

	return ids;
}

int MD2010_SetCurrentDimStyler(CString name)
{
    AcDbObjectId id = MD2010_GetDimstylerID(name);
    if(id == 0) 
        return 0;
    Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->setDimstyle(id);
    return es;
}

int MD2010_SetCurrentDimStyler(AcDbObjectId id)
{
    Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->setDimstyle(id);
    return es;
}

AcDbObjectId MD2011_CopyEntity_ToLayer(AcDbObjectId id, const WCHAR *lyername)
{
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForWrite);

	AcDbEntity* pEntCopy = NULL;
	AcGeMatrix3d xform;
	xform = AcGeMatrix3d::kIdentity;
	Acad::ErrorStatus es = pEnt->getTransformedCopy(xform, pEntCopy);

	AcDbObjectId idd = MD2010_PostModalToBlockTable(ACDB_MODEL_SPACE, pEntCopy);
	es = pEntCopy->setLayer(lyername);
	pEntCopy->close();

	pEnt->close();
	return idd;
}

bool MD2012_CheckIsCurve(AcDbObjectId id, AcGePoint3d *startpt, AcGePoint3d *endpt)
{
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);

	AcDbCurve *pCurve = AcDbCurve::cast(pEnt);        
	if(pCurve == NULL)
	{
		pEnt->close();
		return false;
	}  
	if(startpt != NULL)
		pCurve->getStartPoint(startpt[0]);
	if(endpt != NULL)
		pCurve->getEndPoint(endpt[0]);
	pEnt->close();
	return true;
}

bool MD2012_CheckIsPolyline(AcDbObjectId id, vAcGePoint3d *pts)
{
	AcDbEntity *pEnt;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);

	AcDbPolyline *pCurve = AcDbPolyline::cast(pEnt);        
	if(pCurve == NULL)
	{
		pEnt->close();
		return false;
	}  
	if(pts != NULL)
	{
		AcGePoint3d pt;
		for(int i = 0; i < (int)pCurve->numVerts(); i++)
		{
            pCurve->getPointAt(i,pt);
			pts[0].push_back(pt);
		}
	}
	pEnt->close();
	return true;
}

int MD2012_Select1(const ACHAR *str, vAcDbObjectId &ids)
{
	//提示用户选择边界
	ads_name ss;
	int rt = acedSSGet(str, NULL, NULL, NULL, ss);
	AcDbObjectIdArray objIds;
	//初始化填充边界的ID数组
	if (rt == RTNORM)
	{
		Adesk::Int32 length;
		acedSSLength(ss, &length);
		for (int i = 0; i < length; i++)
		{
			ads_name ent;
			acedSSName(ss, i, ent);
			AcDbObjectId objId;
			acdbGetObjectId(objId, ent);
			ids.push_back(objId);
		}
	}
	acedSSFree(ss); // 释放选择集
	return (int)ids.size();
}

void testfunc()
{
    /*AcDbObjectId a = MD2010_CreateLine(0,0,100,100);
	AcDbObjectId b = MD2010_CreateCycle(100,100,0,100);
	AcDbObjectId c = MD2010_CreateArc(200,200,100,0,PI);
	MD2010_SetCurrentLayout(L"布局1");
	MD2010_CreateLayout();*/
	//MD2010_CreateNewLayer(L"你好");
	//MD2010_CreateViewport(L"Template", AcGePoint3d(100,100,0), 30,30);
	//MD2010_AddTextToPaperSpace(L"Template", L"封面", 20,
	//	20,L"2.5", AcGePoint3d(30,30,0));
	CString aa = 
	CF_STR_get_single_font_str_from_str(L"你好", L"YOUYUAN",3.0,1.0);
    MD2010_AddMTextToPaperSpace_MIDDLELEFT(L"布局2", aa, 2.5,
        3, L"2.5_En", AcGePoint3d(0,0,0));
	return;
	AcDbObjectId id = MD2010_AddHatchRectangle(AcGePoint3d( 0,0,0), AcGePoint3d(300,300,0));
	MD2010_SetEntityColor(id, 8);
}

bool CreateThumbnailBmp(CString p_sDwgFilePath, CString p_sBmpFilePath)
{
	CFile dwgFile;
	if(!dwgFile.Open(p_sDwgFilePath, CFile::modeRead))
		return false;

	int iPosSentinel;
	dwgFile.Seek(13, CFile::begin);
	dwgFile.Read(&iPosSentinel, 4);
	char iNum;
	dwgFile.Seek(iPosSentinel + 20, CFile::begin);
	dwgFile.Read(&iNum, 1);
	for(char i = 0; i < iNum; i++)
	{
		char iType;
		int iPos, iLen;
		dwgFile.Read(&iType, 1);
		dwgFile.Read(&iPos, 4);
		dwgFile.Read(&iLen, 4);
		if(iType == 2)
		{
			dwgFile.Seek(iPos, CFile::begin);
			char* bmpData = new char[iLen];
			dwgFile.Read(bmpData, iLen);

			BITMAPFILEHEADER fileHead;
			fileHead.bfType = 0x4D42;
			fileHead.bfOffBits = 0x436;
			fileHead.bfSize = fileHead.bfOffBits + iLen;

			CFile bmpFile;
			bmpFile.Open(p_sBmpFilePath, CFile::modeCreate|CFile::modeWrite);
			bmpFile.Write(&fileHead, sizeof(fileHead));
			bmpFile.Write(bmpData, iLen);

			delete[] bmpData;
			dwgFile.Close();
			bmpFile.Close();
			return true;
		}
	}
	dwgFile.Close();
	return false;
}


Acad::ErrorStatus MD2010_InsertDwgFile(const WCHAR *p_dwgPath, AcGePoint3d p_origin)
{
	acDocManager->lockDocument(curDoc());

	Acad::ErrorStatus es;

	AcDbDatabase *pSrcDwg = NULL;

	try
	{
		AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();

		pSrcDwg = new AcDbDatabase(Adesk::kFalse);
		es = pSrcDwg->readDwgFile(p_dwgPath);
		if (Acad::eOk != es)
			throw(es);

		AcGeMatrix3d mat3d;
		mat3d.setToIdentity();
		mat3d.setToTranslation(p_origin.asVector());

		es = pCurDb->insert(mat3d, pSrcDwg);
		if (Acad::eOk != es)
			throw(es);
	}
	catch (Acad::ErrorStatus)
	{
		acutPrintf(L"\n插入块错误.");
	}

	if (pSrcDwg != NULL)
	{
		delete pSrcDwg;
		pSrcDwg = NULL;
	}

	acDocManager->unlockDocument(curDoc());

	return es;
}


Acad::ErrorStatus  MD2010_InsertDwgFile2(const WCHAR *p_dwgPath, AcGePoint3d p_origin, AcDbObjectIdArray & p_idsOut)
{
	p_idsOut.removeAll();

	Acad::ErrorStatus es;

	AcDbDatabase *pSrcDwg = NULL;

	acDocManager->lockDocument(curDoc());
	try
	{
		AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable *pCurBlockTable = NULL;
		es = pCurDb->getBlockTable(pCurBlockTable, AcDb::kForRead);
		if (Acad::eOk != es)
			throw(es);

		AcDbBlockTableRecord *pCurBlockTableRecord;
		es = pCurBlockTable->getAt(ACDB_MODEL_SPACE, pCurBlockTableRecord, AcDb::kForRead);
		if (Acad::eOk != es)
			throw(es);

		AcDbObjectId curDbMsId = pCurBlockTableRecord->objectId();
		pCurBlockTableRecord->close();
		pCurBlockTable->close();


		//////////////////////////////////////////////////////////////////////////
		pSrcDwg = new AcDbDatabase(Adesk::kFalse);
		es = pSrcDwg->readDwgFile(p_dwgPath);
		if (Acad::eOk != es)
			throw(es);

		AcGeMatrix3d mat3d;
		mat3d.setToIdentity();
		mat3d.setToTranslation(p_origin.asVector());

		//////////////////////////////////////////////////////////////////////////

		AcDbBlockTable *pBlockTable;
		es = pSrcDwg->getBlockTable(pBlockTable, AcDb::kForRead);
		if (Acad::eOk != es)
			throw(es);

		AcDbBlockTableRecord *pBlkTblRcd;
		pBlockTable->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);

		AcDbObjectIdArray  sourceIds;

		// 创建遍历器，依次访问模型空间的每一个实体
		AcDbBlockTableRecordIterator *it = NULL;
		pBlkTblRcd->newIterator(it);
		for (it->start(); !it->done(); it->step())
		{
			AcDbObjectId idTemp;
			es = it->getEntityId(idTemp);
			if (Acad::eOk == es)
			{
				sourceIds.append(idTemp);
			}
		}
		delete it;

		AcDbIdMapping idMap;
		es = pSrcDwg->wblockCloneObjects(sourceIds, curDbMsId, idMap, AcDb::kDrcReplace, false);
		//Acad::ErrorStatus es = pSrcDwg->deepCloneObjects(sourceIds, curDbMsId, idMap);
		if (es == Acad::eOk)
		{
			//变换处理
			AcDbIdMappingIter iter(idMap);
			for (iter.start(); !iter.done(); iter.next())
			{
				AcDbIdPair idPair;
				if (!iter.getMap(idPair))
					continue;

				if (idPair.isCloned())
				{
					AcDbEntity* clonedEnt;
					es = acdbOpenAcDbEntity(clonedEnt, idPair.value(), AcDb::kForWrite);
					if (es == Acad::eOk)
					{
						if (idPair.isPrimary())
						{
							p_idsOut.append(idPair.value());
							clonedEnt->transformBy(mat3d);
						}
						clonedEnt->close();
					}
				}
			}
		}

		pBlockTable->close();
		pBlkTblRcd->close();
	}
	catch (Acad::ErrorStatus)
	{
		acutPrintf(L"\n插入块错误.");
	}

	if (pSrcDwg != NULL)
	{
		delete pSrcDwg;
		pSrcDwg = NULL;
	}

	acDocManager->unlockDocument(curDoc());

	return es;
}

AcDbObjectId   GetBlockRecordId(const ACHAR* entryName)
{
	AcDbObjectId curDbMsId = AcDbObjectId::kNull;

	Acad::ErrorStatus es;
	AcDbDatabase *pCurDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTable *pCurBlockTable = NULL;
	es = pCurDb->getBlockTable(pCurBlockTable, AcDb::kForRead);
	if (Acad::eOk != es)
		return AcDbObjectId::kNull;

	AcDbBlockTableRecord *pCurBlockTableRecord;
	es = pCurBlockTable->getAt(entryName, pCurBlockTableRecord, AcDb::kForRead);
	if (Acad::eOk == es)
	{
		curDbMsId = pCurBlockTableRecord->objectId();
		pCurBlockTableRecord->close();
	}
	pCurBlockTable->close();

	return curDbMsId;
}