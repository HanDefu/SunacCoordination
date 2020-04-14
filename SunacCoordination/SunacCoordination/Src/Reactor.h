#pragma once

class CMyEditReactor : public AcEditorReactor3
{
public:
	CMyEditReactor();
	virtual void commandWillStart(const ACHAR* cmdStr);
	virtual void commandEnded(const ACHAR* cmdStr);
	virtual void commandCancelled(const ACHAR* cmdStr);
	virtual void commandFailed(const ACHAR* cmdStr);
	bool IsInCommandMirror() { return m_bInCommandMirror; }

protected:
	bool m_bInCommandMirror;
};

class CMyDocReactor : public AcApDocManagerReactor
{
public:
	virtual void documentCreated(AcApDocument* pDocCreating);
	virtual void documentToBeDestroyed(AcApDocument* pDocToDestroy);
};

class CMyDbReactor : public AcDbDatabaseReactor
{
public:
	virtual void objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectModified(const AcDbDatabase* dwg, const AcDbObject* dbObj);
	virtual void objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased = true);

protected:
	bool EraseWinTextWhenMirror(AcDbEntity* pEnt);
	void WindowAppand(AcDbEntity* pEnt);
	void WindowModifed(AcDbEntity* pEnt);
	void WindowErase(AcDbEntity* pEnt);
};

extern CMyEditReactor* g_editorReactor;
extern CMyDocReactor* g_docReactor;
extern CMyDbReactor* g_dbReactor;
