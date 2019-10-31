#pragma once

// eliminate spurious "statement has no effect" warnings.
#pragma warning(disable: 4705)


// wrapper for whatever critical section we have
class CCritSec 
{
    // make copy constructor and assignment operator inaccessible
    CCritSec(const CCritSec &refCritSec);
    CCritSec &operator=(const CCritSec &refCritSec);

    CRITICAL_SECTION m_CritSec;

#ifdef _DEBUG
public:
    DWORD   m_currentOwner;
    DWORD   m_lockCount;
    BOOL    m_fTrace;        // Trace this one
public:
    CCritSec();
    ~CCritSec();
    void Lock();
    void Unlock();
#else

public:
    CCritSec() 
	{
        InitializeCriticalSection(&m_CritSec);
    };

    virtual ~CCritSec() 
	{
        DeleteCriticalSection(&m_CritSec);
    };

    virtual void Lock() 
	{
        EnterCriticalSection(&m_CritSec);
    };

    virtual void Unlock() 
	{
        LeaveCriticalSection(&m_CritSec);
    };
#endif
};

class CAutoLock 
{
	// make copy constructor and assignment operator inaccessible
	CAutoLock(const CAutoLock &refAutoLock);
	CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
	CCritSec * m_pLock;

public:
	CAutoLock(CCritSec * plock)
	{
		m_pLock = plock;
		m_pLock->Lock();
	};

	~CAutoLock() 
	{
		m_pLock->Unlock();
	};
};

