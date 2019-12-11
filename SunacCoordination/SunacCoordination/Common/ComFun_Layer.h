#pragma once

#include <vector>
#include <gecomp3d.h>
#include <brshell.h>
#include <brcplx.h >


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetEntityLayer
* Description     : This function set layer of entity if layer exist.
* Note            : If layer dose not exist, this function will return error.
* Input           : entId :  Entity to set layer.
*                   strLayerName :  layer name to set to entity.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetEntityLayer(AcDbObjectId objID, CString strLayerName);


/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetEntityLayer
* Description     : This function set layer of entity if layer exist.
* Note            : If layer dose not exist, this function will return error.
* Input           : entId :  Entity to set layer.
*                   layerID :  layer id.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetEntityLayer(AcDbObjectId objID, AcDbObjectId layerID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityLayer
* Description     : This function get layer name of entity.
* Note            : NULL.
* Input           : objID : Entity to get layer.
* Output          : NULL.
* Return          : String of layer name.
--------------------------------------------------------------------------*/
CString           JHCOM_GetEntityLayer(AcDbObjectId objID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetEntityLayerID
* Description     : This function get layer id of entity.
* Note            : NULL.
* Input           : objID : Entity to get layer id.
* Output          : NULL.
* Return          : layer id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_GetEntityLayerID(AcDbObjectId objID);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_CreateNewLayer
* Description     : This function create a new layer with given new layer name.
* Note            : If layer name has been exist, will return 0.
* Input           : newlayername : new layer name for new created layer.
* Output          : NULL.
* Return          : new created layer id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_CreateNewLayer(CString newlayername,Adesk::UInt16 colorIndex=0);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_DeleteLayer
* Description     : This function delete a layer with given layer name.
* Note            : NULL.
* Input           : layerName : layer to delete.
* Output          : NULL.
* Return          : 0 : successful, 1 : fail.
--------------------------------------------------------------------------*/
int               JHCOM_DeleteLayer(CString layerName);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLayerID
* Description     : This function get layer id for a given layer name.
* Note            : NULL.
* Input           : layerName : layer name to get id.
* Output          : NULL.
* Return          : id of given layer name.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_GetLayerID(CString layerName);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLayerName
* Description     : This function get layer name of a given layer id.
* Note            : if this is not a layer id, return "".
* Input           : id : layer id to get layer name.
* Output          : NULL.
* Return          : layer name of layer id.
--------------------------------------------------------------------------*/
CString           JHCOM_GetLayerName(AcDbObjectId id);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetAllLayers
* Description     : This function get all layer names and ids of current database.
* Note            : NULL.
* Input           : NULL.
* Output          : vstr : all layer names.
*                   vids : all layer ids with same index of vstr
* Return          : layer size.
--------------------------------------------------------------------------*/
int               JHCOM_GetAllLayers(vCString &vstr, vAcDbObjectId &vids);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetLayerCount
* Description     : This function get layer count of current database.
* Note            : NULL.
* Input           : NULL.
* Output          : NULL.
* Return          : layer size.
--------------------------------------------------------------------------*/
int               JHCOM_GetLayerCount();

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetCurrentLayer
* Description     : This function set current layer by layer name.
* Note            : NULL.
* Input           : name : layer name to set to current layer.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetCurrentLayer(CString name);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_GetCurrentLayer
* Description     : This function get current layer name and id.
* Note            : NULL.
* Input           : name : layer name of current layer.
* Output          : NULL.
* Return          : current layer id.
--------------------------------------------------------------------------*/
AcDbObjectId      JHCOM_GetCurrentLayer(CString &name);

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SetLayerColor
* Description     : This function set layer color for given layer name.
* Note            : NULL.
* Input           : layerName : layer name to set to color.
*                   newcolor : new color to set to layer.
* Output          : NULL.
* Return          : Error number.
--------------------------------------------------------------------------*/
int               JHCOM_SetLayerColor(CString layerName,AcCmColor newcolor);

int               JHCOM_SetLayerPrint(CString layerName, bool isPrint = false);