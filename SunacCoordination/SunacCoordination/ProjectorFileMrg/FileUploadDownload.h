#pragma once
#include <vector>

class CUpDownFilePara;

typedef void(*FileUpDownCB)(CUpDownFilePara*);

class CUpDownFilePara
{
public:
	bool bUpload;
	CString sFileLocalPath; //本地文件名
	CString sFileName;		//包含后缀

	CString sDirInProject; //在项目中的目录

	CString sFileUrl;
	CString ftpSaveName;
	CString ftpDir;

	FileUpDownCB  cbFunc; //回调函数
	FileUpDownCB  uiCBFunc; //UI回调函数

	int progress; //进度，0-100, -1表示失败或中断

	void* pCaller; //调用者

public:
	CUpDownFilePara()
	{ 
		bUpload = true;
		cbFunc = NULL; 
		uiCBFunc = NULL;
		progress = 0; 
		pCaller = NULL;
	}
};



class CFileUpDownLoad //项目的数据
{
	CFileUpDownLoad();
public:
	static CFileUpDownLoad* Instance();
	~CFileUpDownLoad();

	void StopAll(); //停止所有上传下载,主要是程序退出时

	//strFileURLInServer待下载文件的URL,  strFileLocalFullPath存放到本地的路径
	static bool DownloadFile(const CString& strFileURLInServer, const CString & strFileLocalFullPath);
	static bool UploadFile(CString p_sFilePath, CString p_ftpSaveName, CString p_ftpDir);

	void DownloadFileByThread(CUpDownFilePara p_upFilePara);
	void UploadFileByThread(CUpDownFilePara p_upFilePara);

	void SetAppQuit(){ m_bWaitingForQuit = true; }

protected:
	static DWORD UploadFileThreadFunc(LPVOID pama);
	static DWORD DownloadFileThreadFunc(LPVOID pama);

protected:
	vector<CUpDownFilePara*> m_allUpFileParas;

	vector<HANDLE> m_allTheadHandle;


	bool m_bWaitingForQuit; //程序退出中
};

