#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >


//Interactive functions
/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectJHSolidOne
* Description     : This function let user select one JHSolid from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : NULL.
* Return          : JHSolid id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_SelectJHSolidOne();

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectJHSolidMany
* Description     : This function let user select several JHSolids from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : JHSolid ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_SelectJHSolidMany(vAcDbObjectId &ids);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectSolidOne
* Description     : This function let user select one AcDb3dSolid from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : NULL.
* Return          : AcDb3dSolid id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_SelectSolidOne();

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectSolidMany
* Description     : This function let user select several AcDb3dSolids from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : AcDb3dSolid ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_SelectSolidMany(vAcDbObjectId &ids);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectCenterLines
* Description     : This function let user select several lines,arcs and cycles from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : selected ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void              JHCOM_SelectCenterLines(vAcDbObjectId &ids);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetString
* Description     : This function let user input one string from the cad interface.
* Note            : NULL.
* Input           : prompt : prompt to tell user for input.
*                   str : string input by user.
* Output          : NULL.
* Return          : Has input : true, no input : false.
--------------------------------------------------------------------------*/
bool              JHCOM_GetString(CString prompt, CString &str);
