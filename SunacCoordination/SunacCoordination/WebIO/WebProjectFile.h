#pragma once
#include "..\ProjectorFileMrg\ProjectFileMrg.h"
#include "../Tool/MarkupXml/Markup.h"

class CWebProjectFile
{
	CWebProjectFile();
public:
	static CWebProjectFile* Instance();
	~CWebProjectFile();

	//获取当前用户下的项目
	bool GetAllProjectInfo();
	
	bool UpdateFile(CString p_prjectId, CString p_dir, CString p_fileSaveName, CString p_fileName);
	bool DeleteFile(CString p_prjectId, CString p_dir, CString p_fileName);

	bool NewFolder(CString p_prjectId, CString p_sNewDirPath);
	bool RenameFolder(CString p_prjectId, CString p_sRenameDirPath, CString p_sNewDirName);
	bool DeleteFolder(CString p_prjectId, CString p_dir);


	void ParseProjectInfoFromXML(CMarkup xml);
	void ParseNewFileDirFromXML(CMarkup xml);
	void ParseDeleteFileFromXML(CMarkup xml);
	void ParseRenameFileDirFromXML(CMarkup xml);
	void ParseUpdateFileFromXML(CMarkup xml);

protected:

};

