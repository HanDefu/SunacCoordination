#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_PostToModelSpace
* Description     : This function post one AcDbEntity to cad database.
* Note            : NULL.
* Input           : pEnt : AcDbEntity to post in.   
* Output          : NULL.
* Return          : id : Object id returned by database.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_PostToModelSpace(AcDbEntity* pEnt, CString entry = ACDB_MODEL_SPACE);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DeleteCadObject
* Description     : This function delete one cad object from cad database.
* Note            : NULL.
* Input           : id : Object to delete.   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_DeleteCadObject(AcDbObjectId id);
void              JHCOM_DeleteCadObjectArray(AcDbObjectIdArray ids);

//Query functions
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetStartPoint
* Description     : This function get start point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get start point.
* Output          : NULL.
* Return          : Start point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_GetStartPoint(AcDbObjectId curveID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEndPoint
* Description     : This function get end point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get end point.
* Output          : NULL.
* Return          : end point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_GetEndPoint(AcDbObjectId curveID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveStart
* Description     : This function get the start point of a curve. 
* Note            : NULL.
* Input           : pCurve-curve to get start point.
* Output          : NULL.
* Return          : Start point.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_GetCurveStart(AcGeCurve3d* pCurve);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveEnd
* Description     : This function get the end point of a curve. 
* Note            : NULL.
* Input           : pCurve-curve to get end point.
* Output          : NULL.
* Return          : End point.
--------------------------------------------------------------------------*/
AcGePoint3d JHCOM_GetCurveEnd(AcGeCurve3d* pCurve);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetMidPoint
* Description     : This function get mid point of a curve.
* Note            : NULL.
* Input           : curveID : curve to get mid point.
* Output          : NULL.
* Return          : mid point of curve.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_GetMidPoint(AcDbObjectId curveID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveStartPointVector
* Description     : This function get a AcGeVector3d from the start point of a curve.
* Note            : This AcGeVector3d start from start point and point to the tangent direction
*                   of this start point, for one line, just from start point to end point.
* Input           : curveId : curveId to get start AcGeVector3d.
* Output          : NULL.
* Return          : start AcGeVector3d of input curve.
--------------------------------------------------------------------------*/
AcGeVector3d      JHCOM_GetCurveStartPointVector(AcDbObjectId curveId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLineStartPoint
* Description     : This function set the start point of a line.
* Note            : NULL.
* Input           : lineid - Line id to set start point.
*                   start - start point to set.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetLineStartPoint(AcDbObjectId lineid,AcGePoint3d start);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLineEndPoint
* Description     : This function set the end point of a line.
* Note            : NULL.
* Input           : lineid : Line id to set end point.
*                   end - end point to set.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetLineEndPoint(AcDbObjectId id,AcGePoint3d end);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLineStartEndPoint
* Description     : This function set the start and end point of a line.
* Note            : NULL.
* Input           : lineid : Line id to set end point.
*                   start - start point to set.
*                   end - end point to set.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetLineStartEndPoint(AcDbObjectId id,AcGePoint3d start,AcGePoint3d end);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLineVector
* Description     : This function get a AcGeVector3d from line start point to line end point.
* Note            : NULL.
* Input           : lineid : Line to get  AcGeVector3d.
* Output          : NULL.
* Return          : Line's AcGeVector3d.
--------------------------------------------------------------------------*/
AcGeVector3d      JHCOM_GetLineVector(AcDbObjectId lineid);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveLength
* Description     : This function get the length of a curve.
* Note            : NULL.
* Input           : curveId : curveId to get length.
* Output          : NULL.
* Return          : length of input curve.
--------------------------------------------------------------------------*/
double            JHCOM_GetCurveLength(AcDbObjectId curveId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurveLength
* Description     : This function get the length of a curve.
* Note            : NULL.
* Input           : pCurve : pointer to curve.
* Output          : NULL.
* Return          : length of input curve.
--------------------------------------------------------------------------*/
double            JHCOM_GetCurveLength(AcGeCurve3d* pCurve);

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
int               JHCOM_ExtendLine(AcDbObjectId line, bool extendStart, bool extendEnd, double valueStart, double valueEnd);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsLine
* Description     : Check whether input id is a line or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is Line : true, is not line : false.
--------------------------------------------------------------------------*/
bool              JHCOM_IsLine(AcDbObjectId id); 

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsArc
* Description     : Check whether input id is a arc or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is arc : true, is not arc : false.
--------------------------------------------------------------------------*/
bool              JHCOM_IsArc(AcDbObjectId id); 

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsCircle
* Description     : Check whether input id is a Circle or not.
* Note            : NULL.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is Circle : true, is not Circle : false.
--------------------------------------------------------------------------*/
bool              JHCOM_IsCircle(AcDbObjectId id); 

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsLineSimilar
* Description     : Check whether input id is similar to a line.
* Note            : This function need more optimization.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is similar to Line : true, is not similar to Line : false.
--------------------------------------------------------------------------*/
bool              JHCOM_IsLineSimilar(AcDbObjectId id); 

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsArcSimilar
* Description     : Check whether input id is similar to a arc.
* Note            : This function need more optimization.
* Input           : id :  Id to check.
* Output          : NULL.
* Return          : Is similar to arc : true, is not similar to arc : false.
--------------------------------------------------------------------------*/
bool              JHCOM_IsArcSimilar(AcDbObjectId id, double arcRadTol = 100); 

//Geometry Compare functions
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
int               JHCOM_GetTwoLineStatus(AcGePoint3d a, AcGePoint3d b, AcGePoint3d c, AcGePoint3d d);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetTwoLineStatus
* Description     : This function help to check the 3d status of two lines.
* Note            : NULL.
* Input           : line1 :  id of first line.
*                   line2 :  id second line.
* Output          : NULL.
* Return          : 0 same line 1 parallal, 2 X.
--------------------------------------------------------------------------*/
int               JHCOM_GetTwoLineStatus(AcDbObjectId line1, AcDbObjectId line2);


//layer operation
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetEntityColor
* Description     : This function set color of entity by color index.
* Note            : NULL.
* Input           : entId :  Entity to set color.
*                   colorIndex :  color index to set to entity.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetEntityColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityColor
* Description     : This function get color of entity by color index.
* Note            : NULL.
* Input           : entId :  Entity to get color.
* Output          : NULL.
* Return          : color index of entity..
--------------------------------------------------------------------------*/
Adesk::UInt16     JHCOM_GetEntityColor(AcDbObjectId entId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateText
* Description     : This function create a text in the specify position.
* Note            : This is old code has not been refined.
* Input           : ptInsert : text position.
*                   text : text string.
* Output          : NULL.
* Return          : Created text id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_CreateText(const AcGePoint3d &ptInsert, const wchar_t *text);

AcDbObjectId      JHCOM_CreateMText(const AcGePoint3d &ptInsert, const wchar_t *text, int color);

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
AcDbObjectId JHCOM_CreateText(const AcGePoint3d &ptInsert,const AcGeVector3d &normal, double height, double angle, const wchar_t *text);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateLine
* Description     : This function create a line with given start and end point.
* Note            : NULL.
* Input           : ptStart :  Start point of line.
*                   ptEnd : End point of line.
* Output          : NULL.
* Return          : Created line id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd);


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
AcDbObjectId      JHCOM_CreateLine(AcGePoint3d ptStart,AcGePoint3d ptEnd,Adesk::UInt16 color);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Create3dPolyline
* Description     : This function create a AcDb3dPolyline with points.
* Note            : NULL.
* Input           : points : Points to create AcDb3dPolyline.
*                   type-polyline type
* Output          : NULL.
* Return          : Created 3dPolyline id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_Create3dPolyline(AcGePoint3dArray points,AcDb::Poly3dType type=AcDb::k3dSimplePoly);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Create3dPolyline
* Description     : This function create a AcDb3dPolyline with points.
* Note            : NULL.
* Input           : points - Points to create AcDb3dPolyline.
*                   color - Color of  this polyline.
*                   type-polyline type
* Output          : NULL.
* Return          : Created 3dPolyline id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_Create3dPolyline(AcGePoint3dArray points, Adesk::UInt16 color,AcDb::Poly3dType type=AcDb::k3dSimplePoly);

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
AcDbObjectId      JHCOM_CreateArc(const AcGePoint3d &center, AcGeVector3d &normal, 
					             double radius, double startangle, double endangle);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateRegionFromCurve
* Description     : This function create a AcDbRegion from input AcDbCurve.
* Note            : NULL.
* Input           : ploy : input polyline to create region.
* Output          : NULL.
* Return          : AcDbRegion pointer.
--------------------------------------------------------------------------*/
AcDbRegion *      JHCOM_CreateRegionFromCurve(AcDbCurve * ploy);

//Other functions
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ChangeObjectColor
* Description     : This function set a color to object by color index.
* Note            : NULL.
* Input           : entId : Object  to set color.
*                   colorIndex : colorIndex to set on entId
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ChangeObjectColor(AcDbObjectId entId, Adesk::UInt16 colorIndex);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ChangeObjectColor
* Description     : This function set a color to object by AcCmColor.
* Note            : NULL.
* Input           : entId : Object  to set color.
*                   color : color to set on entId
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ChangeObjectColor(AcDbObjectId entId, const AcCmColor& color);

int JHCOM_GetObjectColor(AcDbObjectId entId, AcCmColor &color);
int JHCOM_GetObjectColor(AcDbObjectId entId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCenterLine
* Description     : This function get centerline id for a jhsolid.
* Note            : NULL.
* Input           : objId : JH Object id to get center line.
*                   lineobjId : Center line id.
* Output          : NULL.
* Return          : Dose center line get successfully?
--------------------------------------------------------------------------*/
bool              JHCOM_GetCenterLine(AcDbObjectId objId,AcDbObjectId& lineobjId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Explode
* Description     : This function explode one object and store the exploded objects in entitySet.
* Note            : NULL.
* Input           : id : Object to explode.
* Output          : entitySet : Exploded objects.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_Explode(AcDbObjectId id, AcDbVoidPtrArray &entitySet);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Explode
* Description     : This function explode one object (pointer) and store the exploded objects in entitySet.
* Note            : NULL.
* Input           : pEnt : Object pointer to explode.
* Output          : entitySet : Exploded objects.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_Explode(AcDbEntity *pEnt, AcDbVoidPtrArray &entitySet);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateGroup
* Description     : This function create a group with given ids.
* Note            : This is old code has not been refined.
* Input           : objIds :  Object ids to created group.
*                   pGroupName : Created group name.
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_CreateGroup(AcDbObjectIdArray& objIds, wchar_t* pGroupName, AcDbObjectId &groupId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetGroupObject
* Description     : This function get the objects from a input group id.
* Note            : NULL.
* Input           : groupId-Group id to get objects.
*                   ids- out, the object ids getted from group.
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
int               JHCOM_GetGroupObject(AcDbObjectId groupId, AcDbObjectIdArray &ids);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetObjectIDFromAcDbHandle
* Description     : This function get object id from the input object handle.
* Note            : NULL.
* Input           : Handle- Handle id to get object id.
*                   Id- object id, out.
* Output          : NULL.
* Return          : Error status.
--------------------------------------------------------------------------*/
bool              JHCOM_GetObjectIDFromAcDbHandle(AcDbHandle Handle,AcDbObjectId& Id);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_handleStrToObjId
* Description     : This function convert one handle str to a object id.
* Note            : NULL.
* Input           : db :CAD database. 
*                   handleStr :Handle str. 
*                   errorSpeak : print error or not.
* Output          : objId : Converted object id.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_handleStrToObjId(AcDbDatabase* db, LPCTSTR handleStr, AcDbObjectId& objId, bool errorSpeak);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetNextGroupName
* Description     : This function get the group names from the cad group dictory
*                   and then get the next name with method JHGROUP_XX.
* Note            : NULL.
* Input           : NULL.
* Output          : NULL.
* Return          : Next group name.
--------------------------------------------------------------------------*/
CString           JHCOM_GetNextGroupName();

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_HilightObject
* Description     : This function set highlight or not to an object.
* Note            : NULL.
* Input           : objId-object to set highlight.
*                   highlight-Highlight or not
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_HilightObject(const AcDbObjectId& objId, bool highlight);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_HilightGroup
* Description     : This function set highlight or not to a group.
* Note            : NULL.
* Input           : grpId-Group to set highlight.
*                   highlight-Highlight or not
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_HilightGroup(const AcDbObjectId& grpId, bool highlight);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DeleteGroup
* Description     : This function delete a group and sub children.
* Note            : NULL.
* Input           : grpId-Group to delete.
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_DeleteGroup(const AcDbObjectId& grpId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsObjectShown
* Description     : This function help to check whether an object is shown or not.
* Note            : NULL.
* Input           : objId-Object to check shown status.
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
bool JHCOM_IsObjectShown(const AcDbObjectId& objId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ShowObject
* Description     : This function set show status of an object.
* Note            : NULL.
* Input           : objId-Object to set show status.
*                   show - show or not.
*                   refresh - regfresh the graphics immediately or not.
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_ShowObject(const AcDbObjectId& objId, bool show, bool refresh = false);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ShowGroup
* Description     : This function set show status of a group.
* Note            : NULL.
* Input           : objId-Group to set show status.
*                   show - show or not.
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_ShowGroup(const AcDbObjectId& objId, bool show);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CheckPointWithRect
* Description     : In the mid xy plane, there is a rect with width and height, and r,
*                   then this function help to check whether this pnt is in the rect. 
* Note            : pnt - pnt in xy plane.
*                   widX - rect width.
*                   heiy - rect height.
*                   r - rad of the rect.
* Input           : objId-Object to check.
* Output          : NULL.
* Return          : 0 on, 1 out, -1 in.
--------------------------------------------------------------------------*/
int JHCOM_CheckPointWithRect(AcGePoint3d pnt, double widX, double heiY, double r);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsPointOnRect
* Description     : Similar to JHCOM_CheckPointWithRect, this function help to check if the pnt
*                   in one rect or not. 
* Note            : pnt - pnt in xy plane.
*                   widX - rect width.
*                   heiy - rect height.
*                   r - rad of the rect.
* Input           : objId-Object to check.
* Output          : NULL.
* Return          : true-in, false-out.
--------------------------------------------------------------------------*/
bool JHCOM_IsPointOnRect(AcGePoint3d pnt, double widX, double heiY, double r);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IsPointOnCircle
* Description     : Similar with JHCOM_IsPointOnRect, this function help to check if the input
*                   pnt on a circle or not, r is the radius. 
* Note            : NULL.     
* Input           : pnt - pnt in xy plane.
*                   objId-Object to check.
*                   r - rad of the circle.
* Output          : NULL.
* Return          : true-in, false-out.
--------------------------------------------------------------------------*/
bool JHCOM_IsPointOnCircle(AcGePoint3d pnt, double r);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetObjectMinMaxPoint
* Description     : This function get the min and max point of the input object, with function getGeomExtents. 
* Note            : NULL.
* Input           : entId - Entity to get min and max point.
* Output          : minPt-Min point of the bounding rect.
*                 : minPt-Min point of the bounding rect.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_GetObjectMinMaxPoint(AcDbObjectId entId, AcGePoint3d &minPt, AcGePoint3d &maxPt);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ZoomToEntity
* Description     : This function use command method to zoom the view to an object. 
* Note            : NULL.
* Input           : objId - Entity to zoom to.
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_ZoomToEntity(const AcDbObjectId& objId);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_Get3dPolylinePoints
* Description     : This function get the min and max point of the input object, with function getGeomExtents. 
* Note            : NULL.
* Input           : id - AcDb3dPolyline object id.
* Output          : vpoints-pnts of this polyline.
* Return          : 0-successful, other-fail.
--------------------------------------------------------------------------*/
int    JHCOM_Get3dPolylinePoints(AcDbObjectId id, vAcGePoint3d &vpoints);

int JHCOM_SetEntityType(AcDbObjectId id, CString type);

int MD2010_OpenAndCloseRecord(const WCHAR * blockname);

//单个刷新对象（特别是图块）的显示，不用整个发送regen命令或者flush了
int TYCOM_FlushBlockDisplay(AcDbObjectId blkId);

