/*==============================================================================

        Copyright (c) 2018 Shang Hai Tu Yuan Inc. 
            Unpublished - All rights reserved

================================================================================
File description:

    This implementation file of class RCWindow.

================================================================================
   Date      Name                    Description of Change
16-July-2019  Han Defu                Create.
==============================================================================*/

#include "StdAfx.h"
#include "RCWindow.h"
#include <float.h>
#include <algorithm>
#include <vector>
#include "../../Common/ComFun_Sunac.h"
#include "../../Common/ComFun_ACad.h"
#include "../../Common/ComFun_Str.h"
#include "../../Common/ComFun_ACAD_Common.h"
#include "../../Common/ComFun_String.h"
#include "../../Common/ComFun_Layer.h"
#include "../../Tool/DocLock.h"
#include "../../GlobalSetting.h"

//Constructor
RCWindow::RCWindow(void)
{
	m_pAttribute = 0;
}

//Destructor
RCWindow::~RCWindow(void)
{

}


//Constructor
RCWindow::RCWindow(const RCWindow &other):RCDynamicBlock(other)
{
	m_pAttribute = other.m_pAttribute;
}

//Operator = 
RCWindow & RCWindow::operator=(const RCWindow &other)
{
	m_pAttribute = other.m_pAttribute;
	return *this;
}


void RCWindow::Draw()
{
}

RCWindow * RCWindow::Clone()
{
	RCWindow * newObj = new RCWindow;

	return newObj;
}

double RCWindow::GetH()
{
	double value = 0;
	GetParameter(L"H",value);
	return value;
}
double RCWindow::GetH1()
{
	double value = 0;
	GetParameter(L"H1",value);
	return value;
}
double RCWindow::GetH2()
{
	double value = 0;
	GetParameter(L"H2",value);
	return value;
}
double RCWindow::GetW()
{
	double value = 0;
	GetParameter(L"W",value);
	return value;
}
double RCWindow::GetW1()
{
	double value = 0;
	GetParameter(L"W1",value);
	return value;
}
double RCWindow::GetW2()
{
	double value = 0;
	GetParameter(L"W2",value);
	return value;
}
double RCWindow::GetW3()
{
	double value = 0;
	GetParameter(L"W3",value);
	return value;
}
double RCWindow::GetA()
{
	double value = 0;
	GetParameter(L"A",value);
	return value;
}

int RCWindow::SetH(double newValue)
{
	return SetParameter(L"H", newValue);
}
int RCWindow::SetH1(double newValue)
{
	return SetParameter(L"H1", newValue);
}
int RCWindow::SetH2(double newValue)
{
	return SetParameter(L"H2", newValue);
}
int RCWindow::SetW(double newValue)
{
	return SetParameter(L"W", newValue);
}
int RCWindow::SetW1(double newValue)
{
	return SetParameter(L"W1", newValue);
}
int RCWindow::SetW2(double newValue)
{
	return SetParameter(L"W2", newValue);
}
int RCWindow::SetW3(double newValue)
{
	return SetParameter(L"W3", newValue);
}
int RCWindow::SetA(double newValue)
{
	return SetParameter(L"A", newValue);
}

AttrWindow * RCWindow::GetAttribute()
{
	if (m_pAttribute == NULL)
	{
		AcDbObject * pDataEnt = 0;
		TY_GetAttributeData(m_id, pDataEnt);
		m_pAttribute = dynamic_cast<AttrWindow *>(pDataEnt);
	}
	return m_pAttribute;
}

void RCWindow::AddAttribute(AttrWindow * attr)
{
	if (attr != 0)
	    TY_AddAttributeData(m_id, attr);
}

bool RCWindow::isEqualTo(RCObject*other)
{
	if (other == 0)
		return false;

	RCWindow * pObj = dynamic_cast<RCWindow *>(other);
	if (pObj == 0)
		return false;

	if (!RCDynamicBlock::isEqualTo(other))
		return false;

	AttrWindow * thisAttr = GetAttribute();
	AttrWindow * otherAttr = pObj->GetAttribute();
	if (!thisAttr->isEqualTo(otherAttr))
		return false;

	return true;
}

//门窗面积
double RCWindow::GetWindowArea()
{
    return GetH() * GetW();
}


void RCWindow::SetInstanceCode(CString str)
{
	GetAttribute()->SetInstanceCode(str);
}
CString RCWindow::GetInstanceCode()
{
	return GetAttribute()->GetInstanceCode();
}

void RCWindow::ModifyLayerName(AcDbObjectId BlockDefineId)
{
	AcDbBlockTable *pBlkTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlkTbl, AcDb::kForWrite);
	AcDbBlockTableRecord *pBlkDefine = NULL;
	acdbOpenObject(pBlkDefine, BlockDefineId, AcDb::kForWrite);
	AcDbBlockTableRecordIterator *BlkDefineIt;
	pBlkDefine->newIterator(BlkDefineIt);
	for (;!BlkDefineIt->done(); BlkDefineIt->step())
	{
		AcDbEntity *Entity;
		BlkDefineIt->getEntity(Entity, AcDb::kForWrite);
		CString LayerName = Entity->layer();
		if (GSINST->m_winSetting.m_sWinFrameLayer != GSINST->m_winSetting.GetWinFrameLayerDefault() ||\
			GSINST->m_winSetting.m_sWinHardwareLayer != GSINST->m_winSetting.GetWinHardwareLayerDefault() ||\
			GSINST->m_winSetting.m_sWinLayer != GSINST->m_winSetting.GetWinLayerDefault() ||\
			GSINST->m_winSetting.m_sWinNumberLayerLimian != GSINST->m_winSetting.GetWinNumberLayerLimianDefault() ||\
			GSINST->m_winSetting.m_sWinNumberLayerPingmian != GSINST->m_winSetting.GetWinNumberLayerPingmianDefault() ||\
			GSINST->m_winSetting.m_sWinOpenLayer != GSINST->m_winSetting.GetWinOpenLayerDefault() ||\
			GSINST->m_winSetting.m_sWinWallLayer != GSINST->m_winSetting.GetWinWallLayerDefault())
		{
			if (LayerName == GSINST->m_winSetting.GetWinFrameLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinFrameLayer);
				Entity->setLayer(GSINST->m_winSetting.m_sWinFrameLayer);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinHardwareLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinHardwareLayer);
				Entity->setLayer(GSINST->m_winSetting.m_sWinHardwareLayer);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinLayer);
				Entity->setLayer(GSINST->m_winSetting.m_sWinLayer);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinNumberLayerLimianDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinNumberLayerLimian);
				Entity->setLayer(GSINST->m_winSetting.m_sWinNumberLayerLimian);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinNumberLayerPingmianDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinNumberLayerPingmian);
				Entity->setLayer(GSINST->m_winSetting.m_sWinNumberLayerPingmian);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinOpenLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinOpenLayer);
				Entity->setLayer(GSINST->m_winSetting.m_sWinOpenLayer);
			}
			else if (LayerName == GSINST->m_winSetting.GetWinWallLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinWallLayer);
				Entity->setLayer(GSINST->m_winSetting.m_sWinWallLayer);
			}

		}
		pBlkDefine->close();
	}
}

AcDbObjectId RCWindow::Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	m_blockRecordName = FilePathToFileNameWithoutExtension(fileName);

	acDocManager->lockDocument(curDoc());
	//检查图层是否存在，不存在则创建
	AcDbObjectId layerId = JHCOM_GetLayerID(layerName);
	if (layerId==AcDbObjectId::kNull)
	{
		JHCOM_CreateNewLayer(layerName);
	}

	CString name;
	MD2010_GetCurrentLayer(name);
	MD2010_SetCurrentLayer(layerName);
	AcDbObjectId BlockDefineId = MD2010_InsertBlockDefineFromPathName(fileName,m_blockRecordName);

	//TODO 叶明远
	//ModifyLayerName(BlockDefineId);

	MD2010_InsertBlockReference_Layout(ACDB_MODEL_SPACE, m_blockRecordName, m_id, origin, angle, AcGeScale3d(1), color);
	MD2010_SetCurrentLayer(name);
	acDocManager->unlockDocument(curDoc());
	return m_id;
}