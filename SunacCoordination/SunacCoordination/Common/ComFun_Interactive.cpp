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
#include "ComFun_Def.h"

using namespace std;

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectJHSolidOne
* Description     : This function let user select one JHSolid from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : NULL.
* Return          : JHSolid id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_SelectJHSolidOne()
{
	ads_name ssname;
	AcDbObjectId objId = 0;

	int rt = acedSSGet(L"_+.:S",NULL,NULL,NULL,ssname); 
	if (rt == RTNORM)
	{
		ads_name ent;
		acedSSName(ssname, 0, ent);
		acdbGetObjectId(objId, ent);
		acedSSFree(ssname);
		if (objId == NULL)
		{
			return NULL;
		}
		return objId;
	}
	else
	{
		return NULL;
	}
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectSolidOne
* Description     : This function let user select one AcDb3dSolid from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : NULL.
* Return          : AcDb3dSolid id.
--------------------------------------------------------------------------*/
AcDbObjectId JHCOM_SelectSolidOne()
{
	ads_name ssname;
	AcDbObjectId objId = 0;

	int rt = acedSSGet(L"_+.:S",NULL,NULL,NULL,ssname); 
	if (rt == RTNORM)
	{
		ads_name ent;
		acedSSName(ssname, 0, ent);
		acdbGetObjectId(objId, ent);
		acedSSFree(ssname);
		if (objId == NULL)
		{
			return NULL;
		}

		AcDbEntity *pEnt  = NULL;
		acdbOpenObject(pEnt, objId, AcDb::kForRead);
		AcDb3dSolid* pSolidMain = dynamic_cast<AcDb3dSolid*>(pEnt);

		if(pSolidMain == NULL)
		{
			pEnt->close();
			return NULL;
		}
		pEnt->close();
		return objId;
	}
	else
	{
		return NULL;
	}
}



/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectSolidMany
* Description     : This function let user select several AcDb3dSolids from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : AcDb3dSolid ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_SelectSolidMany(vAcDbObjectId &ids)
{
	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	struct resbuf* rb;
	rb = acutBuildList(RTDXF0, L"*SOLID", RTNONE);
	int rt = acedSSGet(NULL, NULL, NULL, rb, ssname); // 提示用户选择对象
	if (rt == RTNORM)
	{
		Adesk::Int32 length;
		acedSSLength(ssname, &length);
		for (int i=0;i<length;i++)
		{
			acedSSName(ssname,i, ent);
			acdbGetObjectId(id, ent);
			ids.push_back(id);
		}
		acedSSFree(ssname);
	}
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectJHSolidMany
* Description     : This function let user select several JHSolids from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : JHSolid ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_SelectJHSolidMany(vAcDbObjectId &ids)
{
	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	struct resbuf* rb;
	rb = acutBuildList(RTDXF0, L"*SOLID", RTNONE);
	int rt = acedSSGet(NULL, NULL, NULL, rb, ssname); // 提示用户选择对象
	if (rt == RTNORM)
	{
		Adesk::Int32 length;
		acedSSLength(ssname, &length);
		for (int i=0;i<length;i++)
		{
			acedSSName(ssname,i, ent);
			acdbGetObjectId(id, ent);
				ids.push_back(id);
		}
		acedSSFree(ssname);
	}
}

/*-------------------------------------------------------------------------
* Function Name   : JHCOM_SelectCenterLines
* Description     : This function let user select several lines,arcs and cycles from the cad interface.
* Note            : NULL.
* Input           : NULL.   
* Output          : ids : selected ids.
* Return          : NULL.
--------------------------------------------------------------------------*/
void JHCOM_SelectCenterLines(vAcDbObjectId &ids)
{
	ads_name ssname;
	struct resbuf* rb;
	rb = acutBuildList(RTDXF0, "LINE", RTNONE);
	int rt = acedSSGet( NULL, NULL, NULL, NULL, ssname);
	
	if (rt == RTNORM)
	{
		Adesk::Int32 length = 0;
		acedSSLength(ssname, &length); // 获得选择集中的对象个数
		for (int i = 0; i < length; i++)
		{
			ads_name ent;
			acedSSName(ssname, i, ent);
			AcDbObjectId objId;
			acdbGetObjectId(objId, ent);
			//排除spline pline eyellipse等等
			if(JHCOM_IsLine(objId) || JHCOM_IsArc(objId) || JHCOM_IsCircle(objId))
			    ids.push_back(objId);
		}
		acedSSFree(ssname);
	}
}

void JHCOM_SelectEnts(vAcDbObjectId &ids, const ACHAR* pstr/* = NULL*/)
{
	AcDbObjectId id;
	ads_name ssname;
	ads_name ent;
	//struct resbuf* rb;
	//rb = acutBuildList(RTDXF0, L"*SOLID", RTNONE);
	int rt = acedSSGet(pstr, NULL, NULL, NULL, ssname); // 提示用户选择对象
	if (rt == RTNORM)
	{
		long length;
		acedSSLength(ssname, &length);
		for (int i=0;i<length;i++)
		{
			acedSSName(ssname,i, ent);
			acdbGetObjectId(id, ent);
			ids.push_back(id);
		}
		acedSSFree(ssname);
	}
}

