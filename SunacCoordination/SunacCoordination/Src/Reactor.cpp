#include "StdAfx.h"
#include "Reactor.h"
#include "../GlobalSetting.h"
#include "..\Object\WindowDoor\AttrWindow.h"
#include "..\Tangent\TangentOpen.h"
#include "..\Tool\DocLock.h"
#include "..\src\DocumentData.h"
#include "..\Common\ComFun_ACAD_Common.h"

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

	AcDbObjectId curId = pEnt->objectId();
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(pEnt->objectId());
	if (pWinAtt == NULL)
		return ;

	CDocLock docLock;
	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;
		
		//////////////////////////////////////////////////////////////////////////

		AcDbExtents ext;
		pEnt->getGeomExtents(ext);
		const AcDbObjectId owner =  pEnt->ownerId();
		const AcDbObjectId modespaceId = GetBlockRecordId(ACDB_MODEL_SPACE);

		//在copyclip(ctrl+c)和pasterclip(ctrl+v)复制粘贴时会调用objectAppended两次，第一次是拷贝到临时的块定义中，第二次是加入到modelspace，在此进行判断
		if (owner==modespaceId)
		{
			AcGePoint3d centerPt = (ext.minPoint() + ext.maxPoint().asVector()) / 2;

			CTOpenData tWinData;
			tWinData.width = pWinAtt->GetW();
			tWinData.height = pWinAtt->GetH();
			tWinData.bottomHeight = pWinAtt->GetHeightUnderWindow();
		
			CString str; 
			str.Format(_T("%s_%d"), pWinAtt->GetInstanceCode(), s_count);
			s_count++;
			pWinAtt->SetInstanceCode(str);

			tWinData.sWinCode = pWinAtt->GetInstanceCode();

			AcDbObjectId tWinOpenIdOut = AcDbObjectId::kNull;
			HRESULT hr = CTangentOpen::InsertWinOpenning(centerPt, tWinData, tWinOpenIdOut);

			pWinAtt->SetWinTangentOpenId(curId, tWinOpenIdOut);
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

	AcDbObjectId curId = pEnt->objectId();
	Acad::ErrorStatus es;
	AttrWindow * pWinAtt = AttrWindow::GetWinAtt(pEnt->objectId());
	if (pWinAtt == NULL)
		return;

	CDocLock docLock;
	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;

		if (pWinAtt->GetWinTangentOpenId() == AcDbObjectId::kNull)
		{
			AcDbObjectId tangentOpenId = GetWinTangentOpenMap()->GetTangentOpenId(curId);
			if (tangentOpenId == AcDbObjectId::kNull)
			{
				throw Acad::eNullObjectId;
			}
			pWinAtt->SetWinTangentOpenId(curId, tangentOpenId);
		}

		//更新位置
		AcDbExtents extWin;
		es = pEnt->getGeomExtents(extWin);
		if (es != Acad::eOk)
			throw Acad::eFailed;
		
		AcDbEntity *pTOpenningEnt = NULL;
		es = acdbOpenObject(pTOpenningEnt, pWinAtt->GetWinTangentOpenId(), AcDb::kForWrite);
		if (es == Acad::eOk &&pTOpenningEnt != NULL)
		{
			AcDbExtents extWinOpening;
			es = pTOpenningEnt->getGeomExtents(extWinOpening);
			if (es==Acad::eOk)
			{
				AcGeMatrix3d xform;
				xform.setToTranslation(extWin.minPoint() - extWinOpening.minPoint());
				es = pTOpenningEnt->transformBy(xform);
			}

			pTOpenningEnt->close();
		}


		//更新尺寸等信息
		CTOpenData tWinData;
		tWinData.width = pWinAtt->GetW();
		tWinData.height = pWinAtt->GetH();
		tWinData.bottomHeight = pWinAtt->GetHeightUnderWindow();
		tWinData.sWinCode = pWinAtt->GetInstanceCode();
		CTangentOpen::SetTangentOpenProp(pWinAtt->GetWinTangentOpenId(), tWinData);
	}
	catch (Acad::ErrorStatus)
	{

	}

	pWinAtt->close();
}
void CMyDbReactor::WindowErase(AcDbEntity* pEnt)
{
	if (GlobalSetting::GetWinSetting()->m_bDrawTangentOpen == false)
		return;
resbuf
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

	//AttrWindow * pWinAtt = AttrWindow::GetWinAtt(pEnt->objectId());
	if (pWinAtt == NULL)
		return;

	CDocLock docLock;
	try
	{
		if (pWinAtt->GetViewDir() != E_VIEW_TOP) //只有平面图才绘制门洞
			throw Acad::eOk;

		if (pWinAtt->GetWinTangentOpenId() == AcDbObjectId::kNull)
		{
			AcDbObjectId tangentOpenId = GetWinTangentOpenMap()->GetTangentOpenId(curId);
			if (tangentOpenId == AcDbObjectId::kNull)
			{
				throw Acad::eNullObjectId;
			}
			pWinAtt->SetWinTangentOpenId(curId, tangentOpenId);
		}

		AcDbEntity *pTOpenningEnt = NULL;
		Acad::ErrorStatus es = acdbOpenObject(pTOpenningEnt, pWinAtt->GetWinTangentOpenId(), AcDb::kForWrite);
		if (es == Acad::eOk &&pTOpenningEnt != NULL)
		{
			pTOpenningEnt->erase();
			pTOpenningEnt->close();
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
	//if (bSuc)
	//	return;

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
