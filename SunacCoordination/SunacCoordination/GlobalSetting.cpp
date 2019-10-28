#include "StdAfx.h"
#include "GlobalSetting.h"
#include <float.h>
#include <acdocman.h>
#include <dbxutil.h>
#include "Common/ComFun_Sunac.h"
#include <algorithm>

GlobalSetting::GlobalSetting()
{
	InitLocalFiles();
	m_syncOK = false;
}

GlobalSetting::~GlobalSetting()
{
	
}

int GlobalSetting::InitLocalFiles()
{
	//if (m_localFiles.size() > 0)
		//return 0;

	CString localFilePath = TY_GetPrototypeFilePath();
	//CTime curTime1 = GetTickCount();
	m_localFiles = TY_FindFilesInDirecotry(L"*.dwg",localFilePath);
	//CTime curTime2 = GetTickCount();
	//CTimeSpan sp = curTime2 - curTime1;
	//int second = sp.GetSeconds();
	return 0;
}

bool GlobalSetting::GetLocalFile(CString fileName,CString &filePathName)
{
	InitLocalFiles();

	std::pair<CString,CString> filePair;
	filePair.first = fileName;
	filePair.second = TY_GetPrototypeFilePath() + fileName;
	vector<pair<CString,CString>>::iterator it = 
		std::find(m_localFiles.begin(),m_localFiles.end(), filePair);
	if (it != m_localFiles.end())
	{
		filePathName = filePair.second;
		return true;
	}
	return false;
}

