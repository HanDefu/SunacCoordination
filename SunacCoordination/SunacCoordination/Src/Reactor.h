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
};

extern CMyEditReactor* g_editorReactor;
extern CMyDocReactor* g_docReactor;
extern CMyDbReactor* g_dbReactor;
