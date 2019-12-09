#include "StdAfx.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "WindowGen.h"
#include "..\..\GlobalSetting.h"


//////////////////////////////////////////////////////////////////////////


CWinTranslationPara CWindowGen::InitTransPara(const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir)
{
	CWinTranslationPara insertPara;
	insertPara.insertPos = pos;
	insertPara.rotateAngle = 0;
	insertPara.bNeedMirror = curWinAtt.IsInstanceNeedMirror();;
	insertPara.mirrorBasePt = AcGePoint3d(pos.x + curWinAtt.GetW() / 2, 0, 0);
	insertPara.mirrorAxis = AcGeVector3d(0, 1, 0);

	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			insertPara.rotateAngle = PI;
			offsetXY.x += curWinAtt.GetW();
			break;
		case E_DIR_RIGHT:
			insertPara.rotateAngle = -PI / 2;
			offsetXY.y += curWinAtt.GetW();
			insertPara.mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			insertPara.mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			insertPara.rotateAngle = PI / 2;
			insertPara.mirrorBasePt = AcGePoint3d(0, pos.y + curWinAtt.GetW() / 2, 0);
			insertPara.mirrorAxis = AcGeVector3d(1, 0, 0);
			break;
		case E_DIR_UNKNOWN:
		default:
			break;
		}

		insertPara.insertPos += offsetXY;
	}

	return insertPara;
}

	//更新参数
void CWindowGen::UpdateRcWindowPara(const AcDbObjectId p_id, const AttrWindow& curWinAtt, eViewDir p_view, bool p_bDetailWnd)
{
	RCWindow oneWindow;
	oneWindow.m_id = p_id;
	oneWindow.InitParameters();

	oneWindow.SetParameter(L"H", curWinAtt.GetH());
	oneWindow.SetParameter(L"W", curWinAtt.GetW());

	if (p_bDetailWnd || GlobalSetting::GetInstance()->m_winSetting.m_bUseAinLimian)
	{
		oneWindow.SetParameter(L"A", curWinAtt.GetA());
	}

	oneWindow.SetParameter(L"D", curWinAtt.GetD());

	oneWindow.SetParameter(L"W1", curWinAtt.GetW1());
	if (curWinAtt.HasValue(_T("W2")))
		oneWindow.SetParameter(L"W2", curWinAtt.GetW2());
	if (curWinAtt.HasValue(_T("W3")))
		oneWindow.SetParameter(L"W3", curWinAtt.GetW3());

	if (curWinAtt.HasValue(_T("H1")))
		oneWindow.SetParameter(L"H1", curWinAtt.GetH1());
	if (curWinAtt.HasValue(_T("H2")))
		oneWindow.SetParameter(L"H2", curWinAtt.GetH2());
	if (curWinAtt.HasValue(_T("H3")))
		oneWindow.SetParameter(L"H3", curWinAtt.GetH3());

	oneWindow.RunParameters();
	//////////////////////////////////////////////////////////////////////////
	//处理可见性
	if (p_view == E_VIEW_FRONT || p_view == E_VIEW_EXTEND)
	{
		DQ_SetDynamicAttribute(oneWindow.m_id, _T("可见性1"), p_bDetailWnd ? _T("详图") : _T("立面"));
	}
}

void CWindowGen::AddWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt)
{
	//把数据记录在图框的扩展字典中
	AttrWindow * pWinAtt = new AttrWindow(p_winAtt);
	TY_AddAttributeData(p_id, pWinAtt);
	pWinAtt->close();
}
void CWindowGen::UpdateWinAtt(const AcDbObjectId p_id, AttrWindow p_winAtt)
{
	//把数据记录在图框的扩展字典中
	AttrWindow * pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		*pWinAtt = p_winAtt;
		pWinAtt->close();
	}
}


AcDbObjectId  CWindowGen::GenerateWindow(AttrWindow curWinAtt, const AcGePoint3d pos,
	E_DIRECTION p_winDir, bool p_bDetailWnd, const AcDbObjectId p_fromWinId, CString p_sLayerName)
{
	eViewDir p_view = curWinAtt.m_viewDir;
	const CWinTranslationPara transPara = InitTransPara(curWinAtt, pos, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, transPara.insertPos, transPara.rotateAngle, p_sLayerName, 256);

	//针对USC坐标处理
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}

	curWinAtt.m_instanceCodeId = InsertWindowDoorCode(curWinAtt.GetW(), curWinAtt.GetH(), pos, curWinAtt.GetInstanceCode(), p_view);

	//////////////////////////////////////////////////////////////////////////
	UpdateRcWindowPara(id, curWinAtt, p_view, p_bDetailWnd);
	AddWinAtt(id, curWinAtt);
	
	return id;
}

AcGePoint3d CWindowGen::GetWindowLeftBottomPos(AcDbObjectId p_id)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况
	return minPt;
}

AcGePoint3d CWindowGen::GetWindowInsertPos(AcDbObjectId p_id)
{
	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk )
	{
	}
	
	AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);
	if (pBlockReference==NULL)
	{
		return AcGePoint3d::kOrigin;
	}

	AcGePoint3d pos = pBlockReference->position();
	pBlockReference->close();
	return pos;
}

E_DIRECTION CWindowGen::GetWindowInsertDir(AcDbObjectId p_id)
{
	E_DIRECTION winDir = E_DIR_TOP; //TODO 自动分析视图方向

	return winDir;
}

CWinInsertPara CWindowGen::GetWindowInsertPara(AcDbObjectId p_id) //根据已插入的门窗获取其插入的信息
{
	CWinInsertPara insertPara;
	insertPara.insertPos = GetWindowInsertPos(p_id);
	insertPara.leftBottomPos = GetWindowLeftBottomPos(p_id);

	insertPara.sLayerName = JHCOM_GetEntityLayer(p_id);
	insertPara.insertDir = GetWindowInsertDir(p_id);

	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL)
	{
		insertPara.viewDir = pWinAtt->m_viewDir;
		insertPara.bDetailWnd = false;
		insertPara.fromWinId = pWinAtt->m_fromWinId;
		insertPara.relatedWinIds = pWinAtt->m_relatedWinIds;
	}
	else
	{
		insertPara.bDetailWnd = true;
		insertPara.viewDir = E_VIEW_EXTEND;
		insertPara.fromWinId = AcDbObjectId::kNull;
	}
	return insertPara;
}

bool CWindowGen::IsWindowMirror(AcDbObjectId p_id) 
{
	return false; //TODO,根据门窗实例判断当前门窗在图上是否镜像
}
AttrWindow* CWindowGen::GetWinAtt(AcDbObjectId p_id)
{
	AcDbObject* pObj = NULL;
	TY_GetAttributeData(p_id, pObj);
	AttrWindow *pWinAtt = dynamic_cast<AttrWindow *>(pObj);

	return pWinAtt;
}
bool CWindowGen::IsPrototypeCodeSame(const AcDbObjectId p_id, const AttrWindow& newWinAtt)
{
	AttrWindow *pOldWinAtt = GetWinAtt(p_id);
	if (pOldWinAtt != NULL)
	{
		pOldWinAtt->close();
		return pOldWinAtt->m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	}
	else
	{
		return true;
	}
}

//只修改尺寸等数据信息，不调整
void CWindowGen::ModifyOneWindow(const AcDbObjectId p_id, AttrWindow newWinAtt)
{
	if (p_id==AcDbObjectId::kNull)
	{
		assert(false);
		return;
	}
	AcDbEntity *pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk || pEnt==NULL)
	{
		return;
	}
	pEnt->close();

	const CWinInsertPara insertPara = GetWindowInsertPara(p_id);
	//以下信息保持和原来的不变
	newWinAtt.m_viewDir = insertPara.viewDir; 
	newWinAtt.m_fromWinId = insertPara.fromWinId;
	newWinAtt.m_relatedWinIds = insertPara.relatedWinIds;
	
	//更新尺寸信息
	UpdateRcWindowPara(p_id, newWinAtt, insertPara.viewDir, insertPara.bDetailWnd);
	UpdateWinAtt(p_id, newWinAtt);

	//更新位置
	const CWinTranslationPara transPara = InitTransPara(newWinAtt, insertPara.insertPos, insertPara.viewDir, insertPara.insertDir);
	AcGeVector3d offset = transPara.insertPos - insertPara.insertPos;
	TYCOM_Move(p_id, offset);

}
AcDbObjectId CWindowGen::UpdateWindow(const AcDbObjectId p_id, AttrWindow newWinAtt, const bool bUpdateRelatedWin)
{
	if (p_id == AcDbObjectId::kNull)
		return p_id;

	const CWinInsertPara insertPara = GetWindowInsertPara(p_id);
	AttrWindow *pOldWinAtt = GetWinAtt(p_id);
	if (pOldWinAtt==NULL)
	{
		JHCOM_DeleteCadObject(p_id);

		AcDbObjectId newId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull, insertPara.sLayerName);
		return newId;
	}
	
	AcDbObjectId fromWinId = AcDbObjectId::kNull;
	AcDbObjectIdArray relatedWinIds;
	if (pOldWinAtt->m_fromWinId ==AcDbObjectId::kNull) //自身是父节点门窗
	{
		fromWinId = p_id;
		relatedWinIds = pOldWinAtt->m_relatedWinIds;
	}
	else  //自身是关联生成的门窗
	{
		fromWinId = pOldWinAtt->m_fromWinId;
		AcDbObjectId tempId;
		GetWinRelationIDs(fromWinId, tempId, relatedWinIds);
	}

	//若原型没有改变，则只需更新尺寸数据
	AcDbObjectId outId = p_id;
	const bool bProtoTypeSame = pOldWinAtt->m_prototypeCode.CompareNoCase(newWinAtt.m_prototypeCode) == 0;
	if (bProtoTypeSame)
	{
		if (bUpdateRelatedWin)
		{
			ModifyOneWindow(fromWinId, newWinAtt);
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				ModifyOneWindow(relatedWinIds[i], newWinAtt);
			}
		}
		else
		{
			ModifyOneWindow(p_id, newWinAtt);
		}
	}
	else //若原型改变，则需要全部删除和重新生成
	{
		if (bUpdateRelatedWin)
		{
			AcDbObjectId newMainId = AcDbObjectId::kNull;
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(fromWinId);
				JHCOM_DeleteCadObject(fromWinId);

				newMainId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull, newInsertPara.sLayerName);
				if (p_id==fromWinId)
					outId = newMainId;
			}

			AcDbObjectIdArray newRelatedIds;
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(relatedWinIds[i]);
				JHCOM_DeleteCadObject(relatedWinIds[i]);

				AcDbObjectId newId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull, newInsertPara.sLayerName);
				newRelatedIds.append(newId);
				if (p_id == relatedWinIds[i])
					outId = newId;

				AcDbObjectIdArray relatedIds;
				SetWinRelationIDs(newId, newMainId, relatedIds);
			}

			SetWinRelationIDs(newMainId, AcDbObjectId::kNull, newRelatedIds);
		}
		else
		{
			JHCOM_DeleteCadObject(p_id);
			outId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull, insertPara.sLayerName);
		}		
	}	

	return outId;
}

bool CWindowGen::SetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId p_fromWinId, AcDbObjectIdArray p_relatedIds)
{
	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt==NULL)
		return false;

	pWinAtt->m_fromWinId = p_fromWinId;
	pWinAtt->m_relatedWinIds = p_relatedIds;

	pWinAtt->close();

	return true;
}

bool CWindowGen::GetWinRelationIDs(AcDbObjectId p_id, AcDbObjectId& p_fromWinId, AcDbObjectIdArray& p_relatedIds)
{
	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt == NULL)
		return false;

	p_fromWinId = pWinAtt->m_fromWinId;
	p_relatedIds = pWinAtt->m_relatedWinIds;

	pWinAtt->close();
	return true;

}

//插入门窗编号
AcDbObjectId CWindowGen::InsertWindowDoorCode(double p_width, double p_height, AcGePoint3d p_origin, CString p_number, eViewDir p_viewDir)
{
	acDocManager->lockDocument(curDoc());

	AcDbObjectId sWindowDoorTextId;
	CString oldLayerName;
	CString sWindowDoorLayerName = L"";
	MD2010_GetCurrentLayer(oldLayerName);

	if (p_viewDir == E_VIEW_TOP)
	{
		sWindowDoorLayerName = _T("Sunac_WinNumber_Pingmian");

		if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
		}

		MD2010_SetCurrentLayer(sWindowDoorLayerName);

		//门窗编号插入点
		sWindowDoorTextId = JHCOM_CreateText(AcGePoint3d(0, p_origin.y + 100, 0),
			AcGeVector3d(0, 0, 1),
			120, 0,
			p_number);

		AcDbEntity *pEnt;
		acdbOpenAcDbEntity(pEnt, sWindowDoorTextId, AcDb::kForWrite);

		AcDbExtents extents;
		pEnt->getGeomExtents(extents);
		double textLength = extents.maxPoint().x - extents.minPoint().x;
		AcGeVector3d offsetXYZ = AcGeVector3d(p_origin.x + (p_width - textLength) / 2, 0, 0);

		AcGeMatrix3d xform;
		xform.setToTranslation(offsetXYZ);
		pEnt->transformBy(xform);
		pEnt->close();
	}
	else
	{
		sWindowDoorLayerName = _T("Sunac_WinNumber_Limian");

		if (JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
		}
	}

	acDocManager->unlockDocument(curDoc());

	return sWindowDoorTextId;
}
