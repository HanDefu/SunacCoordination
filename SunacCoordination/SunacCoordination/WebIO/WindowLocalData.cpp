#include "StdAfx.h"
#include "WindowLocalData.h"
#include "..\Common\Excel\Excel.h"

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