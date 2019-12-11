#include "StdAfx.h"
#include "..\..\Common\ComFun_Sunac.h"
#include "..\..\Common\ComFun_ACad.h"
#include "..\..\Common\ComFun_Layer.h"
#include "WindowGen.h"
#include "..\..\GlobalSetting.h"


//////////////////////////////////////////////////////////////////////////

bool CWindowGen::MirrorObjectByCenter(const AcDbObjectId p_id, E_DIRECTION p_winDir)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况

	AcGePoint3d mirrorBasePt;
	AcGeVector3d mirrorAxis;

	AcGeVector3d offsetXY(0, 0, 0);
	switch (p_winDir)
	{
	case E_DIR_BOTTOM:
	case E_DIR_TOP:
		mirrorBasePt = AcGePoint3d((minPt.x + maxPt.x) / 2, 0, 0);
		mirrorAxis = AcGeVector3d(0, 1, 0);
		break;
	case E_DIR_RIGHT:
	case E_DIR_LEFT:
		mirrorBasePt = AcGePoint3d(0, (minPt.y + maxPt.y) / 2, 0);
		mirrorAxis = AcGeVector3d(1, 0, 0);
		break;
	default:
		return false;
		break;
	}

	TYCOM_Mirror(p_id, mirrorBasePt, mirrorAxis);

	return true;
}

double CWindowGen::GetBlockRotateAngle(const AttrWindow& curWinAtt, eViewDir p_view, E_DIRECTION p_winDir)
{
	double rotateAngle = 0;
	if (p_view == E_VIEW_TOP)
	{
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			rotateAngle = PI;
			break;
		case E_DIR_RIGHT:
			rotateAngle = -PI / 2;
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			rotateAngle = PI / 2;
			break;
		case E_DIR_UNKNOWN:
		default:
			break;
		}
	}

	return rotateAngle;
}

AcGePoint3d CWindowGen::GetBlockInsertPos(const AcDbObjectId p_id, const AttrWindow& curWinAtt, const AcGePoint3d pos, eViewDir p_view, E_DIRECTION p_winDir)
{
	double W = curWinAtt.GetW();

	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况
	W = (maxPt.x - minPt.x);

	AcGePoint3d insertPos = pos;
	if (p_view == E_VIEW_TOP)
	{
		AcGeVector3d offsetXY(0, 0, 0);
		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			offsetXY.x += W;
			break;
		case E_DIR_RIGHT:
			W = (maxPt.y - minPt.y);
			offsetXY.y += W;
			break;
		case E_DIR_TOP:
			break;
		case E_DIR_LEFT:
			break;
		default:
			break;
		}

		insertPos += offsetXY;
	}

	return insertPos;
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
	const eViewDir p_view = curWinAtt.m_viewDir;
	const double rotateAngle = GetBlockRotateAngle(curWinAtt, p_view, p_winDir);

	CString sBlockDwgFileName = curWinAtt.GetPrototypeDwgFilePath(p_view);
	RCWindow oneWindow;
	AcDbObjectId id = oneWindow.Insert(sBlockDwgFileName, AcGePoint3d::kOrigin, rotateAngle, p_sLayerName, 256); //先插入原点，再移动

	//门窗编号生成
	if (p_bDetailWnd == false)
	{
		if (GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber || p_view == E_VIEW_TOP)
		{
			curWinAtt.m_instanceCodeId = InsertWindowDoorCode(p_view, curWinAtt.GetInstanceCode());
		}
	}

	//////////////////////////////////////////////////////////////////////////
	UpdateRcWindowPara(id, curWinAtt, p_view, p_bDetailWnd);
	AddWinAtt(id, curWinAtt);

	//移动编号
	if (p_bDetailWnd == false)
	{
		if (GlobalSetting::GetInstance()->m_winSetting.m_bShowLimianNumber || p_view == E_VIEW_TOP)
		{
			double winWidth = GetWinWidth(id);
			double winHeight = GetWinHeight(id);
			MoveWindowDoorCode(curWinAtt.m_instanceCodeId, winWidth, winHeight, pos, p_view, p_winDir);
		}
	}

	//移动窗型
	AcGePoint3d insertPos = GetBlockInsertPos(id, curWinAtt, pos, p_view, p_winDir);
	TYCOM_Move(id, insertPos.asVector());

	//处理镜像
	if (curWinAtt.IsInstanceNeedMirror())
	{
		MirrorObjectByCenter(oneWindow.m_id, p_winDir);
	}

	//////////////////////////////////////////////////////////////////////////
	//针对USC坐标处理
	AcGeMatrix3d mat;
	Acad::ErrorStatus es = acedGetCurrentUCS(mat);
	if (mat.isEqualTo(AcGeMatrix3d::kIdentity) == false)
	{
		TYCOM_Transform(id, mat);
	}

	return id;
}

AcGePoint3d CWindowGen::GetWindowLeftBottomPos(AcDbObjectId p_id)
{
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况
	return minPt;
}

bool CWindowGen::GetWindowInsertPos(AcDbObjectId p_id, AcGePoint3d &p_insertPos, double& p_angle)
{
	p_insertPos = AcGePoint3d::kOrigin;
	p_angle = 0;

	AcDbEntity* pEnt = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pEnt, p_id, AcDb::kForRead);
	if (es!=Acad::eOk )
	{
		return false;
	}
	
	AcDbBlockReference *pBlockReference = AcDbBlockReference::cast(pEnt);
	if (pBlockReference==NULL)
	{
		return false;
	}

	p_insertPos = pBlockReference->position();
	p_angle = pBlockReference->rotation();
	pBlockReference->close();
	return true;
}

E_DIRECTION CWindowGen::GetWindowInsertDir(AcDbObjectId p_id)
{
	E_DIRECTION winDir = E_DIR_TOP; //TODO 自动分析视图方向

	return winDir;
}

CWinInsertPara CWindowGen::GetWindowInsertPara(AcDbObjectId p_id) //根据已插入的门窗获取其插入的信息
{
	CWinInsertPara insertPara;

	double angle;
	GetWindowInsertPos(p_id, insertPara.insertPos, angle);
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
	//定义点在左下角点
	AcGePoint3d minPt, maxPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minPt, maxPt); //TODO 处理ucs坐标下的情况

	AcGePoint3d insertPos;
	double angle;
	GetWindowInsertPos(p_id, insertPos, angle);

	//通过矩阵判断

	//正常情况插入点在左下角，根据
	

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

	const CWinInsertPara oldInsertPara = GetWindowInsertPara(p_id);
	//以下信息保持和原来的不变
	newWinAtt.m_viewDir = oldInsertPara.viewDir; 
	newWinAtt.m_fromWinId = oldInsertPara.fromWinId;
	newWinAtt.m_relatedWinIds = oldInsertPara.relatedWinIds;
	
	//更新尺寸信息
	UpdateRcWindowPara(p_id, newWinAtt, oldInsertPara.viewDir, oldInsertPara.bDetailWnd);
	UpdateWinAtt(p_id, newWinAtt);

	//处理镜像
	if (newWinAtt.IsInstanceNeedMirror() && IsWindowMirror(p_id)==false ||
		newWinAtt.IsInstanceNeedMirror()==false && IsWindowMirror(p_id))
	{
		MirrorObjectByCenter(p_id, GetWindowInsertDir(p_id));
	}

	//更新位置
	AcGePoint3d newLeftBottomPos = GetWindowLeftBottomPos(p_id);
	AcGeVector3d offset = oldInsertPara.leftBottomPos - newLeftBottomPos;
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
		DeleteWindowObj(p_id);

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
				DeleteWindowObj(fromWinId);

				newMainId = GenerateWindow(newWinAtt, newInsertPara.leftBottomPos, newInsertPara.insertDir, newInsertPara.bDetailWnd, AcDbObjectId::kNull, newInsertPara.sLayerName);
				if (p_id==fromWinId)
					outId = newMainId;
			}

			AcDbObjectIdArray newRelatedIds;
			for (int i = 0; i < relatedWinIds.length(); i++)
			{
				const CWinInsertPara newInsertPara = GetWindowInsertPara(relatedWinIds[i]);
				DeleteWindowObj(relatedWinIds[i]);

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
			DeleteWindowObj(p_id);
			outId = GenerateWindow(newWinAtt, insertPara.leftBottomPos, insertPara.insertDir, insertPara.bDetailWnd, AcDbObjectId::kNull, insertPara.sLayerName);
		}		
	}	

	return outId;
}

bool CWindowGen::DeleteWindowObj(const AcDbObjectId p_id)
{
	AttrWindow *pWinAtt = GetWinAtt(p_id);
	if (pWinAtt!=NULL &&pWinAtt->m_instanceCodeId!=AcDbObjectId::kNull)
	{
		JHCOM_DeleteCadObject(pWinAtt->m_instanceCodeId);
		pWinAtt->close();
	}

	JHCOM_DeleteCadObject(p_id);
	return true;
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
AcDbObjectId CWindowGen::InsertWindowDoorCode(eViewDir p_viewDir, CString p_number)
{
	acDocManager->lockDocument(curDoc());

	AcDbObjectId sWindowDoorTextId;
	CString oldLayerName;
	CString sWindowDoorLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;

	MD2010_GetCurrentLayer(oldLayerName);

	if (p_viewDir == E_VIEW_TOP)
	{
		if (sWindowDoorLayerName.GetLength() && JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
		}
	}
	else
	{
		sWindowDoorLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian;

		if (sWindowDoorLayerName.GetLength() && JHCOM_GetLayerID(sWindowDoorLayerName) == AcDbObjectId::kNull)
		{
			JHCOM_CreateNewLayer(sWindowDoorLayerName);
			JHCOM_SetLayerPrint(sWindowDoorLayerName);
		}
	}

	MD2010_SetCurrentLayer(sWindowDoorLayerName);

	//门窗编号插入点
	sWindowDoorTextId = JHCOM_CreateText(AcGePoint3d(0, 0, 0),
		AcGeVector3d(0, 0, 1),
		GlobalSetting::GetInstance()->m_winSetting.m_numberTextSize, 0,
		p_number);

	MD2010_SetCurrentLayer(oldLayerName);

	acDocManager->unlockDocument(curDoc());

	return sWindowDoorTextId;
}

void CWindowGen::MoveWindowDoorCode(AcDbObjectId p_id, double p_winWidth, double p_winHeight, AcGePoint3d p_origin, eViewDir p_viewDir, E_DIRECTION p_winDir)
{
	AcGePoint3d minWinTextPt, maxWinTextPt;

	JHCOM_GetObjectMinMaxPoint(p_id, minWinTextPt, maxWinTextPt);
	double textWidth = maxWinTextPt.x - minWinTextPt.x;
	double textHeight = maxWinTextPt.y - minWinTextPt.y;

	if (p_viewDir == E_VIEW_TOP)
	{
		double rotateAngle = 0;
		AcGeVector3d offsetXYZ = AcGeVector3d(p_origin.x + (p_winWidth - textWidth) / 2, p_origin.y + 100, 0);
		AcGeVector3d offsetLeftRightXYZ = AcGeVector3d(0, 0, 0);

		switch (p_winDir)
		{
		case E_DIR_BOTTOM:
			offsetXYZ = AcGeVector3d(p_origin.x + (p_winWidth - textWidth) / 2, p_origin.y - textHeight - 100, 0);
			break;
		case E_DIR_TOP:
			offsetXYZ = AcGeVector3d(p_origin.x + (p_winWidth - textWidth) / 2, p_origin.y + 100, 0);
			break;
		case E_DIR_RIGHT:
			rotateAngle = PI / 2;
			offsetXYZ = AcGeVector3d(p_origin.x, p_origin.y, 0);
			offsetLeftRightXYZ = AcGeVector3d(100 + textHeight, (p_winHeight - textWidth) / 2, 0);
			break;
		case E_DIR_LEFT:
			rotateAngle = PI / 2;
			offsetXYZ = AcGeVector3d(p_origin.x, p_origin.y, 0);
			offsetLeftRightXYZ = AcGeVector3d(- 100, (p_winHeight - textWidth) / 2, 0);
			break;
		case E_DIR_UNKNOWN:
			break;
		default:
			break;
		}

		TYCOM_Move(p_id, offsetXYZ);
		TYCOM_Rotate(p_id, p_origin, rotateAngle);
		TYCOM_Move(p_id, offsetLeftRightXYZ);
	}
	else
	{
		AcGeVector3d offsetXYZ = AcGeVector3d(p_origin.x + (p_winWidth - textWidth) / 2, p_origin.y - textHeight - 100, 0);
		TYCOM_Move(p_id, offsetXYZ);
	}
}

double CWindowGen::GetWinWidth(AcDbObjectId p_id)
{
	AcGePoint3d minWindowPt, maxWindowPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minWindowPt, maxWindowPt);
	return maxWindowPt.x - minWindowPt.x;
}

double CWindowGen::GetWinHeight(AcDbObjectId p_id)
{
	AcGePoint3d minWindowPt, maxWindowPt;
	JHCOM_GetObjectMinMaxPoint(p_id, minWindowPt, maxWindowPt);
	return maxWindowPt.y - minWindowPt.y;
}