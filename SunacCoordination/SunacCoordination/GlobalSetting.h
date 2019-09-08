#pragma once
#include "Sunac_def.h"
#include <utility>
using namespace std;
class GlobalSetting
{
public:
	static GlobalSetting *GetInstance()
	{
		static GlobalSetting instance;
		return &instance;
	}

	
	//本地是否含有这个文件,如果存在给出本地的全路径地址到filePathName
	bool GetLocalFile(CString fileName,CString &filePathName);

private:
	GlobalSetting();
	~GlobalSetting();
	int InitLocalFiles();

public:
	bool m_syncOK;
	vector<pair<CString,CString>> m_localFiles;
}; 

#define GSINST (GlobalSetting::GetInstance())