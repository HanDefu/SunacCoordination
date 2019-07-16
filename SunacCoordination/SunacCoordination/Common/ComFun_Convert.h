#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcharToChar
* Description     : This function convert wchar_t string to char string.
* Note            : Returned value should be freed by free();.
* Input           : pchar : wchar_t string to convert.
*                   
* Output          : NULL.
* Return          : char string pointer.
--------------------------------------------------------------------------*/
CHAR*             JHCOM_WcharToChar(const WCHAR * pchar);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CharToWchar
* Description     : This function convert char string to wchar_t string.
* Note            : Returned value should be freed by delete;.
* Input           : pchar : char string to convert.
*                   
* Output          : NULL.
* Return          : wchar_t string pointer.
--------------------------------------------------------------------------*/
WCHAR*            JHCOM_CharToWchar(const char * pchar);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_AdsToAcGe
* Description     : This function convert ads_point to AcGePoint3d.
* Note            : NULL.
* Input           : pts : ads_point to convert.
*                   pt : AcGePoint3d to converted to.                   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_AdsToAcGe(ads_point& pts,AcGePoint3d &pt);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_AdsToAcGe
* Description     : This function convert AcGePoint3d to ads_point.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.
*                   pts : ads_point to converted to.                   
* Output          : NULL.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_AcGeToads(AcGePoint3d &pt,ads_point& pts);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DoubleToStr
* Description     : This function convert double to CString with 3 length of decimals.
* Note            : NULL.
* Input           : value : double value to convert.       
* Output          : NULL.
* Return          : CString value of converted.
--------------------------------------------------------------------------*/
CString           JHCOM_DoubleToStr(double value);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_IntToStr
* Description     : This function convert int to CString.
* Note            : NULL.
* Input           : value : int value to convert.       
* Output          : NULL.
* Return          : CString value of converted.
--------------------------------------------------------------------------*/
CString           JHCOM_IntToStr(int value);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcsToUcs
* Description     : This function convert one AcGePoint3d point from wcs to ucs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_WcsToUcs(AcGePoint3d &pt);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_WcsToUcs
* Description     : This function convert one AcGeVector3d point from wcs to ucs.
* Note            : NULL.
* Input           : pt : AcGeVector3d to convert.       
* Output          : NULL.
* Return          : AcGeVector3d value of converted.
--------------------------------------------------------------------------*/
AcGeVector3d      JHCOM_WcsToUcs(AcGeVector3d &vec);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToWcs
* Description     : This function convert one AcGePoint3d point from ucs to wcs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_UcsToWcs(AcGePoint3d &pt);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToWcs
* Description     : This function convert one AcGePoint2d point from ucs to wcs.
* Note            : NULL.
* Input           : pt : AcGePoint2d to convert.       
* Output          : NULL.
* Return          : AcGePoint2d value of converted.
--------------------------------------------------------------------------*/
AcGePoint2d       JHCOM_UcsToWcs(AcGePoint2d &pt);


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToWcs
* Description     : This function convert one AcGeVector3d point from ucs to wcs.
* Note            : NULL.
* Input           : pt : AcGeVector3d to convert.       
* Output          : NULL.
* Return          : AcGeVector3d value of converted.
--------------------------------------------------------------------------*/
AcGeVector3d      JHCOM_UcsToWcs(AcGeVector3d &vec);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_UcsToEcs
* Description     : This function convert one AcGePoint3d point from ucs to ecs.
* Note            : NULL.
* Input           : pt : AcGePoint3d to convert.       
* Output          : NULL.
* Return          : AcGePoint3d value of converted.
--------------------------------------------------------------------------*/
AcGePoint3d       JHCOM_UcsToEcs(AcGePoint3d pt, AcGeVector3d normal);

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
int JHCOM_GetAcDbHandle(AcDbObjectId Id, AcDbHandle &handle);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ConvertDbCurveToGeCurve
* Description     : This function convert AcDbPolyline to AcGeCurve2d.
* Note            : This function get from object arx autodesk form.
* Input           : pPoly : AcDbPolyline to get AcGeCurve2d.
* Output          : pGeCurve : AcGeCurve2d for AcDbCurve.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve2d*& pGeCurve);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ConvertDbCurveToGeCurve
* Description     : This function convert AcDbPolyline to AcGeCurve3d.
* Note            : This function get from object arx autodesk form.
* Input           : pPoly : AcDbPolyline to get AcGeCurve3d.
* Output          : pGeCurve : AcGeCurve3d for AcDbCurve.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_convertPolylineToGeCurve(AcDbPolyline* pPoly, AcGeCurve3d*& pGeCurve);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_ConvertDbCurveToGeCurve
* Description     : This function convert AcDbCurve to AcGeCurve3d.
* Note            : This function get from object arx autodesk form.
* Input           : pDbCurve : AcDbCurve to get AcGeCurve3d.
* Output          : pGeCurve : AcGeCurve3d for AcDbCurve.
* Return          : Error status.
--------------------------------------------------------------------------*/
Acad::ErrorStatus JHCOM_ConvertDbCurveToGeCurve(AcDbCurve *pDbCurve,AcGeCurve3d *&pGeCurve);

AcDbObjectId JHCOM_GetConvertGeCurveToAcDbCurve(AcGeCurve3d &geCurve3d);