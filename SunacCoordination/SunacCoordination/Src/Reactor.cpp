#include "StdAfx.h"
#include <dbobjptr.h>
#include "Reactor.h"
#include "../GlobalSetting.h"
#include "..\Object\WindowDoor\AttrWindow.h"
#include "..\Tangent\TangentOpen.h"
#include "..\Tool\DocLock.h"
#include "..\src\DocumentData.h"
#include "..\Common\ComFun_ACAD_Common.h"
#include "..\Common\ComFun_Math.h"

CMyEditReactor* g_editorReactor = NULL;
CMyDocReactor* g_docReactor = NULL;
CMyDbReactor* g_dbReactor = NULL;

CMyEditReactor::CMyEditReactor() : AcEditorReactor3()
{
	m_bInCommandMirror = false;
}

void CMyEditReactor::commandWillStart(const ACHAR* cmdStr)
{
	if (wcscmp(cmdStr, _T("MIRROR")) == 0)
		m_bInCommandMirror = true;
}

void CMyEditReactor::commandEnded(const ACHAR* cmdStr)
{
	if (wcscmp(cmdStr, _T("MIRROR")) == 0)
		m_bInCommandMirror = false;
}

void CMyEditReactor::commandCancelled(const ACHAR* cmdStr)
{
	if (wcscmp(cmdStr, _T("MIRROR")) == 0)
		m_bInCommandMirror = false;
}

void CMyEditReactor::commandFailed(const ACHAR* cmdStr)
{
	if (wcscmp(cmdStr, _T("MIRROR")) == 0)
		m_bInCommandMirror = false;
}

void CMyDocReactor::documentCreated(AcApDocument* pDocCreating)
{
	pDocCreating->database()->addReactor(g_dbReactor);
}

void CMyDocReactor::documentToBeDestroyed(AcApDocument* pDocToDestroy)
{
	pDocToDestroy->database()->removeReactor(g_dbReactor);
}

//////////////////////////////////////////////////////////////////////////

static int s_count = 1;
void CMyDbReactor::WindowAppand(AcDbEntity* pEnt)
{
	if (GlobalSetting::GetWinSetting()->m_bDrawTangentOpen == false)
		return;

	const AcDbObjectId curId = pEnt->objectId();
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(pEnt->objectId(), false);
	if (pWinAtt == NULL)
		return ;

	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;

		//////////////////////////////////////////////////////////////////////////
		//在copyclip(ctrl+c)和pasterclip(ctrl+v)复制粘贴时会调用objectAppended两次，第一次是拷贝到临时的块定义中，第二次是加入到modelspace，在此进行判断
		const AcDbObjectId owner =  pEnt->ownerId();
		const AcDbObjectId modespaceId = GetBlockRecordId(ACDB_MODEL_SPACE);
		if (owner==modespaceId)
		{
			//CDocLock docLock;
			//门窗编号
			CString sInstanceCode = GetWindowAutoName()->GetWindowName(*pWinAtt);
			pWinAtt->SetInstanceCode(sInstanceCode);
			GetWindowAutoName()->AddWindowType(*pWinAtt, curId);
			//////////////////////////////////////////////////////////////////////////

			AcDbExtents ext;
			pEnt->getGeomExtents(ext);
			AcGePoint3d centerPt = (ext.minPoint() + ext.maxPoint().asVector()) / 2;

			CTOpenData tWinData;
			tWinData.width = pWinAtt->GetW();
			tWinData.height = pWinAtt->GetH();
			tWinData.bottomHeight = pWinAtt->GetHeightUnderWindow();		
			tWinData.sWinCode = pWinAtt->GetInstanceCode();

			AcDbObjectId tWinOpenIdOut = AcDbObjectId::kNull;
			HRESULT hr = CTangentOpen::InsertWinOpenning(centerPt, tWinData, tWinOpenIdOut);

			GetWinTangentOpenMap()->AddWindow(curId, tWinOpenIdOut);
		}
	}
	catch (Acad::ErrorStatus )
	{

	}

	pWinAtt->close();
}

void CMyDbReactor::WindowModifed(AcDbEntity* pEnt)
{
	if (GlobalSetting::GetWinSetting()->m_bDrawTangentOpen == false)
		return;

	const AcDbObjectId curId = pEnt->objectId();
	Acad::ErrorStatus es;
	const AttrWindow * pWinAtt = AttrWindow::GetWinAtt(pEnt->objectId(), true);
	if (pWinAtt == NULL)
		return;
	
	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;

		const AcDbObjectId owner = pEnt->ownerId();
		const AcDbObjectId modespaceId = GetBlockRecordId(ACDB_MODEL_SPACE);
		if (owner != modespaceId)
			throw Acad::eOk;

		//只有tangentOpenId中能找到的实体才更新门洞
		AcDbObjectId tangentOpenId = GetWinTangentOpenMap()->GetTangentOpenId(curId);
		if (tangentOpenId == AcDbObjectId::kNull)
		{
			throw Acad::eNullObjectId;
		}

		//更新位置
		AcDbExtents extWin;
		es = pEnt->getGeomExtents(extWin);
		AcGePoint3d minPt = extWin.minPoint();
		AcGePoint3d maxPt = extWin.maxPoint();
		AcDbBlockReference* pWinRef = AcDbBlockReference::cast(pEnt);
		if (pWinRef!=NULL)
		{
			const double winWidth = pWinAtt->GetW();
			const double winThick = pWinAtt->GetD();
			AcGePoint3d insertPos = pWinRef->position();
			if (JHCOM_equ((extWin.maxPoint().x-extWin.minPoint().x), winWidth, 0.1))
			{
				//南北方向 水平窗
				if (abs(extWin.minPoint().y -  insertPos.y )< (winThick +1)) //门窗平面图在底部，开启扇在上 比较时+1是为了防止精度问题
				{
					maxPt = AcGePoint3d(maxPt.x, minPt.y+winThick, 0);
				}
				else //门窗平面图在顶部，开启扇在下
				{
					minPt = AcGePoint3d(minPt.x, maxPt.y - winThick, 0);
				}				
			}
			else
			{
				//东西方向窗,垂直窗
				if (abs(extWin.minPoint().x - insertPos.x) < (winThick + 1)) //门窗平面图在左侧，开启扇在右侧
				{
					maxPt = AcGePoint3d(minPt.x + winThick, maxPt.y , 0);
				}
				else //门窗平面图在右侧，开启扇在左侧
				{
					minPt = AcGePoint3d(maxPt.x - winThick, minPt.y, 0);
				}
			}
		}
		AcGePoint3d winCenter = (minPt + maxPt.asVector()) / 2;

		if (es != Acad::eOk)
			throw Acad::eFailed;

		CDocLock docLock;
		AcDbObjectPointer<AcDbEntity> pTOpenningEnt(tangentOpenId, AcDb::kForWrite);
		if (pTOpenningEnt.openStatus() == Acad::eOk &&pTOpenningEnt != NULL)
		{
			AcDbExtents extWinOpening;
			es = pTOpenningEnt->getGeomExtents(extWinOpening);
			AcGePoint3d winOpeningCenter = (extWinOpening.minPoint() + extWinOpening.maxPoint().asVector()) / 2;
			if (es==Acad::eOk)
			{
				//通过中心点得到所有的门窗
				AcGeMatrix3d xform;
				xform.setToTranslation(winCenter - winOpeningCenter);
				es = pTOpenningEnt->transformBy(xform);
			}
		}


		//更新尺寸等信息
		CTOpenData tWinData;
		tWinData.width = pWinAtt->GetW();
		tWinData.height = pWinAtt->GetH();
		tWinData.bottomHeight = pWinAtt->GetHeightUnderWindow();
		tWinData.sWinCode = pWinAtt->GetInstanceCode();
		CTangentOpen::SetTangentOpenProp(tangentOpenId, tWinData);
	}
	catch (Acad::ErrorStatus)
	{

	}
}

void CMyDbReactor::WindowErase(AcDbEntity* pEnt)
{
	if (GlobalSetting::GetWinSetting()->m_bDrawTangentOpen == false)
		return;

	AcDbObjectId curId = pEnt->objectId();
	AcDbObjectId dicID = pEnt->extensionDictionary();
	if (dicID == AcDbObjectId::kNull)
		return;

	AttrWindow *pWinAtt = NULL;
	AcDbDictionary *pDict = NULL;
	Acad::ErrorStatus es = acdbOpenObject(pDict, dicID, AcDb::kForRead);
	if (es == Acad::eOk)
	{
		AcDbObject *pDataEnt = NULL;
		es = pDict->getAt(SUNAC_ATTRIBUTE_ENTITY, (AcDbObject*&)pDataEnt, AcDb::kForRead);
		pDict->close();
		if (pDataEnt != NULL)
		{
			pWinAtt = AttrWindow::cast(pDataEnt);
			pDataEnt->close();
		}
	}

	if (pWinAtt == NULL)
		return;

	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;

		const AcDbObjectId owner = pEnt->ownerId();
		const AcDbObjectId modespaceId = GetBlockRecordId(ACDB_MODEL_SPACE);
		if (owner != modespaceId)
			throw Acad::eOk;

		AcDbObjectId tangentOpenId = GetWinTangentOpenMap()->GetTangentOpenId(curId);
		//AcDbObjectId tangentOpenId = pWinAtt->GetWinTangentOpenId();
		if (tangentOpenId != AcDbObjectId::kNull)
		{
			CDocLock docLock;
			AcDbEntity *pTOpenningEnt = NULL;
			Acad::ErrorStatus es = acdbOpenObject(pTOpenningEnt, tangentOpenId, AcDb::kForWrite);
			if (es == Acad::eOk &&pTOpenningEnt != NULL)
			{
				pTOpenningEnt->erase();
				pTOpenningEnt->close();
			}

			GetWinTangentOpenMap()->RemoveWindow(curId);
		}
	}
	catch (Acad::ErrorStatus)
	{

	}

	pWinAtt->close();
}

void CMyDbReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	AcDbEntity* pEnt = AcDbEntity::cast(dbObj);
	if (pEnt == NULL)
		return;
	
	//镜像时移除门窗编号
	//bool bSuc = EraseWinTextWhenMirror(pEnt);

	WindowAppand(pEnt);
}
void CMyDbReactor::objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	AcDbEntity* pEnt = AcDbEntity::cast(dbObj);
	if (pEnt == NULL)
		return;
	
	WindowModifed(pEnt);
}

void CMyDbReactor::objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased)
{
	if (pErased==false)
		return;

	AcDbEntity* pEnt = AcDbEntity::cast(dbObj);
	if (pEnt == NULL)
		return;

	WindowErase(pEnt);
}

//镜像时移除门窗编号
bool CMyDbReactor::EraseWinTextWhenMirror(AcDbEntity* pEnt)
{
	if ((g_editorReactor != NULL) && g_editorReactor->IsInCommandMirror())
	{
		CString sLayerName = pEnt->layer();
		CString sPMLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;
		CString sLMLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian;
		if ((sLayerName != sPMLayerName) && (sLayerName != sLMLayerName)) //只处理门窗文字所在的图层
			return false;

		if (pEnt->isKindOf(AcDbText::desc()) || pEnt->isKindOf(AcDbMText::desc()))
		{
			pEnt->upgradeOpen(); //提升为写权限
			pEnt->erase();

			return true;
		}
	}

	return false;
}
