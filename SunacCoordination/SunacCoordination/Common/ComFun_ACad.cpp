#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <dbxutil.h >
#include <brletrav.h>
#include <brbvtrav.h>
#include <geell3d.h>
#include <genurb3d.h>
#include <geextsf.h>
#include <gexbndsf.h>
#include <gecylndr.h>
#include <gecsint.h>
#include <dbxline.h>
#include <float.h>
#include <gecurv3d.h>
#include <geblok3d.h >
#include <brbstrav.h>
#include <brbctrav.h>
#include <brsftrav.h>
#include <algorithm>
#include <gecomp2d.h>
#include <dbapserv.h>
#include "ComFun_ACAD_Common.h"
#include "ComFun_Def.h"

using namespace std;

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetString
* Description     : This function let user input one string from the cad interface.
* Note            : NULL.
* Input           : prompt : prompt to tell user for input.
*                   str : string input by user.
* Output          : NULL.
* Return          : Has input : true, no input : false.
--------------------------------------------------------------------------*/
bool JHCOM_GetString(CString prompt, CString &str)
{
	int rcc = 0;
	ACHAR num[JHSOLID_MAX_STRING] = L"\0";
	rcc = acedGetString(0,prompt, num);
	if (rcc == RTCAN)
		return false;
	else if (rcc != RTCAN&&rcc != RTNORM)
		return false;

	str = CString(num);
	return true;
}



/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ChangeObjectColor
* Description     : This function set a color to object by color index.
* Note            : NULL.
* Input           : entId : Object  to set color.
*                   colorIndex : colorIndex to set on entId
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ChangeObjectColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	// 修改实体的颜色
	pEntity->setColorIndex(colorIndex);
	// 用完之后，及时关闭
	pEntity->close();
	return Acad::eOk;
}

int JHCOM_GetObjectColor(AcDbObjectId entId, AcCmColor &color)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	// 修改实体的颜色
	color = pEntity->color();
	// 用完之后，及时关闭
	pEntity->close();
	return 0;
}

int JHCOM_GetObjectColor(AcDbObjectId entId)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	// 修改实体的颜色
	int colorIndex = pEntity->colorIndex();
	// 用完之后，及时关闭
	pEntity->close();
	return colorIndex;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ChangeObjectColor
* Description     : This function set a color to object by AcCmColor.
* Note            : NULL.
* Input           : entId : Object  to set color.
*                   color : color to set on entId
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ChangeObjectColor(AcDbObjectId entId, const AcCmColor& color)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForWrite);
	// 修改实体的颜色
	pEntity->setColor(color);
	// 用完之后，及时关闭
	pEntity->close();
	return Acad::eOk;
}



/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DeleteCadObject
* Description     : This function delete one cad object from cad database.
* Note            : NULL.
* Input           : id : Object to delete.   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_DeleteCadObject(AcDbObjectId id)  
{  
	if(id == 0)
		return;
	AcDbEntity* pDel = NULL;  
	acDocManager->lockDocument(curDoc());
	if(Acad::eOk == acdbOpenObject(pDel,id,AcDb::kForWrite))  
	{
		pDel->erase();
		pDel->close();  
	}
	acDocManager->unlockDocument(curDoc());
}  

void JHCOM_DeleteCadObjectArray(AcDbObjectIdArray ids)
{
	acDocManager->lockDocument(curDoc());

	for (int i = 0; i < ids.length(); i++)
	{
		AcDbEntity* pDel = NULL;
		if (Acad::eOk == acdbOpenObject(pDel, ids[i], AcDb::kForWrite))
		{
			pDel->erase();
			pDel->close();
		}
	}

	acDocManager->unlockDocument(curDoc());
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetStartPoint
* Description     : This function get start point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get start point.
* Output          : NULL.
* Return          : Start point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_GetStartPoint(AcDbObjectId curveID)
{
	AcDbObject *pEnt = NULL;

	Acad::ErrorStatus es = acdbOpenAcDbObject(pEnt,curveID,AcDb::kForRead);
	if(es != Acad::eOk)
	{
		return AcGePoint3d(0,0,0);
	}
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve == NULL)
	{
		pEnt->close();
		return AcGePoint3d(0,0,0);
	}

	AcGePoint3d start;
	pCurve->getStartPoint(start);
	pCurve->close();
	return start;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEndPoint
* Description     : This function get end point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get end point.
* Output          : NULL.
* Return          : end point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_GetEndPoint(AcDbObjectId curveID)
{
	AcDbObject *pEnt = NULL;

	Acad::ErrorStatus es = acdbOpenAcDbObject(pEnt,curveID,AcDb::kForRead);
	if(es != Acad::eOk)
	{
	    return AcGePoint3d(0,0,0);
	}

	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve == NULL)
	{
		pEnt->close();
		return AcGePoint3d(0,0,0);
	}

	AcGePoint3d end;
	pCurve->getEndPoint(end);
	pCurve->close();
	return end;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMidPoint
* Description     : This function get mid point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get mid point.
* Output          : NULL.
* Return          : mid point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_GetMidPoint(AcDbObjectId curveID)
{
	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,curveID,AcDb::kForRead);
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve == NULL)
	{
		pEnt->close();
		return AcGePoint3d(0,0,0);
	}

	double startParm, endParm;
	AcGePoint3d pt;
	pCurve->getStartParam(startParm);
	pCurve->getEndParam(endParm);
	pCurve->getPointAtParam((startParm + endParm)/2.0, pt);
	pCurve->close();
	return pt;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLineVector
* Description     : This function get a AcGeVector3d from line start point to line end point.
* Note            : NULL.
* Input           : lineid : Line to get  AcGeVector3d.
* Output          : NULL.
* Return          : Line's AcGeVector3d.
--------------------------------------------------------------------------*/
AcGeVector3d JHCOM_GetLineVector(AcDbObjectId lineid)
{
	AcDbObject *pEnt = NULL;
	AcGePoint3d start,end;

	acdbOpenAcDbObject(pEnt,lineid,AcDb::kForRead);
	
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
	{
		AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
		if (pCurve == NULL)
		{
			pEnt->close();
			return AcGeVector3d(0,0,0);
		}
		pCurve->getStartPoint(start);
		pCurve->getEndPoint(end);
	}
	else
	{
		start = pLine->startPoint();
		end = pLine->endPoint();
	}
	
	pEnt->close();

	return AcGeVector3d(end.x - start.x, end.y - start.y, end.z - start.z);
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveStartPointVector
* Description     : This function get a AcGeVector3d from the start point of a curve.
* Note            : This AcGeVector3d start from start point and point to the tangent direction
*                   of this start point, for one line, just from start point to end point.
* Input           : curveId : curveId to get start AcGeVector3d.
* Output          : NULL.
* Return          : start AcGeVector3d of input curve.
--------------------------------------------------------------------------*/
AcGeVector3d JHCOM_GetCurveStartPointVector(AcDbObjectId curveId)
{
	AcDbObject *pEnt = NULL;
	AcGeVector3d dir;

	acdbOpenAcDbObject(pEnt,curveId,AcDb::kForRead);
	
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve != NULL)
	{
		AcGePoint3d start;
		pCurve->getStartPoint(start);
		pCurve->getFirstDeriv(start, dir);
	}
	else
		dir = AcGeVector3d(0,0,0);

	pEnt->close();
	return dir;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveLength
* Description     : This function get the length of a curve.
* Note            : NULL.
* Input           : curveId : curveId to get length.
* Output          : NULL.
* Return          : length of input curve.
--------------------------------------------------------------------------*/
double JHCOM_GetCurveLength(AcDbObjectId curveId)
{
	AcDbObject *pEnt = NULL;
    double length = 0;
	acdbOpenAcDbObject(pEnt,curveId,AcDb::kForRead);
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt); 
	if (pCurve == NULL)
	{
		pEnt->close();
		return -1;
	}
	double endParm = 0;
	Acad::ErrorStatus es = pCurve->getEndParam(endParm);
	if(es == Acad::eOk)
	{
		pCurve->getDistAtParam(endParm, length);
	}
	pCurve->close();

	return length;
}

double JHCOM_GetCurveLength(AcGeCurve3d* pCurve)
{
	AcGeInterval range;
	pCurve->getInterval(range);
	return pCurve->length(range.lowerBound(), range.upperBound());
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateRegionFromCurve
* Description     : This function create a AcDbRegion from input AcDbCurve.
* Note            : NULL.
* Input           : ploy : input polyline to create region.
* Output          : NULL.
* Return          : AcDbRegion pointer.
--------------------------------------------------------------------------*/
AcDbRegion * JHCOM_CreateRegionFromCurve(AcDbCurve * ploy)
{
	if (ploy == NULL)
		return NULL;
	AcDbVoidPtrArray paRect,paRegion;   
	paRect.append(ploy);
	AcDbRegion::createFromCurves(paRect,paRegion); 
	return (AcDbRegion *)(paRegion[0]); 
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_PostToModelSpace
* Description     : This function post one AcDbEntity to cad database.
* Note            : NULL.
* Input           : pEnt : AcDbEntity to post in.   
* Output          : NULL.
* Return          : id : Object id returned by database.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_PostToModelSpace(AcDbEntity* pEnt)
{
	AcDbBlockTable *pBlockTable;
	if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead)!=Acad::eOk)
	{
		return AcDbObjectId::kNull;
	}

	AcDbObjectId entId = AcDbObjectId::kNull;

	//acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);
	acDocManager->lockDocument(curDoc());
	AcDbBlockTableRecord *pBlockTableRecord;
	Acad::ErrorStatus es = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	if (es==Acad::eOk)
	{
		es = pBlockTableRecord->appendAcDbEntity(entId, pEnt);
		pBlockTableRecord->close();
	}

	pBlockTable->close();
	pEnt->close();
	acDocManager->unlockDocument(curDoc());

	return entId;
}





/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetTwoLineStatus
* Description     : This function help to check the 3d status of two lines.
* Note            : NULL.
* Input           : a :  start point of first line.
*                   b :  end point of first line.
*                   c :  start point of second line.
*                   d :  end point of second line.
* Output          : NULL.
* Return          : 0 same line 1 parallel, 2 X.
--------------------------------------------------------------------------*/
int JHCOM_GetTwoLineStatus(AcGePoint3d a, AcGePoint3d b, AcGePoint3d c, AcGePoint3d d)
{
	if(b.x - a.x == 0)
	{
		if((c.x - d.x == 0))
		{
			if(c.x - a.x == 0) return 0;
			else return 1;
		}
		else return 2;
	}
	else
	{
		if(c.x - a.x == 0) return 2;
		else
		{
			double k1 = fabs((b.y - a.y)/(b.x - a.x));
			double k2 = fabs((d.y - c.y)/(d.x - c.x));
			double k3 = fabs((c.y - a.y)/(c.x - a.x));
			if(JHCOM_equ(k1,k2))
			{
				if(JHCOM_equ(k1, k3)) return 0;
				else return 1;
			}
			else
				return 2;
		}
	}
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetTwoLineStatus
* Description     : This function help to check the 3d status of two lines.
* Note            : NULL.
* Input           : line1 :  id of first line.
*                   line2 :  id second line.
* Output          : NULL.
* Return          : 0 sameline 1 parall, 2 X.
--------------------------------------------------------------------------*/
int JHCOM_GetTwoLineStatus(AcDbObjectId line1, AcDbObjectId line2)
{
	if(line1 == 0 || line2 == 0)
		return -1;
    return JHCOM_GetTwoLineStatus(JHCOM_GetStartPoint(line1), JHCOM_GetEndPoint(line1), JHCOM_GetStartPoint(line2), JHCOM_GetEndPoint(line2));
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsLine
* Description     : Check whether input id is a line or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is Line : true, is not line : false.
--------------------------------------------------------------------------*/
bool JHCOM_IsLine(AcDbObjectId id)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);
	if ((pEnt != NULL) && (pEnt->isKindOf(AcDbLine::desc())))
		is = true;
	else
		is = false;
	pEnt->close();
	return is;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsArc
* Description     : Check whether input id is a arc or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is arc : true, is not arc : false.
--------------------------------------------------------------------------*/
bool JHCOM_IsArc(AcDbObjectId id)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);
	if ((pEnt != NULL) && (pEnt->isKindOf(AcDbArc::desc())))
		is = true;
	else
		is = false;
	pEnt->close();
	return is;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsLineSimilar
* Description     : Check whether input id is similar to a line.
* Note            : This function need more optimization.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is similar to Line : true, is not similar to Line : false.
--------------------------------------------------------------------------*/
bool  JHCOM_IsLineSimilar(AcDbObjectId id) 
{
	if(id == 0)
		return false;

	if(JHCOM_IsArc(id))
		return true;

	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,id,AcDb::kForRead);
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve == NULL)
	{
		pEnt->close();
		return false;
	}

	double startParm, endParm;
	AcGePoint3d pt;
	pCurve->getStartParam(startParm);
	pCurve->getEndParam(endParm);
	int checkSize = 10;
	vAcGePoint3d pnts;
	for(int i = 0; i <= checkSize; i++)
	{
		pCurve->getPointAtParam(i*(startParm + endParm)/checkSize, pt);
		pnts.push_back(pt);
	}
	pCurve->close();

	AcGeLineSeg3d *pGL=new AcGeLineSeg3d;
	pGL->set(pnts[0],pnts[checkSize]);
	double dis = 0;
	for(int i = 1; i < checkSize; i++)
	{
		AcGePoint3d pt2 = pGL->closestPointTo(pnts[i]);
		dis += JHCOM_PointDistance(pt2, pnts[i]);
	}
	delete pGL;
	if(dis/checkSize < 1.0)
		return true;

	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsArcSimilar
* Description     : Check whether input id is similar to a arc.
* Note            : This function need more optimization.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is similar to arc : true, is not similar to arc : false.
--------------------------------------------------------------------------*/
bool  JHCOM_IsArcSimilar(AcDbObjectId id, double arcRadTol)
{
	if(id == 0)
		return false;

	if(JHCOM_IsLine(id))
		return true;

	AcDbObject *pEnt = NULL;
	acdbOpenAcDbObject(pEnt,id,AcDb::kForRead);
	AcDbCurve *pCurve = AcDbCurve ::cast(pEnt);
	if (pCurve == NULL)
	{
		pEnt->close();
		return false;
	}

	double startParm, endParm;
	AcGePoint3d pt;
	pCurve->getStartParam(startParm);
	pCurve->getEndParam(endParm);
	int checkSize = 10;
	vAcGePoint3d pnts;
	for(int i = 0; i <= checkSize; i++)
	{
		pCurve->getPointAtParam(i*(startParm + endParm)/checkSize, pt);
		pnts.push_back(pt);
	}
	pCurve->close();

	AcGeCircArc3d *pGArc=new AcGeCircArc3d(pnts[0],pnts[checkSize/2],pnts[checkSize]);
	if(pGArc->radius() > arcRadTol)
	{
		delete pGArc;
		return false;
	}

	double dis = 0;
	for(int i = 1; i < checkSize; i++)
	{
		AcGePoint3d pt2 = pGArc->closestPointTo(pnts[i]);
		dis += JHCOM_PointDistance(pt2, pnts[i]);
	}
	
	if(dis/checkSize < 1.0)
		return true;
	
	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsCircle
* Description     : Check whether input id is a Circle or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is Circle : true, is not Circle : false.
--------------------------------------------------------------------------*/
bool JHCOM_IsCircle(AcDbObjectId id)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForRead);
	if ((pEnt != NULL) && (pEnt->isKindOf(AcDbCircle::desc())))
		is = true;
	else
		is = false;
	pEnt->close();
	return is;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateText
* Description     : This function create a text in the specify position.
* Note            : This is old code has not been refined.
* Input           : ptInsert : text position.
*                   text : text string.
* Output          : NULL.
* Return          : Created text id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateText(const AcGePoint3d &ptInsert, const wchar_t *text)
{
		AcDbText *pText = new AcDbText(ptInsert, text,AcDbObjectId::kNull,15,0);
		return JHCOM_PostToModelSpace(pText);
}

AcDbObjectId JHCOM_CreateMText(const AcGePoint3d &ptInsert, const wchar_t *text, int color)
{
		AcDbMText *pText = new AcDbMText();
		pText->setLocation(ptInsert);
		pText->setTextHeight(1);
		pText->setContents(text);
		pText->setColorIndex(color);
		return JHCOM_PostToModelSpace(pText);
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateText
* Description     : This function create a text in the specify position.
* Note            : This is old code has not been refined.
* Input           : ptInsert : text position.
*                   text : text string.
*                   height : text height.
*                   angle : text angle.
* Output          : NULL.
* Return          : Created text id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateText(const AcGePoint3d &ptInsert,const AcGeVector3d &normal, double height, double angle, const wchar_t *text)
{
		AcDbText *pText = new AcDbText(ptInsert, text,AcDbObjectId::kNull,height,PI*angle/180);
		pText->setNormal(normal);
		pText->setPosition(ptInsert);
		return JHCOM_PostToModelSpace(pText);
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateLine
* Description     : This function create a line with given start and end point.
* Note            : NULL.
* Input           : ptStart :  Start point of line.
*                   ptEnd : End point of line.
* Output          : NULL.
* Return          : Created line id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd)
{
	AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);
	// 将实体添加到图形数据库
	AcDbObjectId lineId;
	lineId = JHCOM_PostToModelSpace(pLine);

	return lineId;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Create3dPolyline
* Description     : This function create a AcDb3dPolyline with points.
* Note            : NULL.
* Input           : points : Points to create AcDb3dPolyline.
* Output          : NULL.
* Return          : Created 3dPolyline id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_Create3dPolyline(AcGePoint3dArray points,AcDb::Poly3dType type)
{
	AcDb3dPolyline *pPoly3d = new
		AcDb3dPolyline(type, points);
	return JHCOM_PostToModelSpace(pPoly3d);
}


AcDbObjectId JHCOM_Create3dPolyline(AcGePoint3dArray points, Adesk::UInt16 color,AcDb::Poly3dType type)
{
	AcDbObjectId id = JHCOM_Create3dPolyline(points,type);
	JHCOM_SetEntityColor(id, color);
	return id;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateArc
* Description     : This function create a AcDbArc.
* Note            : NULL.
* Input           : center : Arc center.
*                   normal : Arc normal.
*                   radius : Arc radius.
*                   startangle : Arc start angle.
*                   end angle : Arc eng angle.
* Output          : NULL.
* Return          : Created arc id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateArc(const AcGePoint3d &center, AcGeVector3d &normal, 
						             double radius, double startangle, double endangle)
{
	AcDbArc *pArc = new AcDbArc(center, normal, radius, startangle, endangle);
	AcDbObjectId arcID = JHCOM_PostToModelSpace(pArc);
	pArc->close();
	return arcID;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateLine
* Description     : This function create a line with given start and end point
*                   and set the color to it.
* Note            : NULL.
* Input           : ptStart :  Start point of line.
*                   ptEnd : End point of line.
*                   color : color set to line.
* Output          : NULL.
* Return          : Created line id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd,Adesk::UInt16
							colorIndex)
{
	if (ptStart.distanceTo(ptEnd) < TOL * 100)
	    return 0;

	AcDbLine *pLine = new AcDbLine(ptStart, ptEnd);
	pLine->setColorIndex(colorIndex);

	// 将实体添加到图形数据库
	AcDbObjectId lineId;
	lineId = JHCOM_PostToModelSpace(pLine);

	return lineId;
}


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLineStartPoint
* Description     : This function set the start point of a line.
* Note            : NULL.
* Input           : lineid : Line id to set start point.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetLineStartPoint(AcDbObjectId id,AcGePoint3d start)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
	{
        pEnt->close();
		return -1;
	}
	else
	{
        pLine->setStartPoint(start);
	}
	pEnt->close();
	return 0;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLineEndPoint
* Description     : This function set the end point of a line.
* Note            : NULL.
* Input           : lineid : Line id to set end point.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_SetLineEndPoint(AcDbObjectId id,AcGePoint3d end)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
	{
		pEnt->close();
		return -1;
	}
	else
	{
		pLine->setEndPoint(end);
	}
	pEnt->close();
	return 0;
}

int JHCOM_SetLineStartEndPoint(AcDbObjectId id,AcGePoint3d start,AcGePoint3d end)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	AcDbLine *pLine = AcDbLine ::cast(pEnt);
	if (pLine == NULL)
	{
		if(pEnt ==NULL)
			return -2;
		pEnt->close();
		return -1;
	}
	else
	{
		//pLine->upgradeOpen();///!!!!!!!!!!!!!!!!!!!!!!!
		pLine->setStartPoint(start);
		pLine->setEndPoint(end);
		//pLine->downgradeOpen();///!!!!!!!!!!!!!!!!!!!!!!!
		//pLine->recordGraphicsModified();
		pLine->draw();

	}
	pEnt->close();
	return 0;
}

//This function extend a input line by input value.
//You can choose to just extend start or end part of this line.
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ExtendLine
* Description     : This function extend a line with start and end options, if extendStart
*                   is true and valueStart is bigger than zero, start point will be extended.
                    if extendEnd is true and valueEnd is bigger than zero, end point will be extended.
* Note            : NULL.
* Input           : line : Line id to extend.
*                   extendStart : Should start point be extended?
                    extendEnd : Should endPoint be extended?
					valueStart : start extend value, valid when extendStart is true.
*                   valueEnd : End extend value, valid when extendEnd is true.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int JHCOM_ExtendLine(AcDbObjectId line, bool extendStart, bool extendEnd, double valueStart, double valueEnd)
{
	if(line == NULL || (JHCOM_sma(valueStart, 0.0) && JHCOM_sma(valueEnd, 0.0)) || ((!extendStart)&&(!extendEnd)))
		return 1;

	AcGePoint3d start_point = JHCOM_GetStartPoint(line);
	AcGePoint3d end_point = JHCOM_GetEndPoint(line);
	double len = JHCOM_GetCurveLength(line);
	double scaleStart = valueStart/len;
	double scaleEnd= valueEnd/len;

	if(extendStart && JHCOM_big(extendStart,0.0))
	{
		//double pt[3] = {end[0] - start[0], end[1] - start[1], end[2] - start[2]};
		double pt[3] = {start_point[0] + (start_point[0] - end_point[0])*scaleStart, 
			            start_point[1] + (start_point[1] - end_point[1])*scaleStart, 
			            start_point[2] + (start_point[2] - end_point[2])*scaleStart};
		JHCOM_SetLineStartPoint(line, AcGePoint3d(pt[0],pt[1],pt[2]));
	}

	if(extendEnd&& JHCOM_big(extendEnd,0.0))
	{
		double pt[3] = { end_point[0] + (end_point[0] - start_point[0])*scaleEnd, 
			             end_point[1] + (end_point[1] - start_point[1])*scaleEnd, 
			             end_point[2] + (end_point[2] - start_point[2])*scaleEnd};
		JHCOM_SetLineEndPoint(line, AcGePoint3d(pt[0],pt[1],pt[2]));
	}
	return 0;
}




/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Explode
* Description     : This function explode one object and store the exploded objects in entitySet.
* Note            : NULL.
* Input           : id : Object to explode.
* Output          : entitySet : Exploded objects.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_Explode(AcDbObjectId id, AcDbVoidPtrArray &entitySet)
{
	if(id == 0) 
		return;

	AcDbEntity *pEnt = NULL;
	acdbOpenAcDbEntity(pEnt,id,AcDb::kForRead);
	if(pEnt == NULL) 
		return;
	
	pEnt->explode(entitySet);
	if(entitySet.length() == 0) 
	{
		pEnt->close();
		return;
	}
	pEnt->close();
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Explode
* Description     : This function explode one object (pointer) and store the exploded objects in entitySet.
* Note            : NULL.
* Input           : pEnt : Object pointer to explode.
* Output          : entitySet : Exploded objects.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_Explode(AcDbEntity *pEnt, AcDbVoidPtrArray &entitySet)
{
	if(pEnt == NULL) 
		return;
	pEnt->explode(entitySet);
}

//AcGeCurveSurfInt 估计有用
//http://adndevblog.typepad.com/autocad/2013/03/intersection-between-a-surface-and-a-line-using-arx.html
//Intersection between a Surface and a Line using ARX
#include <dbxline.h>

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetSameDataNumber
* Description     : This function get the number of same value in a double vector with tolerance.
* Note            : NULL.
* Input           : data : All double  values to find same ones. 
*                   value : same with which one.
*                   tol : Tolerance
* Output          : NULL.
* Return          : Same value number.
--------------------------------------------------------------------------*/
int JHCOM_GetSameDataNumber(vdouble &data, double value, double tol)
{
	int num=0;
	for(UINT i = 0; i < data.size(); i++)
	{
		if(JHCOM_equ(data[i],value,tol))
			num++;
	}
	return num;
}

int JHCOM_SetEntityColor(AcDbObjectId entId, Adesk::UInt16 colorIndex)
{
	AcDbEntity *pent = NULL;
	acdbOpenAcDbEntity(pent,entId,AcDb::kForWrite);
	pent->setColorIndex(colorIndex);
	pent->close();
	return Acad::eOk;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityColor
* Description     : This function get color of entity by color index.
* Note            : NULL.
* Input           : entId :  Entity to get color.
* Output          : NULL.
* Return          : color index of entity..
--------------------------------------------------------------------------*/
Adesk::UInt16 JHCOM_GetEntityColor(AcDbObjectId entId)
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

CString JHCOM_GetNextGroupName()
{
	CString checkStr;
	AcDbDictionary *pGroupDict;
	if (acdbHostApplicationServices()->workingDatabase()
		->getGroupDictionary(pGroupDict, AcDb::kForRead)==Acad::eOk)
	{
		for(int i = 0; i < INT_MAX; i++)
		{
			checkStr.Format(L"JHGROUP_%d",i);
			if (!pGroupDict->has(checkStr))
			{
				pGroupDict->close();
				return checkStr;
			}
		}
		pGroupDict->close();
		return L"";
	}
	else
		return L"";
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateGroup
* Description     : This function create a group with given ids.
* Note            : This is old code has not been refined.
* Input           : objIds :  Object ids to created group.
*                   pGroupName : Created group name.
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_CreateGroup(AcDbObjectIdArray& objIds, wchar_t* pGroupName, AcDbObjectId &groupId)
{
	AcDbGroup *pGroup = new AcDbGroup(pGroupName);
	for (int i = 0; i < objIds.length(); i++) {
		pGroup->append(objIds[i]);
	}
	// 将组添加到有名对象字典的组字典中
	AcDbDictionary *pGroupDict;
	if (acdbHostApplicationServices()->workingDatabase()
		->getGroupDictionary(pGroupDict, AcDb::kForWrite)==Acad::eOk)
	{
		if (pGroupDict->setAt(pGroupName, pGroup, groupId)==Acad::eOk)
		{
			pGroupDict->close();
			pGroup->close();
			return Acad::eOk;
		}
		else
		{
			pGroupDict->close();
			pGroup->close();
			return Acad::eNotApplicable;
		}
	}
	else
		return Acad::eNotApplicable;
}

int JHCOM_GetGroupObject(AcDbObjectId groupId, AcDbObjectIdArray &ids)
{
	AcDbObject *pobj = NULL;
	if(acdbOpenObject(pobj, groupId, AcDb::kForRead)==Acad::eOk)
	{
		AcDbGroup* pgroup = dynamic_cast<AcDbGroup*>(pobj);
		if(pgroup)
		{
			pgroup->allEntityIds(ids);
		}
		pobj->close();
	}
	return 0;
}



bool JHCOM_GetObjectIDFromAcDbHandle(AcDbHandle Handle,AcDbObjectId& Id)
{
	if (acdbHostApplicationServices()->workingDatabase()->getAcDbObjectId(Id,false,Handle,0)==Acad::eOk)
		return true;
	else
		return false;
}


void JHCOM_HilightObject(const AcDbObjectId& objId, bool highlight)
{
	//TCHAR dummy[133];

	AcDbEntity *pEnt;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if(pEnt == NULL)
		return;

	// Highlight the whole solid.
	//
	if(highlight)
	    pEnt->highlight();
	else
		pEnt->unhighlight();
	pEnt->close();
}

void JHCOM_HilightGroup(const AcDbObjectId& objId, bool highlight)
{
	AcDbObjectIdArray ids;
	JHCOM_GetGroupObject(objId, ids);
	for (int i = 0; i < ids.length(); i++)
	{
        JHCOM_HilightObject(ids.at(i), highlight);
	}
}


void JHCOM_DeleteGroup(const AcDbObjectId& objId)
{
	AcDbObjectIdArray ids;
	JHCOM_GetGroupObject(objId, ids);

	AcDbDictionary *pGroupDict;
	if (acdbHostApplicationServices()->workingDatabase()
		->getGroupDictionary(pGroupDict, AcDb::kForWrite)==Acad::eOk)
	{
		pGroupDict->remove(objId);
		pGroupDict->close();
	}

	for (int i = 0; i < ids.length(); i++)
	{
		JHCOM_DeleteCadObject(ids.at(i));
	}
}

bool JHCOM_IsObjectShown(const AcDbObjectId& objId)
{
	AcDbEntity *pEnt;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForRead);
	if(pEnt == NULL)
		return false;

	AcDb::Visibility  vis = pEnt->visibility();
	pEnt->close();

	if (vis == AcDb::kInvisible)
		return false;
	else
		return true;
}

void JHCOM_ShowObject(const AcDbObjectId& objId, bool show, bool refresh)
{
	AcDbEntity *pEnt;

	acDocManager->lockDocument(curDoc());
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pEnt, objId, AcDb::kForWrite);
	if(pEnt == NULL)
		return;

	if (show)
		pEnt->setVisibility(AcDb::kVisible);
	else
		pEnt->setVisibility(AcDb::kInvisible);

	pEnt->close();
	if (refresh)
	{
		actrTransactionManager->flushGraphics();//必须lock住文档才有效果
		acedUpdateDisplay();
	}
	
	acDocManager->unlockDocument(curDoc());
}

void JHCOM_ShowGroup(const AcDbObjectId& objId, bool show)
{
	AcDbObjectIdArray ids;
	acDocManager->lockDocument(curDoc());
	JHCOM_GetGroupObject(objId, ids);
	for (int i = 0; i < ids.length(); i++)
	{
		JHCOM_ShowObject(ids.at(i),show);
	}
	actrTransactionManager->flushGraphics();//必须lock住文档才有效果
	acedUpdateDisplay();
	acDocManager->unlockDocument(curDoc());
}



int JHCOM_CheckPointWithRect(AcGePoint3d pnt, double widX, double heiY, double r)
{
	if(JHCOM_big(fabs(pnt.x), widX, 0.1)|| JHCOM_big(fabs(pnt.y), heiY, 0.1))
		return 1;//out

	if(JHCOM_big(widX - r, fabs(pnt.x), 0.1) && JHCOM_big(heiY, fabs(pnt.y), 0.1))
		return -1;//inner

	if(JHCOM_big(heiY - r, fabs(pnt.y), 0.1) && JHCOM_big(widX, fabs(pnt.x), 0.1))
		return -1;//inner

	double a = fabs(pnt.x) - (widX - r);
	double b = fabs(pnt.y) - (heiY - r);
	if(JHCOM_big(r, sqrt(a * a + b *b), 0.1))
		return -1;

	if(JHCOM_big(sqrt(a * a + b *b),r, 0.1))
		return 1;

	return 0;
}

bool JHCOM_IsPointOnRect(AcGePoint3d pnt, double widX, double heiY, double r)
{
	if(JHCOM_sma(fabs(pnt.x), widX - r, 0.1) && JHCOM_equ(heiY, fabs(pnt.y), 0.1))
		return true;

	if(JHCOM_sma(fabs(pnt.y), heiY - r, 0.1) && JHCOM_equ(widX, fabs(pnt.x), 0.1))
		return true;

	double a = fabs(pnt.x) - (widX - r);
	double b = fabs(pnt.y) - (heiY - r);
	if(JHCOM_equ(r, sqrt(a * a + b *b), 0.1))
		return true;

	return false;
}

bool JHCOM_IsPointOnCircle(AcGePoint3d pnt, double r)
{
	if(JHCOM_equ(r, sqrt(pnt.x * pnt.x + pnt.y *pnt.y), 0.1))
		return true;

	return false;
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetObjectMinMaxPoint
* Description     : This function get min and max point of an object.
* Note            : NULL.
* Input           : entId : Object  to set color.
* Output          : min : min point.
*                   max : max point. 
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_GetObjectMinMaxPoint(AcDbObjectId entId, AcGePoint3d &minPt, AcGePoint3d &maxPt)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	Acad::ErrorStatus  es = acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if (pEntity == 0)
		return es;
	AcDbExtents ext;
	pEntity->getGeomExtents(ext);
	minPt = ext.minPoint();
	maxPt = ext.maxPoint();
//	JHCOM_CreateLine(minPt,maxPt);
	// 用完之后，及时关闭
	pEntity->close();
	return Acad::eOk;
}

/*s说明:
注1：参数中的空字符串_T(“”)等价于按空格键（相当于取消继续选择对象）
注2：如果不希望在acedCommand执行的过程中，出现命令执行过程提示，可以设置cad系统变量CMDECHO等于0
注3：下面的2段引用文字来源于acedCommand命令帮助说明
参考网页http://www.cadgj.com/?p=226
http://through-the-interface.typepad.com/through_the_interface/2008/06/zooming-to-a-wi.html

An empty string (“”) or null string is equivalent to a space entered from the keyboard.
If the AutoCAD system variable CMDECHO is set to 0, the data sent by acedCommand() 
is not displayed; otherwise, the command and its input values are echoed at the AutoCAD 
Command prompt
*/
void JHCOM_ZoomToEntity(const AcDbObjectId& objId)
{
	/*ads_name en;
	if(Acad::eOk != acdbGetAdsName(en, objId)) 
		return;
	acedCommand(RTSTR, _T("ZOOM"), RTSTR, _T("o"), RTENAME, en, RTSTR, _T(" "), 0);*/
	
	AcGePoint3d minPt, maxPt;
    JHCOM_GetObjectMinMaxPoint(objId, minPt, maxPt);

	if (minPt.x >= 0)
		minPt.x *= 0.9;
	else
        minPt.x *= 1.1;
	
	if (minPt.y >= 0)
		minPt.y *= 0.9;
	else
		minPt.y *= 1.1;

	if (minPt.z >= 0)
		minPt.z *= 0.9;
	else
		minPt.z *= 1.1;


	if (maxPt.x >= 0)
		maxPt.x *= 1.1;
	else
		maxPt.x *= 0.9;

	if (maxPt.y >= 0)
		maxPt.y *= 1.1;
	else
		maxPt.y *= 0.9;

	if (maxPt.z >= 0)
		maxPt.z *= 1.1;
	else
		maxPt.z *= 0.9;

	CString zoomCmd;
	zoomCmd.Format(L"Z\nW\n%f,%f,%f\n%f,%f,%f\n",minPt.x, minPt.y, minPt.z, maxPt.x, maxPt.y, maxPt.z);
	acDocManager->sendStringToExecute(curDoc(), zoomCmd);

	//strCmd.Format(L"copybase\n%f,%f,%f\nG\nHEL\n\n",ptOri[X],ptOri[Y],ptOri[Z]); 
	//acDocManager->sendStringToExecute(acDocManager->curDocument(),strCmd,TRUE,TRUE);

	//API zoom  view acdbview
  
}



bool ComparePoint(const AcGePoint3d& p1, const AcGePoint3d& p2)
{
	return p1.x < p2.x; //从小到大排序
}




int JHCOM_Get3dPolylinePoints(AcDbObjectId entId, vAcGePoint3d &vpoints)
{
	AcDbEntity *pEntity;
	// 打开图形数据库中的对象
	acdbOpenObject(pEntity, entId, AcDb::kForRead);
	if(pEntity == NULL)
		return 0;

	AcDb3dPolyline *pPoly3d = AcDb3dPolyline::cast(pEntity);

	AcDbObjectIterator* itor =  pPoly3d->vertexIterator();
	for (itor->start(); !itor->done(); itor->step())
	{
		AcDbObjectId vid = itor->objectId();
		AcDb3dPolylineVertex *pvertext;
		// 打开图形数据库中的对象
		acdbOpenObject(pvertext, vid, AcDb::kForRead);
		vpoints.push_back(pvertext->position());
		pvertext->close();
	}
	delete itor;
	pEntity->close();
	return 0;
}

AcGePoint3d JHCOM_GetCurveStart(AcGeCurve3d* pCurve)
{
	AcGeInterval range;
	pCurve->getInterval(range);
	return pCurve->evalPoint(range.lowerBound());
}


AcGePoint3d JHCOM_GetCurveEnd(AcGeCurve3d* pCurve)
{
	AcGeInterval range;
	pCurve->getInterval(range);
	return pCurve->evalPoint(range.upperBound());
}

Acad::ErrorStatus JHCOM_handleStrToObjId(AcDbDatabase* db, LPCTSTR handleStr, AcDbObjectId& objId, bool errorSpeak)
{
	ASSERT(handleStr != NULL);
	ASSERT(db != NULL);

	AcDbHandle objHandle(handleStr);
	Acad::ErrorStatus es;

	es = db->getAcDbObjectId(objId, false, objHandle);
	return es;
}

int JHCOM_SetEntityType(AcDbObjectId id, CString type)
{
	AcDbEntity *pEnt = NULL; 
	bool is = false;
	acdbOpenAcDbEntity(pEnt, id, AcDb::kForWrite);
	if (pEnt)
	{
		pEnt->setLinetype(type);
		pEnt->close();
	}
	
	return 0;
}

int MD2010_OpenAndCloseRecord(const WCHAR * blockname)
{
	AcDbObjectId blkDefId = MD2010_GetBlockDefID(blockname);
	if (blkDefId == 0) return 0;

	int count = 0;
	AcDbBlockTableRecord *pBlkDefRcd;
	acdbOpenObject(pBlkDefRcd, blkDefId, AcDb::kForWrite);

	pBlkDefRcd->close();

	return 0;
}

int TYCOM_FlushBlockDisplay(AcDbObjectId blkId)
{
	AcDbEntity *pEnt = 0;
	acDocManager->lockDocument(curDoc());
	if (acdbOpenObject(pEnt,blkId,AcDb::kForWrite)==Acad::eOk)
	{
		pEnt->recordGraphicsModified();
		pEnt->downgradeOpen();
		pEnt->draw();
		pEnt->close();
	}
	acDocManager->unlockDocument(curDoc());
	return 0;
}
