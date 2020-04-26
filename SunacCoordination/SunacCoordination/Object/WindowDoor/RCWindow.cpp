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
	AcDbBlockTableRecord *pBlkDefine = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pBlkDefine, BlockDefineId, AcDb::kForWrite);
	if (es!=Acad::eOk)
	{
		return;
	}

	AcDbBlockTableRecordIterator *BlkDefineIt = NULL;
	pBlkDefine->newIterator(BlkDefineIt);
	for ( ;!BlkDefineIt->done(); BlkDefineIt->step())
	{
		AcDbEntity *pEntity=NULL;
		es = BlkDefineIt->getEntity(pEntity, AcDb::kForWrite);
		if (es!=Acad::eOk || pEntity==NULL)
		{
			assert(false);
			continue;
		}

		CString LayerName = pEntity->layer();

		if (LayerName == GSINST->m_winSetting.GetWinFrameLayerDefault())
		{
			if (GSINST->m_winSetting.m_sWinFrameLayer != GSINST->m_winSetting.GetWinFrameLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinFrameLayer);
				pEntity->setLayer(GSINST->m_winSetting.m_sWinFrameLayer);
			}
		}
		else if (LayerName == GSINST->m_winSetting.GetWinHardwareLayerDefault())
		{
			if (GSINST->m_winSetting.m_sWinHardwareLayer != GSINST->m_winSetting.GetWinHardwareLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinHardwareLayer);
				pEntity->setLayer(GSINST->m_winSetting.m_sWinHardwareLayer);
			}
		}
		else if (LayerName == GSINST->m_winSetting.GetWinOpenLayerDefault())
		{
			if (GSINST->m_winSetting.m_sWinOpenLayer != GSINST->m_winSetting.GetWinOpenLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinOpenLayer);
				pEntity->setLayer(GSINST->m_winSetting.m_sWinOpenLayer);
			}
		}
		else if (LayerName == GSINST->m_winSetting.GetWinWallLayerDefault())
		{
			if (GSINST->m_winSetting.m_sWinWallLayer != GSINST->m_winSetting.GetWinWallLayerDefault())
			{
				JHCOM_CreateNewLayer(GSINST->m_winSetting.m_sWinWallLayer);
				pEntity->setLayer(GSINST->m_winSetting.m_sWinWallLayer);
			}
		}

		pEntity->close();
	}

	pBlkDefine->close();
}

AcDbObjectId RCWindow::Insert(CString fileName, AcGePoint3d origin, double angle, CString layerName, int color)
{
	m_blockRecordName = FilePathToFileNameWithoutExtension(fileName);

	CDocLock doclock;
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

	ModifyLayerName(BlockDefineId);

	MD2010_InsertBlockReference_Layout(ACDB_MODEL_SPACE, m_blockRecordName, m_id, origin, angle, AcGeScale3d(1), color);
	MD2010_SetCurrentLayer(name);
	return m_id;
}