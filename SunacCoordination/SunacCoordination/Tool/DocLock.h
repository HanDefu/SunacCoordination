#pragma once


class CDocLock
{
public:
	CDocLock(AcApDocument* pDoc = NULL, AcAp::DocLockMode lockMode = AcAp::kWrite)
	{
		if (pDoc == NULL)
		{
			pDoc = curDoc();
		}
		m_pDoc = pDoc;
		Acad::ErrorStatus es = acDocManager->lockDocument(pDoc, lockMode);
		assert(es == Acad::eOk);
		if (es!=Acad::eOk)
		{
			acutPrintf(_T("lockÎÄµµÊ§°Ü\n"));
		}
	}
	//CDocLock &operator=(const CDocLock &p_docLock);
	~CDocLock()
	{
		acDocManager->unlockDocument(m_pDoc);
	}

private:
	AcApDocument* m_pDoc;

};

