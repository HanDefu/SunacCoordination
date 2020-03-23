#pragma once
#include "ComFun_Def.h"

#define  DQ_DEBUG

int              MD2010_SetCurrentLayout(const ACHAR * layoutname);
const ACHAR *    MD2010_GetCurrentLayout();

int              MD2010_InsertBlockFromPathName(const WCHAR *layoutname, const WCHAR *pathname, CString &blockDefineName, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, int color = -2);

AcDbObjectId     MD2010_InsertBlockDefineFromPathName(const WCHAR *pathname, CString blockName);
int              MD2010_InsertBlockReference_Layout(const WCHAR * layoutname, const WCHAR * blockname, AcDbObjectId &entId, AcGePoint3d origin, double angle, AcGeScale3d scale, int color = -2);

void AppendAttributeToBlkRef(AcDbBlockReference * pBlkRef, AcDbAttributeDefinition * pAttDef);




