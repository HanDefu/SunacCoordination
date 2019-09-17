#include "StdAfx.h"
#include "WindowLocalData.h"
#include "..\Tool\Excel\Excel.h"

CWindowLocalData::CWindowLocalData()
{
}

CWindowLocalData::~CWindowLocalData()
{
}

CWindowLocalData* CWindowLocalData::GetInstance()
{
	static CWindowLocalData instance;
	return &instance;
}