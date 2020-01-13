#include "StdAfx.h"
#include "Reactor.h"
#include "../GlobalSetting.h"

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

void CMyDbReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	AcDbEntity* pEnt = AcDbEntity::cast(dbObj);
	if (pEnt == NULL)
		return;

	if ((g_editorReactor != NULL) && g_editorReactor->IsInCommandMirror())
	{
		CString sLayerName = pEnt->layer();
		CString sPMLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerPingmian;
		CString sLMLayerName = GlobalSetting::GetInstance()->m_winSetting.m_sWinNumberLayerLimian;
		if ((sLayerName != sPMLayerName) && (sLayerName != sLMLayerName)) //只处理门窗文字所在的图层
			return;

		Acad::ErrorStatus es = pEnt->upgradeOpen(); //提升为写权限
		if (es == Acad::eOk)
		{
			if (pEnt->isKindOf(AcDbText::desc()) || pEnt->isKindOf(AcDbMText::desc()))
				pEnt->erase();
		}
	}
}
