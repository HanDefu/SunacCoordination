#pragma once
#include <vector>

class CFileUpDownLoad //项目的数据
{
	CFileUpDownLoad();
public:
	static CFileUpDownLoad* Instance();
	~CFileUpDownLoad();

	void StopAll(); //停止所有上传下载,主要是程序退出时

	//strFileURLInServer待下载文件的URL,  strFileLocalFullPath存放到本地的路径
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	//static bool DownloadFile(CProjectFile p_prjFile);

	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);


	void UploadFileByThread(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);

protected:


};

