#include "stdafx.h"
#include "ComFun_Def.h"
using namespace std;

#if (!defined ARX_2010) && (!defined ARX_2011)

#include <dbeval.h>
#include <AcValue.h>
#include <AcDbAssocArrayRectangularParameters.h>
#include <AcDbAssocArrayActionBody.h>
#include <AcDbAssocManager.h>
#include "../Common/ComFun_ACad.h"
#include "dbobjptr2.h"
#include <AcDbAssocArrayPathParameters.h>
#include <AcDbAssocArrayPolarParameters.h>

//参考
//https://adndevblog.typepad.com/autocad/2016/05/creating-assoc-array-objects-with-objectarx.html

static AcDbEntity* createDemoEntity(int index);
static bool eOkVerify(Acad::ErrorStatus es);

bool TYCOM_IsArray(AcDbObjectId arrayid)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, arrayid, AcDb::kForRead);
	if(pEnt == 0)
		return false;

	//这个函数是判断一个实体是不是关联对象的一个好方法  
	bool isarray = AcDbAssocArrayActionBody::isAssociativeArray(pEnt);
	pEnt->close();
	return isarray; 
}

AcDbObjectId TYCOM_GetActionId(AcDbObjectId arrayid)
{
	AcDbEntity * pEnt = 0;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, arrayid, AcDb::kForRead);

	if(pEnt==0)
		return 0;

	AcDbObjectId actionid = AcDbAssocArrayActionBody::getControllingActionBody(pEnt);
	pEnt->close();

	return actionid;
}

int TYCOM_ModifyRectArrayParm_DEMO(AcDbObjectId actionid)
{
	acDocManager->lockDocument(curDoc());
	AcDbObject *pobj = 0;
	Acad::ErrorStatus es = acdbOpenObject(pobj, actionid, AcDb::kForWrite);
	if(pobj == 0)
	{
		acDocManager->unlockDocument(curDoc());
		return -1;
	}

	AcDbAssocArrayActionBody * actionidBody = AcDbAssocArrayActionBody::cast(pobj);
	AcDbAssocArrayParameters *parm = actionidBody->parameters();
	AcDbAssocArrayRectangularParameters * driveparm = AcDbAssocArrayRectangularParameters::cast(parm);
	if(driveparm == 0)
	{
		pobj->close();
		acDocManager->unlockDocument(curDoc());
		return -1;
	}
	es = driveparm->setColumnCount(3);
	pobj->close();
	acDocManager->unlockDocument(curDoc());
	return 0;
}

int TYCOM_GetArrayObjects(AcDbObjectId actionid, vAcDbObjectId &ids)
{
	acDocManager->lockDocument(curDoc());
	AcDbObject *pobj = 0;
	Acad::ErrorStatus es = acdbOpenObject(pobj, actionid, AcDb::kForWrite);
	if(pobj == 0)
	{
		acDocManager->unlockDocument(curDoc());
		return -1;
	}

	AcDbAssocArrayActionBody * actionidBody = AcDbAssocArrayActionBody::cast(pobj);
	if(actionidBody == 0)
	{
		pobj->close();
		acDocManager->unlockDocument(curDoc());
		return -1;
	}

	AcArray<AcDbItemLocator> indices;
	actionidBody->getItems(indices);

	for (int i = 0; i < indices.length(); i++)
	{
		const AcDbAssocArrayItem* oneItem = actionidBody->getItemAt(indices[i]);
		AcDbObjectId id = oneItem->entityProvider();
		ids.push_back(id);
	}
	pobj->close();
	acDocManager->unlockDocument(curDoc());
	return 0;
}


int TYCOM_CreateRectArrayDemo()
{
	// argument variables
	double columnSpacing = 10;
	double rowSpacing = 10;
	double levelSpacing = 10;
	int columnCount = 2;
	int rowCount =  2;
	int levelCount = 1;
	double rowElevation = 1;
	double axesAngle = 90;
	double rotation = 0;
	AcGePoint3d basePoint;
	AcDbObjectIdArray profileEntityIdArray;
	AcDbObjectId      arrayId;
	AcDbObjectId      actionBodyId;

	basePoint = basePoint.kOrigin;
	AcDbVertexRef basePointVertex(basePoint);

	// create the profile entity based on the profile entity index
	AcDbEntity * profileEntity = createDemoEntity(1);
	//post the newly created entity to database
	AcDbObjectId profileEntityId = JHCOM_PostToModelSpace(profileEntity);


	if(profileEntityId != 0)
	{
		//Add the profile entity id to the profile entity id array
		profileEntityIdArray.append(profileEntityId);
		// Create the AcDbRectangularParameters object
		AcDbAssocArrayRectangularParameters *rectParam = new AcDbAssocArrayRectangularParameters(columnSpacing,
			rowSpacing,levelSpacing,columnCount,rowCount,
			levelCount,rowElevation,axesAngle);
		//Create the array from the source entities
		if(AcDbAssocArrayActionBody::createInstance(profileEntityIdArray,basePointVertex,rectParam,arrayId,actionBodyId)) return -1;
		// The newly created action will be automatically evaluated at the end
		// of the command.
		//
		// If we want to know whether the evaluation is going to succeed, we can
		// evaluate the network explicitly and check the evaluation status
		// of the action
		//
		bool beval = AcDbAssocManager::evaluateTopLevelNetwork(arrayId.database());

	}

	return 0;
}

void TYCOM_ModifyPathArrayParm_DEMO()
{
	try
	{
		Acad::ErrorStatus es;
		// argument variables
		int nVarCount = 0;
		int index = 0;
		int isAssociative = 1;
		AcGePoint3d basePoint;
		double itemsapcing = 1.573;
		double rowSpacing = 1.5;
		double levelSpacing = 1;
		int itemCount = 1;
		int rowCount = 1;
		int levelCount = 1;
		double rowElevation = 1;
		int method=0;
		AcDbObjectId      profileEntityId;
		AcDbObjectId      CurveEntityId;
		AcDbEntity *      profileEntity = NULL;
		AcDbEntity *      curveEntity = NULL;
		AcDbObjectIdArray profileEntityIdArray;
		AcDbObjectId      arrayId;
		AcDbObjectId      actionBodyId;

		index = 1;
		isAssociative = 1;
		basePoint = basePoint.kOrigin;

		Adesk::Boolean isAssoc = !!isAssociative;
		AcDbVertexRef *basePointVertex = new AcDbVertexRef(basePoint);
		// create the profile entity
		profileEntity = createDemoEntity(index);
		//post the newly created entity to database
		profileEntityId = JHCOM_PostToModelSpace(profileEntity);
		//This will create the path on which the array will be created
		index = 3;
		curveEntity =  createDemoEntity(index); 
		AcDbLine* path =  (AcDbLine*)curveEntity;
		double param1,length;
		path->getEndParam(param1);
		path->getDistAtParam(param1,length);

		itemCount = int(length / itemsapcing);

		//post the newly created entity to database
		CurveEntityId = JHCOM_PostToModelSpace(curveEntity);         
		if(CurveEntityId)
		{
			AcDbSmartObjectPointer<AcDbEntity> profileEntity1(profileEntityId,AcDb::kForRead);
			if(!eOkVerify(profileEntity1.openStatus())) return;
			AcDbSmartObjectPointer<AcDbEntity> curveEntity1(CurveEntityId,AcDb::kForRead);
			if(!eOkVerify(curveEntity1.openStatus()))return ;
			//Add the profile entity id to the profile entity id array
			profileEntityIdArray.append(profileEntityId);
			// Create the PathArrayParameters object
			AcDbAssocArrayPathParameters *pathParam =new AcDbAssocArrayPathParameters(itemsapcing,rowSpacing,
				levelSpacing,itemCount,rowCount,
				levelCount,rowElevation);
			//Add code to set the path of the array
			AcDbEdgeRef edge(curveEntity1);
			es=pathParam->setPath(edge);
			//Set the method Divide or Measure
			if(method==1)
				es= pathParam->setMethod(AcDbAssocArrayPathParameters::kDivide);
			else
				es= pathParam->setMethod(AcDbAssocArrayPathParameters::kMeasure);

			//Create the array from the source entities
			es = AcDbAssocArrayActionBody::createInstance(profileEntityIdArray,*basePointVertex,pathParam,arrayId,actionBodyId);
			// The newly created action will be automatically evaluated at the end
			// of the command.
			//
			// If we want to know whether the evaluation is going to succeed, we can
			// evaluate the network explicitly and check the evaluation status
			// of the action
			//
			bool beval= AcDbAssocManager::evaluateTopLevelNetwork(arrayId.database());
			if(pathParam!=NULL)
				delete pathParam;
		}
	} // try
	catch (...)
	{
	    acutPrintf( _T("Exception Catch"));
	}
}

void TYCOM_ModifyPolarArrayParm_DEMO()
{
	Acad::ErrorStatus es;
	try
	{
		// argument variables
		int NoOfSrcEntities = 2;
		int index = 1;
		double columnSpacing = 0;
		double rowSpacing = 10;
		double levelSpacing = 5;
		int itemCount = 6;
		int rowCount = 2;
		int levelCount = 2;
		double rowElevation = 3;
		double Angle = 90;
		AcGePoint3d basePoint;
		basePoint = basePoint.kOrigin;
		AcDbObjectId      profileEntity1Id;
		AcDbObjectId      profileEntity2Id;
		AcDbEntity *      profileEntity1 = NULL;
		AcDbEntity *      profileEntity2 = NULL;
		AcDbObjectIdArray profileEntityIdArray;
		AcDbObjectId      arrayId;
		AcDbObjectId      actionBodyId;
		AcDbVertexRef *basePointVertex = new AcDbVertexRef(basePoint);
		// create the profile entity based on the profile entity index

		profileEntity1 = createDemoEntity(index);          
		//post the newly created entity to database
		profileEntity1Id = JHCOM_PostToModelSpace(profileEntity1);
		if(profileEntity1Id)
		{
			//Add the profile entity id to the profile entity id array
			profileEntityIdArray.append(profileEntity1Id);
			//Create AcDbAssocArrayParameters
			AcDbAssocArrayPolarParameters *polarParam = new AcDbAssocArrayPolarParameters (Angle,rowSpacing,levelSpacing,itemCount,rowCount,levelCount,rowElevation);
			//Create the array from the source entities
			es = AcDbAssocArrayActionBody::createInstance(profileEntityIdArray,*basePointVertex,polarParam,arrayId,actionBodyId);
			// The newly created action will be automatically evaluated at the end
			// of the command.
			//
			// If we want to know whether the evaluation is going to succeed, we can
			// evaluate the network explicitly and check the evaluation status
			// of the action
			//
			bool beval = AcDbAssocManager::evaluateTopLevelNetwork(arrayId.database());
		}
	}
	catch(...)
	{
		acutPrintf( _T("Exception Catch: Acad::eUnrecoverableErrors"));
	}
}

static bool eOkVerify(Acad::ErrorStatus es)
{
	if (es == Acad::eOk)
		return true;
	return false;
}
/*Util Method To create Entity*/
static AcDbEntity* createDemoEntity(int index)
{
	AcDbEntity* entity = NULL;
	AcGePoint3d startPt(0,0,0);
	AcGePoint3d endPt(20,20,0);
	switch(index)
	{
		/*Profile 1*/
	case 1:
		{
			AcDbCircle* c = new AcDbCircle(AcGePoint3d(0.0,0.0,0.0),AcGeVector3d(0.0,0.0,1.0),0.5);
			entity = (AcDbEntity*)c;
			break;
		}
		/*Profile 2*/
	case 2:
		{
			AcDbPolyline *pRect = new AcDbPolyline();
			AcGePoint2d vertex1(startPt.x,startPt.y);
			AcGePoint2d vertex2(endPt.x,startPt.y);
			AcGePoint2d vertex3(endPt.x,endPt.y);
			AcGePoint2d vertex4(startPt.x,endPt.y);
			pRect->addVertexAt(0,vertex1);
			pRect->addVertexAt(1,vertex2);
			pRect->addVertexAt(2,vertex3);
			pRect->addVertexAt(3,vertex4);
			pRect->setClosed(Adesk::kTrue);
			entity = (AcDbEntity*)pRect;
			break;

		}
		/*Path for Path ARRAY*/
	case 3:
		{
			AcDbLine *line = new AcDbLine(startPt,endPt);
			entity = (AcDbEntity*)line;
			break;
		}
	default:
		break;
	}
	return entity;
}

#endif