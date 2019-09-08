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
		acDocManager->lockDocument(pDoc, lockMode);
	}
	//CDocLock &operator=(const CDocLock &p_docLock);
	~CDocLock()
	{
		acDocManager->unlockDocument(m_pDoc);
	}

private:
	AcApDocument* m_pDoc;

};

